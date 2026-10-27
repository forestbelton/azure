#include "common.h"

/* func_80052144 and what follows it, split off from the carved unit
   unk_1C540 so that its jtbl_8002EFDC gets a `.rodata` of its own. A
   translation unit emits ONE contiguous `.rodata` and the carved unit owns
   several separated runs of jump tables -- see the table in the jump-table
   section of docs/open-matching-problems.md for which piece carries which.

   jtbl_8002EFDC and csrc/unk_264E0.c's jtbl_8002EFF0 are the two halves of
   ONE contiguous run, supplied by two objects through two `.rodata`
   subsegments in two adjacent segments -- the same arrangement the three
   contiguous tables at 0x8002D630 already use.

   Everything above the first function is copied verbatim from the head of
   csrc/unk_1C540.c plus every file-scope declaration that stood above this
   point in the undivided file. The `.size` records are file-scope and change
   every access to those symbols in whichever unit they appear, so every
   piece carries the whole list. */

__asm__(".sdata\n\t.size D_80081550,4\n\t.size D_80081554,4\n"
        "\t.size D_80081558,4\n\t.size D_80080B94,4\n"
        "\t.size D_80080B98,4\n\t.size D_80081540,4\n"
        "\t.size D_80081568,4\n\t.size D_8008156C,4\n"
        "\t.size D_80080B58,4\n\t.size D_80080B5C,4\n"
        "\t.size D_80080B60,4\n\t.size D_80080B64,4\n"
        "\t.size D_80080B80,4\n\t.size D_80080B84,4\n"
        "\t.size D_80080B1B,1\n\t.size D_80080B1C,1\n"
        "\t.size D_80080B30,1\n\t.text");

void func_80048C3C(s32);

void func_8004B568(u8 *);

s32 func_80048F8C(u32 *, s32);

u32 *func_8004B42C(u32 *, s32);

s32 func_80049280(s32);

extern s32 D_800E3DF0w[] __asm__("D_800E3DF0");

u32 func_80021300(u32);

u32 func_8002137C(s32);

u32 func_800492B0(u32);

s32 func_800494FC(u8 *);

void func_8004DD2C(u8 *);

s32 func_800B8C90(void);

s32 func_8004951C(u8 *, s32);

s32 func_80049944(u32);

extern CVECTOR D_80080B28[];

extern CVECTOR D_80080B2C[];

u8 *func_80049CF4(u8 *, u8 *);

extern s8 D_800712E4[];

extern u8 D_800712F0[];

s16 func_80042900(s8 *, s8);

extern u8 D_80080B30;

extern u8 D_8007136C[];

void func_80049F68(u8 *);

u32 func_8004A0C0(u32);

u8 *func_8004DC14(u8 *, s32);

s16 func_8004A10C(u8 *);

void func_8004A030(u8 *, s16, s16, s16);

void func_8004A170(s32, u8 *, u16, s32);

void func_8004A574(s32, s32);

s32 func_8004A6C0(s32, s32);

u8 *func_8004A784(s32, s32);

u8 *func_8004A700(s32, s32);

s32 func_800A6D30(void);

s32 func_8004AA34(s32 *, s32 *);

void func_8004AFC8(s32, s32, s32, void *);

void func_8003E1FC(void);

void func_8003F320(void);

void func_800A6D60(s32);

void func_800A6D98(u32);

void func_8004AA78(s32);

void func_8004B298(s32, s32);

void func_8004B2E0(s32, s32);

void func_8004B324(s32, s32);

u32 func_8004B364(s32, s32, s32);

extern u8 D_80080B48s[8] __asm__("D_80080B48");

void func_8004B634(void);

void func_800253A0(s32, s32, s32, s32);

void func_8004B834(void);

void func_8002537C(s32, s32, s32, s32, s32);

void func_8004B854(s32, s32, s32);

void func_800B6A0C(s32, s32, s32, s32, s32);

u8 *func_8004C010(u8 *, u8 *);

u8 *func_8004BDDC(u8 *, u8 *, u8 *, u8 *, u8 *);

