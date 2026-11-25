/* PARKED. func_s22_80024C74, ovl_dng_s22. 1 member (dngfam.py), 421 words,
   so this body is worth 421 words and no replication.

   421 candidate words against 421 target words, 12 ALIGNED DIFFERENCES
   (ovlverify reports the same 12 masked). The function is word-exact
   everywhere except ONE STATEMENT in case 5, and the residue is a register
   assignment, not a missing or extra instruction: both streams have the same
   14 instructions in rows 363..376.

   THE WHOLE RESIDUE, rows 365..376 (candidate left, target right):

       move  a0,v1          srl   $a0, $v1, 1
       srl   v1,v1,1        sll   $v1, $v1, 1
       sll   a0,a0,1        addu  $a0, $a0, $v1
       addu  v1,v1,a0       addu  $a0, $a0, $v0
       addu  v1,v1,v0       addiu $a0, $a0, 0x10
       addiu v1,v1,16       addu  $v0, $a0, $zero
       slti  v0,v1,256      slti  $a0, $a0, 0x100
       bnez  v0,L           bnez  $a0, L
       li    v1,255         addiu $v0, $zero, 0xFF
       sb    v1,40(s1)      sb    $v0, 0x28($s1)
       sb    v1,41(s1)      sb    $v0, 0x29($s1)

   THE MECHANISM, read off `tools/cdkgcc -da' and not guessed. The statement
   is

       i = (p[0x9] >> 1) + p[0x9] * 2 + (func_800A6D30() & 3) + 0x10;
       j = i;
       if (i >= 0x100) { j = 0xFF; }
       r[0x28] = j; r[0x29] = j;

   and the two reads of p[0x9] expand to DIFFERENT RTL SHAPES, so cse cannot
   merge them. From `.cse' (insns 885..889):

       885  (set (reg:QI 413) (mem:QI (plus (reg 80) 9)))
       886  (set (reg:SI 414) (zero_extend (reg:QI 413)))
       887  (set (reg:SI 415) (lshiftrt (reg:SI 414) 1))
       888  (set (reg:SI 416) (zero_extend (subreg:QI (reg:SI 415))))
       889  (set (reg:SI 417) (zero_extend (mem:QI (plus (reg 80) 9))))

   The `>> 1' term is narrowed to QImode, so its load is a bare `movqi' plus a
   separate `zero_extend'; the `* 2' term can overflow a u8 so its load is a
   single `zero_extendqisi2'. Two pseudos (414 and 417) overlap, which costs
   the `move a0,v1' -- and because that copy already exists local-alloc
   coalesces `j' into `i' instead, so the target's `addu $v0, $a0, $zero'
   is absent and the slti lands in $v0 rather than $a0.

   THE TARGET HAS ONE p[0x9] PSEUDO *AND* THE j COPY. Both are needed; every
   spelling that gives one takes away the other, which is why the figure will
   not move below 12 by changing this statement alone. A source local for
   p[0x9] does give the single pseudo -- and then `j = i' coalesces anyway
   (420 words, one short) AND the lost pseudo exchanges $s0/$s1 across the
   WHOLE function, p taking $s1 and b taking $s0: 122 aligned differences.

   EVERY SPELLING MEASURED AND REJECTED, all on this body, all with the rest
   of the function at 0:

     KEPT  (p9>>1) + p9*2 + (rand&3) + 0x10, j=i, if (i>=0x100) j=0xFF
                                                        421 w,  12
     no `j' at all, clamp `i' in place                   421 w,  12
     j = (i < 0x100) ? i : 0xFF                          421 w,  12
     j = i; if (j >= 0x100) j = 0xFF                     421 w,  12
     r[0x29] = r[0x28] = j                               421 w,  12
     (u8) cast on both stores                            421 w,  12
     j declared before i (pseudo birth order)            421 w,  12
     i and j declared first of all the locals            421 w,  12
     i declared u32                                      421 w,  12
     ((u32)p9 >> 1) + p9*2 + ...                         421 w,  12
     ((u16)p9 >> 1) + p9*2 + ...                         421 w,  12
     ((u32)p9 >> 1) + (u32)p9*2 + ...                    421 w,  12
     p9>>1 + p9 + p9 + (rand&3) + 0x10                   421 w,  12
     p[0x9] written as p[0x9] rather than *(u8 *)(p+9)   421 w,  12
     ((p9>>1) + p9*2) + ((rand&3) + 0x10)                421 w,  12
     j = 0xFF; if (i < 0x100) j = i                      421 w,  14
     if (i < 0x100) j = i; else j = 0xFF                 421 w,  14
     p9/2 + p9*2 + ...                                   420 w,  14
     0x10 + (p9>>1) + p9*2 + (rand&3)                    421 w,  13
     (rand&3) + (p9>>1) + p9*2 + 0x10                    421 w,  14
     p9*2 + (p9>>1) + (rand&3) + 0x10                    420 w,  17
     i split into two statements (two sets on i)         421 w, 119
     block-scoped `s32 d = p[0x9]', then d>>1 and d*2    420 w, 122
     same, with no `j'                                   420 w, 122
     same, with the ternary                              420 w, 122
     same, with `u8 d'                                   420 w, 125
     same, accumulating into j and storing i             420 w, 122
     function-scope local for p[0x9]                     420 w, 122
     a local for the `>> 1' term only                    420 w, 122
     a local for the `* 2' term only                     420 w, 123
     i = p9*2; i = (p9>>1) + i + ...                     420 w, 123

   NEXT STEP, named: this is one statement and it is PERMUTER WORK. The
   permuter should be pointed at the `i = ...; j = i; if (i >= 0x100)'
   statement group in `case 5' ONLY -- everything else in the function is
   word-exact -- and the thing it has to find is a form that gives cse ONE
   pseudo for the two p[0x9] reads WITHOUT losing the `j = i' copy. A hand
   sweep provably cannot reach it: of the 31 spellings above, every one that
   fixes the load count breaks the copy and rotates $s0/$s1. This is the
   brief's "a fix can be worse alone and necessary in combination" shape, so
   the permuter must be allowed to vary BOTH the read of p[0x9] and the clamp
   together.

   WHAT IS ALREADY PROVEN, and must not be re-derived:

   * THE TABLE. This function OWNS jtbl_s22_80024008: 8 `.word's, dispatch
     bound `sltiu $v0, $v1, 0x8', so 8 CASES AND NO PAD, and all 8 targets
     DISTINCT. The range-check `beqz' goes to .Ls22_800252E4, which is the
     shared `*(s16 *)(p + 0x14) = 0' epilogue and is NOT one of the 8 entries,
     so THERE IS NO DEFAULT LABEL IN THE TABLE and all 8 are real arms.
     `switch (*(s16 *)(p + 0xA))' with cases 0..7 and no minval subtraction
     reproduces the `lh'/`sltiu 8' exactly; case 0 FALLS THROUGH into case 1.
   * THREE SEPARATE POINTER LOCALS FOR D_80082E80, one per site (case 0, and
     each arm of the case-1 if/else). Worth 28 aligned differences: one shared
     local spans the function, local-alloc stops tying the `high' to its
     `lo_sum', and the pair comes out `lui $v0'/`addiu $t1, $v0' where the
     target has `lui $t0'/`addiu $t0, $t0'. This is b11's measured fact (2)
     reproducing here. Spelling D_80082E80[0x24] directly is 422 w / 49.
   * A POINTER LOCAL FOR D_800814A8 ASSIGNED BETWEEN THE TWO `u + 0x2'/
     `u + 0x6' STORES. Worth 3 words and 9 rows: with the read spelled
     `*(u16 *)(D_800814A8 + 0x88)' inline the `lw' cannot move above the two
     stores and the body is 422 w / 21 with two unfilled load delays; with the
     local assigned BEFORE both stores it is 422 w / 21 as well. Between them
     is 421 w. This is b11's measured fact (3) reproducing here.
   * A POINTER LOCAL `u = (u8 *)&v' FOR THE sp+0x18 SCRATCH VECTOR, assigned
     before the case-1 if/else. Without it every access is `N(sp)' instead of
     `N($s2)' and it is 422 w / 52.
   * `*(u8 *)(p + 0x1C) = 0' GOES AFTER `*(s16 *)(p + 0xE) = ...' in case 0,
     not before: before it is 4 words early in the schedule, 1 row.
   * THE CASE-1 GATE IS AN `|', NOT AN `&'. `(*(u16 *)(n + 0x1E) | 0x2000)'
     emits `lhu'/`ori'/`beqz' -- an always-false test the ROM really has. An
     `&' gives `andi' and is wrong.
   * The octant index is INLINE in the func_800A0818 argument list, not a
     local: as a local `k' the argument evaluation order inverts.
   * `(s16)func_800A45D8w(...)' through the s32-returning alias gives the
     `sll 16'/`bnez' pair; so does `(s16)func_800A5690()'.
   * `--raw' not yet run on this body.

==== THE PERMUTER HAS BEEN SPENT ON THIS PARK, 2026-11-25, dispatcher. NEGATIVE.

50 minutes, 240,111 iterations, 8 jobs, on the statement group this park names,
seeded from the park body itself. `base = 280 -> best = 125' on the permuter's
own score -- and measured by hand the winner is WORSE than this body:

    this park's body                      421 words / 12 aligned
    the permuter's best (output-125-1)     420 words / 14 aligned

One word short and two rows worse. I verified it twice: by reconstructing the
change in a spliced unit and by compiling the permuter's own
`output-125-1/source.c' directly. Both give 420/14.

THE SHAPE IT FOUND IS WORTH KEEPING EVEN SO, because it is one no agent tried
and it answers the park's own objection. The park records that a source local
for `p[0x9]' gives the single pseudo and then costs 122 rows because `j = i'
coalesces and the lost pseudo exchanges $s0/$s1 across the function. The
permuter's answer is an EMBEDDED ASSIGNMENT:

    i = ((nv = *(u8 *)(p + 0x9)) >> 1) + nv * 2 + (func_800A6D30() & 3) + 0x10;

which confines the new pseudo to the expression instead of making it a
statement-level local. That is a real distinction and it is not the 122-row
shape -- it lands at 420/14 rather than 420/122. **So the embedded form is the
right family and it is still one word short**, which says the second read is
not what the target shares; something else supplies the word.

WHAT THAT LEAVES. The park's 31 measured spellings plus this search have now
covered the two reads and the clamp as a group. The untried direction is the
one the park names last: the `j = i` copy and the `i >= 0x100` clamp are the
other half, and no run so far has varied the CLAMP while holding an embedded
read. If anyone spends more budget here, that is the pair -- and rank by
aligned differences and word count, not by the permuter's score, which
improved by a factor of two while the body got worse.

DECLARATIONS
typedef struct UnkDngS22LineSP
{
    u8 pad_00[0x18];
    u8 *unk_18;
    u8 pad_1C[0x48];
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u8 pad_6A[0x2];
    u16 unk_6C;
    u16 unk_6E;
    u16 unk_70;
    u8 pad_72[0x2];
    u16 unk_74;
    u16 unk_76;
    u16 unk_78;
    u8 pad_7A[0xA];
    s32 unk_84;
    s32 unk_88;
    u8 pad_8C[0x28];
    s32 unk_B4;
    u8 pad_B8[0x20];
    u16 unk_D8;
    u16 unk_DA;
    u16 unk_DC;
    u16 unk_DE;
    u16 unk_E0;
    u16 unk_E2;
} UnkDngS22LineSP;
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
UnkNode *func_s22_800240F8(u8 *, UnkVec *, s16);
UnkNode *func_s22_800242F0(u8 *, UnkVec *, s16);
void func_s22_80024A44(u8 *, UnkVec *, s16);
extern u8 D_80082E80[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u32 D_800814A8;
extern u32 D_800814A0;
extern s32 D_8008346C;
s32 func_800A0818(s16, s16, s16, s16, u16 *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s16 func_800BCAD0(u8 *);
s16 func_8009A350(s16, s16, s32, u16 *);
s32 func_800A45D8w(u16, u16, s32) __asm__("func_800A45D8");
s32 func_800A5690(void);
s32 func_800A56E0(s32);
void func_800419EC(u16, u32);
u8 *func_800D1C70(s16, s16, s16, s16);
void func_80042640_u(u8 *, s16) __asm__("func_80042640");
u16 func_800A6D30(void);
END DECLARATIONS
*/
void func_s22_80024C74(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    u8 *n;
    u8 *m;
    u8 *r;
    u8 *t;
    u8 *t2;
    u8 *t3;
    u8 *u;
    UnkVec v;
    u16 out[4];
    u16 w[2];
    u16 z;
    s32 i;
    s32 j;
    u8 *y;

    q = *(u8 **)p;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        t = D_80082E80;
        *(s16 *)(q + 0x2A) =
            func_800A0818(*(u8 *)(*(u8 **)(q - 0x14) + 0x24), *(u8 *)(*(u8 **)(q - 0x14) + 0x25),
                          t[0x24] + *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7) * 2 + (s32)D_8006CCD8),
                          t[0x25] + *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7) * 2 + (s32)D_8006CCE8), w);
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(q + 0x2A) >> 9) & 7;
        *(u8 *)(p + 0x1C) = 0;
        *(u32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        n = q - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(n + 0xC) + 0x8), (UnkSlot *)*(u8 **)(n + 0xC), (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        m = *(u8 **)(n + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(m + 0x6);
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        u = (u8 *)&v;
        if ((*(u16 *)(n + 0x1E) | 0x2000) != 0)
        {
            t2 = D_80082E80;
            *(s16 *)(p + 0x16) =
                t2[0x24] + *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7) * 2 + (s32)D_8006CCD8);
            *(s16 *)(p + 0x18) =
                t2[0x25] + *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7) * 2 + (s32)D_8006CCE8);
        }
        else
        {
            t3 = D_80082E80;
            *(s16 *)(p + 0x16) = t3[0x24] + *(u16 *)(*(s16 *)(p + 0xE) * 2 + (s32)D_8006CCD8);
            *(s16 *)(p + 0x18) = t3[0x25] + *(u16 *)(*(s16 *)(p + 0xE) * 2 + (s32)D_8006CCE8);
        }
        *(s16 *)(u + 0x2) = *(s16 *)(p + 0x16) * 64 + 0x20;
        y = (u8 *)D_800814A8;
        *(s16 *)(u + 0x6) = *(s16 *)(p + 0x18) * 64 + 0x20;
        *(s16 *)(u + 0xA) = *(u16 *)(y + 0x88) - 0x20;
        *(s16 *)(u + 0xA) = func_800BCAD0(u);
        *(s16 *)(p + 0x1A) = *(s16 *)(u + 0xA);
        func_8009A350((s16)(*(u16 *)(p + 0x16) - 1), *(s16 *)(p + 0x18), 0, &w[1]);
        if ((w[1] & 0x3300) != 0 || *(s16 *)(u + 0xA) >= 0x201 ||
            (s16)func_800A45D8w(*(u16 *)(u + 0x2), *(u16 *)(u + 0x6), *(s16 *)(u + 0xA)) != 0 ||
            (s16)func_800A5690() == 0)
        {
            *(s16 *)(p + 0xA) = 7;
        }
        else
        {
            if ((*(u16 *)(n + 0x1E) | 0x2000) != 0)
            {
                *(u8 *)(p + 0x1C) = 1;
            }
            else
            {
                *(u8 *)(p + 0x1C) = 2;
            }
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        out[0] = abs((s16)*(u16 *)(u + 0x2) - *(s16 *)((u8 *)b + 0x2));
        out[1] = abs((s16)*(u16 *)(u + 0x6) - *(s16 *)((u8 *)b + 0x6));
        out[2] = abs(*(s16 *)(u + 0xA) - *(s16 *)((u8 *)b + 0xA));
        *(s16 *)(p + 0x12) = 0xC;
        b->unk_0C = (*(s32 *)(u + 0x0) - b->unk_00) / *(s16 *)(p + 0x12);
        b->unk_10 = (*(s32 *)(u + 0x4) - b->unk_04) / *(s16 *)(p + 0x12);
        b->unk_14 = (*(s32 *)(u + 0x8) - b->unk_08) / *(s16 *)(p + 0x12);
        func_s22_800240F8(p, b, *(s16 *)(p + 0x12));
        func_s22_800240F8(p, b, *(s16 *)(p + 0x12));
        func_s22_800240F8(p, b, *(s16 *)(p + 0x12));
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_s22_800242F0(p, b, 0xC);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 8)
        {
            func_800A56E0(0x300);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 5)
        {
            func_s22_80024A44(p, b, 0x10);
            func_800419EC(8, 0x24);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x10) >= 0x11)
        {
            if (*(u8 *)(p + 0x1C) != 0)
            {
                r = func_800D1C70(*(u8 *)(p + 0x1C), *(s16 *)(p + 0x16), *(s16 *)(p + 0x18), *(s16 *)(p + 0x1A));
                func_80042640_u(r, 0x33);
                i = (*(u8 *)(p + 0x9) >> 1) + *(u8 *)(p + 0x9) * 2 + (func_800A6D30() & 3) + 0x10;
                j = i;
                if (i >= 0x100)
                {
                    j = 0xFF;
                }
                r[0x28] = j;
                r[0x29] = j;
            }
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 7:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0xA) = 6;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
