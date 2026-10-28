#include "common.h"

/* The second of the four pieces the carved unit unk_1C540 is cut into, split
   off at func_80052CE0 so that its jtbl_8002EFF0 gets a `.rodata` of its own.
   A translation unit emits ONE contiguous `.rodata`, and the carved unit owns
   four separated runs of jump tables -- see the table in the jump-table
   section of docs/open-matching-problems.md for which piece carries which.

   Note that jtbl_8002EFF0 is the SECOND table of its run: jtbl_8002EFDC
   belongs to func_80052144, which is still INCLUDE_ASM back in
   csrc/unk_1C540.c, so the run is placed from 0x27f0 and its first five
   words stay raw in a `bin`. When func_80052144 lands it wants a cut of its
   own and this piece's segment moves back to 0x27dc.

   Everything above the first function is copied verbatim from the head of
   csrc/unk_1C540.c plus every file-scope declaration that stood above this
   point in the undivided file. The `.size` records are file-scope and change
   every access to those symbols in whichever unit they appear, so all four
   pieces carry the whole list. */

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

void func_8004D294(u16 *, u8 *, s32);

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

extern u8 D_80071E44[];

UnkNode *func_8003FC64(s32);

u8 *func_8003FD64(s32, u8 *);

void func_8003F540(u32, u32, u32, u32);

void func_80052144(void);

void func_80051708(u8 *, s32);

void func_80051BFC(u8 *, s32);

void func_80051E94(u8 *, s32);

void func_800520B4(u8 *, s32);

void func_80053374(u8 *, s32);

void func_80052A90(u32, s32, s16);

void func_80052774(s16, s16, s16, s16);

/* The eight-state curtain/wipe driver, dispatched through jtbl_8002EFF0 on
   +0x00 while +0x02 counts frames. States 0..3 all end in the same
   `func_80052774(+0x10, +0x0C + 0xF, +0x0E, 0)` call, written out in each and
   cross-joined by jump2; 5 and 6 end without one; 7 clears the block and sets
   the owner's +0x14 bit that 0 cleared.

   The subject goes through an `s32` local READ BEFORE the +0x02 increment,
   because the listing's `lh 0x0` precedes the `sh 0x2` and sched cannot move
   a load across a store it must assume aliases it.

   State 4 is the one with any content, and the thing to know is that +0x04 is
   read as BOTH types on purpose. The `- 0xF` reads `*(u16 *)(p + 4)` and the
   doubling reads `*(s16 *)(p + 4)`: sign_extend and zero_extend of the same
   HImode MEM are two expressions to cse, which is why the listing has `lh`
   and `lhu` of +0x04 side by side in the branch that also COMPARES it. In the
   other branch there is no comparison, so the doubling is written
   `(s16) *(u16 *)(p + 4) * 2` and the single `lhu` serves both uses, which is
   the listing's `sll 16` / `sra 15`. Both calls read `*(u16 *)(p + 2)` for
   the last argument and cse decides the rest: after MoveImage it reloads,
   before it forwards the value just stored. */