u8 *func_8004B954(u8 *, u8 *, u8 *, u8 *, u8 *, s32, s32);

void func_80067DF0(u8 *, RECT *);

u8 *func_8004C080(u8 *, u8 *, u8 *, u8 *, u8 *, s32);

void func_8004D12C(u8 *);

void func_8004D294(s16 *, s32, u16);

void func_80041900(u8 *);

void func_8004D4AC(void);

void func_8004D690(u8 *, u8 *, s32);

void func_8004D75C(u8 *, u8 *);

u32 func_8004D880(u32);

void func_8004D98C(u8, s32, u8 *);

void func_8004E264(u16 *, u32);

void func_8004D9E0(u8 *, s32, s32, s32, s32);

u32 func_8004B404(s32);

s32 func_8004B4DC(u8 *);

s32 puts(u8 *);

void func_8003528C(u32);

void func_80033AE8(s32);

void func_8004DCA8(u8 *);

s32 func_80033B2C(s32);

void func_800352B4(void);

extern u32 D_80071410[];

u8 *func_8004E188(u8 *, u32);

void func_8004E4C0(s32, u16, s32, s32);

u8 *func_8004E478(u8 *, u16);

void func_8004B1A4(s32);

s32 func_8004E9EC(u8 *, s32);

s32 func_8004EBDC(void);

void func_8004EE90(u8 *);

void func_8004EB8C(u8 *);

void func_8004ECAC(u8 *);

extern u32 D_80081480a[] __asm__("D_80081480");

s32 func_8004EC20(u8 *);

void func_8004ED00(u8 *);

void func_8004EB3C(u8 *);

s16 func_80053DA8(u16);

UnkNode *func_8004EDA8(s32, s32);

u8 *func_8004EF90(u8 *);

u8 *func_8004EFF4(u8 *);

u8 *func_8004F068(u8 *);

u8 *func_8004F1E8(u8 *);

void func_8004F0DC(u8 *);

u32 func_800491F4(s32, u8 *, s32);

void func_8004EEFC(u8 *, u32 *, u32 *);

void func_8004F2BC(u8 *);

extern UnkNode D_80084138;

extern u8 D_80027DD0[];

UnkNode *func_8004B4A8(UnkNode *);

UnkNode *func_8003FE78(s32, UnkNode *, s16);

s32 func_8004491C(UnkNode *, u32);

void func_8004F52C(u8 *);

void func_8004F330(u8 *, s32);

void func_8004F3D8(u8 *, u32);

void func_8004EEBC(u8 *);

s32 func_8004F308r(s32) __asm__("func_8004F308");

void func_8004FE78(u32);

void func_8004F558(u8 *);

u32 func_8004FDE0(u32);

extern u8 D_80071784[];

extern u8 D_80071790[];

s32 func_8004F85C(u32 *, u32 *);

void func_80044AAC(UnkNode *, UnkNode *);

void func_8004F684(u8 *);

void func_8004F780(u8 *);

void func_8004F884(u8 *);

void func_8004F67C(void);

void func_8004F9AC(u8 *);

void func_8004F95C(u8 *);

s32 func_80049DE8(s32, s32, s32);

s32 func_8004FD78(u8 *, s32);

void func_8004FA2C(u8 *, s32);

void func_8004FAA4(u8 *);

u32 func_80021448(u8 *);

s32 func_80018174(s32, s32, s32, s32, s32);

s32 func_8004FC98(u8 *);

void func_8001832C(u32);

s32 func_800213AC(void);

s32 func_8004FF20(u8 *);

void func_80050EA8(u8 *);

void func_80050DA8(u8 *);

u32 func_800502E0(u8 *);

s32 func_8004FFA8(u8 *);

void func_800500B4(u32, u8 *);

void func_80050308(u8 *);

void func_80050550(u8 *, s32, s32);

void func_800504B4(u8 *, s32, s32);

void func_800505BC(u8 *, s32, s32);

extern u8 D_80077E3C[];

extern u8 D_80077E48[];

extern u8 D_80077E54[];

extern u8 D_80077E60[];

extern u8 D_80077E6C[];

