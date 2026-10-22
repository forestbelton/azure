# m2c output that needs checking

m2c is the right generator for this project, but a few of its inferences are
wrong in ways that compile cleanly and so pass unnoticed.

## It over-infers callee arity

m2c reads a write to an argument register before a `jal` as an argument being
passed. It is often the caller using that register for its own purposes.

`func_8003FE78` calls `func_8003DB4C` and m2c reported
`func_8003DB4C(u32, s16, s32)` -- three parameters. The call site is:

    sll  $a2, $a2, 16      ; the CALLER's own third parameter
    sra  $s2, $a2, 16      ; sign-extended and saved
    addu $a0, $s0, $zero
    jal  func_8003DB4C
     addu $a1, $s2, $zero  ; passed as $a1

`$a2` is never an argument here. All eight call sites of `func_8003DB4C` set
up `$a0` and `$a1` only, so it takes two parameters, which is what it was
decompiled as.

To check arity, read every call site rather than trusting the signature m2c
prints:

    grep -rn "jal.*func_XXXXXXXX" asm/nonmatchings/*/*.s

and look at which of `$a0`-`$a3` are set in the instructions before the `jal`
and in its delay slot.

## It prints pointer arithmetic as byte offsets

`var += 4` on an `s32 *` means `var++`. Compiles either way, silently wrong.

## It prints array indexing as pointer arithmetic

`(&D_80083110)[i]` or `*(ptr + i)` where the original indexed a real array.
Under this compiler the two are not equivalent: an array reference splits the
symbol into `lui` + `%lo` displacement and keeps a strength-reduced offset as
its own `addiu`, pointer arithmetic folds it. Declare the array
(`extern u32 D_80083110[4];`) and index it; see `docs/compiler-version.md`.

## It flattens adjacent symbols into one

Where several labels sit next to each other m2c may attribute accesses to the
first. `func_800439F8` touches `D_80013180`, `D_80013184` and `D_80013186`;
m2c reported all three as `D_80013180`.

## Its struct field widths come from the access, not the type

A field read once with `lbu` and once with `lw` will be reported at whichever
width m2c saw; `tools/fieldscan.py --emit` flags those cases explicitly.
