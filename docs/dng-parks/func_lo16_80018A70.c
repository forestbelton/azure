/* func_lo16_80018A70 -- ovl_dng_lo16, 260 target words.
   PARKED by agent h181, batch 35, 2026-11-25.  260 WORDS -- EXACT -- and 12
   ALIGNED DIFFERENCES.  Every register name in the body is right, every
   instruction is present, and the twelve rows are ONE SCHEDULING WINDOW of
   thirteen insns in the third arm of the opening gate.  Words 0..56 and
   69..259 are identical.

   WHAT IT IS: the dungeon entry.  It picks or re-seeds the RNG state at the
   constant address 0x80013710 according to the two low bits of its +0x4 flag
   word, publishes the seed to D_80081468, reseeds, clears the four
   D_800E3CD8 marks, clamps the floor counter at 0x80010234 to 0x63, and then
   either replays a canned scenario out of the D_8001F62C five-byte table or
   generates one and spins func_lo16_80019AF8 until the map is accepted.
   No twin: dngtwin and xsibs were both run over the whole unit and neither
   finds a sibling for it.

   THE RESIDUE, exactly.  Target words 57..68 against candidate 57..68:

       target                        candidate
       57 lui   $a0, %hi(D_80083780) 57 sll   $v0, $v0, 16
       58 addiu $a0, %lo(D_80083780) 58 or    $s0, $s0, $v0
       59 lui   $a1, %hi(D_80081468) 59 lui   $a0, %hi(D_80083780)
       60 sll   $v0, $v0, 16         60 lhu   $v0, 0x4($s2)
       61 sh    $zero, 0x8($s1)      61 addiu $a0, %lo(D_80083780)
       62 sh    $zero, 0x6($s1)      62..65 the four zero stores
       63 sb    $zero, 0x11($s1)     66 xor   $s0, $s0, $v0
       64 sb    $zero, 0x10($s1)     67 lui   $v0, %hi(D_80081468)
       65 lhu   $v1, 0x4($s2)        68 sw    $s0, %lo(D_80081468)($v0)
       66 or    $s0, $s0, $v0
       67 xor   $s0, $s0, $v1
       68 sw    $s0, %lo(D_80081468)($a1)

   The target schedules the two address `lui's FIRST and keeps the `sll', the
   `lhu' and the `or' LAST; the candidate does the reverse.  The diagnostic is
   that in the target the `lhu $v1, 0x4($s2)' sits AFTER the four stores
   through $s1 even though its dependence chain is three deep against the
   stores' one, which means sched1 has a MEMORY DEPENDENCE between them that
   the candidate does not have -- in the candidate gcc proves the `$s1 + K'
   stores disjoint from the `$s2 + 4' load and hoists it.  Everything else
   follows: with the load late, $v1 is free for it and the `or' can stay in
   $s0; with the load early, $v0 is reused and the `or' is forced ahead of it.
   THE SOURCE ORDER DOES NOT MOVE IT.  Measured, all 12 aligned differences:
   the four zero stores before / between / after the `t |= u' and
   `t ^= ...' statements (six permutations), the two publishing stores in
   either order, `if (1)' around the stores and around the `or',
   `t |= rand() << 16' with no `u' temp (13), `u' holding the unshifted
   result, `t' and `u' as `s32' or `u32' in all four combinations, the xor
   operand spelled `*(u16 *)(b + 4)' with `b' a `u8 *', a `u16 *' or an
   `Unk80083160 *' (all 12), `D_80083160.unk_04' (259 words / 18),
   `D_80083164[0]' (258 / 19), and `*(s32 *)&D_80081468h[0]' for the publish
   (12).  Reversing the four stores' order is 18.  Putting the `bzero' before
   the `func_8004D0C8' call is 264 / 32.

   AND IT IS *NOT* A MEMORY-DEPENDENCE QUESTION, NOR A PERMUTER JOB.  My first
   reading of it was that the target had an aliasing dependence the candidate
   lacked; the dispatcher's rule corrected that, and a second sweep confirms
   it.  Distinct constant offsets off one base provably do not overlap, so no
   aliasing permission is needed by either side, and making the stores
   MEM_IN_STRUCT_P -- `r' retyped as a struct pointer with named fields for
   every one of its seven offsets -- is 260 words / 19, strictly WORSE.  Both
   13-insn windows contain the SAME MULTISET of instructions; only the order
   differs, and what is out of place is the position of an outgoing-ARGUMENT
   SETUP (`lui'/`addiu %lo(D_80083780)' for the call in the NEXT block) and of
   a `lui'/`%lo' SPLIT (`lui $a1, %hi(D_80081468)', nine insns from its `sw'),
   which are the two classes the dispatcher's refined rule names as neither
   statement-position nor permuter-reachable.
   THE FLAT SWEEP IS THE POSITIVE RESULT.  Everything the rule says to try
   after a flat permutation sweep was tried and is also flat at 12:
       a local for the stored zero (`zz = 0' and four stores of `zz')     12
       a SECOND pointer local used only for arm 3's four stores          12
       `f' declared `s16' instead of `u16'                               12
       a `u16' local for the xor operand, read after the stores          12
       the same local declared `s32'                                     12
       the same local read BEFORE the stores                             12
       `t' and `u' as `s32'/`u32' in all four combinations               12
       `w' read into a local at either side of the store block           12
   and the two that move it both move it the wrong way: `r' as a struct
   pointer 19, `D_80083160.unk_04' 259 words / 18.
   NEXT AGENT: do not re-sweep arm 3.  If this one is worth more effort the
   question is why sched1 ranks an argument setup and a `high' above a
   four-deep chain in this block at all -- a `-da' `.sched' dump on both sides
   would settle it, and nothing in the source reaches it.

   MEASURED AND LOAD-BEARING (each with the cost of the alternative):

   1. 0x80013710 IS A POINTER LOCAL `r = (u8 *)0x80013710', materialised
      `lui'/`ori' because the low half is nonzero.  Every field of the RNG
      record is a displacement off it: +0x00 word, +0x04 u16 flags, +0x06,
      +0x08, +0x0A u16, +0x10, +0x11 bytes.  Same rule as
      func_lo16_800163F4's `b = (u8 *)0x80010000'.  0x80010234 and 0x80012090
      in the SECOND half of the body share ONE pointer local
      `b2 = (u8 *)0x80010000' -- the listing uses one `$s1' base for both --
      while the two early 0x80010234 reads are BARE constant-address LOADS
      (`lui $a1' / `lhu $a1, %lo($a1)', the same register twice), which is
      what a load can do and a store cannot.
   2. `*(u16 *)(r + 4) &= 0xFFFC' IN ARM 1 IS A READ-MODIFY-WRITE, not a
      store of the gate's cached value: the target re-reads `lhu $v1,
      0x4($s1)' at word 30 even though the gate's read is still live in $v1,
      because the three intervening stores through $s1 invalidate cse.  The
      gate itself reads the flag word ONCE into `f' and both tests use it.
   3. ARM 1's THREE ZERO STORES COME AFTER THE func_800A6D30 CALL and the
      sum is a TEMP: `n = *(s32 *)r + func_800A6D30();' then the stores then
      `*(s32 *)r = n;'.  The target's `lw $v1, 0x0($s1)' is at word 20, after
      the `jal', and the `sw' at 27, after the stores -- which only the temp
      gives.  With the stores before the call it is 258 words / 34.
   4. `n' MUST BE A SEPARATE LOCAL FROM ARM 3's `t'.  Sharing one variable
      across the two arms makes it one pseudo whose priority beats `r' and
      the two exchange $s0 and $s1 across the whole body: 260 words / 30.
      Separate, `r' is 19 refs over 51 insns (4 * 19 / 51 = 1.49) and `t' is
      the one that takes $s0.
   5. `t |= u;' AND `t ^= *(u16 *)(b + 4);' MUST BE TWO STATEMENTS.  Written
      `t = (t | u) ^ *(u16 *)(b + 4);' the whole arm's allocation rotates:
      260 words / 30, the same figure as sharing the temp.  A
      `do { } while (0)' reference bump on that one statement is 261 / 35 and
      one around the two publishing stores is 260 / 32.
   6. THE SECOND `% 24' IS SIGNED AND THE FIRST IS UNSIGNED, and that is a
      c-typeck rule, not a range analysis.  `func_800A6D30() % 24' is
      SHORTENED by build_binary_op -- both operands fit in `u16', so the
      result type is unsigned and gcc emits the 0xAAAAAAAB `multu' / `srl 4'
      sequence, which is what the FIRST arm's `(u8)(D_lo16_8001F584 % 24)'
      correctly wants.  The second arm's target is the SIGNED 0x2AAAAAAB
      `mult' / `sra 2' / `subu' sequence, and the only way to get it is to
      assign the call result to an `s32' FIRST: `j = func_800A6D30();' then
      `j = j % 24;'.  As one expression it is the unsigned sequence and the
      body is 258 words / 34.  The `andi $v0, $v0, 0xFFFF' is the `u16'
      return widening in both arms and comes free.
   7. THE FIRST ARM'S 24-MODULO IS WRITTEN TWICE and the value is RE-READ:
      the target has two `lbu %lo(D_lo16_8001F584)' and two full magic
      sequences, because the func_800B0544 call between them invalidates cse.
      The `(u8)' cast is the `andi $a0, $a0, 0xFF' and is what lets the
      `+ 3' need no sign extension, where the second arm's `s32 j' needs the
      `sll 16'/`sra 16' pair.
   8. `tb = D_8001F62C; q = &tb[k * 5];' -- TWO POINTER LOCALS.  The target
      computes the `lui'/`addiu %lo' pair BEFORE the `sll'/`addu' index
      arithmetic and the final add is `addu $s0, $v0, $v1' -- INDEX FIRST.
      Off the bare symbol the address pair lands after the index: 260 / 30.
      Same rule as func_lo16_800199C8's D_800E3CD8 base in this unit.
   9. `base = D_800E3CD8;' then `for (i = 3; i >= 0; i--) base[i].unk_00 = 0;'
      -- the two `addiu's (`%lo' then `+0x18') are the two-pointer-locals
      rule again.
  10. THE LOOP AT THE END IS A `do { } while (f() != 0 || D_80081468h[3] == 0)'.
      D_80081468 is reached through the unit's existing `s16 []' alias with a
      cast for the word accesses: element 0 goes SYMBOLIC off the `lui'
      (`sw $s0, %lo(D_80081468)($a1)') and +0x4/+0x6 go through the
      `addiu %lo' base, which is the split-at-index-0 shape of
      func_lo16_8001703C.  A second `extern' under the same asm name for the
      word accesses is 262 words / 32.

   PROTOTYPES THIS BODY FIXES FOR THE UNIT, from its own call sites -- SAY SO
   WHEN LANDING, because three of these functions are still stubs:
      void func_lo16_80018464(s32);     argument is a `u8' table field
      s32  func_lo16_80018E80(void);    result tested `beqz' with no
                                        extension and scaled by 5, so `s32'
      s16  func_lo16_80019AF8(void);    result tested after `sll 16'
      void func_lo16_80017A9C(void);

   DECLARATIONS
   extern u8 D_80083780[];
   extern Unk80083160 D_80083160;
   extern s16 D_80081468h[] __asm__("D_80081468");
   extern u16 D_80082E76;
   extern s32 D_800E3D6C;
   extern u8 D_8001F62C[];
   extern Unk41284 *D_8001F594[];
   extern Unk41284 *D_lo16_8001F588[];
   extern s16 D_lo16_8001F586;
   extern u8 D_lo16_8001F584;
   extern u8 D_800F0000[];
   extern u32 D_800E296C;
   typedef struct UnkDungeonMark
   {
       s16 unk_00;
       s16 unk_02;
       s16 unk_04;
       s16 unk_06;
   } UnkDungeonMark;
   extern UnkDungeonMark D_800E3CD8[];
   u16 func_800A6D30();
   void func_800A6D98(u32);
   void func_800A0E44(void);
   void func_800B0544(s16);
   void func_80041284(Unk41284 *);
   void func_80046E38(s16, u8 *);
   void func_800C7D54(s16);
   void func_800BC228(s16);
   void func_8004D0C8(u32);
   void func_lo16_80016E6C(void);
   void func_lo16_8001744C(void);
   void func_lo16_80017560(void);
   void func_lo16_800177A8(void);
   void func_lo16_8001784C(void);
   void func_lo16_80017A9C(void);
   void func_lo16_80018464(s32);
   s32 func_lo16_80018E80(void);
   s16 func_lo16_80019AF8(void);
   void func_lo16_8001E96C(void);
   void func_lo16_8001F32C(void);
   END DECLARATIONS */