extern u8 D_80077E78[];

extern u8 D_80077E84[];

extern u8 D_80077F80[];

extern u8 D_80077F8C[];

extern u8 D_80081485[];

u32 func_80049E6C(s32);

void func_8004FFF4(u8 *);

u8 *func_800508F0(u8 *, u8 *);

void func_8005097C(u8 *);

void func_80050640(u8 *);

void func_800509C4(u8 *, s32);

void func_8005084C(u8 *, u8 *, u32 *, u32 *);

u8 *func_80049150(u8 *, u32, s32);

void func_80050A58(u32, u8 *);

s32 func_80048FBC(u8 *, s32);

s32 func_80050AF0(u8 *, s32);

void func_80050A00(u8 *, s32);

void func_8004E130(void);

void func_8004B248(u32 *);

void func_80050694(u8 *);

void func_8004CAA0(void);

s32 func_80050B7Cr(s32) __asm__("func_80050B7C");

s32 func_80050BA4(s32);

void func_80050CDC(u8 *, s32, u8 **);

void func_8003AEF8(u8 *);

extern Unk41284 D_80080BAC;

void func_8003E140(s8 *);

void func_80041344(u32, u32);

void func_80050FF4(u8 *);

u32 func_80126890(u32);

void func_80051228(u8 *);

void func_800510DC(u8 *);

void func_800510A0(u8 *);

void func_80051164(u8 *);

extern Unk41284 D_80080BB4;

extern Unk41284 D_80080BBC;

extern Unk41284 D_80080BC4;

extern Unk41284 D_80080BCC;

extern Unk41284 D_80080BD4;

extern Unk41284 D_80080BDC;

extern Unk41284 D_80080BE4;

extern Unk41284 D_80080BEC;

extern Unk41284 D_80080BF4;

extern Unk41284 D_80080BFC;

extern Unk41284 D_80080C04;

extern Unk41284 D_80080C0C;

extern Unk41284 D_80080C14;

extern u8 D_800D381A;

extern u8 D_800136B8;

extern u8 D_800D2FB4[];

extern u8 D_8012F004[];

void func_80046E38(s16, u8 *);

void func_80051548(u8 *, u8 *, u8 *);

extern u32 D_800814A0;

void func_800478B8(s32);

s32 func_800644B8(s32);

s32 func_80064584(s32);

extern UnkElem D_80072044[];

void func_80044BB0(void);

void func_800517CC(u8 *, u8 *, u8 *);

s32 rand(void);

extern UnkElem D_800720AC[];

void func_80051CC4(u8 *, s32, u8 *);

extern u32 D_80071A68[];

void func_80051F58(u8 *, s32, u8 *);

void func_80051528(u8 *);

void func_800517AC(u8 *);

void func_80051CA4(u8 *);

void func_80051F38(u8 *);

void func_800530A4(u8 *);

void func_800530C4(u8 *);

s32 func_80053EF0(s32);

void func_80041094(u16, s16, s16, s16, u16);

/* The five-state cutscene conductor, dispatched through jtbl_8002EFDC on
   +0x00 while +0x02 counts frames. It owns four actor slots at +0x0C, +0x10,
   +0x14 and +0x18 and a fifth thing at +0x1C, and the same TEAR-DOWN of all
   five appears twice: once in the guard that aborts the scene when
   D_80083160's +0x10 has 0x40 set, and once in the default arm. Eight copies
   of the slot block in all, and jump2 merges none of them.

   Three shapes, each measured:

   * D_80083160 is read through a POINTER LOCAL, `Unk80083160 *s =
     &D_80083160;` -- func_80052774's idiom in this same unit. That is what
     gives the listing's `lui $v1` hoisted above the first branch, `addiu $v1`
     in its delay slot and `lw $v0, 0x10($v1)`: the base materialised once and
     the field reached by displacement. Written `D_80083160.unk_10` it is the
     symbolic `lui`/`lw %lo()` pair at the use site -- two words shorter, but
     both of them land in load-delay nops, so it reads as two words SHORT and
     a worse schedule.

   * Each slot block reads D_800814A0 and +0x06 into LOCALS before clearing
     the slot pointer. The listing's order of memory operations is `lw
     D_800814A0`, `lhu +0x06`, `sw 0 -> slot`, `sw D_800814A0`, `sh +0x06`,
     and sched1 cannot produce that from the natural statement order: a
     SYMBOL_REF address against a `(plus (reg) const)` address falls through
     memrefs_conflict_p to its `return 1`, so neither load may be hoisted past
     the slot store. Written naturally the block is three load-delay nops
     instead of one -- fifteen extra nops over the eight copies.

   * +0x06 is incremented IN PLACE in its local (`n = n + 1;` then the store),
     which is the listing's `addiu $v1, $v1, 1`; `*(u16 *)(p + 6) = n + 1`
     gives the increment a second register.

   The tail that cases 4 and the default share -- D_80082E60's +0x16 bit, the
   func_80041094 call with the value read BACK through cse, +(-2)'s flag and
   D_800814A0 -- is written out in both and cross-joined. */
