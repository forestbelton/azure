"""Render a KDT sequence to a WAV using the game's own VAB samples.

This is a listening aid, not part of the lossless path: it mixes down to PCM
and throws the structure away. Extraction and repacking go through
parse-vab.py's raw .vag files plus vab.json, which round-trip byte for byte.

The sequence model follows the driver's dispatcher in csrc/unk_2CDC0.c
(func_800595C0): a note with velocity zero is a note-off for that key, 0x4A
and 0x4B release the last key touched, and every command outside 0x46..0x4B
and 0x7F is a control change.
"""

import argparse
import importlib.util
import math
import os
import struct

TRACK_DATA = 0x50
TEMPO_BIAS = 2
RATE = 44100

# The driver's tempo is literally (param * 2) + TEMPO_BIAS, but a sequence
# measured against an emulator recording runs about 1.4% slower than that:
# song1's dominant period is 0.8301s on the emulator against 0.8189s at a
# literal 110 BPM. The driver scales its tick accumulator by the video mode in
# func_80059814, which is the likely home of the difference but is not yet
# traced, so this is an EMPIRICAL correction rather than a derived one. Note
# that roughly a third of it may be the emulator's own clock, since its pitch
# also measures ~10 cents flat of this renderer's.
TEMPO_SCALE = 0.9865

# The driver works in pitch units of 1/128 of a semitone (func_80056654 packs
# a semitone in the high byte and a 7-bit fraction in the low bits).
PITCH_UNITS = 128
BEND_CENTRE = 0x40

# func_80056948 sweeps the modulation LFO on every eleventh frame tick. The
# depth and the triangle shape below are exact; this rate is the one number
# here not read off the driver, so a vibrato may run faster or slower than the
# hardware even though its depth is right.
LFO_TICK_HZ = 60.0 / 11.0

# The SPU's envelope generator runs at the output rate with a 15-bit level.
# Each phase has a shift (rate) and a step, and may be linear or exponential;
# increase steps are +7,+6,+5,+4 and decrease steps -8,-7,-6,-5. Decay and
# release always use the largest step. See the ADSR register layout in
# psx-spx; the release tail is what makes held notes ring, and on this bank it
# runs for well over a second.
ADSR_INCREASE_STEPS = (7, 6, 5, 4)
ADSR_DECREASE_STEPS = (8, 7, 6, 5)
ADSR_MAX_LEVEL = 0x7FFF
ADSR_MAX_RELEASE = 6 * RATE
ADSR_SILENCE = 24  # ~-62dB of full scale; below this the voice is inaudible

ATTACK = 0
DECAY = 1
SUSTAIN = 2
RELEASE = 3


def load_parse_vab():
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "parse-vab.py")
    spec = importlib.util.spec_from_file_location("parse_vab", path)
    assert spec is not None and spec.loader is not None, path
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


pv = load_parse_vab()


def find_kdt(buf):
    if buf[:4] == b"KDT1":
        return 0, False
    assert buf[:4] == b"KCET", "not a KCET container"
    kdt = 0x24
    assert buf[kdt : kdt + 4] == b"KDT ", "no KDT chunk at +0x24"
    return kdt, True


def read_sequence(buf, base):
    """Per-track event lists of (tick, kind, a, b)."""
    tickdiv = struct.unpack_from("<H", buf, base + 8)[0]
    ntracks = struct.unpack_from("<H", buf, base + 0xC)[0]
    sizes = struct.unpack_from("<%dH" % ntracks, buf, base + 0x10)

    offset = base + TRACK_DATA
    tracks = []
    for size in sizes:
        end = offset + size
        events = []
        tick = 0
        running = 0
        last_note = 0
        while offset < end:
            if not running:
                ticks = buf[offset] & 0x7F
                more = buf[offset] & 0x80
                offset += 1
                while more:
                    ticks = (ticks << 7) | (buf[offset] & 0x7F)
                    more = buf[offset] & 0x80
                    offset += 1
                tick += ticks
                running = 1
                continue

            byte = buf[offset]
            if byte & 0x80:
                cmd = byte & 0x7F
                if cmd in (0x4A, 0x4B):
                    events.append((tick, "off", last_note, 0))
                    running = 1 if cmd == 0x4B else 0
                    offset += 1
                else:
                    param = buf[offset + 1] & 0x7F
                    running = buf[offset + 1] & 0x80
                    offset += 2
                    events.append((tick, "cmd", cmd, param))
            else:
                note = byte & 0x7F
                velocity = buf[offset + 1] & 0x7F
                running = buf[offset + 1] & 0x80
                offset += 2
                last_note = note
                events.append((tick, "on" if velocity else "off", note, velocity))
        tracks.append(events)
    return tickdiv, tracks


