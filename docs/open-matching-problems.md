# Open matching problems

Four functions in Azure Dreams (SLUS-00614) that reproduce the original logic
but not the original instructions. Each is within one to three instructions,
and in every case the difference is a code-generation choice -- register
allocation, branch arm order, or constant folding -- that I could not steer
from the C.

They are grouped here because they look like one problem rather than four: the
logic is right every time and the gap is not expressible in the source. Someone
who has fought gcc 2.7.2's MIPS backend before may well recognise these on
sight.

## Toolchain

The compiler is the one that shipped in PSY-Q 4.0, run under wine:

    cc1psx    GNU C 2.7.2.SN32.3.7 Build 0001 [AL 1.1, MM 40] Sony Playstation
    aspsx     Psy-Q ASPSX version 2.56        (emulated by maspsx)
    ccpsx     Psy-Q CC 3.02 Build 0002

This is SN Systems' patched gcc, not vanilla GNU 2.7.2, which does not produce
matching code. The exact two-step build of a C file is:

    tools/ccpsx -O2 -G8 -Iinclude -S -o build/csrc/UNIT.s csrc/UNIT.c
    python3 maspsx.py --aspsx-version=2.56 --force-stdin --run-assembler \
        --gnu-as-path=mips64-linux-gnu-as --use-comm-section -G8 \
        -EL -32 -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 \
        -o build/csrc/UNIT.o < build/csrc/UNIT.s

Note gas is left at its default `-G0` (maspsx forces it) while maspsx itself
knows `-G8`. That split matters: given a nonzero `-G`, gas assumes an
*undefined* symbol is small data and emits `GPREL16`, whereas ASPSX assumes
externs are not small. Two units whose functions genuinely want `$gp`-relative
addressing are compiled with `--dont-force-G0` instead; see
`docs/addressing-and-translation-units.md`.

## Verifying a candidate

`make check` rebuilds and compares the whole 512 KB executable against the
original, reporting the first differing offset and which splat subsegment owns
it. That is the only authority -- instruction counts agree while bytes differ
often enough to be a trap (`func_80040044` once matched at 7/7 with wrong
bytes).

Per-function assembly lives in `asm/nonmatchings/<unit>/<func>.s`, and
`tools/m2c` regenerates a C starting point. `docs/m2c-pitfalls.md` lists the
m2c inferences already known to be wrong in ways that still compile.

In the target listings below, an instruction indented one space further than
its neighbours is in the preceding branch's delay slot.

decomp-permuter is set up and working (`tools/permuter_compile.sh` runs the
real pipeline, so its scores reflect the real toolchain). It was tried on the
first function below without success.


---

## func_8003E14C  (unk_11940)

Reads a mode byte and clamps a halfword against 0x29.

### Target

      lui        $v0, %hi(D_80082E6A)
      lbu        $v1, %lo(D_80082E6A)($v0)
      addiu      $v0, $zero, 0x2
      bne        $v1, $v0, .L8003E180
       addu      $v0, $zero, $zero
      lui        $a0, %hi(D_8008146C)
      lh         $a0, %lo(D_8008146C)($a0)
      nop
      slti       $v0, $a0, 0x29
      beqz       $v0, .L8003E17C
       addiu     $v1, $zero, 0x28
      addu       $v1, $a0, $zero
      addu       $v0, $v1, $zero
      jr         $ra
       nop

### Best-effort C

    s32 func_8003E14C(void) {
        s32 v;
    
        if (D_80082E6A != 2) {
            return 0;
        }
    
        v = 0x28;
        if (D_8008146C < 0x29) {
            v = D_8008146C;
        }
        return v;
    }

### What that builds to

    0000000c <func_8003E14C>:
           c:	3c030000 	lui	v1,0x0
    			c: R_MIPS_HI16	D_80082E6A
          10:	90630000 	lbu	v1,0(v1)
    			10: R_MIPS_LO16	D_80082E6A
          14:	24020002 	li	v0,2
          18:	14620009 	bne	v1,v0,40 <func_8003E14C+0x34>
          1c:	00001021 	move	v0,zero
          20:	3c040000 	lui	a0,0x0
    			20: R_MIPS_HI16	D_8008146C
          24:	84840000 	lh	a0,0(a0)
    			24: R_MIPS_LO16	D_8008146C
          28:	00000000 	nop
          2c:	28820029 	slti	v0,a0,41
          30:	10400002 	beqz	v0,3c <func_8003E14C+0x30>
          34:	24030028 	li	v1,40
          38:	00801821 	move	v1,a0
          3c:	00601021 	move	v0,v1
          40:	03e00008 	jr	ra
          44:	00000000 	nop