void func_80052144(u8 *p)
{
    Unk80083160 *s = &D_80083160;
    u8 *f;

    *(u16 *)(p + 2) = *(u16 *)(p + 2) + 1;
    if (*(s16 *)(p + 0) < 4 && (s->unk_10 & 0x40) != 0)
    {
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0xC);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0xC) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0x10);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0x10) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0x14);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0x14) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0x18);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0x18) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        if (*(u8 **)(p + 0x1C) != NULL)
        {
            func_800530C4(*(u8 **)(p + 0x1C));
        }
        func_80053DA8(0xB4);
        *(u16 *)(p + 2) = 0;
        *(s16 *)(p + 0) = 4;
    }
    switch (*(s16 *)(p + 0))
    {
    case 0:
        if (func_80053EF0(4) == 1)
        {
            func_80051528(*(u8 **)(p + 0xC));
            func_800517AC(*(u8 **)(p + 0x10));
            func_800530A4(*(u8 **)(p + 0x1C));
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        return;
    case 1:
    {
        u8 *e;

        e = *(u8 **)(p + 0x14);
        if (e != NULL)
        {
            if (**(s16 **)(p + 8) == *(s16 *)(p + 2))
            {
                func_80051CA4(e);
                f = *(u8 **)(p + 8);
                *(u8 **)(p + 8) = f + 0x10;
                if (*(s16 *)(f + 0x10) == 0)
                {
                    *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
                }
            }
        }
        else
        {
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        return;
    }
    case 2:
        if (*(s16 *)(p + 2) == 0x1C09)
        {
            func_80051528(*(u8 **)(p + 0xC));
            func_800517AC(*(u8 **)(p + 0x10));
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        return;
    case 3:
        if (*(s16 *)(p + 2) == 0x1CCF)
        {
            func_80051F38(*(u8 **)(p + 0x18));
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        return;
    case 4:
        if (*(s16 *)(p + 4) == *(s16 *)(p + 6))
        {
            D_80082E60.unk_16 = D_80082E60.unk_16 | 0x8000;
            func_80041094(6, 0, 0, 0, D_80082E60.unk_16 ^ 1);
            *(u16 *)(p - 2) = *(u16 *)(p - 2) | 0x8000;
            D_800814A0 |= 0x8000;
        }
        return;
    default:
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0xC);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0xC) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0x10);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0x10) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0x14);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0x14) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        {
            u8 *e;
            u32 g;
            u16 n;

            e = *(u8 **)(p + 0x18);
            if (e != NULL)
            {
                *(u16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                g = D_800814A0;
                n = *(u16 *)(p + 6);
                *(u8 **)(p + 0x18) = NULL;
                D_800814A0 = g | 0x8000;
                n = n + 1;
                *(u16 *)(p + 6) = n;
            }
        }
        if (*(u8 **)(p + 0x1C) != NULL)
        {
            func_800530C4(*(u8 **)(p + 0x1C));
        }
        if (*(s16 *)(p + 4) == *(s16 *)(p + 6))
        {
            func_80053DA8(0xB4);
            D_80082E60.unk_16 = D_80082E60.unk_16 | 0x8000;
            func_80041094(6, 0, 0, 0, D_80082E60.unk_16 ^ 1);
            *(u16 *)(p - 2) = *(u16 *)(p - 2) | 0x8000;
            D_800814A0 |= 0x8000;
        }
        return;
    }
}

