/* PARK: func_s03_80024020, csrc/ovl_dng_s03.c, agent-h249, 2026-11-26.

   TARGET 357 words. BEST BUILT 339 words, 208 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py): 137 register-only (`R') rows, 30 `X', 31
   target-only and 10 candidate-only. 1 member, 357 words (dngfam) -- no
   replication value.

   VERDICT: LENGTH, 18 words short, AND EVERY MISSING WORD IS A SPILL
   RELOAD. This is not a shape fault. The two instruction streams agree on
   the dispatch, all six arms, the jump-table arm order, both loops, the
   magic divide and the `abssi2' trio; the entire delta is that the TARGET
   KEEPS FOUR VALUES IN MEMORY AND THIS BODY KEEPS TWO.

     target frame -0x58:  args 0x20 + vars 0x10 (3 slots) + 10 saves 0x28
     built  frame -0x50:  args 0x20 + vars 0x08 (2 slots) + 10 saves 0x28

   The target spills `o' (sp+0x20), `dx' (sp+0x24), `dy' (sp+0x28) AND the
   parameter `b', which goes to its own incoming argument home at sp+0x5C
   (`sw $a1, 0x5C($sp)' in the prologue, then eleven `lw $t0, 0x5C($sp)'
   reloads through $t0). This body spills only `dx' and `dy' and keeps `o'
   in $s7 and `b' in $fp. Listing the target-only words confirms it: they
   are `lw $t0, 0x20($sp)' x5, `lw $t0, 0x5C($sp)' x4, the two extra
   prologue `sw's, three load-delay `nop's and the two `sw's of the spill
   itself. Remove the spills and the bodies are the same program.

   THE ARITHMETIC, from a single-function `-da' probe of THIS body
   (`probe.c.lreg' plus the `;; 25 regs to allocate:' line of
   `probe.c.greg'). `global.c' ranks on `floor_log2(n_refs)*n_refs/
   live_length' and there are exactly NINE call-preserved registers:

     pseudo 87 = `n'   36 refs /  35 insns /  1 call   5*36/35  = 5.143  $s0
     pseudo 83 = `q'   12 refs /  33 insns /  2 calls  3*12/33  = 1.091  $s1
     pseudo 90 = `i'   16 refs /  80 insns /  5 calls  4*16/80  = 0.800  $s2
     pseudo 84 = `s'   23 refs / 199 insns /  8 calls  4*23/199 = 0.462  $s3
     pseudo 92 = `ax'   7 refs /  31 insns /  2 calls  2* 7/31  = 0.452  $s4
     pseudo 93 = `ay'   7 refs /  32 insns /  2 calls  2* 7/32  = 0.438  $s5
     pseudo 80 = `p'   33 refs / 536 insns / 10 calls  5*33/536 = 0.308  $s6
     pseudo 85 = `o'   13 refs / 139 insns /  5 calls  3*13/139 = 0.281  $s7
     pseudo 81 = `b'   28 refs / 432 insns /  8 calls  4*28/432 = 0.259  $fp
     pseudo 94 = `dx'   5 refs / 125 insns /  5 calls  2* 5/125 = 0.080  SPILL
     pseudo 95 = `dy'   5 refs / 133 insns /  5 calls  2* 5/133 = 0.080  SPILL

   `o' and `b' are the eighth and ninth, i.e. the last two to fit. For the
   target's allocation this body needs **TWO MORE call-crossing allocnos
   ranked above 0.281**, which pushes `o' and `b' out. The gap is enormous
   (anything above 0.281 does it), so this is not a near-miss on a number --
   it is two values the source has that this body does not.

   WHERE THE TWO MISSING VALUES SHOW IN THE LISTING. In the case-0 search
   loop the target holds FOUR quantities where this body holds two:

     target:  addiu $s2, $v1, 0x20   /  andi $s1, $s2, 0xFFFF   (x_raw, x)
              addiu $s4, $v0, 0x20   /  andi $s0, $s4, 0xFFFF   (y_raw, y)
     built:   addu  $17, $17, 32     /  andi $17, $17, 0xffff    (one reg)

   and at the +0x72/+0x73 compare the target emits the THREE-word `abssi2'
   (`bgez $s2 / addu $s3, $s2, $zero / negu $s3, $s3', two registers) where
   this body emits the TWO-word form (`bgez $v1 / negu $v1, $v1', one
   register).

   **BUT NEITHER IS REACHABLE BY DECLARING MORE LOCALS, AND THAT IS THE
   MEASURED FINDING OF THIS PARK.** Three spellings of the raw/masked split
   all compile to the identical 339 words:

   | spelling                                               | words | rows |
   |--------------------------------------------------------|-------|------|
   | `u16 x, y;' truncating on assignment              KEPT |  339  | 208  |
   | `s32 x, y;' with `(u16)' casts at the call sites       |  339  | 211  |
   | `s32 xr, yr;' + `s32 x = (u16)xr;' (four locals)       |  339  | 211  |
   | separate loop counters for the two loops (FREE)        |  339  | 208  |

   The four-local form is byte-identical to the one-local form because the
   raw value DIES AT THE `and', so the allocator reuses its register -- the
   same coalescing that docs/matching-levers.md records for a plain copy
   ("`abssi2' NEEDS TWO DIFFERENT REGISTERS"), now measured for `and' as
   well. So the raw/masked split and the three-word `abs' are CONSEQUENCES
   of the higher pressure, not its cause: under the target's pressure reload
   has no free register to coalesce into. Chasing either one directly is a
   dead end, and this park exists to stop the next agent spending a run on
   it.

   WHAT I DID NOT SETTLE: where the two extra call-crossing values come
   from. The one hypothesis I could not test cheaply is that the target
   takes the ADDRESS of `b' somewhere, which would make `assign_parms' keep
   it in its stack home from the start (that is the one mechanism that puts
   a register-passed parameter in memory without reload choosing to) and
   free a register that then changes every later decision. I state that as a
   HYPOTHESIS: the prologue `sw $a1, 0x5C($sp)' is equally consistent with
   an ordinary reload spill, and I have no second witness.

   IT OWNS A JUMP TABLE AND THE ARM ORDER IS DIRECTLY VERIFIED.
   jtbl_s03_80024008, 6 words = 6 cases + 0 pad, dispatch bound 6, 5
   distinct targets (tools/decomp/jtcheck.py). Entries 1 and 2 SHARE an arm
   (both .Ls03_8002439C), so cases 1 and 2 are written as adjacent labels on
   one body. A standalone `tools/cdkgcc -S' of this source emits

     .rdata / .align 3 / $L34: .word $L5, $L17, $L17, $L27, $L29, $L31

   and those labels occur in `.text' in the order $L5 < $L17 < $L27 < $L29 <
   $L31, which is the target's .148 < .39C < .4D4 < .518 < .53C with the
   same shared second entry. **The arm order is correct**; `ovlverify' is
   blind to this and would report the same numbers for a wrong order.

   STRUCTURE (all of it confirmed by the matching words):
   The bank's six-state cutscene director, dispatched on `*(s16 *)(p + 0xA)'
   after an unconditional `*(s16 *)(p + 0x50) -= 1'. Before the switch it
   resolves `s = *(u8 **)p', `o = *(u8 **)(s - 0x14)', `w = s - 0x20' and a
   pair of step deltas out of the facing table
   (`D_8006CCD8[(*(u16 *)(s + 0x2A) >> 9) & 7]' and the same index into
   D_8006CCE8), and in state 1 only it advances `b' by its own velocity half
   and re-grounds its z with func_800BCB04. State 0 asks func_800A05A4 for a
   target slot; with none it walks up to two facing steps looking for a free
   cell with func_800A4688, parks the reached cell in s+0x72/+0x73 and seeds
   `p + 0x50' with `max(|dx|,|dy|) * 12' and b's velocity with
   `(delta << 22) / 12' -- the divisor is 12 and that is PROVEN, not
   guessed: the magic is 0x2AAAAAAB with a post-shift of 1, which is
   `ceil(2^33/12)', where /6 would take the same magic with no post-shift.
   States 1 and 2 share one arm that spawns three 0x201 nodes running
   func_s03_800245B4 with func_s03_80024A1C as their renderer. State 3
   notifies func_8009CE1C, state 4 waits out the timer, state 5 tears the
   actor down through D_8008346C / D_800814A0.

   DONORS: `dngcallee.py' found func_s34_80024020 (9/14 symbols) and
   func_s12_80024020 (9/14); `dngtwin --unit s03' found NOTHING for any stub
   in this unit. func_s12_80024020 (csrc/ovl_dng_s12.c, 524 words) is the
   one to read -- same `s'/`o'/`w' dispatcher skeleton, same six-state
   switch, same state-5 teardown, and its header carries the
   func_8003FD64/func_8004491C spawn idiom. Its frame (-0x50, 10 saves
   including $fp) is one 8-byte step from this one's; func_s34_80024020 owns
   a table at the SAME address with six distinct arms and is 326 words.

   AN ARITY/TYPE ALIAS IS LOAD-BEARING HERE. func_800A4688's real definition
   (csrc/ovl_dungeon_8009F644.c:4167) is
   `s32 func_800A4688(u16 px, u16 py, u16 face, u16 ang, s32 flag)', and
   that signature CANNOT produce this call site: arguments 3 and 4 arrive as
   `sll 16'/`sra 16' and a bare `lh', which are s16 conversions, where a u16
   parameter would emit `andi 0xFFFF'. The park declares
   `s32 func_800A4688h(u16, u16, s16, s16, u32) __asm__("func_800A4688");'
   -- the project's standard remedy, which leaves the landed definition
   alone. */