### The difference

One instruction. The target puts the `%hi` of the first global in `$v0`
and loads through it into `$v1`; we put it in `$v1` and load through `$v1`
into `$v1`, reusing the register as both base and destination.

    target:  lui $v0, %hi(D_80082E6A)   /  lbu $v1, %lo(...)($v0)
    built:   lui $v1, %hi(D_80082E6A)   /  lbu $v1, %lo(...)($v1)

Everything else is byte-identical, including the `addu $v0, $zero, $zero`
that an earlier formulation got wrong.

### Ruled out

* `s16` locals for the result and the temporary: adds four sign-extension
  instructions (19 vs 15). `s32` removes them, which is how the size was
  fixed in the first place -- the `lh` already sign-extends and 0x28 fits.
* Single return through a result variable rather than two returns: puts the
  result in `$a0` instead of `$v0`, so two registers were wrong rather than
  one. Early return fixed the result register.
* Inverting the inner comparison (`v = D_8008146C; if (v >= 0x29) v = 0x28;`):
  identical output.
* decomp-permuter, ~5000 iterations across 4 workers, `--stop-on-zero`. Base
  score 10, and nothing ever scored below it: the distribution was
  `{10: 1396, 15: 81, 20: 64, 35: 2}`. Mutations either preserved the
  difference or made it worse.


---

## func_800422A8  (unk_14934)

Unsigned divide, then return the quotient as a halfword or -1.

### Target

      subu       $a0, $a0, $a1
      andi       $a2, $a2, 0xFFFF
      divu       $zero, $a0, $a2
      mflo       $a0
      andi       $a3, $a3, 0xFFFF
      sltu       $a3, $a0, $a3
      beqz       $a3, .L800422D0
       sll       $v0, $a0, 16
      j          .L800422D4
       sra       $v0, $v0, 16
      addiu      $v0, $zero, -0x1
      jr         $ra
       nop

### Best-effort C

    s16 func_800422A8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
        u32 q = (u32) (arg0 - arg1) / (u32) (arg2 & 0xFFFF);
    
        if (q < (u32) (arg3 & 0xFFFF)) {
            return (s16) q;
        }
        return -1;
    }

### What that builds to

    00001174 <func_800422A8>:
        1174:	00852023 	subu	a0,a0,a1
        1178:	30c6ffff 	andi	a2,a2,0xffff
        117c:	0086001b 	divu	zero,a0,a2
        1180:	00002012 	mflo	a0
        1184:	30e7ffff 	andi	a3,a3,0xffff
        1188:	0087382b 	sltu	a3,a0,a3
        118c:	14e00003 	bnez	a3,119c <func_800422A8+0x28>
        1190:	00041400 	sll	v0,a0,0x10
        1194:	08000468 	j	11a0 <func_800422A8+0x2c>
    			1194: R_MIPS_26	.text
        1198:	2402ffff 	li	v0,-1
        119c:	00021403 	sra	v0,v0,0x10
        11a0:	03e00008 	jr	ra
        11a4:	00000000 	nop

### The difference

The two arms of the branch are the other way round. The target branches
*when the comparison is false* to the `-1` block and lets the sign-extend path
fall through; ours branches when true.

    target:  beqz $a3, .L800422D0   (then-path falls through)
    built:   bnez a3, ...           (else-path falls through)

Instruction count matches at 13.

### Ruled out

* Two early returns (`if (q < x) return (s16) q; return -1;`): the layout
  above, arms swapped.
* A single return through a result variable, which flips the branch to `beqz`
  correctly -- but then gcc restructures the tail into
  `li v0,-1 / move v0,a0 / sll / jr / sra` and comes out 12 instructions
  instead of 13. Right branch, wrong body.
* Note the target's `divu $zero, $a0, $a2` carries no divide-by-zero check,
  so whatever produced it was not emitting one here.