void func_lo16_80018A70(void)
{
    u8 *r;
    u8 *b;
    u8 *b2;
    u8 *q;
    u8 *tb;
    UnkDungeonMark *base;
    u16 f;
    s32 n;
    s32 t;
    s32 u;
    s32 k;
    s32 j;
    s32 i;

    r = (u8 *)0x80013710;
    b = (u8 *)&D_80083160;
    f = *(u16 *)(r + 4);
    if ((f & 2) != 0)
    {
        func_800A6D98(*(u32 *)r);
        n = *(s32 *)r + func_800A6D30();
        *(u16 *)(r + 8) = 0;
        *(u16 *)(r + 0xA) = 0;
        *(u16 *)(r + 6) = 0;
        *(s32 *)r = n;
        *(s32 *)D_80081468h = *(s32 *)r;
        D_80081468h[2] = *(u16 *)0x80010234;
        *(u16 *)(r + 4) &= 0xFFFC;
        func_8004D0C8((u32)D_80083780);
        bzero((u8 *)0x80013720, 0x2000);
    }
    else if ((f & 1) != 0)
    {
        *(u16 *)(r + 8) = 0;
        *(u16 *)(r + 0xA) = 0;
        *(s32 *)D_80081468h = *(s32 *)r;
        D_80081468h[2] = *(u16 *)0x80010234;
        func_8004D0C8((u32)D_80083780);
    }
    else
    {
        *(u16 *)(r + 4) = 0;
        t = rand();
        u = rand() << 16;
        *(u16 *)(r + 8) = 0;
        *(u16 *)(r + 6) = 0;
        r[0x11] = 0;
        r[0x10] = 0;
        t |= u;
        t ^= *(u16 *)(b + 4);
        *(s32 *)D_80081468h = t;
        *(s32 *)r = t;
        func_8004D0C8((u32)D_80083780);
        bzero((u8 *)0x80013720, 0x2000);
    }
    srand(*(s32 *)D_80081468h);
    func_800A6D98(*(u32 *)D_80081468h);
    func_800A0E44();
    D_80082E76 = 0;
    D_800E3D6C = 0;
    func_lo16_8001F32C();
    func_lo16_8001E96C();
    base = D_800E3CD8;
    for (i = 3; i >= 0; i--)
    {
        base[i].unk_00 = 0;
    }
    b2 = (u8 *)0x80010000;
    if (*(u32 *)(b2 + 0x234) >= 0x64)
    {
        *(u32 *)(b2 + 0x234) = 0x63;
    }
    k = func_lo16_80018E80();
    if (k != 0)
    {
        k--;
        func_lo16_80017560();
        tb = D_8001F62C;
        q = &tb[k * 5];
        func_800B0544(q[2]);
        func_80041284(D_lo16_8001F588[q[3]]);
        func_80046E38(q[0], D_800F0000);
        func_lo16_8001744C();
        func_lo16_800177A8();
        if (q[1] != 0)
        {
            func_lo16_80018464(q[1]);
        }
        if (q[4] != 0)
        {
            func_800C7D54(q[4] - 1);
        }
        D_800E296C &= 0xDFFFFFFF;
    }
    else
    {
        if (*(s32 *)(b2 + 0x2090) != 2)
        {
            func_800B0544((u8)(D_lo16_8001F584 % 24) + 3);
            func_80041284(D_8001F594[(u8)(D_lo16_8001F584 % 24)]);
        }
        else
        {
            j = func_800A6D30();
            j = j % 24;
            func_800B0544(j + 3);
            func_80041284(D_8001F594[j]);
        }
        D_800E296C |= 0x20000000;
        func_80046E38(0x29, D_800F0000);
        func_800BC228(2);
        do
        {
            func_lo16_8001744C();
            func_lo16_800177A8();
        } while (func_lo16_80019AF8() != 0 || D_80081468h[3] == 0);
    }
    func_lo16_80016E6C();
    if (D_lo16_8001F586 == 0)
    {
        func_lo16_8001784C();
    }
    func_lo16_80017A9C();
}