extern u8 D_80071E44[];

UnkNode *func_8003FC64(s32);
u8 *func_8003FD64(s32, u8 *);
void func_8003F540(u32, u32, u32, u32);
s16 func_80053DA8(u16);
s32 func_80033B2C(s32);
void func_80052144(u8 *);
void func_80051708(u8 *, s32);
void func_80051BFC(u8 *, s32);
void func_80051E94(u8 *, s32);
void func_800520B4(u8 *, s32);
void func_80053374(u8 *, s32);

/* The handle takes a BLOCK-SCOPED local in each of the five slots, not one
   shared `h` across all of them. As one variable it is a single allocno with
   twenty references over a short total live length, which outranks `s` -- the
   sub-struct base, thirty references but live across the whole body -- in
   global-alloc's priority, so it takes $s0 and `s` takes $s1, the reverse of
   the listing. Five separate allocnos each lose to `s`. 45 DIFF either way at
   the exact word count, every word of it those two registers. */
void func_800525D4(void)
{
    UnkNode *n = func_8003FC64(0);
    u8 *s;

    if (n != NULL)
    {
        s = (u8 *)(n + 1);
        n->unk_10 = (u32)func_80052144;
        *(u16 *)(n + 1) = 0;
        *(u16 *)(s + 2) = 0;
        *(u16 *)(s + 4) = 0;
        *(u16 *)(s + 6) = 0;
        *(u32 *)(s + 8) = (u32)D_80071E44;
        *(u32 *)(s + 0xC) = 0;
        *(u32 *)(s + 0x10) = 0;
        *(u32 *)(s + 0x14) = 0;
        *(u32 *)(s + 0x18) = 0;
        *(u32 *)(s + 0x1C) = 0;
        func_8003F540(0, 0x15B7C, 0, 0x10093C8);
        func_80053DA8(0x300);
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_80051708(h, (s32)s);
                *(u32 *)(s + 0xC) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
        if (func_80033B2C(0x1392) == 0)
        {
            {
                u8 *h = func_8003FD64(0x212, (u8 *)n);

                if (h != NULL)
                {
                    func_80051BFC(h, (s32)s);
                    *(u32 *)(s + 0x10) = (u32)h;
                    *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
                }
            }
        }
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_80051E94(h, (s32)s);
                *(u32 *)(s + 0x14) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_800520B4(h, (s32)s);
                *(u32 *)(s + 0x18) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_80053374(h, (s32)s);
                *(u32 *)(s + 0x1C) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
    }
}

/* Three stacked LINE_F2 primitives -- a bright one at y and two dimmed ones at
   y-1 and y+1 -- with the brightness taken from the fade step `n`. Six
   spellings, and every one of them was worth words:

   * `n` is used DIRECTLY, with no `s32` copy. An `s16` parameter's
     sign-extension is emitted at its first integer USE, not by assign_parms,
     so it lands after the `&D_80083160` materialisation, which is the
     listing's order. `s32 k = n;` as the first statement puts the `sll`/`sra`
     at the top and the `lui`/`addiu` behind it -- four words, and the only
     thing left after everything else was right.
   * TWO primitive-pointer variables: `p` for the first line and `q` for the
     second and third. One variable is one pseudo and one register; the
     listing has $t0 for the first and $a2 shared by the other two.
   * the dimmed colours are assigned back into `c` and `d`. With separate
     locals there are nine long-lived quantities and the allocation spills
     into $a0/$a2/$a3, where the listing keeps seven in $t0..$t6 by letting
     each colour die into its dimmed value (34 differing words).
   * the dimming is `((u32)(c / 3) << 16) >> 15`, which is the listing's
     `sll 16` / `srl 15`. `(c / 3) * 2` makes the second shift arithmetic
     (`sra`), and `(u16)(c / 3) * 2` comes out as `andi 0xFFFF` / `sll 1`.
   * `d` is computed AFTER the `c == 0x100` test, so reorg has it to put in
     the `bne`'s delay slot; before the test, the slot takes a nop.
   * the four coordinate stores are written out in BOTH arms of each
     `if (x > 0)`. Cross-jumping merges the three identical trailing `sh`s
     into the join and leaves the differing `sh 0x8` and the duplicated
     `x + w - 1` in each arm, which is exactly what the listing has. */