---

## func_8003D8B0  (unk_102A8)

Zero two buffers, clear four words, then three calls. Called from main.

### Target

      addiu      $sp, $sp, -0x18
      sw         $ra, 0x10($sp)
      lui        $a0, %hi(D_80083120)
      addiu      $a0, $a0, %lo(D_80083120)
      jal        func_8003DB4C
       addiu     $a1, $zero, 0x10
      lui        $a0, %hi(D_80082EC0)
      addiu      $a0, $a0, %lo(D_80082EC0)
      jal        func_8003DB4C
       addiu     $a1, $zero, 0x80
      addiu      $v1, $zero, 0x3
      lui        $v0, %hi(D_80083110)
      addiu      $v0, $v0, %lo(D_80083110)
      addiu      $v0, $v0, 0xC
      sw         $zero, 0x0($v0)
      addiu      $v1, $v1, -0x1
      bgez       $v1, .L8003D8E8
       addiu     $v0, $v0, -0x4
      lui        $at, %hi(D_80013714)
      sh         $zero, %lo(D_80013714)($at)
      jal        func_800499BC
       nop
      addiu      $a0, $zero, 0x2A
      jal        func_8004437C
       addu      $a1, $zero, $zero
      jal        func_8004425C
       addu      $a0, $zero, $zero
      lw         $ra, 0x10($sp)
      addiu      $sp, $sp, 0x18
      jr         $ra
       nop

### Best-effort C

    void func_8003D8B0(void) {
        u32 *p;
        s32 i;
    
        func_8003DB4C((int *) &D_80083120, 0x10);
        func_8003DB4C((int *) &D_80082EC0, 0x80);
    
        p = &D_80083110 + 3;
        i = 3;
        do {
            *p = 0;
            i--;
            p--;
        } while (i >= 0);
    
        D_80013714 = 0;
        func_800499BC();
        func_8004437C(0x2A, 0);
        func_8004425C(0);
    }

### What that builds to

    00000e08 <func_8003D8B0>:
         e08:	27bdffe8 	addiu	sp,sp,-24
         e0c:	afbf0010 	sw	ra,16(sp)
         e10:	3c040000 	lui	a0,0x0
    			e10: R_MIPS_HI16	D_80083120
         e14:	24840000 	addiu	a0,a0,0
    			e14: R_MIPS_LO16	D_80083120
         e18:	0c000000 	jal	0 <func_8003CAA8>
    			e18: R_MIPS_26	func_8003DB4C
         e1c:	24050010 	li	a1,16
         e20:	3c040000 	lui	a0,0x0
    			e20: R_MIPS_HI16	D_80082EC0
         e24:	24840000 	addiu	a0,a0,0
    			e24: R_MIPS_LO16	D_80082EC0
         e28:	0c000000 	jal	0 <func_8003CAA8>
    			e28: R_MIPS_26	func_8003DB4C
         e2c:	24050080 	li	a1,128
         e30:	3c030000 	lui	v1,0x0
    			e30: R_MIPS_HI16	D_80083110
         e34:	2463000c 	addiu	v1,v1,12
    			e34: R_MIPS_LO16	D_80083110
         e38:	24020003 	li	v0,3
         e3c:	ac600000 	sw	zero,0(v1)
         e40:	2442ffff 	addiu	v0,v0,-1
         e44:	0441fffd 	bgez	v0,e3c <func_8003D8B0+0x34>
         e48:	2463fffc 	addiu	v1,v1,-4
         e4c:	3c010000 	lui	at,0x0
    			e4c: R_MIPS_HI16	D_80013714
         e50:	a4200000 	sh	zero,0(at)
    			e50: R_MIPS_LO16	D_80013714
         e54:	0c000000 	jal	0 <func_8003CAA8>
    			e54: R_MIPS_26	func_800499BC
         e58:	00000000 	nop
         e5c:	2404002a 	li	a0,42
         e60:	0c000000 	jal	0 <func_8003CAA8>
    			e60: R_MIPS_26	func_8004437C
         e64:	00002821 	move	a1,zero
         e68:	0c000000 	jal	0 <func_8003CAA8>
    			e68: R_MIPS_26	func_8004425C
         e6c:	00002021 	move	a0,zero
         e70:	8fbf0010 	lw	ra,16(sp)
         e74:	27bd0018 	addiu	sp,sp,24
         e78:	03e00008 	jr	ra
         e7c:	00000000 	nop