def tempo_map(tracks):
    """Global (tick, bpm) changes, in tick order."""
    changes = []
    for events in tracks:
        for tick, kind, a, b in events:
            if kind == "cmd" and a == 0x47:
                changes.append((tick, min(TEMPO_BIAS + b * 2, 255)))
    changes.sort()
    return changes or [(0, 120)]


def make_clock(changes, tickdiv):
    """tick -> seconds, honouring tempo changes."""
    marks = [(0, 0.0, changes[0][1])]
    for tick, bpm in changes:
        last_tick, last_time, last_bpm = marks[-1]
        if tick == last_tick:
            marks[-1] = (tick, last_time, bpm)
        else:
            elapsed = (tick - last_tick) * 60.0 / (tickdiv * last_bpm * TEMPO_SCALE)
            marks.append((tick, last_time + elapsed, bpm))

    def seconds(tick):
        lo, hi = 0, len(marks) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if marks[mid][0] <= tick:
                lo = mid
            else:
                hi = mid - 1
        mark_tick, mark_time, bpm = marks[lo]
        return mark_time + (tick - mark_tick) * 60.0 / (tickdiv * bpm * TEMPO_SCALE)

    return seconds


def loop_span(tracks):
    """(start, end) ticks of the sequence's loop, from the 0x63 markers."""
    start = end = None
    for events in tracks:
        for tick, kind, a, b in events:
            if kind == "cmd" and a == 0x63:
                if b == 20 and start is None:
                    start = tick
                elif b == 30:
                    end = tick
    return start, end


def collect_notes(events):
    """(start_tick, end_tick, note, velocity, program, volume, expression, pan, bend)."""
    program = 0
    volume = 127
    expression = 127
    pan = 64
    bend = 64
    sounding = {}
    notes = []
    for tick, kind, a, b in events:
        if kind == "cmd":
            if a == 0x49:
                program = b
            elif a == 0x07:
                volume = b
            elif a == 0x0B:
                expression = b
            elif a == 0x0A:
                pan = b
            elif a == 0x48:
                bend = b
        elif kind == "on":
            sounding[a] = (tick, b, program, volume, expression, pan, bend)
        elif kind == "off":
            if a in sounding:
                start, vel, prog, vol, expr, pn, bnd = sounding.pop(a)
                notes.append((start, tick, a, vel, prog, vol, expr, pn, bnd))
    final = max((e[0] for e in events), default=0)
    for note, (start, vel, prog, vol, expr, pn, bnd) in sounding.items():
        notes.append((start, final, note, vel, prog, vol, expr, pn, bnd))
    return notes


def vag_loop(buf, start, size):
    """Loop points of a waveform, in samples."""
    blocks = size // pv.ADPCM_BLOCK_SIZE
    loop_start = 0
    for block in range(blocks):
        if buf[start + block * pv.ADPCM_BLOCK_SIZE + 1] & 4:
            loop_start = block
            break
    return loop_start * pv.ADPCM_BLOCK_SAMPLES, blocks * pv.ADPCM_BLOCK_SAMPLES


def adsr_envelope(adsr1, adsr2, held, cache={}):
    """Per-sample envelope gains for a note held `held` samples.

    Returns the whole contour including the release tail, so its length is
    what decides how long the voice actually sounds.
    """
    key = (adsr1, adsr2, held)
    hit = cache.get(key)
    if hit is not None:
        return hit

    sustain_level = ((adsr1 & 0x0F) + 1) * 0x800
    decay_shift = (adsr1 >> 4) & 0x0F
    attack_step = ADSR_INCREASE_STEPS[(adsr1 >> 8) & 0x03]
    attack_shift = (adsr1 >> 10) & 0x1F
    attack_exponential = (adsr1 >> 15) & 1
    release_shift = adsr2 & 0x1F
    release_exponential = (adsr2 >> 5) & 1
    sustain_decreasing = (adsr2 >> 14) & 1
    sustain_step = (
        ADSR_DECREASE_STEPS if sustain_decreasing else ADSR_INCREASE_STEPS
    )[(adsr2 >> 6) & 0x03]
    sustain_shift = (adsr2 >> 8) & 0x1F
    sustain_exponential = (adsr2 >> 15) & 1

    env = []
    level = 0
    phase = ATTACK
    counter = 0
    limit = held + ADSR_MAX_RELEASE
    i = 0
    while i < limit:
        if i >= held and phase != RELEASE:
            phase = RELEASE
            counter = 0

        if phase == ATTACK:
            shift, step = attack_shift, attack_step
            exponential, decreasing = attack_exponential, False
        elif phase == DECAY:
            shift, step = decay_shift, 8
            exponential, decreasing = True, True
        elif phase == SUSTAIN:
            shift, step = sustain_shift, sustain_step
            exponential, decreasing = sustain_exponential, sustain_decreasing
        else:
            shift, step = release_shift, 8
            exponential, decreasing = release_exponential, True

        cycles = 1 << max(0, shift - 11)
        amount = step << max(0, 11 - shift)
        if exponential:
            if decreasing:
                # the shifted product truncates to zero once the level is low,
                # which would stall the decay around -24dB and cut the tail off
                # audibly; keep it moving by at least one step
                amount = max(1, amount * level >> 15)
            elif level > 0x6000:
                cycles *= 4

        counter += 1
        if counter >= cycles:
            counter = 0
            level += -amount if decreasing else amount
            level = max(0, min(ADSR_MAX_LEVEL, level))

        if phase == ATTACK and level >= ADSR_MAX_LEVEL:
            phase = DECAY
        elif phase == DECAY and level <= sustain_level:
            phase = SUSTAIN

        env.append(level / float(ADSR_MAX_LEVEL))
        i += 1

        # a decreasing phase that has run out of step would never reach zero
        if phase == RELEASE and (level <= ADSR_SILENCE or amount == 0):
            break

    if len(cache) < 4096:
        cache[key] = env
    return env


