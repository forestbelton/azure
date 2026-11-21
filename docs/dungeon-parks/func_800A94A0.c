/* func_800A94A0 (ovl_dungeon) -- PARK, best candidate so far.

   ====================================== 2026-11-21: 41 rows -> 39, AND THE
                                          DEVICE SWEEPS WERE STALE

   Two more duplicated-arm devices, found by tools/decomp/devsweep.py:
   `if (cc)` around `cc = c;` and `if (q0)` around the else-arm's
   `h = func_80099194(*(u8 **)nm, h);`.

   THE POINT IS NOT THE DEVICES, IT IS THAT THIS PARK HAD ALREADY BEEN SWEPT
   FOR EXACTLY THESE TWO DEVICES AND THE SWEEP CAME BACK EMPTY.  It was swept
   on the body before the cancelled pair; with `n++; n--;` in place the
   contested register changed hands and two wrappers that had been worth
   nothing are worth a row each.  Re-run the sweeps after landing anything --
   that is now three parks where a stale negative was hiding a row.

   THE CONDITION IS THE REFERENCE, AND `cc` IS INDETERMINATE THERE.  `if (cc)`
   reads cc before it is set.  Both arms assign `cc = c;`, so nothing the
   function computes depends on the branch, and gcc 2.7.2 does not exploit the
   UB -- but it is worth knowing WHY the spelling matters: the device's value
   is a free reference on the CONDITION's variable, not on the wrapped
   statement.  `if (c)`, `if (q)`, `if (a)` and `if (n)` -- all defined there
   -- measure 40, one row worse, because the reference lands on the wrong
   name.  A cancelled pair on cc instead (`cc = c; cc++; cc--;`) costs two
   WORDS (229), unlike the one on the pointer `n`, so there is no way to buy
   cc's reference from a defined value yet.  Resolve that before landing.
   target 227 words, candidate 227 words, 39 aligned differences

   ============================== 2026-11-21: 59 rows -> 41

   `n++; n--;` immediately after `n = func_8003FD64(...)`.  The pair compiles
   to NOTHING -- combine cancels the `addiu $x, 0x58` / `addiu $x, -0x58` --
   but it is four more references to n in flow.c's count, and that is worth
   18 rows of the register rotation this park spent a day failing to move.

   THIS PARK SAID THE RESIDUE WAS NOT PERMUTER FOOD.  It was wrong, and the
   reasoning is the interesting part: the residue IS the alloc class (equal
   word counts, every difference a register number), docs/dungeon-sweep.md
   does say that class is `.greg` work, and 210 declaration permutations had
   already come back flat.  What that argument misses is that a free-reference
   device changes the alloc ORDER without changing a single instruction, so an
   alloc-class residue is exactly what a reference-count device can move.  The
   permuter found it in 45 minutes; the `.greg` reading had me reading dumps.

   `n += 1; n -= 1;` and `n = &n[1]; n = &n[-1];` are the same 41 rows -- the
   same cancelled pair, three spellings.  Measured and WORSE:
   * `do { n = func_8003FD64(...); } while (0);` -- 66 rows.  The do-while
     weights the references inside it, and the only reference inside is the
     ASSIGNMENT, which is not where the extra weight is wanted.
   * a duplicated-arm `if (D_80083498)` around the call -- 231 words.  Two
     calls that jump2 does not merge, because the arms differ in the
     condition's own evaluation.
   * inverting to an early `if (n == NULL) { return 0; }` (equivalent, since
     the function returns `(s32)n` and that is 0 on the NULL path): 229 words,
     61 rows -- the inversion costs two words on its own, and 44 rows with the
     device.  The park's `if (n != NULL)` + `return (s32)n` tail is right.
   * hoisting `g[0x24]` into a `u8` local, which the permuter's find also
     carried: 59 rows, i.e. nothing.  The find's whole 18 rows is the
     free-reference pair; the hoist rode along.  (Splitting a find into its
     individual changes and measuring each half, every time.)
   target 227 words, candidate 227 words, 41 aligned differences

   ============================== 2026-11-20: 76 rows -> 59

   The body below carries one device: a duplicated-arm `if (q)` around `r = func_800990FC();`.

   A `do { } while (0);` wrapper and a duplicated-arm `if (v) { s } else { s }`
   are the same lever from two directions -- flow.c:2128 weights each
   reference by `loop_depth`, and jump2's cross-jumping merges identical arms
   -- so both cost nothing and both are ordinary C.  SWEPT, NOT REASONED:
   every statement position times both devices over every local, on a
   `tools/decomp/onefunc.py` file where a candidate is 0.09 s.  Nothing in
   this park's diagnosis points at the winning statement, and that is the
   normal case rather than the exception.
   target 227 words, candidate 227 words, 76 aligned differences
   (ovlverify masked: 76 differing).

   NEEDS THESE DECLARATIONS at the top of the unit:
       s32 func_80069EF8(void);
       extern u8 D_80089080[];
       extern u8 D_800DD880[];
       extern u8 D_800DD8B4[];
       extern u8 D_800E1C58[];

   RESIDUE: every opcode, immediate and displacement matches except ONE
   instruction, and the rest is a register rotation.
   * word 70: the target has `addu $s3, $v0, $zero` -- func_80069EF8's result
     assigned to a variable that is NEVER READ on any path (every path from
     there either returns or re-assigns that register from func_800990FC).
     gcc deletes the copy for every spelling tried, so the candidate fills
     that slot with an `addu $a0, $s1` reorg moved up instead; the word count
     still matches because the candidate then needs one `addu $a1` fewer.
   * the saved-register assignment is rotated:
       candidate  a=$s3 q=$s0 c=$s1 e=$s7 n=$s2 q0=$s1 (s16)c=$s4 cc=$s6
       target     a=$s4 q=$s3 c=$s0 e=$s6 n=$s1 q0=$s2 (s16)c=$s0 cc=$s7
     Note the target reuses c's own register for `(s16)c` (c dies at the
     sign-extend) while the candidate gives it a register of its own, because
     cse rewrites the sign-extend's operand to the `cc` copy.
   * words 184-185/195-196: the target materialises `%hi/%lo(D_8006DE24)`
     separately in each arm of the last `if`; the candidate hoists one pair.

   THE TWO LEVERS THAT MADE IT, BOTH ALREADY IN docs/matching-levers.md AND
   BOTH FOUND THERE RATHER THAN BY SWEEPING:
   * `:25773` "A SECOND SET IS WHAT KEEPS A SIGN EXTENSION ALIVE" is about
     THIS function. `q0` set once from an `lbu` has reg_nonzero_bits 0xFF and
     combine folds away all three `sll`/`sra` pairs on it -- 220 words of 227.
     Giving it the second set `q0 = func_800A9400(q0);` restores all three:
     226 words.  Worth 6 words.
   * `:25837` `>> 1`, not `/ 2`, for the two D_800DCEAC/D_800DCEBC halves.

   MEASURED AND REJECTED
   * `s32 c` used directly for the later `(s16)c != 0` tests: 226 words,
     68 aligned -- one word short, the target's `addu $s7, $s0, $zero` copy.
     `s32 cc = c;` with the later tests written `(s16)cc != 0` is 227/76;
     `s16 cc` (the narrow-copy lever) is 226/77, `u16 cc` 226/77.
   * q0 as `s32` with `(s16)q0` written at each index: 220 words, 67 aligned
     -- combine folds the cast away, see the lever above.
   * `D_80082E80` inline rather than a `u8 *g` pointer local: 220/113; the
     pointer local is what gives the target's single `$t0` base reused for
     `0x14`, `0x24`, `0x25` and the call argument.
   * `*(u16 *)0x80013714` as a constant address: same word count, 2 differing
     -- here it is the DECLARED symbol D_80013714 (`lui %hi`/`lhu %lo`),
     unlike func_800A9A0C three screens away which needs the raw address.
   * `(u8 *)D_8006DE24 + i * 0x14 + 8` folds the `+8` into the `%lo`, giving
     `lw 0($v1)`; via the `u8 *nm` pointer local it stays a displacement.
   * func_80069EF8's result assigned to `r`, to `h`, to `src`, to a dedicated
     unused `s32`, as a bare call, with and without a cast: all six are the
     same bytes (identical md5), so the dead copy is not reachable from C.
   * `cc = c;` moved: after the func_800A982C call 226/74, immediately before
     the `if ((s16)c == 0)` 226/74, at the very top 226/77, and two positions
     inside the head give the base's bytes exactly.  None of them is 227 AND
     below 76.
   * the residue is the ALLOC class by docs/landing-a-function.md's test
     (word counts equal, every difference a register number apart from the one
     word at index 70), so per docs/dungeon-sweep.md it is `.greg` work and
     NOT permuter food.  The allocation ORDER is what differs:
       target     c=$s0 n=$s1 q0=$s2 q=$s3 a=$s4 s=$s5 e=$s6 cc=$s7
       candidate  q=$s0 c=$s1 n=$s2 a=$s3 (s16)c=$s4 s=$s5 cc=$s6 e=$s7
     and the one structural consequence is that the target's `(s16)c` reuses
     c's own register because c dies at the sign-extend, where the candidate's
     cse rewrites the sign-extend's operand to the `cc` copy and `(s16)c`
     therefore needs a register of its own.
   * ALL 210 single-move permutations of the 15 local declarations were swept
     and NONE beats 227 words / 76 aligned, so declaration order is not the
     tie-break that decides this rotation. */
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e)
{
    UnkNode *n;
    UnkVec *m;
    u8 *p;
    u8 *s;
    u8 *g;
    u8 *src;
    u8 *o;
    void *r;
    void *h;
    u8 *nm;
    s16 q0;
    s32 cc;
    u8 q1;
    s16 k;
    s32 t;

    s = *(u8 **)(a - 0x14);
    n = func_8003FD64(0x12, D_80083498);
    n++;
    n--;
    if (n != NULL)
    {
        if (cc)
        {
            cc = c;
        }
        else
        {
            cc = c;
        }
        q0 = q[0];
        D_800E3D68 = q[1];
        t = func_800A982C(q0, (s16)c, a[0x13]);
        if (t == 0)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            return 0;
        }
        n->unk_10 = t;
        p = (u8 *)n + 0x20;
        *(u8 **)p = a;
        *(u8 **)(p + 0x4) = e;
        p[0x8] = q0;
        D_800E3CC8[0] = q0;
        q1 = q[1];
        p[0x9] = q1;
        D_800E3CC8[1] = q1;
        if ((s16)c == 0)
        {
            m = n->unk_08;
            src = *(u8 **)(a - 0x18);
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(src + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(src + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(src + 0xA);
            r = (void *)func_80069EF8();
            if ((D_80013714 & 1) == 0)
            {
                o = *(u8 **)(a + 0x60);
                if (o != NULL)
                {
                    func_8009FD40(*(u8 **)(a - 0x14), *(u8 **)(o - 0x14));
                }
            }
            func_800C77D0(n, (u8 *)m, 8, 0x300);
        }
        else
        {
            g = D_80082E80;
            if ((*(u16 *)(s + 0x14) & 0x8000) == 0 && (*(u16 *)(g + 0x14) & 0x8000) == 0)
            {
                func_800C7A3C(g, s, *(s16 *)(D_800E3D7C + 0x88), *(s16 *)(a + 0x88), 8, 0x300);
            }
            else
            {
                k = (*(u16 *)(D_800E3D7C + 0x2A) >> 9) & 7;
                func_800C78A0(a - 0x20, (g[0x24] << 6) + (*(s16 *)&D_800DCEAC[k] >> 1) + 0x20,
                              (g[0x25] << 6) + (*(s16 *)&D_800DCEBC[k] >> 1) + 0x20,
                              *(s16 *)(D_800E3D7C + 0x88), 8, 0x300);
            }
        }
        if ((s16)cc != 0)
        {
            func_800AD594(a, D_800DD8B4[q0] << 6);
        }
        else
        {
            func_800AD594(a, D_800DD880[q0] << 8);
        }
        if (n != NULL && *(s8 *)(a + 0x13) > 0)
        {
            if (q)
            {
                r = func_800990FC();
            }
            else
            {
                r = func_800990FC();
            }
            h = func_80099734(a, r);
            h = func_80099194(D_800E1C58, h);
            if ((s16)cc != 0)
            {
                q0 = func_800A9400(q0);
                nm = (u8 *)&D_8006DE24_n[q0];
                h = func_80099194(*(u8 **)(nm + 8), h);
            }
            else
            {
                nm = (u8 *)&D_8006DE24_n[q0];
                if (q0)
                {
                    h = func_80099194(*(u8 **)nm, h);
                }
                else
                {
                    h = func_80099194(*(u8 **)nm, h);
                }
            }
            func_80099290(func_80099194(D_80089080, h));
            func_800A5720(r);
        }
    }
    return (s32)n;
}