### The difference

One instruction short, all of it in the loop setup. The target
materialises the array base and *then* adds the byte offset as a separate
`addiu`; gcc folds the offset into the `%lo`.

    target:  addiu $v1, $zero, 0x3            (counter first)
             lui   $v0, %hi(D_80083110)
             addiu $v0, $v0, %lo(D_80083110)
             addiu $v0, $v0, 0xC              <-- separate add

    built:   lui   v1, %hi(D_80083110)
             addiu v1, v1, 12                 <-- folded into the %lo
             li    v0, 3

The rest of the function, including both calls and the three at the end,
matches.

### Ruled out

* Pointer form: `p = &D_80083110 + 3;` then `*p = 0; i--; p--;` in a
  do-while. Folds.
* Indexed form with the symbol retyped to `extern u32 D_80083110[];` and a
  `for (i = 3; i >= 0; i--) D_80083110[i] = 0;`. Also folds, same count.
* Both forms produce the same 30 instructions, so the fold is not sensitive to
  how the traversal is written.


---

## func_8003FE78  (unk_12DE0)

Zero a node, push it on the list head, set flags, then compute one or two offsets into the node's tail.

### Target

      addiu      $sp, $sp, -0x20
      sw         $s1, 0x14($sp)
      ori        $s1, $a0, 0x80
      sw         $s0, 0x10($sp)
      addu       $s0, $a1, $zero
      sll        $a2, $a2, 16
      sw         $s2, 0x18($sp)
      sra        $s2, $a2, 16
      addu       $a0, $s0, $zero
      sw         $ra, 0x1C($sp)
      jal        func_8003DB4C
       addu      $a1, $s2, $zero
      lui        $v0, %hi(D_80081498)
      lw         $v0, %lo(D_80081498)($v0)
      lui        $at, %hi(D_80081498)
      sw         $s0, %lo(D_80081498)($at)
      sw         $zero, 0x4($s0)
      beqz       $v0, .L8003FEC8
       sw        $v0, 0x0($s0)
      sw         $s0, 0x4($v0)
      ori        $v0, $s1, 0x4000
      sh         $v0, 0x1E($s0)
      sll        $v0, $s2, 2
      addu       $v1, $s0, $v0
      andi       $v0, $s1, 0x6
      beqz       $v0, .L8003FEEC
       andi      $v0, $s1, 0x41
      j          .L8003FEF8
       addiu     $v1, $v1, -0x24
      beqz       $v0, .L8003FF00
       andi      $v0, $s1, 0x57
      addiu      $v1, $v1, -0x18
      sw         $v1, 0xC($s0)
      andi       $v0, $s1, 0x57
      beqz       $v0, .L8003FF0C
       addiu     $v1, $v1, -0x18
      sw         $v1, 0x8($s0)
      addu       $v0, $s0, $zero
      lw         $ra, 0x1C($sp)
      lw         $s2, 0x18($sp)
      lw         $s1, 0x14($sp)
      lw         $s0, 0x10($sp)
      addiu      $sp, $sp, 0x20
      jr         $ra
       nop

### Best-effort C

    UnkNode *func_8003FE78(s32 flags, UnkNode *n, s16 count) {
        s32 f = flags | 0x80;
        UnkNode *old;
        u8 *p;
    
        func_8003DB4C((int *) n, count);
    
        old = D_80081498;
        D_80081498 = n;
        n->unk_04 = NULL;
        n->unk_00 = old;
        if (old != NULL) {
            old->unk_04 = &n->unk_00;
        }
        n->unk_1E = f | 0x4000;
    
        p = (u8 *) n + count * 4;
        if (f & 6) {
            p -= 0x24;
            n->unk_0C = (u32) p;
        } else if (f & 0x41) {
            p -= 0x18;
            n->unk_0C = (u32) p;
        }
        if (f & 0x57) {
            p -= 0x18;
            n->unk_08 = (u32) p;
        }
        return n;
    }