void func_80052774(s16 x, s16 y, s16 w, s16 n)
{
    Unk80083160 *s = &D_80083160;
    u8 *p;
    u8 *q;
    s16 c;
    s16 d;

    if (n < 0x11)
    {
        c = n * 8 + 0x80;
        if (c == 0x100)
        {
            c = 0xFF;
        }
        d = n * 8 + 0x40;
    }
    else
    {
        c = 0x200 - n * 16;
        d = 0x100 - n * 8;
    }
    p = *(u8 **)(s->unk_00 + 0x8D0);
    *(u8 **)(s->unk_00 + 0x8D0) = p + 0x10;
    p[3] = 3;
    p[7] = 0x40;
    p[4] = d;
    p[5] = d;
    p[6] = c;
    if (x > 0)
    {
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 0xA) = y;
        *(s16 *)(p + 0xC) = x + w - 1;
        *(s16 *)(p + 0xE) = y;
    }
    else
    {
        *(s16 *)(p + 8) = x;
        *(s16 *)(p + 0xA) = y;
        *(s16 *)(p + 0xC) = x + w - 1;
        *(s16 *)(p + 0xE) = y;
    }
    addPrim(s->unk_00 + 0x230, p);
    c = ((u32)(c / 3) << 16) >> 15;
    d = ((u32)(d / 3) << 16) >> 15;
    q = *(u8 **)(s->unk_00 + 0x8D0);
    *(u8 **)(s->unk_00 + 0x8D0) = q + 0x10;
    q[3] = 3;
    q[7] = 0x42;
    q[6] = c;
    q[4] = d;
    q[5] = d;
    if (x > 0)
    {
        *(s16 *)(q + 8) = 0;
        *(s16 *)(q + 0xA) = y - 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y - 1;
    }
    else
    {
        *(s16 *)(q + 8) = x;
        *(s16 *)(q + 0xA) = y - 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y - 1;
    }
    addPrim(s->unk_00 + 0x230, q);
    q = *(u8 **)(s->unk_00 + 0x8D0);
    *(u8 **)(s->unk_00 + 0x8D0) = q + 0x10;
    q[3] = 3;
    q[7] = 0x42;
    q[4] = d;
    q[5] = d;
    q[6] = c;
    if (x > 0)
    {
        *(s16 *)(q + 8) = 0;
        *(s16 *)(q + 0xA) = y + 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y + 1;
    }
    else
    {
        *(s16 *)(q + 8) = x;
        *(s16 *)(q + 0xA) = y + 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y + 1;
    }
    addPrim(s->unk_00 + 0x230, q);
}

/* The counter is consumed and incremented in one expression at the TOP of the
   body (`i++ << 4`), which is where the target's `addiu $a0, $a0, 1` sits --
   as a `for`'s increment it comes out last (12 DIFF). The ten stores go
   through a walked pointer, which loop.c splits into the two induction
   variables the listing has: one at +0 and one biased to +0xB. */
void func_80052A20(void) {
    u8 *p = D_80084478;
    s32 i = 0;

    do {
        u8 v = i++ << 4;

        p[0] = 0xC0;
        p[1] = 0x2C;
        p[2] = 0;
        p[3] = 0;
        *(s16 *) (p + 4) = 0xE;
        *(s16 *) (p + 6) = 0x7080;
        p[8] = 0;
        p[9] = v;
        p[0xA] = 0xFC;
        p[0xB] = 0x10;
        p += 0xC;
    } while (i < 0x10);
}

