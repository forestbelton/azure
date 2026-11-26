/* func_s66_8002401C -- ovl_dng_s66 -- PARK, agent-h237, batch 35.

   174 words target, 174 words candidate, 22 ALIGNED DIFFERENCES.
   dngfam: 1 member, 174 words -- no replication, this body lands once.

   THIS FUNCTION OWNS jtbl_s66_80024008 AND THE ARM ORDER IS SETTLED.
   jtcheck.py: 5 words = 5 cases + 0 pad, dispatch bound 5, 5 distinct
   targets. Entries, in table order and strictly ascending by address:

       case 0 -> .Ls66_8002406C      case 3 -> .Ls66_8002414C
       case 1 -> .Ls66_800240A0      case 4 -> .Ls66_80024218
       case 2 -> .Ls66_80024124

   Settled TWO ways, because neither ovlverify nor aligndiff can see arm
   order. (a) PER ARM: all five arms carry a distinguishing non-branch
   instruction (case 0 the D_s66_80024A64 store, case 1 the func_80053EF0 /
   func_800A56E0 cue pair, case 2 the 0x10 reload, case 3 the 0x10000000 or,
   case 4 the D_80083460 stores), so no two are word-identical after
   cross-joining and none is in the invisible masked-jump class.
   (b) INDEPENDENTLY, a standalone `tools/cdkgcc -O2 -G0 -S' of the unit
   emits the table as `.word $L3, $L4, $L8, $L10, $L17' with those five
   labels appearing in the text in that same order and NO trailing pad word.
   Writing the arms ascending 0..4 is therefore correct and is what this body
   does. The odd case count is why there is no pad.

   WHAT IS ALREADY RIGHT: every opcode, every immediate, every displacement
   and the whole control flow. The word count is exact. The residue is a
   single REGISTER PERMUTATION confined to case 3 plus the one instruction
   that materialises `g'. Twenty of the 22 rows are inside case 3 and the
   remaining two are row 9 and its wake.

   THE RESIDUE, instruction by instruction (candidate | target):

     row   9   addiu a1,v0,%lo(D_80083160)  | addiu $a2, $v0, %lo(...)
     rows 79-94   (case 3, first half)
                 q   in $a0  | $a1
                 g   in $a1  | $a2
                 the q-0x14 pointer in $a0 (reusing q) | $a2 (reusing g)
                 the 0x80FF80/0 colour in $v1 | $a0
     rows 110-121 (case 3, second half)
                 q2  in $a2  | $a1
                 0xEFFFFFFF  in $a0 | $v0
                 0x808080    in $a1 | $a0
                 the 0x1C value in $v0 | $v1
                 the q2-0x14 pointer in $v1 | $a2

   So the candidate allocates, in priority order, colour($v1), q($a0),
   g($a1); the target allocates colour($a0), q($a1), g($a2). The target's
   $v0 and $v1 are left to the block-local constants. Nothing is moved and no
   word is gained or lost.

   CAUSE: UNKNOWN. STATED AS A HYPOTHESIS ONLY. The candidate's first global
   allocno finds $v1 free where the target's finds only $a0, which means two
   more hard registers are reserved across the colour pseudo's range in the
   target than in the candidate -- most plausibly by local-alloc, which runs
   before global-alloc and takes the block-local constants. I did not confirm
   that from a dump and the next agent should not assume it.

   WHAT IS RULED OUT, MEASURED -- this is the useful half of this park:

     allocation PRIORITY is not the lever. Every permutation of the local
     declaration order measures IDENTICALLY at 22, and so does merging or
     splitting the two case-3 pointers. Per docs/matching-levers.md that
     means the cause is in the arithmetic or the statement order.

   EVERY SPELLING MEASURED, with its score. All figures are aligned rows from
   aligndiff at 174 words unless stated:

     the body as parked                                          174 /  22
     `n = (u8 *)D_800814A8' as a local in case 0, with the
       +0xF4 store through it                                    174 /  26
     case 3 written with ONE pointer local for both halves
       instead of q and q2                                       174 /  22
     case 3 with explicit `r'/`r2' locals for the two
       *(u8 **)(q - 0x14) reads                                  174 /  22
     local declaration order permuted (q, q2, n, h, g, arr)      174 /  22
     case 3's stores as ARRAY REFS -- ((s32 *)q)[7] |= ...,
       ((s32 *)(((u8 **)q)[-5]))[3] = ..., ((u16 *)g)[2] & 1     174 /  22
     the 0x80FF80/0 choice as `if'/`else' with a store in
       each arm instead of the conditional expression            178 /  35

   SO: the `?:' IS LOAD-BEARING HERE and the two-arm `if'/`else' is FOUR
   WORDS LONG. That contradicts the general guidance that two if/else stores
   cost nothing because jump2 cross-joins them -- it does not cross-join
   here, because each arm's store goes through the same base but the arms are
   reached from a gate whose own delay slot the target fills with the zero
   set. jump.c's hoist of one constant set above the comparison is exactly
   what the target shows, and only the conditional expression produces it.

   AND MEM_IN_STRUCT_P IS NOT MEASURABLE ON THIS BODY. Rewriting all of case
   3's loads and stores as array refs gives the IDENTICAL 22 rows, row for
   row, so this body says nothing about that lever in either direction.
   Recorded as NOT MEASURABLE, not as FREE.

   FACTS CARRIED FROM THE DONOR, csrc/ovl_dng_s55.c's landed
   func_s55_80024078 (dngcallee 6 of 11 symbols, a clean gap to the 5/11
   tier; dngtwin finds nothing at 0.70). Its fact (1) -- the state bumps must
   be u16 ARRAY REFS -- is used here for the +0xA and +0x16 bumps and is
   assumed, not re-priced. Its cue-gate fact HOLDS: this body's case 1 wants
   `if (func_80053EF0(4) != 2) { func_800A56E0(0x300); } else
   { func_800A56E0(0x4300); }', which is what gives the target's `beq' with
   the 0x4300 in the delay slot; that spelling is in the parked body.

   DECLARATIONS
   UnkNode *func_8003FD64(s32, void *);
   extern u32 D_800814A8;
   extern s16 D_80024A70;
   extern u32 D_800814A0;
   extern u16 D_80082E94;
   extern u8 D_s66_80024A64[];
   extern u8 D_80024A74[];
   void func_800B835C(u8 *, s32 *, s32, s32);
   s32 func_80053EF0(s32);
   s32 func_800A56E0(s32);
   void func_s66_800246F4(u8 *p, u8 *b);
   extern s16 D_80083228;
   END DECLARATIONS
*/
void func_s66_8002401C(u8 *p)
{
    u8 *g;
    u8 *n;
    u8 *q;
    u8 *q2;
    u8 *h;
    s32 arr[2];

    ((u16 *)p)[0xB] = ((u16 *)p)[0xB] + 1;
    g = (u8 *)&D_80083160;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)((u8 *)D_800814A8 + 0xF4) = (u32)D_s66_80024A64;
        func_s66_800246F4(*(u8 **)(p + 0x0) - 0x20, *(u8 **)(p + 0x4));
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) != 0)
        {
            n = (u8 *)D_800814A8;
            *(s16 *)(p + 0x18) = 0x12;
            *(s16 *)(n + 0xA6) -= 1;
            if (func_80053EF0(4) != 2)
            {
                func_800A56E0(0x300);
            }
            else
            {
                func_800A56E0(0x4300);
            }
            *(u8 *)((u8 *)D_800814A8 + 0xA8) = p[0x8];
            *(s16 *)(p + 0x14) = *(u16 *)((u8 *)D_800814A8 + 0x2A);
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 2:
        if ((*(s16 *)(p + 0x18) -= 1) <= 0)
        {
            *(s16 *)(p + 0x18) = 0x10;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 3:
        q = *(u8 **)((u8 *)D_800814A8 + 0x60);
        if (q != NULL)
        {
            *(s32 *)(q + 0x1C) |= 0x10000000;
            *(s32 *)(*(u8 **)(q - 0x14) + 0xC) = (*(u16 *)(g + 0x4) & 1) ? 0x80FF80 : 0;
        }
        if ((D_80082E94 & 0x8000) != 0 || (*(s16 *)(p + 0x18) -= 1) < 0)
        {
            q2 = *(u8 **)((u8 *)D_800814A8 + 0x60);
            if (q2 != NULL)
            {
                *(s32 *)(q2 + 0x1C) &= 0xEFFFFFFF;
                *(s32 *)(*(u8 **)(q2 - 0x14) + 0xC) = 0x808080;
            }
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 4:
        if (D_80024A70 == 0)
        {
            h = (u8 *)&D_80083460;
            *(s32 *)(h + 0xC) = 0;
            *(s16 *)(h + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if (*(s16 *)(p + 0xA) >= 2)
    {
        arr[0] = 0x1000360;
        arr[1] = 0x200020;
        func_800B835C(D_80024A74, arr, 1, 0);
        D_80024A74[8] += 0x10;
    }
    D_80024A70 = 0;
}
