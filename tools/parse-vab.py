import argparse
import dataclasses
import struct
from typing import BinaryIO


@dataclasses.dataclass
class VAB:
    magic: int
    version: int
    vab_id: int
    waveform_size: int
    reserved1: int
    num_programs: int
    num_tones: int
    num_vag: int
    master_volume: int
    master_pan: int
    bank_attr1: int
    bank_attr2: int
    reserved2: int
    program_attrs: list[int]
    tone_attrs: list[list[int]]
    vag_offsets: list[int]


(VAB_MAGIC,) = struct.unpack(">I", b"VABp")


def parse_vab(f: BinaryIO) -> VAB:
    (
        magic,
        version,
        vab_id,
        waveform_size,
        reserved1,
        num_programs,
        num_tones,
        num_vag,
        master_volume,
        master_pan,
        bank_attr1,
        bank_attr2,
        reserved2,
    ) = struct.unpack("<IIIIHHHHBBBBI", f.read(32))
    assert magic == VAB_MAGIC
    program_attrs = list(struct.unpack("<128H", f.read(256)))
    tone_attrs: list[list[int]] = []
    for _ in range(num_programs):
        program_tone_attrs = struct.unpack("<16I", f.read(64))
        tone_attrs.append(list(program_tone_attrs))
    vag_offsets = struct.unpack("<256H", f.read(512))
    vag_offsets = [offset << 3 for offset in vag_offsets]
    print(f.tell())
    return VAB(
        magic=magic,
        version=version,
        vab_id=vab_id,
        waveform_size=waveform_size,
        reserved1=reserved1,
        num_programs=num_programs,
        num_tones=num_tones,
        num_vag=num_vag,
        master_volume=master_volume,
        master_pan=master_pan,
        bank_attr1=bank_attr1,
        bank_attr2=bank_attr2,
        reserved2=reserved2,
        program_attrs=program_attrs,
        tone_attrs=tone_attrs,
        vag_offsets=vag_offsets,
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("vabfile")
    args = parser.parse_args()
    with open(args.vabfile, "rb") as f:
        vab = parse_vab(f)
        print(vab)


if __name__ == "__main__":
    main()