def pick_tone(tones, note):
    for tone in tones:
        if tone.note_min <= note <= tone.note_max:
            return tone
    return tones[0] if tones else None


def root_units(tone):
    """A tone's root pitch in 1/128 semitones (func_80056654's b+0x18).

    `shift` detunes downward from `center - 1`, so the pair encodes a root a
    fraction of a semitone below the nominal key; a zero shift means the key
    itself.
    """
    if tone.shift:
        return (tone.center - 1) * PITCH_UNITS + (0x7F - tone.shift)
    return tone.center * PITCH_UNITS


def bend_units(tone, value):
    """func_800565D8: per-tone bend, where a zero bound disables that side."""
    if value < BEND_CENTRE:
        if not tone.bend_min:
            return 0
        return -((tone.bend_min << 1) * (0x3F - value))
    if value == BEND_CENTRE:
        return 0
    if not tone.bend_max:
        return 0
    return (tone.bend_max << 1) * (value - BEND_CENTRE)


def channel_timeline(events, clock, ticks):
    """Per-LFO-tick (bend value, modulation offset) for one track.

    Mirrors func_80057D20's control changes and func_80056800's triangle
    sweep. CC 1 sets either depth or rate depending on CC 3's mode, and CC 2
    sets both at once.
    """
    controls = sorted(
        (tick, a, b) for tick, kind, a, b in events
        if kind == "cmd" and a in (0x01, 0x02, 0x03, 0x48)
    )

    out = []
    bend = BEND_CENTRE
    enabled = depth = rate = step = level = 0
    rising = False
    mode = 0
    cursor = 0
    for t in range(ticks):
        now = t / LFO_TICK_HZ
        while cursor < len(controls) and clock(controls[cursor][0]) <= now:
            _, cc, d = controls[cursor]
            cursor += 1
            if cc == 0x48:
                bend = d
            elif cc == 0x03:
                mode = d
            elif cc == 0x02:
                rate = d
                depth = d << 1
            elif cc == 0x01:
                enabled = d
                if mode < 0x40:
                    depth = d << 1
                    if rate and depth:
                        step = depth * 4 // rate
                    else:
                        level = 0
                else:
                    rate = d
                    if rate and depth:
                        step = depth // rate * 2
                    else:
                        level = 0

        if enabled:
            if rising:
                if level + step < depth:
                    level += step
                else:
                    rising = False
                    level = depth
            else:
                if level - step > -depth:
                    level -= step
                else:
                    rising = True
                    level = -depth
        else:
            level = 0
        out.append((bend, level))
    return out


