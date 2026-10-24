#!/usr/bin/env python3
"""magic.py 0x86186187 [post_shift] -- which divisor does this magic come from?

A `mult` by a large constant followed by `mfhi`, maybe an add-back of the
dividend, a `sra`, and `sra 31`/`subu` is gcc's constant division. Guessing the
divisor from the arithmetic does not work: the shift and the magic are chosen
together, and the obvious estimate 2^(32+shift)/magic is off by enough to send
you looking for a non-integer divisor. func_80054D64's 0x80010003 with `sra 14`
estimates to 10922.56 and is division by 32767.

This runs gcc 2.7.2's own choose_multiplier (expmed.c) over every divisor and
reports the ones that produce the magic, with the post_shift each one implies
so you can check it against the listing. Signed division uses precision
`size - 1`, unsigned `size`; both are tried.

    python3 tools/decomp/magic.py 0x80010003        # -> 32767, post_shift 14
    python3 tools/decomp/magic.py 0x86186187 4

Read the sequence as:  q = ((n + highpart(n * magic)) >> post_shift) - (n >> 31)
when the magic has its top bit set (the add-back form), and without the `+ n`
otherwise. The trailing `- (n >> 31)` is the signed correction and is NOT part
of the divisor.

For an UNSIGNED add-back the shape is different and the shift in the listing is
one less than the post_shift printed here: gcc emits
`q = (((n - t) >> 1) + t) >> (post_shift - 1)` with `t = highpart(n * magic)`,
so func_80050A58's `srl 1` / `addu` / `srl 4` is post_shift 5, division by 21.
"""
import sys

LIMIT = 1 << 24


def ceil_log2(d):
    return (d - 1).bit_length()


def choose_multiplier(d, n, precision):
    """gcc 2.7.2 expmed.c choose_multiplier, n <= HOST_BITS_PER_WIDE_INT."""
    lgup = ceil_log2(d)
    if lgup > n:
        return None
    pow_ = n + lgup
    pow2 = n + lgup - precision
    mlow = (1 << pow_) // d
    mhigh = ((1 << pow_) + (1 << pow2)) // d
    post_shift = lgup
    while post_shift > 0:
        ml = mlow >> 1
        mh = mhigh >> 1
        if ml >= mh:
            break
        mlow, mhigh = ml, mh
        post_shift -= 1
    mask = (1 << n) - 1
    return mhigh & mask, post_shift, int(mhigh >= mask)


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    magic = int(sys.argv[1], 0)
    want_shift = int(sys.argv[2], 0) if len(sys.argv) > 2 else None
    hits = []
    for d in range(2, LIMIT):
        for precision, kind in ((31, "signed"), (32, "unsigned")):
            r = choose_multiplier(d, 32, precision)
            if r is None:
                continue
            m, post, addback = r
            if m != magic:
                continue
            if want_shift is not None and post != want_shift:
                continue
            hits.append((d, kind, post, addback))
    if not hits:
        print("no divisor under %d produces 0x%X%s" %
              (LIMIT, magic, "" if want_shift is None else
               " with post_shift %d" % want_shift))
        return 1
    for d, kind, post, addback in hits:
        print("divisor %d (0x%X)  %s  post_shift %d  %s" %
              (d, d, kind, post, "add-back" if addback else "no add-back"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
