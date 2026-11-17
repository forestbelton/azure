/* PARK: func_800B835C, 98 of 98 words, 8 aligned differences.
   ovl_dungeon.  Measured 2026-11-16, re-measured and re-diagnosed 2026-11-16
   by j61 (residue unchanged, diagnosis replaced).  The three-primitive tint
   overlay: a draw-area primitive, func_800B84E4's output, an optional
   0x60000000 rectangle, and a second draw-area primitive, all hung on the
   +0x8B0 OT row.

   NEW DECLARATION NEEDED (func_800B84E4 is defined below this function):

       void func_800B84E4(s16 *, u8 *, u8 *, s32);

   The prototype `void func_800B835C(u8 *, s32 *, s32, s32);` at line 518 is
   already pinned by four landed call sites -- do not change it.  The `u16`
   narrowings on the third and fourth parameters are written out in the body.

   `csrc/ovl_dungeon.c`'s func_8009DF0C (line ~11525) is the line-for-line
   template: same +0x8D0 cursor, same +0x8B0 OT row, same `lean` test, same
   0x60000000 / `u[3] = 3` rectangle.  Read its comment first.

   ------------------------------------------------------------------ residue

   TWO PARAMETER REGISTERS, SWAPPED, AND NOTHING ELSE.  `p` is in $s6 here and
   $s7 in the target; `c` is in $s7 here and $s6 in the target.  That is all 8
   rows -- the two prologue saves, the two parameter copies, the `addu $a1` at
   the func_800B84E4 call and the `andi` before the gate.  Every other word in
   the function, including the whole frame, matches.

   ------------------------------------- the allocator numbers, read not guessed

   The earlier header said "both measure 44 and the tie falls through to
   `return v1 - v2`".  THAT IS WRONG AND IT COST THE NEXT AGENT ITS BUDGET.
   There is no tie.  From `tools/decomp/dump.sh` on a one-function file,
   the `.lreg` flow dump of THIS body:

       Register 80 (p)  used 2 times across 70 insns; crosses 2 calls
       Register 82 (c)  used 2 times across 78 insns; crosses 3 calls
       Register 89 (dd) used 2 times across 30 insns; crosses 2 calls
       ;; 12 regs to allocate: 92 91 86 88 81 87 131 93 89 84 80 82

   `allocno_compare` = floor_log2(refs)*refs/live_length, in DOUBLE, so
   p scores 2/70 = 0.0286 and c scores 2/78 = 0.0256: p sorts first by a
   clear margin and takes $s6, c takes $s7.  The printed order above
   reproduces every one of the twelve priorities exactly, so the model is
   confirmed, not assumed.  THE TARGET NEEDS len(c) < len(p), i.e. c at 69
   or less, or p at 79 or more.  Nine insns, not a tie-break.

   -------------------------------- why no source spelling reaches it, measured

   `reg_live_length` is computed ONCE, by `flow_analysis` (toplev.c:3463,
   before combine), and is then re-counted after sched1: this function reads
   45/47 in `.flow`, 35/39 in `.shorten`, 70/78 in `.lreg` (exactly 2x the
   `.shorten` figures).  The ratio is what matters and it never inverts, for
   a structural reason:

   * The four parameter copies sit BEFORE NOTE_INSN_FUNCTION_BEG and are
     outside sched1's region -- `sched.py --sched1` prints the block as
     "from 4 to 109" and lists no priority for insns 4, 6, 8.  So p's def is
     insn 4 and c's is insn 8, two insns apart, and nothing moves them.
   * p dies at `(set (reg a1) p)`, the second argument move of the
     func_800B84E4 call.  c dies at the `zero_extend` of the gate, which is
     FOUR insns later: the a2 move, the a3 `andi`, the call, the extend.
     The extend carries a LOG_LINK on the call insn, so sched1 cannot hoist
     it above the call either.
   * Two insns earlier at the head against four insns later at the tail is
     why c is longer, and there is no source edit that adds insns between
     two adjacent parameter copies or removes an argument move.

   So c can only get shorter by having its narrowing moved BEFORE the call --
   and every spelling that does that also lets combine fold the `andi` into
   the parameter copy, which costs the word the target spends on the separate
   `addu`.  That is the whole trap, and it is why every hoist below is 97.

   ------------------------------------------- what j61 measured and rejected

   Base here is 98 words / 8 differing.

   | spelling                                               | words | diff |
   | `cc = c;` plain, at EVERY one of 13 positions           |  98   |  8   |
   |   (before `dd`, it is 10; everywhere else 8 -- and the  |       |      |
   |    `.lreg` numbers are byte-identical to the base, so   |       |      |
   |    the copy is propagated away before flow even runs)   |       |      |
   | `s16 cc; cc = c;` + `if ((u16)cc != 0)`, 13 positions   |  98   |  8   |
   |   (same: 70/78 unchanged, HImode does not stop it)      |       |      |
   | `cc = (u16)c;` first, `dd = d;` plain                   |  97   |  5   |
   | `cc = (u16)c;` + `dd = (u16)d;`, cc first               |  97   |  5   |
   | `cc = c;` first + `dd = d;` second                      |  98   | 10   |
   | `cc = c;` first, no `dd` at all, call takes `(u16)d`    |  98   | 10   |
   | `cc = (u16)c;` at positions 2..7 (before the 1st call)  |  97   | 7-8  |
   | `cc = (u16)c;` after SetDrawArea / AddPrim              |  97   | 20+  |
   | `cc = (u16)c;` just before the call (pos 12, 13)        | 101   | 17   |
   | tail duplicated per arm, `s32 lean`, both narrowings    |  97   |  8   |
   |   hoisted (the 2026-11-15 shape, reconstructed)         |       |      |

   THE TWO 97/5 ROWS ARE THE INTERESTING ONES AND THE NEXT AGENT SHOULD START
   THERE.  With both narrowings hoisted, EVERY REGISTER IS CORRECT -- $s7 = p,
   $s6 = c, $s5 = d -- and all five differing rows are the one mechanism:

       cand  andi $s6, $a2, 0xFFFF     target  addu $s6, $a2, $zero
       cand  andi $s5, $a3, 0xFFFF     target  addu $s5, $a3, $zero
       cand  move $a3, $s5             target  andi $a3, $s5, 0xFFFF
       cand  (nothing)                 target  andi $v0, $s6, 0xFFFF
       cand  beqz $s6                  target  beqz $v0

   combine folds each hoisted `zero_extend` back into the parameter copy
   because the parameter is dead after it, and the function comes out one
   word short.  A spelling that keeps a hoisted narrowing from being folded
   into the parameter copy -- without giving the parameter a second reference
   that cse will then merge -- IS THE WHOLE LANDING.  Nothing I tried does it:
   a second `(u16)c` is CSEd into the first, a plain copy is propagated away,
   and a copy placed after a call (where `a2` is clobbered, so combine cannot
   reach back) moves the `andi` 20 words away from where the target has it.

   An alternative route, equally unfound: give `c` a THIRD reference that
   survives cse but emits nothing.  floor_log2(3)*3/78 = 0.0385 beats p's
   0.0286 outright, which would also leave the parameter copy intact because
   combine could no longer fold.  I found no natural source form for it.

   -------------------------------------------------- correction to the record

   The earlier header's closing line, "This is `.greg` work, not the
   permuter", should be read the other way round now: the `.greg` work HAS
   been done and it says no source-level lever exists on this body shape.
   docs/matching-levers.md's "THE PERMUTER DOES NOT CRACK A SMALL PURE-
   ALLOCATION RESIDUE" still applies to the 98/8 body, but the 97/5 hoisted
   body is a DIFFERENT residue -- a missing instruction, not a misnumbered
   register -- and that is the permuter's win column.  Seed it with the
   hoisted body below the main one if a run is spent here.

   -------------------------------------------------------------- what paid

   99/19 -> 98/8, in order, and all three are worth knowing:

   * `s16 lean;` and NOT `s32 lean;`.  99 -> 98 words and 19 -> 18.  As an
     `s32` the boolean is computed into $s0 before the two calls and then
     COPIED to $s1 -- a `move` the target does not have.  The narrower local
     lands where it is wanted first time.  `u8 lean` is 99/18, so it is the
     s32 that is wrong rather than the s16 that is right.
   * A POINTER LOCAL `g = &D_80083160;` for the tail block only.  18 -> 13.
     The target holds `&D_80083160` in $s1 from word 30 to word 75 and reads
     the tail's base with a one-word `lw $v1, 0x0($s1)`; spelled
     `D_80083160.unk_00` the tail rebuilds the address (`lui` + `lw`) and the
     extra word has to come out of somewhere else.  Keeping the address live
     also stops the MIDDLE block rebuilding its own `%hi`, which is the `nop`
     the target has at word 52.  NOTE the ordering: this only pays once `lean`
     is `s16` -- added to the `s32 lean` body it measures 101 words.  Reading
     `g->unk_00` twice in the tail and reading it once into a temp are
     byte-identical.
   * A SEPARATE LOCAL for the middle block's halfword temp.  13 -> 8.  Sharing
     `t` with the one in the first block makes the middle block read into $v1
     where the target uses $v0, and that renames three rows.
   * `dd = d;` as a PLAIN copy placed immediately before `g = &D_80083160;`.
     Worth 2.  The narrowing form `dd = (u16)d;` is 10 -- the `andi` has to
     stay at the call site, where the target has it.  Moving the copy anywhere
     later measures 13: after `g`, after `r`, after the first `AddPrim`, after
     `w[1] = t`, all 13.

     j61's addendum, and it explains the 2 words: `dd = d;` in that position
     is the ONLY body copy, so combine folds the parameter's own prologue
     insn into it and the surviving pseudo is defined INSIDE the schedulable
     region.  sched1 then sinks it twenty insns (`.lreg` 30 against p's 70),
     which is what keeps d ahead of both p and c in `allocno_order`.  Adding
     a second body copy destroys it: with `cc = c;` first, dd's length goes
     30 -> 72 and the function is 10 differing.  ONLY THE FIRST BODY COPY GETS
     THIS, so the device cannot be used twice.

   Also measured, no better: hoisting BOTH narrowings into locals (`cc` before
   `dd` and `dd` before `cc`) is 97 words -- one SHORT -- because the hoisted
   `andi` replaces the parameter copy and the gate's own `andi` folds away;
   the target keeps both.  Declaring `w[2]` first, last, or between the
   pointers is byte-identical. */

void func_800B835C(u8 *p, s32 *b, s32 c, s32 d)
{
    Unk80083160 *g;
    u8 *bp;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s32 dd;
    s16 lean;
    s32 t;
    s32 t2;

    dd = d;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(r, q);
    w[0] = *(u16 *)b + ((s16) * ((u16 *)b + 2) >> 1);
    t = *((s16 *)b + 1) + ((s16) * ((u16 *)b + 3) >> 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    func_800B84E4(w, p, r, (u16)dd);
    if ((u16)c != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)b;
        t2 = *((s16 *)b + 1);
        if (lean)
        {
            t2 -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t2;
        *(u32 *)(u + 0xC) = b[1];
        AddPrim(r, u);
    }
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, b);
    AddPrim(r, q);
}