def render(kdt_path, vab_path, vab_offset, out_path, loops=1):
    with open(kdt_path, "rb") as f:
        seq = f.read()
    base, _ = find_kdt(seq)
    tickdiv, tracks = read_sequence(seq, base)
    clock = make_clock(tempo_map(tracks), tickdiv)

    with open(vab_path, "rb") as f:
        vab = pv.parse_vab(f, vab_offset)

    samples = {}
    for index, (start, size) in enumerate(vab.vag_spans, 1):
        pcm = pv.decode_adpcm(vab.buf, start, size)
        samples[index] = (pcm, *vag_loop(vab.buf, start, size))

    all_notes = [(t, n) for t, events in enumerate(tracks) for n in collect_notes(events)]

    # the sequence's loop markers let the song repeat seamlessly instead of
    # stopping at the end of the written material
    loop_start, loop_end = loop_span(tracks)
    if loops > 1 and loop_start is not None and loop_end is not None:
        period = loop_end - loop_start
        base = list(all_notes)
        for pass_index in range(1, loops):
            shift = period * pass_index
            for track, n in base:
                if n[0] >= loop_start:
                    all_notes.append((track, (n[0] + shift, n[1] + shift) + n[2:]))
        print("  looping %d times over ticks %d..%d" % (loops, loop_start, loop_end))
    # leave room for the longest release a voice could still be ringing out
    duration = max((clock(n[1]) for _, n in all_notes), default=0.0)
    total = int(duration * RATE) + ADSR_MAX_RELEASE + 1
    left = [0.0] * total
    right = [0.0] * total

    # bend and modulation move while a note sounds, so each track carries a
    # control timeline sampled at the LFO rate rather than one value per note
    lfo_ticks = int(total / RATE * LFO_TICK_HZ) + 2
    samples_per_tick = RATE / LFO_TICK_HZ
    timelines = [channel_timeline(events, clock, lfo_ticks) for events in tracks]

    skipped = 0
    for track, note in all_notes:
        # bend now comes from the track's timeline, not a note-on snapshot
        start_tick, end_tick, key, vel, prog, vol, expr, pan, _ = note
        tones = vab.tones_for(prog)
        tone = pick_tone(tones, key)
        if tone is None or tone.vag not in samples:
            skipped += 1
            continue
        pcm, loop_start, loop_end = samples[tone.vag]
        if not pcm:
            skipped += 1
            continue

        program_volume = vab.program_attrs[prog].master_volume
        gain = (
            (vel / 127.0)
            * (tone.volume / 127.0)
            * (program_volume / 127.0 if program_volume != 0xFF else 1.0)
            * (vol / 127.0)
            * (expr / 127.0)
            * (vab.master_volume / 127.0)
        )

        placement = min(127, max(0, tone.pan + (pan - 64)))
        angle = placement / 127.0 * (math.pi / 2)
        gain_l = gain * math.cos(angle)
        gain_r = gain * math.sin(angle)

        begin = int(clock(start_tick) * RATE)
        held = max(1, int((clock(end_tick) - clock(start_tick)) * RATE))
        env = adsr_envelope(tone.adsr1, tone.adsr2, held)
        span = loop_end - loop_start

        timeline = timelines[track]
        root = root_units(tone)
        tick = -1
        step = 1.0

        position = 0.0
        for i, envelope in enumerate(env):
            out = begin + i
            if out >= total:
                break
            # re-derive the pitch only when the control timeline moves on
            now = int(out / samples_per_tick)
            if now != tick:
                tick = now
                value, level = timeline[min(now, len(timeline) - 1)]
                units = key * PITCH_UNITS + bend_units(tone, value) + level - root
                step = 2.0 ** (units / float(PITCH_UNITS * 12))
            if position >= loop_end:
                if span <= 0:
                    break
                position = loop_start + (position - loop_end) % span
            index = int(position)
            # the sample after the last one in the loop is the loop's first
            following = index + 1
            if following >= loop_end:
                following = loop_start
            frac = position - index
            value = pcm[index] + (pcm[following] - pcm[index]) * frac
            value *= envelope
            left[out] += value * gain_l
            right[out] += value * gain_r
            position += step

    peak = max(max(abs(v) for v in left), max(abs(v) for v in right), 1.0)
    scale = 0.89 * 32767.0 / peak

    # trim the silence left by the release headroom
    threshold = peak / 10000.0
    while total > 1 and abs(left[total - 1]) < threshold and abs(right[total - 1]) < threshold:
        total -= 1

    frames = bytearray()
    for i in range(total):
        frames += struct.pack(
            "<hh",
            int(max(-32768, min(32767, left[i] * scale))),
            int(max(-32768, min(32767, right[i] * scale))),
        )

    with open(out_path, "wb") as f:
        f.write(b"RIFF" + struct.pack("<I", 36 + len(frames)) + b"WAVEfmt ")
        f.write(struct.pack("<IHHIIHH", 16, 1, 2, RATE, RATE * 4, 4, 16))
        f.write(b"data" + struct.pack("<I", len(frames)) + bytes(frames))

    print(
        "%s: %d tracks, %d notes (%d unmapped), %.1fs, peak %.0f -> %s"
        % (
            os.path.basename(kdt_path),
            len(tracks),
            len(all_notes),
            skipped,
            total / float(RATE),
            peak,
            out_path,
        )
    )


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("kdt")
    parser.add_argument("vab")
    parser.add_argument("output")
    parser.add_argument(
        "--vab-offset", type=lambda s: int(s, 0), default=0,
        help="offset of the VAB inside the vab file",
    )
    parser.add_argument(
        "--loops", type=int, default=1,
        help="render this many passes through the sequence's loop",
    )
    args = parser.parse_args()
    render(args.kdt, args.vab, args.vab_offset, args.output, args.loops)


if __name__ == "__main__":
    main()
