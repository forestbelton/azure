/* PARK: func_lo20_80026BFC, csrc/ovl_dng_lo20.c, agent-h161, 2026-11-25.

   TARGET 9 words. BEST BUILT 9 words, 3 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify agrees at 3 since the word count
   matches). 1 member, 9 words total (tools/decomp/dngfam.py), so no
   replication value beyond its own body. No park existed for it.

   WHAT IT DOES. Returns 1 when `p' is non-NULL and its +0x28 word is zero,
   and 0 otherwise. Nine words, one load, two branches.

   THE TARGET, word for word:

     0  beqz  $a0, L            1   addu  $v0, $zero, $zero   (delay)
     2  lw    $v1, 0x28($a0)    3   nop
     4  beqz  $v1, L            5   addiu $v0, $zero, 0x1     (delay)
     6  addu  $v0, $zero, $zero
     L: 7  jr $ra               8   nop

   Both branches target word 7. Word 6 is reached ONLY by falling out of
   word 4, so the fall-through arm stores 0 and the branch-taken arm stores 1
   -- that is, the source's `then' arm is the ZERO one, and both delay slots
   were filled by reorg copying the first insn of each branch's target block
   and retargeting past it.

   THE RESIDUE, and it is the same three rows for every 9-word spelling tried:

     row 4   bnez $v1, L   where the target has  beqz $v1, L
     row 5   nop           where the target has  addiu $v0, $zero, 0x1
     row 6   addiu $v0, $zero, 0x1   where the target has  addu $v0,$zero,$zero

   i.e. the two inner arms are MIRRORED. The body below gets the right word
   count, the right register split (the load in $v1, the result in $v0) and
   the right two branch targets; only which arm is the fall-through is wrong,
   and with it the one delay slot reorg can fill.

   THE CAUSE, confirmed with `tools/cdkgcc -da' rather than inferred.
   `jump.c''s store-flag transform ("1) x = a; if (...) x = b; and either A or
   B is zero", jump.c:1139-1200 in scratch/gcc-src/gcc-2.7.2.3) fires on every
   spelling that writes BOTH arms as plain stores of 1 and 0, collapsing the
   whole gate to `sltiu $v0, $v0, 1' -- 7 words. The sltu first appears in
   `q.c.jump' and is absent from `q.c.rtl', so it is that pass and not fold,
   cse or combine. The guard's `BRANCH_COST >= 2 || HAVE_conditional_move'
   term is therefore satisfied in cc1psx, which is worth knowing: the
   R3000 BRANCH_COST of 1 does NOT disable it.

   The transform can be blocked by making ONE arm an escape (a `return'),
   because then that arm is not a single SET of x. But every such spelling
   then loses the OTHER arm's store: with `r' pre-set to 0 the `r = 0' arm is
   redundant and gcc deletes it (8 words), and with `r' not pre-set the
   NULL path needs its own `r = 0' block, which jump.c cross-joins with the
   inner zero arm and the transform fires again (7 words). That is the bind,
   and it is why 23 spellings all land on 7, 8 or 9-with-the-arms-mirrored.

   WHAT I WOULD TRY NEXT. Something that keeps BOTH arms as stores while
   making one of them invisible to jump.c's single-SET pattern, i.e. a second
   insn in the zero arm at `jump' time that a LATER pass deletes (jump runs
   before cse). I did not find one in plain C. A permuter run on this body is
   cheap -- 9 words -- and the search space is the gate's shape only.

   EVERY SPELLING MEASURED, with its word count and aligned differences.
   `X' is `*(s32 *)(p + 0x28)'. All measured against the body otherwise
   unchanged; note that these are NOT prices for independent facts, they are
   23 whole-body shapes, because at 9 words the gate IS the body.

   | spelling                                                   | words | rows |
   |------------------------------------------------------------|-------|------|
   | r=0; if(p){ if(X!=0) r=0; else return 1; } return r;  KEPT  |   9   |  3   |
   | as KEPT but with `v = X' hoisted into an s32 local          |   9   |  3   |
   | r=0; if(p){ if(X!=0) return 0; r=1; } return r;             |   9   |  4   |
   | r=0; if(p){ if(X!=0) r=0; else { r=1; return r; } } ...     |   9   |  4   |
   | if(p==NULL) r=0; else if(X!=0) r=0; else return 1;          |   9   |  4   |
   | r=1; if(p==NULL) r=0; else if(X!=0) r=0; return r;          |   9   |  7   |
   | if(p==NULL) return 0; if(X==0) return 1; else return 0;     |   8   |  9   |
   | if(p==NULL) return 0; if(X==0) return 1; r=0; return r;     |   8   |  9   |
   | if(p==NULL) return 0; if(X!=0) return 0; return 1;          |   8   |  9   |
   | if(p){ if(X==0) r=1; else r=0; return r; } return 0;        |   8   |  9   |
   | if(p){ if(X==0) return 1; else return 0; } return 0;        |   8   |  9   |
   | if(p){ if(X!=0) r=0; else r=1; } else r=0;                  |   8   |  9   |
   | if(p){ if(X==0) r=1; else r=0; } else r=0;                  |   8   |  9   |
   | if(p==NULL) return 0; if(X==0) return 1; return 0;          |   8   |  9   |
   | r=0; if(p){ if(X==0) r=1; else r=0; } return r;             |   7   |  5   |
   | r=0; if(p){ r=1; if(X!=0) r=0; } return r;                  |   7   |  5   |
   | if(p==NULL) r=0; else if(X==0) r=1; else r=0;               |   7   |  5   |
   | if(p==NULL) r=0; else if(X!=0) r=0; else r=1;               |   7   |  5   |
   | if(p==NULL) return 0; if(X==0) r=1; else r=0; return r;     |   7   |  5   |
   | if(p==NULL) return 0; if(X!=0) r=0; else r=1; return r;     |   7   |  5   |
   | r=0; if(p){ v=X; if(v!=0) r=0; else r=1; } return r;        |   7   |  5   |
   | if(p==NULL) return 0; if(X!=0) r=0; else return 1; ret r;   |   7   |  5   |
   | r=0; if(p==NULL) goto out; if(X==0){r=1;goto out;} r=0;...  |   7   |  5   |
   | r=0; if(p==NULL) goto out; if(X!=0) goto zero; r=1; ...     |   7   |  5   |
   | if(p==NULL) goto zero; if(X!=0) goto zero; return 1; ...    |   7   |  5   |
   | return p != NULL && X == 0;                                 |   7   |  5   |
   | r=0; if(p) r = (X!=0) ? 0 : 1; return r;                    |   7   |  5   |
   | r=0; if(p){ do { r=1; } while(0); ... } (device, both arms) |   7   |  5   |
   | `*(volatile s32 *)(p + 0x28) == 0' in the condition         |   7   |  5   |
   | `volatile s32 r'                                            |  16   | 15   |
   | r=1; if(p==NULL) return 0; if(X!=0) r=0; return r;          |  11   | 11   |

   The 7-word rows are all the SAME collapsed body (`sltiu'), so they are one
   fact measured 14 times, not 14 facts -- recorded in full only because
   which spellings have been tried is the part re-measuring cannot recover.
   The `do { } while (0)' and `volatile' rows say the two usual devices do
   not reach this: notes are skipped by `next_nonnote_insn' and a volatile
   MEM in the condition does not stop `emit_store_flag'.

   DECLARATIONS
   (none -- the body needs nothing this unit does not already have)
   END DECLARATIONS */

s32 func_lo20_80026BFC(u8 *p)
{
    s32 r;

    r = 0;
    if (p != NULL)
    {
        if (*(s32 *)(p + 0x28) != 0)
        {
            r = 0;
        }
        else
        {
            return 1;
        }
    }
    return r;
}