/* Blits a Shift-JIS string one 3x16 cell at a time: maps each two-byte code
   to a font index, pulls the cell out of VRAM at 0x340 with StoreImage and
   puts it back at (x, y) with LoadImage.

   Five shapes, and the last of them is the whole reason this was parked at
   one word:

   * The flag is 32 bits WIDE and tested through a NARROW local. The listing
     spends `addu $v0, $s3, $zero` and then `beqz $v0` -- a plain reg-to-reg
     copy of the flag into the branch's register. `u8 ok` gives
     `andi $v0, $s3, 0xFF` (the right count, the wrong opcode); `u32 ok`
     tested directly gives `beqz $s3` alone and the function is a word short;
     and `u32 t = ok;` is copy-propagated away. `u8 t = ok;` is
     `(set (reg:QI t) (subreg:QI (reg:SI ok)))`, which cse's `insert` does not
     recognise as a register copy -- it looks for a REG of the same mode in
     the source's class -- so nothing merges the quantities, and MIPS emits a
     QImode reg-reg move as `addu`. `s8` and `u16` do the same thing. See the
     entry in docs/matching-levers.md.

   * The last three arms of the code chain are a SWITCH, not two more
     `else if`s, with `default:` written FIRST. As an if/else chain each body
     follows its own test and the `beq` comes out as a `bne`; as a two-case
     switch gcc emits both equality tests and then the bodies in source
     order, which is the listing's default, 0xE, 0xD layout.

   * The first and fourth arms produce the SAME value (`c + 0x7DC1`), which is
     why the listing has one `j` serving both -- cross-jumping merged them, so
     both arms have to be written out.

   * `c` is a `u16`, so each use as an int is a zero-extension; the three
     equality tests share one, and the rect computation gets its own through
     `d = c;`. Without that local gcc folds the first mask into `andi 7` and
     re-extends for the shift.

   * Both RECT fills are written out in full, w and h included, even though
     both are 3 and 0x10: the listing re-stores $s5 and $s4 the second time,
     and loop.c hoists both constants into the preheader. */
void func_80052A90(u32 s, s32 x, s16 y)
{
    RECT r;
    u8 buf[0x80];
    u32 ok = 1;
    u8 t;
    u16 c;
    u32 d;

    while (*(u8 *) s != 0)
    {
        c = (*(u8 *) s << 8) | *(u8 *) (s + 1);
        s += 2;
        if (c == 0x8140)
        {
            c = 0;
        }
        else if ((u16) (c + 0x7DA0) < 0x1A)
        {
            c = c + 0x7DC1;
        }
        else if ((u16) (c + 0x7D7F) < 0x1A)
        {
            c = c + 0x7DC0;
        }
        else if ((u16) (c + 0x7DB1) < 0xA)
        {
            c = c + 0x7DC1;
        }
        else
        {
            switch (c)
            {
            default:
                ok = 0;
                break;
            case 0x8144:
                c = 0xE;
                break;
            case 0x817C:
                c = 0xD;
                break;
            }
        }
        t = ok;
        if (t != 0)
        {
            d = c;
            r.x = (d & 7) * 3 + 0x340;
            r.y = (d >> 3) << 4;
            r.w = 3;
            r.h = 0x10;
            StoreImage(&r, (u_long *) buf);
            r.x = x;
            r.y = y;
            r.w = 3;
            r.h = 0x10;
            LoadImage(&r, (u_long *) buf);
            DrawSync(0);
        }
        x += 3;
    }
}

void func_80052A90(u32, s32, s16);

void func_80052C10(s16 k, u8 *p) {
    RECT r1;
    RECT r2;

    r1.x = 0x380;
    r1.y = k * 16;
    r1.w = 0x40;
    r1.h = 0x10;
    ClearImage(&r1, 0, 0, 0);
    r2.x = 0x3C0;
    r2.y = k * 16;
    r2.w = 0x40;
    r2.h = 0x10;
    ClearImage(&r2, 0, 0, 0);
    func_80052A90(*(u32 *) (p + 8), 0x380, k * 16);
    func_80052A90(*(u32 *) (p + 0xC), 0x3C0, k * 16);
}