#include "common.h"

s32 rand(void);
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A3820(s16);
s32 func_800A4688h(u16, u16, s16, s16, u32) __asm__("func_800A4688");
s32 func_800A56E0(s32);
s32 func_800BCB04(s32, s32, s16);
void func_s03_800245B4(u8 *p);
s32 func_s03_80024A1C(u8 *arg0);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
extern u8 D_80083498[];
extern u32 D_800814A0;

void func_s03_80024020(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q;
    u16 x;
    u16 y;
    s32 i;
    s32 k;
    s32 ax;
    s32 ay;
    s32 dx;
    s32 dy;
    s32 r;
    s32 d;
    s32 e;
    s32 dd;
    s32 ee;
    s32 f;

    s = *(u8 **)p;
    o = *(u8 **)(s - 0x14);
    k = (*(u16 *)(s + 0x2A) >> 9) & 7;
    dx = D_8006CCD8[k];
    dy = D_8006CCE8[k];
    w = s - 0x20;
    if (*(s16 *)(p + 0xA) == 1)
    {
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        r = (s16)func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                               *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30);
        if (r < 0x200)
        {
            *(s16 *)((u8 *)b + 0xA) = r;
        }
    }
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        *(s32 *)(s + 0x60) =
            func_800A05A4(s, o[0x24], o[0x25], *(s16 *)(s + 0x2A), (s16)func_800A3820(3));
        if (*(s32 *)(s + 0x60) == 0)
        {
            i = 0;
            ay = 0;
            ax = 0;
            do
            {
                x = (o[0x24] + ax) * 0x40 + 0x20;
                y = (o[0x25] + ay) * 0x40 + 0x20;
                if ((s16)func_800A4688h(x, y, func_800BCB04(x, y, -0x400),
                                        *(s16 *)(s + 0x2A), *(u32 *)(s + 0x60)) != 0)
                {
                    break;
                }
                i++;
                ay += dy;
                ax += dx;
            } while (i < 2);
            s[0x72] = o[0x24] + dx * i;
            s[0x73] = o[0x25] + dy * i;
        }
        else
        {
            q = *(u8 **)(*(u8 **)(s + 0x60) - 0x14);
            s[0x72] = q[0x24];
            s[0x73] = q[0x25];
            if ((*(u16 *)(q + 0x14) & 0x8000) != 0 && (*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s16 *)(p + 0xA) = 3;
                break;
            }
        }
        dd = *(s8 *)(s + 0x72) - o[0x24];
        ee = *(s8 *)(s + 0x73) - o[0x25];
        d = __builtin_abs(dd);
        e = __builtin_abs(ee);
        if (d < e)
        {
            d = e;
        }
        *(s16 *)(p + 0x50) = d * 12;
        b->unk_00 = (o[0x24] * 0x40 + 0x20) << 16;
        b->unk_04 = (o[0x25] * 0x40 + 0x20) << 16;
        b->unk_0C = (dx << 22) / 12;
        b->unk_10 = (dy << 22) / 12;
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
    case 2:
        b->unk_14 += 0x100;
        f = 0;
        if (*(s32 *)(s + 0x60) == 0)
        {
            f = -1;
        }
        for (i = 2; i >= 0; i--)
        {
            n = func_8003FD64(0x201, D_80083498);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_s03_800245B4;
                func_8004491C(n, (u32)func_s03_80024A1C);
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x54) = f;
                *(s16 *)(q + 0x52) = *(u16 *)(p + 0x50);
                *(s16 *)(q + 0x4C) = rand() & 0xFFF;
                *(s16 *)(q + 0x4E) = *(u16 *)((u8 *)b + 0xA);
                *(s32 *)(q + 0x4) = b->unk_00;
                *(s32 *)(q + 0x8) = b->unk_04;
                *(s32 *)(q + 0xC) = 0;
                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x1C) = b->unk_0C;
                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x24) = 0xFFFD0000;
                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x20) = b->unk_10;
                *(s16 *)(q + 0x50) = *(u16 *)((u8 *)b + 0xA);
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(q + 0x56) = *(u16 *)(p + 0xA) - 1;
            }
        }
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0x50) = 5;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            if (*(s32 *)(s + 0x60) == 0)
            {
                *(s16 *)(p + 0xA) = 3;
            }
        }
        break;
    case 3:
        if (*(s32 *)(s + 0x60) != 0)
        {
            func_8009CE1C(*(u8 **)(s + 0x60), 0x10, *(u8 *)(p + 0x9), 4, *(s16 *)(s + 0x2A),
                          (u32)s, 2);
        }
        *(s16 *)(p + 0x50) = 0x10;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 4:
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
