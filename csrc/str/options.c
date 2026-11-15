/*
 * The GAME OPTION screen's string pool, from main.bin's 0x80088760 overlay:
 * 28 plain-ASCII entries, each NUL-terminated and padded with zeros to the next
 * 4-byte boundary, which is the default entry form.
 *
 * Covers rom 0x021894..0x02194C.
 *
 * NOTHING IN ANY TEXT REFERENCES THIS POOL. The pointer table at +0x24350 in
 * the overlay's own .data points at it, and splat emits that table as raw
 * `.word` literals, so spimdisasm has no reference to infer a symbol from --
 * which is why `D_800887F4 = 0x800887F4;` has to be declared by hand in
 * splat/main.bin_symbol_addrs.txt for these bytes to disassemble at all.
 *
 * The first thirteen entries are the menu labels, in screen order. The fifteen
 * single characters after them are the per-glyph renderer's alphabet: B/G/R are
 * the colour channels of the wall-colour picker, and the rest spell out the
 * labels it draws one character at a time.
 *
 * Each entry is a separate object, so the compiler's own `.align 2` supplies
 * the zero padding up to the next 4-byte boundary -- the same bytes the old
 * generator emitted as `.balign 4, 0`. ORDER IS THE LAYOUT: cc1psx emits
 * file-scope objects in source order, so moving a definition moves the bytes.
 * The last entry carries an explicit size where the pool needs trailing
 * padding, because nothing follows it to align against.
 */

const char D_800887F4[] = "GAME OPTION";

static const char str_to_familiar[] = "  TO FAMILIAR";

static const char str_attack[] = "ATTACK";

static const char str_on[] = "ON";

static const char str_off[] = "OFF";

static const char str_magic_effect[] = " MAGIC EFFECT";

static const char str_sound_effect[] = " SOUND EFFECT";

static const char str_bgm[] = " BGM";

static const char str_volume[] = "VOLUME";

static const char str_max[] = "MAX";

static const char str_min[] = "MIN";

static const char str_wall_color[] = "  WALL COLOR";

static const char str_dungeon[] = "DUNGEON";

static const char str_b[] = "B";

static const char str_g[] = "G";

static const char str_r[] = "R";

static const char str_i[] = "I";

static const char str_m[] = "M";

static const char str_d[] = "D";

static const char str_n[] = "N";

static const char str_o[] = "O";

static const char str_t[] = "T";

static const char str_u[] = "U";

static const char str_x[] = "X";

static const char str_entry[] = " ";

static const char str_s[] = "S";

static const char str_e[] = "E";

static const char str_p[4] = "P";