void func_80052CE0(u8 *p, u8 *q, u8 *e)
{
    RECT rect;
    s32 s;
    s32 t;

    s = *(s16 *)(p + 0);
    *(u16 *)(p + 2) = *(u16 *)(p + 2) + 1;
    switch (s)
    {
    case 0:
        if (*(s16 *)(p + 6) == 1)
        {
            *(u16 *)(p + 2) = 0;
            *(s16 *)(p + 6) = 2;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
            *(u16 *)(e + 0x14) = *(u16 *)(e + 0x14) & 0xFF7F;
            *(s32 *)(q + 0) = *(s16 *)(p + 0xA) << 16;
            *(s32 *)(q + 4) = *(s16 *)(p + 0xC) << 16;
            func_80052774(*(s16 *)(p + 0x10), *(u16 *)(p + 0xC) + 0xF, *(s16 *)(p + 0xE), 0);
        }
        return;
    case 1:
        *(u16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 6;
        if (*(s16 *)(p + 2) >= 0x10)
        {
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        func_80052774(*(s16 *)(p + 0x10), *(u16 *)(p + 0xC) + 0xF, *(s16 *)(p + 0xE), 0);
        return;
    case 2:
        *(u16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 6;
        *(u16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 8;
        if (*(s16 *)(p + 2) >= 0x30)
        {
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        *(s32 *)(q + 0) = *(s16 *)(p + 0xA) << 16;
        func_80052774(*(s16 *)(p + 0x10), *(u16 *)(p + 0xC) + 0xF, *(s16 *)(p + 0xE), 0);
        return;
    case 3:
        if (*(s16 *)(p + 2) >= 0x50)
        {
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        func_80052774(*(s16 *)(p + 0x10), *(u16 *)(p + 0xC) + 0xF, *(s16 *)(p + 0xE), 0);
        return;
    case 4:
        t = *(u16 *)(p + 2) + 1;
        *(u16 *)(p + 2) = t;
        if ((s16) t >= 2)
        {
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
            rect.x = 0x3C0;
            rect.y = (*(s16 *)(p + 8) + 1) * 0x10 - *(u16 *)(p + 4);
            rect.w = 0x3F;
            rect.h = 1;
            MoveImage(&rect, 0x380, (*(s16 *)(p + 8) + 1) * 0x10 - *(s16 *)(p + 4));
            if (*(s16 *)(p + 4) < 0x10)
            {
                func_80052774(*(s16 *)(p + 0x10), *(u16 *)(p + 0xC) - (*(u16 *)(p + 4) - 0xF),
                              *(s16 *)(p + 0xE), *(u16 *)(p + 2) + *(s16 *)(p + 4) * 2);
            }
            else
            {
                *(u16 *)(p + 2) = 0;
                *(u16 *)(p + 4) = 0;
                *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
            }
            return;
        }
        func_80052774(*(s16 *)(p + 0x10), *(u16 *)(p + 0xC) - (*(u16 *)(p + 4) - 0xF),
                      *(s16 *)(p + 0xE), *(u16 *)(p + 2) + (s16) *(u16 *)(p + 4) * 2);
        return;
    case 5:
        if (*(s16 *)(p + 2) >= 0x50)
        {
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        return;
    case 6:
        *(u16 *)(p + 0xA) = *(u16 *)(p + 0xA) - 8;
        if (*(s16 *)(p + 2) >= 0x30)
        {
            *(u16 *)(p + 2) = 0;
            *(u16 *)(p + 0) = *(u16 *)(p + 0) + 1;
        }
        *(s32 *)(q + 0) = *(s16 *)(p + 0xA) << 16;
        return;
    case 7:
        *(u16 *)(e + 0x14) = *(u16 *)(e + 0x14) | 0x80;
        *(s16 *)(p + 0) = 0;
        *(s16 *)(p + 2) = 0;
        *(s16 *)(p + 4) = 0;
        *(s16 *)(p + 6) = 0;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 0xC) = 0;
        *(s16 *)(p + 0xE) = 0;
        *(s16 *)(p + 0x10) = 0;
        return;
    }
}

void func_80052CE0(u8 *, u8 *, u8 *);
void func_80044BB0(void);

/* The 0x808080 store comes FIRST of the five through `e`: anywhere else and
   the `ori` completing that constant is scheduled before the handler
   pointer's lui/addiu instead of after the `addiu $v0, $s0, 0x20` (6 DIFF one
   slot later, 22 two). */
void func_80052FE8(u8 *p, s16 k) {
    u8 *q = p + 0x20;
    u8 *e;
    u8 *f;

    *(u32 *) (p + 0x10) = (u32) func_80052CE0;
    *(u16 *) (p + 0x20) = 0;
    *(u16 *) (q + 2) = 0;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 0xC) = 0;
    *(u16 *) (q + 0xE) = 0;
    *(u16 *) (q + 8) = k;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 0xC) = 0x808080;
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    *(u32 *) (e + 8) = (u32) (D_80084478 + k * 12);
    *(u16 *) (e + 0x14) = *(u16 *) (e + 0x14) | 0x80;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    f = *(u8 **) (p + 8);
    *(u32 *) (f + 0) = 0;
    *(u32 *) (f + 4) = 0;
    *(u32 *) (f + 8) = 0x600000;
}

void func_8005313C(u8 *);

void func_800530A4(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_8005313C) {
        *(u16 *) (p + 0x2A) = 1;
    }
}

/* Both read-modify-writes in the loop are split into a read, a read, then the
   two writes: cc1psx fills a load's delay slot with the insn that follows it
   in the source and nothing else, so the global's `lw` needs the entry's `lhu`
   next (17 DIFF with the two `|=` written whole, one nop each).

   The last `D_800814A0 |= 0x8000` re-uses the value the loop left in the
   register -- section 5's exception, since the global's address does not
   vary. */
void func_800530C4(u8 *p) {
    u8 **b = (u8 **) (p + 0x20);
    s32 i;

    *(u32 *) (*b + 0x1C) = 0;
    *(u16 *) (*b + 6) += 1;
    for (i = 0; i < 0x10; i++) {
        u8 *e = b[i + 4];
        u32 g = D_800814A0;
        u16 f = *(u16 *) (e + 0x1E);

        D_800814A0 = g | 0x8000;
        *(u16 *) (e + 0x1E) = f | 0x8000;
    }
    *(u16 *) ((u8 *) b - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* The three-state tick installed at +0x10. State 1 walks the 0x10-byte
   command records at +0x0C, claiming a free slot from the sixteen at +0x10 for
   each one whose frame number matches, and advances the state when the record
   list ends; state 0 just advances it once +0x0A is set; state 2 waits out
   0x190 frames and then marks all sixteen slots and D_800814A0 dirty.

   Two shapes, 30 DIFF down to a match:

   * `*(s16 *) (p + 8)` is STORED and then read back for the slot index. cse
     forwards the store, so the read is the stored register plus a HImode
     sign-extension, and combine folds that into the listing's single
     `sll 16` / `sra 14` with the `* 4`. Spelled through a local the `* 4` is
     a plain `sll 2`.

   * In state 2's loop the slot's +0x1E halfword is read into a LOCAL before
     the `D_800814A0 |= 0x8000` store. A store to a global and a load through
     a pointer are a real dependence to sched, so written in the obvious order
     the second load cannot rise into the first load's delay slot and the
     function is a word long with eleven registers permuted behind it. This is
     advise.py's DELAY-NOPS lever, and it named it from the nop count alone.

   The trailing `D_800814A0 |= 0x8000;` after the loop is one word, not five:
   cse still has the value the loop's last iteration stored, so only the `sw`
   survives. */
void func_8005313C(u8 *p)
{
    u8 *q;
    u8 *t;
    s32 n;
    s32 w;
    s32 i;
    u32 k;

    n = *(u16 *) (p + 6) + 1;
    *(u16 *) (p + 6) = n;
    switch (*(s16 *) (p + 4))
    {
    case 0:
        if (*(s16 *) (p + 0xA) != 0)
        {
            *(s16 *) (p + 0xA) = 0;
            *(u16 *) (p + 6) = 0;
            *(u16 *) (p + 4) = *(u16 *) (p + 4) + 1;
        }
        break;
    case 1:
        if (**(s16 **) (p + 0xC) == (s16) n)
        {
            do
            {
                *(s16 *) (p + 8) = (*(s16 *) (p + 8) + 1) % 16;
                q = *(u8 **) (p + *(s16 *) (p + 8) * 4 + 0x10) + 0x20;
                if (*(s16 *) (q + 6) == 0)
                {
                    *(s16 *) (q + 6) = 1;
                    *(s16 *) (q + 0xA) = *(u16 *) (*(u8 **) (p + 0xC) + 2) - 0x180;
                    *(s16 *) (q + 0x10) = *(u16 *) (*(u8 **) (p + 0xC) + 2) - 0x180;
                    *(s16 *) (q + 0xC) = *(u16 *) (*(u8 **) (p + 0xC) + 4);
                    *(s16 *) (q + 0xE) = *(s16 *) (*(u8 **) (p + 0xC) + 6) * 12 + 0xC;
                    func_80052C10(*(s16 *) (p + 8), *(u8 **) (p + 0xC));
                }
                t = *(u8 **) (p + 0xC);
                *(u8 **) (p + 0xC) = t + 0x10;
                w = *(s16 *) (t + 0x10);
                if (w == 0)
                {
                    *(u16 *) (p + 6) = 0;
                    *(u16 *) (p + 4) = *(u16 *) (p + 4) + 1;
                    return;
                }
            } while (w == *(s16 *) (p + 6));
        }
        break;
    case 2:
        if ((s16) n >= 0x190)
        {
            *(u32 *) (*(u32 *) p + 0x1C) = 0;
            *(u16 *) (*(u32 *) p + 6) = *(u16 *) (*(u32 *) p + 6) + 1;
            i = 0;
            do
            {
                u8 *r = *(u8 **) (p + i * 4 + 0x10);

                k = *(u16 *) (r + 0x1E);
                D_800814A0 |= 0x8000;
                *(u16 *) (r + 0x1E) = k | 0x8000;
                i++;
            } while (i < 0x10);
            *(u16 *) (p - 2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

extern u8 D_80071A84[];
void func_8005313C(u8 *);
u8 *func_8003FD64(s32, u8 *);
void func_80052FE8(u8 *, s16);
void func_80052A20(void);

void func_80053374(u8 *p, s32 a) {
    u8 *q = p + 0x20;
    s32 i;

    *(u32 *) (p + 0x10) = (u32) func_8005313C;
    *(u32 *) (p + 0x20) = a;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 8) = 0;
    *(u16 *) (q + 0xA) = 0;
    *(u32 *) (q + 0xC) = (u32) D_80071A84;
    for (i = 0; i < 0x10; i++) {
        u8 *e = func_8003FD64(0x212, p);

        if (e != 0) {
            func_80052FE8(e, i);
            *(u32 *) (q + i * 4 + 0x10) = (u32) e;
        }
    }
    func_80052A20();
}

/* func_80053604's twin, the fade-IN: the grey level is `0x100 - (p->unk_02 <<
   8) / p->unk_04` instead of the quotient itself, the guard returns 1 when
   +0x02 has caught up with +0x04, and +0x02 is advanced at the end rather
   than conditionally. Every spelling in func_80053604's comment applies here
   unchanged -- `col` reused for the quotient, the inner grouping of the grey
   ramp explicit, the eight vertex halfwords in value order, `m = 1` then the
   override, and `addPrim` for the two OT links. */
s32 func_80053428(Unk80083120 *p)
{
    Unk80083160 *s;
    u8 *q;
    u8 *r;
    s32 m;
    u32 col;

    s = &D_80083160;
    if (p->unk_02 >= p->unk_04)
    {
        return 1;
    }
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = q + 0xC;
    r = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = r + 0x18;
    m = 1;
    if (p->unk_06 == 0)
    {
        m = 2;
    }
    func_80067F20((u32 *)q, 0, 0, GetTPage(0, m, 0, 0), 0);
    if (p->unk_02 == 0)
    {
        col = 0xFFFFFF;
    }
    else
    {
        col = 0x100 - ((p->unk_02 << 8) / p->unk_04);
        col = col + ((col << 8) + (col << 16));
    }
    *(u32 *)(r + 4) = col;
    SetPolyF4((POLY_F4 *)r);
    SetSemiTrans(r, 1);
    *(s16 *)(r + 0x14) = 0x140;
    *(s16 *)(r + 0x10) = 0x140;
    *(s16 *)(r + 0xC) = 0;
    *(s16 *)(r + 8) = 0;
    *(s16 *)(r + 0x12) = 0;
    *(s16 *)(r + 0xA) = 0;
    *(s16 *)(r + 0x16) = 0xE0;
    *(s16 *)(r + 0xE) = 0xE0;
    addPrim(s->unk_00 + 0x70, r);
    addPrim(s->unk_00 + 0x70, q);
    p->unk_02 = p->unk_02 + 1;
    return 0;
}

/* The screen fade quad: allocate a 0xC-byte tpage primitive and a 0x18-byte
   POLY_F4 out of the +0x8D0 bump allocator, build a full-screen semi-transparent
   quad whose grey level tracks +0x02 against +0x04, and link both into the OT
   at +0x70. Five spellings:

   * `col` is REUSED for the quotient. With a separate `k` local the two are
     different quantities and land in $v0 and $v1, where the listing has the
     `mflo` and the final `addu` both writing $a1.
   * the grey ramp is `col + ((col << 8) + (col << 16))`, with the inner
     grouping explicit. `col * 0x10101` makes synth_mult pick the three-op
     factorisation `((k << 8) + k) << 8) + k`, and the left-grouped
     `col + (col << 8) + (col << 16)` adds `k` first -- both are the same four
     instructions in the wrong order.
   * the eight vertex halfwords go in the LISTING's order, grouped by value
     (0x14, 0x10, then the four zeros, then 0x16, 0xE). In x0,y0,x1,y1 order
     both constants are live at once, so they take two registers instead of
     one and the whole tail shifts.
   * `m = 1; if (p->unk_06 == 0) { m = 2; }` -- an init and an override, not an
     if/else: the listing sets $a1 to 1 before the `lh` and only the 2 is in a
     branch. And the 0xFFFFFF arm is written FIRST, since the `bnez` into the
     divide makes it the fall-through.
   * the two OT links are `addPrim` from libgpu.h, i.e. P_TAG bitfield stores.
     A hand-written `|` cannot match -- see the landed func_800453E0 in
     csrc/gprel_18B40.c for the expand_binop reason. */
s32 func_80053604(Unk80083120 *p)
{
    Unk80083160 *s;
    u8 *q;
    u8 *r;
    s32 c = 0;
    s32 m;
    u32 col;

    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = q + 0xC;
    r = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = r + 0x18;
    s = &D_80083160;
    m = 1;
    if (p->unk_06 == 0)
    {
        m = 2;
    }
    func_80067F20((u32 *)q, 0, 0, GetTPage(0, m, 0, 0), 0);
    if (p->unk_02 > p->unk_04)
    {
        c = 1;
    }
    if (p->unk_02 >= p->unk_04)
    {
        col = 0xFFFFFF;
    }
    else
    {
        col = (p->unk_02 << 8) / p->unk_04;
        col = col + ((col << 8) + (col << 16));
    }
    if (c == 0)
    {
        p->unk_02 = p->unk_02 + 1;
    }
    *(u32 *)(r + 4) = col;
    SetPolyF4((POLY_F4 *)r);
    SetSemiTrans(r, 1);
    *(s16 *)(r + 0x14) = 0x140;
    *(s16 *)(r + 0x10) = 0x140;
    *(s16 *)(r + 0xC) = 0;
    *(s16 *)(r + 8) = 0;
    *(s16 *)(r + 0x12) = 0;
    *(s16 *)(r + 0xA) = 0;
    *(s16 *)(r + 0x16) = 0xE0;
    *(s16 *)(r + 0xE) = 0xE0;
    addPrim(s->unk_00 + 0x70, r);
    addPrim(s->unk_00 + 0x70, q);
    return c;
}

/* The fixed-width decimal formatter: fills buf[0..n-1] right to left with v's
   digits and then spaces, so `blank` latches once the quotient runs out.

   `v = q;` GOES BEFORE THE `q == 0` TEST, and that one line is three words.
   Written the other way round -- test, then assign -- `q`'s live range ends at
   the copy and `v`'s begins there, so the two never overlap, local-alloc ties
   them into one quantity, and the division writes straight into `v`'s register
   with nothing left for the branch's delay slot:

       built   subu $a0, $v1, $v0 ; bnez $a0 ; nop
       target  subu $v1, $v1, $v0 ; bnez $v1 ; addu $a0, $v1, $zero  (slot)

   With the copy FIRST, `q` is still live after it -- the test reads it -- so
   `q` and `v` CONFLICT, `q` cannot have $a0, and the copy survives into the
   slot. The `.greg` dump is where this is visible: the quotient's pseudo
   prefers $v0, $v1 AND $a0, the last inherited from the copy, and does not
   conflict with the parameter. See "two ways a copy disappears" in
   matching-levers section 3 -- this is the ALLOCATION kind, so no number of
   sets on `v` would have helped and the cure has to create a conflict.

   Also load-bearing, from the park: `u8 *d = buf + i;` in the loop body, which
   stops loop.c strength-reducing `buf[i]` into a walked pointer, and the digit
   arm FIRST (`if (blank == 0)`) rather than the space arm. Measured and worse:
   the digit store moved after the division (29 words), and testing `v` rather
   than `q` after the copy (3 DIFF). */

void func_800537D0(s32 v, s32 n, u8 *buf)
{
    s32 blank = 0;
    s32 i;
    s32 q;

    for (i = n - 1; i >= 0; i--)
    {
        u8 *d = buf + i;

        if (blank == 0)
        {
            *d = v % 10 + 0x30;
        }
        else
        {
            *d = 0x20;
        }
        q = v / 10;
        v = q;
        if (q == 0)
        {
            blank = 1;
        }
    }
}

u8 *func_8004E21C(u8 *, u8);

/* func_80053A88's sibling, one level further in: for every node in the -0x8
   chain it walks the NUL-terminated byte string at +0x04, turns each byte into
   a sprite through func_8004E21C and links it, advancing a running x by each
   glyph's width, then links one tpage primitive per node.

   `r = p` is a SECOND pointer for the body. The listing copies it in the
   guard branch's delay slot (`addu $s1, $s5, $zero`) and reads every field
   through it, keeping `p` for the string guard, the running x and the -0x8
   link. One variable gives one register and the function is three words
   short, because the extra callee-saved register is what the frame is sized
   for.

   `x` is an `s32` holding `*(s16 *) (p + 0x14)`, not an `s16` local: as an
   s16 the load comes out `lhu` and the running sum is done narrow.

   `f` is a SEPARATE variable for the tpage primitive, not `e` reused. With
   `e` reused the final `addPrim`'s mask lands in $v1; with its own variable
   the listing's in-place `and $s0, $s0, $s3` comes back, because the
   primitive pointer dies into its own mask. 5 words. */
s32 func_80053858(u8 *p)
{
    Unk80083160 *s = &D_80083160;
    u8 buf[4];
    u8 *e;
    u8 *f;
    u8 *q;
    u8 *r;
    s32 i;
    s32 x;

    while (1)
    {
        i = 0;
        x = *(s16 *) (p + 0x14);
        r = p;
        if ((*(u8 **) (p + 4))[0] != 0)
        {
            do
            {
                func_8004E21C(buf, (*(u8 **) (r + 4))[i]);
                e = *(u8 **) (s->unk_00 + 0x8D0);
                *(u8 **) (s->unk_00 + 0x8D0) = e + 0x14;
                *(s16 *) (e + 8) = x;
                *(s16 *) (e + 0xA) = *(u16 *) (r + 0x16);
                *(s16 *) (e + 0xE) = *(u16 *) (r + 0x1A);
                *(u32 *) (e + 4) = *(u32 *) (r + 0x10);
                e[0xC] = buf[0];
                e[0xD] = buf[1];
                *(s16 *) (e + 0x10) = buf[2];
                *(s16 *) (e + 0x12) = buf[3];
                x = x + buf[2];
                SetSprt((SPRT *) e);
                if ((*(u16 *) (r + 0x1C) & 1) != 0)
                {
                    SetSemiTrans(e, 1);
                }
                if ((*(u16 *) (r + 0x1C) & 2) == 0)
                {
                    addPrim(s->unk_00 + 0x70, e);
                }
                i++;
            } while ((*(u8 **) (r + 4))[i] != 0);
        }
        f = *(u8 **) (s->unk_00 + 0x8D0);
        *(u8 **) (s->unk_00 + 0x8D0) = f + 0xC;
        func_80067F20((u32 *) f, 0, 0, GetTPage(0, *(s16 *) (r + 0x18), 0x3C0, 0), 0);
        addPrim(s->unk_00 + 0x70, f);
        q = *(u8 **) (p - 8);
        if (q == 0)
        {
            break;
        }
        p = q + 0x20;
    }
    return 0;
}

/* Walks a linked list of tile records, emitting a TILE and a tpage primitive
   for each and linking both into the OT at +0x74. The loop is a `while (1)`
   with an explicit `break` on the null next-pointer, NOT a `do`/`while (q !=
   0)`: the two differ only in which register `q` gets -- $a0 for the break
   form, $v0 for the tested-condition form -- and that is the last three
   words. */
s32 func_80053A88(u8 *p)
{
    Unk80083160 *s = &D_80083160;
    u8 *e;
    u8 *q;

    while (1)
    {
        e = *(u8 **)(s->unk_00 + 0x8D0);
        *(u8 **)(s->unk_00 + 0x8D0) = e + 0x10;
        *(s16 *)(e + 8) = *(u16 *)(p + 0xC);
        *(s16 *)(e + 0xA) = *(u16 *)(p + 0xE);
        *(s16 *)(e + 0xC) = *(u16 *)(p + 0x10);
        *(s16 *)(e + 0xE) = *(u16 *)(p + 0x12);
        *(u32 *)(e + 4) = *(u32 *)(p + 8);
        SetTile((TILE *)e);
        if ((*(u16 *)(p + 0x16) & 1) != 0)
        {
            SetSemiTrans(e, 1);
        }
        if ((*(u16 *)(p + 0x16) & 2) == 0)
        {
            addPrim(s->unk_00 + 0x74, e);
        }
        e = *(u8 **)(s->unk_00 + 0x8D0);
        *(u8 **)(s->unk_00 + 0x8D0) = e + 0xC;
        func_80067F20((u32 *)e, 0, 0, GetTPage(0, *(s16 *)(p + 0x14), 0, 0), 0);
        if ((*(u16 *)(p + 0x16) & 2) == 0)
        {
            addPrim(s->unk_00 + 0x74, e);
        }
        q = *(u8 **)(p - 8);
        if (q == 0)
        {
            break;
        }
        p = q + 0x20;
    }
    return 0;
}

void func_80053C3C(u8 *p) {
    func_8003E4FC(6, p, 0);
    func_8003F320();
}

void func_80053C6C(void) {
    u32 *b = (u32 *) D_8006ADBC;
    u32 v = b[1];

    *(u32 *) D_8006ADBC = v;
    if (v != 0) {
        func_80053C3C((u8 *) v);
    }
}

void func_80053C6C(void);

void func_80053CA4(void) {
    D_8006ADC0[0] = (u32) D_800812C8;
    func_80053C6C();
}

void func_80053C6C(void);

void func_80053CD0(void) {
    D_8006ADC0[0] = (u32) D_800812D0;
    func_80053C6C();
}

/* $gp group (D_80081568 and D_8008156C added to the unit's sdata_entries
   block; nothing else in the unit touches them). The pair is filled with an
   8-BYTE STRUCT COPY, which is what makes the following `D_80081568` read a
   real `lw` again: as two word stores cse keeps the value it just stored and
   the function is a word short. Verified in a scratch file carrying the block,
   since verify.py compiles standalone. */
void func_80053CFC(u32 *p, u32 b) {
    *(Unk80081568 *) &D_80081568 = *(Unk80081568 *) p;
    D_80081568 = (((D_80081568 + 0x7FF) >> 11) << 23) | (b & 0x7FFFFF);
    func_8003E4FC(6, (u8 *) &D_80081568, b);
    func_8003F320();
}

s32 func_80053D64(void) {
    u8 loc[8];

    if (CdSync(1, 0) == 2) {
        CdControl(0x10, 0, loc);
    }
    return CdPosToInt((CdlLOC *)loc);
}

s32 func_80055778(s32);

s16 func_80053DA8(u16 a) {
    return func_80055778(a);
}

void func_80054D64(s32);

void func_80053DCC(s32 a) {
    D_8008480C[0] = a;
    func_80054D64(a);
}

void func_800552C8(s32);

void func_80053DF0(s32 a) {
    D_8008480A[0] = a;
    func_800552C8(a);
}

void func_80053E14(s16 v) {
    D_80084808[0] = v;
}

void func_800552C8(s32);
void func_80054D64(s32);

/* The case bodies are emitted in SOURCE order, which the compare tree (built
   from the sorted values) does not show: case 4 first, then 1, then 2, which
   is why the `bne` for 4 falls straight into its own body. Written 2, 1, 4 the
   three blocks come out in the other order -- 18 DIFF with every instruction
   right. Both callees take the first parameter, which arrives in $a0 and is
   never moved. */
void func_80053E20(s32 a, s32 k) {
    switch (k) {
    case 4:
        D_8008480C[0] = a;
        func_80054D64(a);
        break;
    case 1:
        D_8008480A[0] = a;
        func_800552C8(a);
        break;
    case 2:
        D_80084808[0] = a;
        break;
    }
}

/* func_80053E20's getter, same case order. */
s16 func_80053E90(s32 k) {
    switch (k) {
    case 4:
        return D_8008480C[0];
    case 1:
        return D_8008480A[0];
    case 2:
        return D_80084808[0];
    }
    return 0;
}

extern u32 D_800848FC[];
extern u32 D_8008485C[];

/* A `switch`, not an if/else chain: both tests branch INTO their arm and the
   default is the fall-through `j`. And each masked flag goes into its OWN
   local before the `!= 0` -- written `return (f & 0x100) != 0;` combine turns
   the pair into `srl 8` / `andi 1` where the target has `andi 0x100` / `sltu`
   (4 DIFF; `> 0` and `!!` do the same). */
s32 func_80053EF0(s32 k) {
    u32 f;
    u32 g;

    switch (k) {
    case 1:
        f = D_800847D0[0];
        if ((f & 0x1000) != 0) {
            return 2;
        }
        if (D_800848FC[0] == 2) {
            return 3;
        }
        g = f & 0x100;
        return g != 0;
    case 4: {
        u8 *b = (u8 *) D_800847D0;

        if ((*(u32 *) (b + 4) & 0x200) != 0) {
            return 0x100;
        }
        f = D_800847D0[0];
        if ((f & 0x4000) != 0) {
            return 2;
        }
        if (D_8008485C[0] == 2) {
            return 3;
        }
        g = f & 0x400;
        return g != 0;
    }
    }
    return -1;
}

extern u8 D_80084548[];
void func_8005A2FC(void);
void func_8005A39C(u32);
s32 func_8005A3F0(u32);
/* func_8005A420 is an empty `(void)` function in this unit but is called here
   with three arguments; an __asm__ alias gives this one call site the real
   signature without touching that definition. */
void func_8005A420c(u8 *, s32, s32) __asm__("func_8005A420");
void func_8005ACD0(u16);
s16 func_8005B378(s16);
void func_8005B320(void);
void func_8005B3D8(s32, s32);
void func_8005A5C8(s16, s16);
void func_800559B4(void);
void func_80055864(void);
void func_800557BC(void);
void func_8005A37C(void);

void func_80053F98(void) {
    func_8005A2FC();
    func_8005A39C(1);
    func_8005A3F0(0x18);
    func_8005A420c(D_80084548, 1, 1);
    func_8005ACD0(0);
    func_8005B378(4);
    func_8005E7E0(4);
    func_8005B320();
    func_8005B3D8(0x3C, 0x3C);
    func_8005A5C8(0x7F, 0x7F);
    func_8005A4E8(0, 0, 0);
    func_8005A56C(0, 0, 0);
    func_80055B44(2);
    func_800559B4();
    func_80055864();
    func_800557BC();
    func_8005A37C();
}

/* Nested ifs with ONE return 0 at the end, and the 1 returned from the inner
   arm. Every shape that assigns a variable in the arms puts the result in a
   callee-chosen register with a `move $v0` at the end: a variable live to the
   single return is a global allocno, local-alloc has already given the second
   mask chain $v0, and the result is pushed to $a1. Returning directly makes
   hard reg $v0 live from each arm, so local-alloc routes the second chain
   around it ($v1/$a0) exactly as the target does. 13 DIFF.

   `r = 1` before the second test and `r = 0` in the arm is also what jump.c
   store-flags into one `sltu`, which three earlier spellings all hit. */
s16 func_8005405C(s16 n) {
    if ((D_800847D0[1] & (0x1000000 << n)) == 0) {
        if ((D_800847D0[0] & (0x10000 << n)) != 0) {
            return 1;
        }
    }
    return 0;
}

void func_800540A8(void) {
    u8 *p;

    func_8005A4E8(0, 0, 0);
    func_80055990(D_80084858);
    /* Assigned AFTER the calls: as an initialiser it lives across them in
       $s0, which costs the save, the restore and the frame slot (3 words). */
    p = (u8 *) D_800847D0;
    *(u32 *) (p + 0x8) = 0;
    *(u32 *) (p + 0xC) = 0;
    p[0x30] = 0;
    p[0x32] = 0;
    *(u32 *) (p + 0x10) = 0;
    *(u32 *) (p + 0x14) = 0;
    p[0x31] = 0;
    p[0x33] = 0;
    *(u32 *) (p + 0x18) = 0;
}

extern u8 D_80032E0C[];

/* UNPARKED. Copies the 8 bytes at D_80032E0C into a local `s16 t[4]` and then,
   for each of 0x30 outer steps and the first three entries of t, calls
   func_80055730 when func_8005405C accepts the entry.

   The inner loop is written with `goto`, which is what keeps it out of
   loop.c's hands: the C front end emits NOTE_INSN_LOOP_BEG only for a loop
   STATEMENT, so a goto back edge gets no strength reduction and no invariant
   motion. As a `for`, loop.c reduced `t[j]` to a walking cursor and hoisted
   `$sp + 0x10` into a register of its own -- two callee-saved registers and
   four words the target does not spend, where the listing recomputes
   `sll $v0, $s1, 1` / `addu $v0, $sp, $v0` / `lh 0x10($v0)` every iteration.
   Five subscript spellings had been measured against that and none of them
   reached it; the loop's own form is the lever.

   `j = 0` is the FIRST statement of the outer body, ahead of `hi` and `base`.
   Loop rotation leaves one copy in the preheader and one in the outer back
   edge's delay slot, which is where the listing has them; written after the
   two expressions, the delay slot takes `andi $v0, $s2, 0xFF` instead.

   `((u32) i & 0xFF) >> 4` unsigned, or the shift is `sra` where the listing
   has `srl`. */
void func_80054104(void)
{
    s16 t[4];
    s32 i;
    s32 j;

    memcpy(t, D_80032E0C, 8);
    for (i = 0; i < 0x30; i++)
    {
        s32 hi;
        s32 base;

        j = 0;
        hi = ((u32) i & 0xFF) >> 4;
        base = ((i & 0xF) << 10) + 0x1C00;
    inner:
        {
            s16 v = t[j];
            s32 k = ((v << 8) & 0xFF00) | hi;

            if (v == 0)
            {
                k += 0x10;
            }
            if (func_8005405C(v))
            {
                func_80055730(k, base);
            }
        }
        j++;
        if (j < 3)
        {
            goto inner;
        }
    }
}

void func_8005E7E0(s32);

void func_800541E8(void) {
    func_8005B300();
    func_8005B418(0);
    func_8005B348();
    func_8005E7E0(4);
    func_8005B320();
}

void func_800553D4(s32);
void func_80055C50(s16);
void func_8005B418(s32);
void func_8005A428(void);
void func_8005A4B8(void);

void func_80054228(void) {
    s16 i;

    if ((D_800847D0[0] & 0x100) != 0) {
        func_800553D4(0x71);
    }
    func_8005B418(0);
    for (i = 0; i < 4; i++) {
        func_80055C50(i);
    }
    func_8005A428();
    func_8005A4B8();
}

s32 func_800542A4(void) {
    return 0;
}

void func_800542AC(void) {
    D_800847F6[0] = -1;
}

extern u32 D_80084850[];

u16 func_800557C8(void);
void func_80054788(u16);
void func_80054B08(u16);
void func_8005500C(u16);
void func_8005552C(u16);
void func_800543C8(void);

/* The dispatch is a real `switch`, so gcc builds the balanced comparison tree
   the listing has (0x500 at the root, then 0x200, then 0x300/0 and
   0x700/0x600/0x800) rather than a chain. The four handler bodies are laid out
   in SOURCE order -- 0, 0x300, 0x200, then the group -- which is the order the
   labels come in.

   `func_8005552C`'s delay slot re-computes `v & 0xFFFF` where the three
   single-case arms inherit the mask from the loop guard: its block has four
   predecessors, so cse's extended basic block ends at that label and the
   value is not available there. Nothing to spell differently, but it is why
   one arm looks a word more expensive than its siblings. */
void func_800542BC(void)
{
    u16 v;

    D_80084850[0]++;
    for (;;)
    {
        v = func_800557C8();
        if (v == 0)
        {
            break;
        }
        switch (v & 0xF00)
        {
        case 0x000:
            func_80054788(v);
            break;
        case 0x300:
            func_80054B08(v);
            break;
        case 0x200:
            func_8005500C(v);
            break;
        case 0x500:
        case 0x600:
        case 0x700:
        case 0x800:
            func_8005552C(v);
            break;
        }
    }
    func_800543C8();
    func_800557BC();
}

void func_80055CF4(void);
void func_800544A4(void);
void func_8005440C(void);

void func_800543C8(void) {
    if ((D_800847D4 & 0xFF000000) != 0) {
        func_80055CF4();
    }
    func_800544A4();
    func_8005440C();
}

void func_800550E8(void);
void func_80054538(u8 *);
s32 func_800545F4(u8 *);
void func_800553D4(s32);
s32 func_80059D60(void);

void func_8005440C(void) {
    u8 *b = (u8 *) D_800847D0;

    if ((*(u32 *) (b + 4) & 2) != 0) {
        func_800550E8();
    }
    if ((D_800847D0[0] & 0x100) != 0) {
        if (func_80059D60() == 3 ||
            func_800545F4((u8 *) D_800848F8) == -1) {
            func_800553D4(0x71);
        }
    }
    func_80054538((u8 *) D_800848F8);
}

void func_80054538(u8 *);
void func_800546B0(void);

/* +0x04 through a `u8 *` base and +0x00 as `D_800847D0[0]`: that is the pair
   of forms the target has, one `lui` shared between the materialised base and
   the symbolic load. */
void func_800544A4(void) {
    u8 *b = (u8 *) D_800847D0;
    u32 f;
    u8 *c;

    if (*(u32 *) (b + 4) & 0x200) {
        func_80054704();
    }
    f = D_800847D0[0];
    if (f & 0x400) {
        if (f & 0x4000) {
            c = D_80084858;
            func_80054538(c);
            if (*(u32 *) (c + 0xC) == 3) {
                func_80054E00(0x74);
            }
        } else {
            func_800546B0();
        }
    }
}

void func_80054538(u8 *p) {
    switch (*(s32 *) (p + 0xC)) {
    case 1:
        *(u16 *) (p + 0x10) = *(u16 *) (p + 0x10) + *(u16 *) (p + 0x14);
        if (*(s16 *) (p + 0x10) >= 0x80) {
            *(u16 *) (p + 0x10) = 0x7F;
            *(s32 *) (p + 0xC) = 3;
        }
        break;
    case 2:
        *(u16 *) (p + 0x10) = *(u16 *) (p + 0x10) - *(u16 *) (p + 0x14);
        if (*(s16 *) (p + 0x10) < *(s16 *) (p + 0x12)) {
            *(u16 *) (p + 0x10) = *(u16 *) (p + 0x12);
            *(s32 *) (p + 0xC) = 3;
        }
        break;
    default:
        return;
    }
    if (*(u32 *) p != 0) {
        (*(void (**)(void)) p)();
    }
}

s32 func_800545F4(u8 *p) {
    switch (*(s32 *) (p + 4)) {
    case 1:
        *(u16 *) (p + 8) = *(u16 *) (p + 8) + *(u16 *) (p + 0x16);
        if (*(s16 *) (p + 0xA) < *(s16 *) (p + 8)) {
            *(u16 *) (p + 8) = *(u16 *) (p + 0xA);
            *(s32 *) (p + 4) = 3;
        }
        break;
    case 2:
        *(u16 *) (p + 8) = *(u16 *) (p + 8) - *(u16 *) (p + 0x18);
        if (*(s16 *) (p + 8) <= 0) {
            *(u16 *) (p + 8) = 0;
            return -1;
        }
        break;
    default:
        return 0;
    }
    if (*(u32 *) p != 0) {
        (*(void (**)(void)) p)();
    }
    return 0;
}

s32 func_800545F4(u8 *);
void func_80054E00(s32);

void func_800546B0(void) {
    if ((u32) func_80053D64() >= D_800847DC ||
        func_800545F4(D_80084858) == -1) {
        func_80054E00(0x74);
    }
}

s32 func_8003F5AC(void);
void func_80054E00(s32);
/* func_80054D64 is called here with NO argument -- $a0 still holds the ~0x200
   mask from the `&=` above and nothing sets it. An `__asm__` alias gives this
   one call site a (void) declaration while the rest of the unit keeps the
   `void(s32)` one; passing `~0x200` explicitly costs a word, because cse does
   not delete the second `li` of a constant into $a0. */
void func_80054D64v(void) __asm__("func_80054D64");
void func_8005A4E8(u8, u8, u8);

void func_80054704(void) {
    if ((func_8003F5AC() & 4) != 0) {
        D_800847D0[1] &= ~0x200;
        if (D_800847D0[4] != 0) {
            func_80054E00(0x74);
        } else if ((D_800847D0[0] & 0x4000) == 0) {
            D_800847D0[0] |= 0x400;
            func_80054D64v();
            func_8005A4E8(0, 0, 1);
        }
    }
}

/* The 0xF0-group dispatch. Three things decide it:

   * the case BODIES are laid out in the listing's order -- 0x10, 0x20, 0x70,
     0xE0, 0xF0, then the shared 0xB0/0xC0/0xD0 body, then the default -- which
     is not numeric order; gcc emits bodies in source order and builds the
     comparison tree separately (root 0xB0, the median of the eight labels).
   * the two D_800847D0 tests in the shared body are NESTED IFS, not one
     `&&` chain. `fold_truthop` merges two comparisons against one operand
     into a single masked compare, so `(x & 0x100) != 0 && (x & 0x1000) == 0`
     comes out as `andi 0x1100` against 0x100 -- two words per group, four in
     all. Nested `if`s are separate statements, which fold cannot reach, and
     cse still shares the one `lw`.
   * func_8005497C and func_800549FC are called with NO PROTOTYPE in scope --
     both are defined below this function and nothing declares them above it
     -- so the `u16` argument takes the default promotion to `int` and the
     listing's `andi $a0, 0xFFFF` is that zero-extension. Their own
     definitions take `u8`, which would mask with 0xFF instead. */
void func_800553D4(s32);
void func_80054104(void);
void func_80054E00(s32);
void func_80054F9C(s32, u8 *);

void func_80054788(u16 n)
{
    switch (n & 0xF0)
    {
    case 0x10:
        func_8005497C(n);
        break;
    case 0x20:
        func_800549FC(n);
        break;
    case 0x70:
        if ((n & 1) != 0)
        {
            func_800553D4(0x71);
        }
        if ((n & 2) != 0)
        {
            func_80054104();
        }
        if ((n & 4) != 0)
        {
            func_80054E00(0x74);
        }
        break;
    case 0xE0:
        if ((n & 1) != 0)
        {
            func_800553D4(0xE1);
        }
        if ((n & 4) != 0)
        {
            func_80054E00(0xE4);
        }
        break;
    case 0xF0:
        if ((n & 1) != 0)
        {
            func_800553D4(0xF1);
        }
        if ((n & 4) != 0)
        {
            func_80054E00(0xF4);
        }
        break;
    case 0xB0:
    case 0xC0:
    case 0xD0:
        if ((n & 1) != 0)
        {
            if ((D_800847D0[0] & 0x100) != 0)
            {
                if ((D_800847D0[0] & 0x1000) == 0)
                {
                    func_80054F9C(n & 0xFFF1, D_800848F8);
                }
            }
        }
        if ((n & 4) != 0)
        {
            if ((D_800847D0[0] & 0x400) != 0)
            {
                if ((D_800847D0[0] & 0x4000) == 0)
                {
                    func_80054F9C(n & 0xFFF4, D_80084858);
                }
            }
        }
        break;
    default:
        func_80054A7C(n & 0xFF);
        break;
    }
}

void func_800558FC(u16);
void func_80055BD8(s16);

/* Four cases that all call the same function with different constants;
   cross-jumping merges the four calls into the one the listing shows and reorg
   puts two of the four argument loads into delay slots. */
void func_8005497C(u8 k) {
    func_800558FC(k);
    switch (k) {
    case 0x11:
        func_80055BD8(0);
        break;
    case 0x12:
        func_80055BD8(1);
        break;
    case 0x14:
        func_80055BD8(2);
        break;
    case 0x18:
        func_80055BD8(3);
        break;
    }
}

void func_800553D4(s32);
void func_80055ADC(u8 *, u32);

/* A switch, not an if/else chain: two `beq`s that branch TO their bodies and a
   `j` past them, where the chain tests and falls into each body in turn (23
   DIFF). The shared `lui %hi(D_800847F0)` in the second test's delay slot is
   the other half of that reading. */
void func_800549FC(u8 k) {
    func_800553D4(0x71);
    switch (k) {
    case 0x21:
        D_800847F0[0] = 0;
        func_80055ADC(D_801BEE40, 0);
        break;
    case 0x22:
        D_800847F0[0] = 1;
        func_80055ADC(D_801C4640, 1);
        break;
    }
}

void func_80055B44(u16);
void func_800540A8(void);
void func_800541E8(void);

/* Cases 1 and 2 share a body and there is no default: {1,2} is a RANGE node,
   and after `!= 3`, `< 4` and `!= 0` nothing is left to test, so
   node_is_bounded holds for it and emit_case_nodes branches on the LEFT with
   `blt` instead of the usual `bgt` on the right -- which is the branch sense
   the listing shows. Written as a default, the test comes out the other way
   round and the trailing `j` goes to the default body instead of the
   epilogue. */
void func_80054A7C(u16 k) {
    switch (k & 0xFF) {
    case 1:
    case 2:
        func_80055B44(k);
        break;
    case 3:
        func_800540A8();
        break;
    case 4:
        func_800541E8();
        break;
    }
}

s32 func_80054AF0(s32 a) {
    if (a == 1) {
        return 0x5F;
    }
    return 0x7F;
}

INCLUDE_ASM("asm/nonmatchings/unk_264E0", func_80054B08);

void func_80054C58(void) {
    u32 *p = D_800847D0;
    u8 *q = D_80084858;
    u32 a = p[4];
    u32 b = p[5];
    u8 c = *((u8 *) p + 0x31);
    u8 d = *((u8 *) p + 0x33);

    p[4] = 0;
    p[5] = 0;
    *((u8 *) p + 0x31) = 0;
    *((u8 *) p + 0x33) = 0;
    *(u32 *) (q + 0xC) = 3;
    *(s16 *) (q + 0x10) = 0x80;
    p[2] = a;
    p[3] = b;
    *((u8 *) p + 0x30) = c;
    *((u8 *) p + 0x32) = d;
    D_800847D0[0] &= ~0x400;
    D_800847D0[0] &= ~0x4000;
    p[1] &= ~0x200;
}

void func_8005A4E8(u8, u8, u8);
void func_8005A56C(s16, s16, s16);

/* D_80084904 through the incomplete-array alias, so the compiler materialises
   its `%hi` into a register instead of letting the assembler spend $at -- and
   the store goes BEFORE the base assignment, which is what gets that `lui`
   emitted first. As `D_80084904 = 2;` on a scalar: `lui $at` + `sw 0($at)`,
   10 DIFF; with the base in its declaration, 9. */
void func_80054CD4(void) {
    u8 *b;

    D_80084904a[0] = 2;
    b = (u8 *) D_800847D0;
    b[0x2C] = b[0x32];
    b[0x2D] = b[0x30];
    *(u32 *) (b + 4) |= 0x200;
    func_8005A4E8(0, 0, 0);
    func_8005A56C(0, 0, 0);
    func_8003E4FC(0xE, 0, (s32) (b + 0x28));
    func_8003E4FC(0xD, *(u8 **) (b + 8), (s32) (b + 0x2C));
}

void func_8005A56C(s16, s16, s16);

/* `/ 32767`, not the 10922.56 that 2^(32+14)/0x180010003 estimates: the magic
   and the shift are chosen together, so invert the magic with
   tools/decomp/magic.py instead of dividing. The base is declared INSIDE the
   if so its lui/addiu land there rather than ahead of the guard. */
void func_80054D64(s32 a) {
    if ((D_800847D0[0] & 0x400) != 0) {
        u8 *b = D_80084858;
        s16 t = *(s16 *) (b + 8) * D_8008480C[0] / 32767;
        s16 v = t * *(s16 *) (b + 0x10) / 128;

        func_8005A56C(0, v, v);
    }
}

/* Each case materialises D_800847D0's base for ITSELF -- a `u8 *` local inside
   the case block, not one at function scope. At function scope the base is
   live across the 0x74 arm's two calls and takes a callee-saved register,
   where the listing creates it in $a2 after them.

   D_80084858 takes a pointer local too. Written as `*(u32 *)(D_80084858 + 4)`
   the first store goes out symbolically and gcc then materialises
   `D_80084858 + 4` as the base for the two halfword stores, so their offsets
   come out 4 and 6 where the listing has 8 and 0xA off the unbiased base --
   the same bias func_80054B08 is parked on. */
extern u32 D_80084864[];

void func_8005A4E8(u8, u8, u8);
s32 func_80054AF0(s32);
s32 func_80053D64(void);
void func_80054C58(void);
void func_80054CD4(void);

void func_80054E00(s32 n)
{
    switch (n & 0xFF)
    {
    case 0x74:
    {
        u8 *b;
        u8 *s;
        s32 v;

        func_8005A4E8(0, 0, 0);
        func_8003E4FC(9, NULL, 0);
        b = (u8 *)D_800847D0;
        D_800847D0[0] &= ~0x400;
        if (*(u32 *)(b + 0x18) == 0)
        {
            s = D_80084858;
            D_80084904a[0] = 1;
            *(u32 *)(s + 4) = 0;
            if (*(u32 *)(b + 0x10) != 0)
            {
                v = func_80054AF0(*(s16 *)(b + 0x1C));
                *(s16 *)(s + 8) = v;
                *(s16 *)(s + 0xA) = v;
                func_80054C58();
                func_80054CD4();
            }
        }
        break;
    }
    case 0xE4:
    {
        u8 *b = (u8 *)D_800847D0;

        if ((*(u32 *)(b + 4) & 0x200) != 0)
        {
            func_8003E4FC(9, NULL, 0);
        }
        else if ((D_800847D0[0] & 0x400) != 0)
        {
            u32 v = func_80053D64();

            *(u32 *)(b + 0x18) = v;
            if (*(u32 *)(b + 8) >= v)
            {
                *(u32 *)(b + 0x18) = *(u32 *)(b + 8);
            }
            D_80084864[0] = 2;
            *(u32 *)(b + 0x10) = *(u32 *)(b + 0x18);
            *(u32 *)(b + 0x14) = *(u32 *)(b + 0xC);
            b[0x31] = b[0x30];
            b[0x33] = b[0x32];
            D_800847D0[0] |= 0x4000;
        }
        break;
    }
    case 0xF4:
        if ((D_800847D0[0] & 0x4000) != 0)
        {
            *(u32 *)((u8 *)D_800847D0 + 0x18) = 0;
            func_80054C58();
            func_80054CD4();
        }
        break;
    }
}