### What that builds to

    00000898 <func_8003FE78>:
         898:	27bdffd8 	addiu	sp,sp,-40
         89c:	afb20018 	sw	s2,24(sp)
         8a0:	00809021 	move	s2,a0
         8a4:	afb00010 	sw	s0,16(sp)
         8a8:	00a08021 	move	s0,a1
         8ac:	00063400 	sll	a2,a2,0x10
         8b0:	afb3001c 	sw	s3,28(sp)
         8b4:	00069c03 	sra	s3,a2,0x10
         8b8:	02002021 	move	a0,s0
         8bc:	02602821 	move	a1,s3
         8c0:	afbf0020 	sw	ra,32(sp)
         8c4:	0c000000 	jal	0 <func_8003F5E0>
    			8c4: R_MIPS_26	func_8003DB4C
         8c8:	afb10014 	sw	s1,20(sp)
         8cc:	3c020000 	lui	v0,0x0
    			8cc: R_MIPS_HI16	D_80081498
         8d0:	8c420000 	lw	v0,0(v0)
    			8d0: R_MIPS_LO16	D_80081498
         8d4:	36510080 	ori	s1,s2,0x80
         8d8:	3c010000 	lui	at,0x0
    			8d8: R_MIPS_HI16	D_80081498
         8dc:	ac300000 	sw	s0,0(at)
    			8dc: R_MIPS_LO16	D_80081498
         8e0:	ae000004 	sw	zero,4(s0)
         8e4:	10400002 	beqz	v0,8f0 <func_8003FE78+0x58>
         8e8:	ae020000 	sw	v0,0(s0)
         8ec:	ac500004 	sw	s0,4(v0)
         8f0:	36424080 	ori	v0,s2,0x4080
         8f4:	a602001e 	sh	v0,30(s0)
         8f8:	00131080 	sll	v0,s3,0x2
         8fc:	02021821 	addu	v1,s0,v0
         900:	32220006 	andi	v0,s1,0x6
         904:	10400003 	beqz	v0,914 <func_8003FE78+0x7c>
         908:	32220041 	andi	v0,s1,0x41
         90c:	08000248 	j	920 <func_8003FE78+0x88>
    			90c: R_MIPS_26	.text
         910:	2463ffdc 	addiu	v1,v1,-36
         914:	10400004 	beqz	v0,928 <func_8003FE78+0x90>
         918:	32220057 	andi	v0,s1,0x57
         91c:	2463ffe8 	addiu	v1,v1,-24
         920:	ae03000c 	sw	v1,12(s0)
         924:	32220057 	andi	v0,s1,0x57
         928:	10400002 	beqz	v0,934 <func_8003FE78+0x9c>
         92c:	2463ffe8 	addiu	v1,v1,-24
         930:	ae030008 	sw	v1,8(s0)
         934:	02001021 	move	v0,s0
         938:	8fbf0020 	lw	ra,32(sp)
         93c:	8fb3001c 	lw	s3,28(sp)
         940:	8fb20018 	lw	s2,24(sp)
         944:	8fb10014 	lw	s1,20(sp)
         948:	8fb00010 	lw	s0,16(sp)
         94c:	27bd0028 	addiu	sp,sp,40
         950:	03e00008 	jr	ra
         954:	00000000 	nop

### The difference

Three instructions over. The visible cause is that gcc folds the two
flag ORs together:

    target:  ori $s1, $a0, 0x80      (held in $s1, reused by the bit tests)
             ...
             ori $v0, $s1, 0x4000

    built:   ori v0, s2, 0x4080      (0x80 | 0x4000 merged)

The target keeps `flags | 0x80` as a live value that the later `& 6`,
`& 0x41` and `& 0x57` tests all read, so it is computed once early and never
re-derived. Register allocation also differs around it (`$s2` vs `$s3` for the
count), which may be a consequence rather than a separate problem.

The branch structure itself reproduces correctly, including the shared
fall-through into the `unk_0C` store from two different predecessors.

### Ruled out

* `s32 f = flags | 0x80;` used for every subsequent test -- which is what
  the target appears to do -- still folds at the one site where the result
  feeds another OR.
* Writing the stores with a `u8 *` cursor and byte offsets (`p -= 0x24`)
  rather than scaled pointer arithmetic, to keep the arithmetic literal.
