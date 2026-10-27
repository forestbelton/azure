#include "common.h"

/* The middle third of the carved unit unk_1C540, split off at func_80054F9C.
   Like csrc/unk_2CDC0.c this exists because a translation unit emits ONE
   contiguous `.rodata` and the carved unit owns three separated runs of jump
   tables: jtbl_8002E964 at 0x8002E964 (func_800517CC, in csrc/unk_1C540.c),
   jtbl_80032E14 and jtbl_80032EA4 at 0x80032E14 (func_80054F9C and
   func_80057A94, below), and jtbl_8003310C at 0x8003310C (func_800595C0, in
   csrc/unk_2CDC0.c). Three runs need three objects, and the boundaries are
   the function addresses the run order implies -- which is also the evidence
   that the original build had several translation units here. See the
   jump-table section of docs/open-matching-problems.md.

   Everything above the first function is copied verbatim from the head of
   csrc/unk_1C540.c plus every file-scope declaration that stood above this
   point in the undivided file. The `.size` records are file-scope and change
   every access to those symbols in whichever unit they appear, so all three
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

void func_80052CE0(void);

void func_8005313C(u8 *);

extern u8 D_80071A84[];

void func_80052FE8(u8 *, s16);

void func_80052A20(void);

u8 *func_8004E21C(u8 *, u8);

void func_80053C6C(void);

s32 func_80055778(s32);

void func_80054D64(s32);

void func_800552C8(s32);

extern u32 D_800848FC[];

extern u32 D_8008485C[];

extern u8 D_80084548[];

void func_8005A2FC(void);

void func_8005A39C(u32);

s32 func_8005A3F0(u32);

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

extern u8 D_80032E0C[];

void func_8005E7E0(s32);

void func_800553D4(s32);

void func_80055C50(s16);

void func_8005B418(s32);

void func_8005A428(void);

void func_8005A4B8(void);

extern u32 D_80084850[];

u16 func_800557C8(void);

void func_80054788(u16);

void func_80054B08(u16);

void func_8005500C(u16);

void func_8005552C(u16);

void func_800543C8(void);

void func_80055CF4(void);

void func_800544A4(void);

void func_8005440C(void);

void func_800550E8(void);

void func_80054538(u8 *);

s32 func_800545F4(u8 *);

s32 func_80059D60(void);

void func_800546B0(void);

void func_80054E00(s32);

s32 func_8003F5AC(void);

void func_80054D64v(void) __asm__("func_80054D64");

void func_8005A4E8(u8, u8, u8);

void func_80054104(void);

void func_80054F9C(s32, u8 *);

void func_800558FC(u16);

void func_80055BD8(s16);

void func_80055ADC(u8 *, u32);

void func_80055B44(u16);

void func_800540A8(void);

void func_800541E8(void);

void func_8005A56C(s16, s16, s16);

extern u32 D_80084864[];

s32 func_80054AF0(s32);

s32 func_80053D64(void);

void func_80054C58(void);

void func_80054CD4(void);

/* The channel's note-off / release shaper: forces the state word at +0x04 to
   2, snapshots the +0x08 level into +0x0A, and writes a release rate at +0x18
   chosen by the status byte through jtbl_80032E14. 36 entries over
   0xB1..0xD4, pairing 0xB1/0xB4 -> 0xF, 0xC1/0xC4 -> 4 and 0xD1/0xD4 -> 2
   against a default of 0xA -- read off the table, since the arms are
   otherwise indistinguishable.

   The first parameter stays `s32` with the mask INSIDE the switch
   (`switch (k & 0xFF)`): the callers in func_80054788 pass `n & 0xFFF1` and
   `n & 0xFFF4` and already match, and a `u8` parameter would move the
   zero-extension to entry and drop the `andi $v0, $a0, 0xFF` the listing has
   on the dispatch. The `if` writes +0x04 before +0x0A because the listing's
   `sw` precedes its `lhu`; the halfword copy is `*(s16 *) (q + 0xA) =
   *(u16 *) (q + 8)`, unsigned source and signed destination, which is a bare
   `lhu`/`sh` pair with no extension either side. */
void func_80054F9C(s32 k, u8 *q)
{
    s32 r;

    if (*(u32 *) (q + 4) != 2)
    {
        *(u32 *) (q + 4) = 2;
        *(s16 *) (q + 0xA) = *(u16 *) (q + 8);
    }
    switch (k & 0xFF)
    {
    case 0xB1:
    case 0xB4:
        r = 0xF;
        break;
    case 0xC1:
    case 0xC4:
        r = 4;
        break;
    case 0xD1:
    case 0xD4:
        r = 2;
        break;
    default:
        r = 0xA;
        break;
    }
    *(s16 *) (q + 0x18) = r;
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_8005500C);

extern u8 D_80084810[];

s16 func_8005405C(s16);
s32 func_8005B470(s16, u8 *);
void func_8005537C(s16);
s16 func_8005AE08(u32 *, s16);
void func_8005AE90(s16, u8, s16);
extern void func_800552C8b(void) __asm__("func_800552C8");

/* The turn-commit: validates the queued move at D_800847D0+0x26, charges its
   cost through func_8005537C, looks the move up in two parallel tables indexed
   by [+0x20][nibble of +0x26], starts its animation, clears the pending state,
   and sets up the follow-up in D_800848F8 or D_800848FC by the top nibble.

   D_80084878 holds POINTERS, not rows of data: the listing does `lw $a0, 0x0`
   off the computed slot and passes the loaded word on, hence `*(u32 **)`.

   Each lookup's ROW ADDRESS IS ITS OWN STATEMENT (`r = t + (x << 5); v =
   *(s16 *)(r + c * 2);`). As one expression the base is added after both
   terms are shifted -- `sll x; sll c; addu x+t; addu` where the listing has
   `sll x; addu x+t; sll c; addu` -- and every register name after it follows,
   since the pseudos are born in a different order; expr.c's INDIRECT_REF
   expands its address under EXPAND_SUM, so a nested PLUS comes back as an
   unforced rtx and both `addu`s are emitted when the address is finally
   forced. Reverting the first row local is 3 DIFF, the second 7, both 10; one
   row local shared by the two lookups is 121 words, 77 DIFF. The two tables
   likewise want SEPARATE pointer locals: one local serving both is two sets
   in two blocks, a global allocno whose `lui` and `addiu` land in different
   registers where the listing ties them (121 words, 79 DIFF).

   The D_800847D0 base is RE-MATERIALISED for the last call, inline as
   func_800553D4 does it, so one `lui $s0` serves the symbolic `|= 0x100` and
   the `addiu` rebuilds the base in the call's delay slot. Reading through the
   function-scope `b` is 16 DIFF ($s0 stays live from the lookups and nothing
   fills the delay slot); reassigning `b` before the `|= 0x100` puts the
   `addiu` between the `lw` and the `ori`, untied from the `lui`.

   The three-way `+0x16` store is a `switch` with `case 0x9000` written FIRST:
   gcc emits a two-case switch as `beq <first> -> its body` then
   `bne <second> -> default`, which is the listing's `beq 0x8000` /
   `bne 0x9000` with the `= 4` arm falling through. An if-else chain gives
   `bne`/`bne` and the bodies in source order (48-51 DIFF in four orderings).

   func_8005537C is called TWICE, once per arm of the func_8005B470 test:
   jump2 cross-joins the two `jal`s, where a single call site lets reorg put
   the 0x64 in the test's delay slot and drops the `j`. func_800552C8 is
   called with NO argument through an alias, as in func_800553D4. */
void func_800550E8(void)
{
    u8 *b = (u8 *)D_800847D0;
    u8 tmp[0x20];
    s32 n;
    s32 c;
    s32 v;
    u16 w;
    u16 m;
    u8 *t;
    u8 *r;
    u8 *r2;
    u8 *t2;

    if (*(s16 *)(b + 0x26) == -1)
    {
        func_800553D4(0x71);
        return;
    }
    if (func_8005405C(0) == 0)
    {
        *(u32 *)(b + 4) |= 2;
        return;
    }
    n = -((b[0x26] >> 4) * 2);
    if ((s16)func_8005B470(0, tmp) == 0)
    {
        func_8005537C(tmp[0x18] + n);
    }
    else
    {
        func_8005537C(0x64);
    }
    b = (u8 *)D_800847D0;
    w = *(u16 *)(b + 0x26);
    c = w & 0xF;
    *(s16 *)(b + 0x26) = c;
    t = D_80084810;
    r = t + (*(u16 *)(b + 0x20) << 5);
    v = *(s16 *)(r + c * 2);
    m = w & 0xF000;
    if (v != 0)
    {
        func_8005B3D8(v, v);
    }
    t2 = D_80084878;
    r2 = t2 + (*(u16 *)(b + 0x20) << 6);
    *(s16 *)(b + 0x22) = func_8005AE08(*(u32 **)(r2 + *(s16 *)(b + 0x26) * 4), 0);
    *(s16 *)(b + 0x26) = -1;
    D_800847D0[0] &= ~0x1000;
    *(u32 *)(b + 4) &= ~2;
    if (m != 0)
    {
        u8 *d = (u8 *)D_800848F8;

        *(u32 *)(d + 4) = 1;
        *(s16 *)(d + 8) = 0;
        switch (m)
        {
        case 0x9000:
            *(s16 *)(d + 0x16) = 4;
            break;
        case 0x8000:
            *(s16 *)(d + 0x16) = 2;
            break;
        default:
            *(s16 *)(d + 0x16) = 0xA;
            break;
        }
    }
    else
    {
        D_800848FC[0] = 3;
    }
    D_800847D0[0] |= 0x100;
    func_800552C8b();
    func_8005AE90(*(s16 *)((u8 *)D_800847D0 + 0x22), 1, 1);
}

/* The magic 0x80010003 with post_shift 14 is a DIVISION BY 32767 (magic.py;
   do not estimate it from the arithmetic). D_800848F8 is read through a
   pointer local so the two constant indices come out as displacements off a
   materialised base. func_8005B27C is reached through an __asm__ alias with
   `(s16, s16, s16)`: the target passes the s16 result twice unmasked, where
   the unit's own `(s16, u8, u8)` definition would make the caller `andi`. */
s32 func_80055750(s16);
void func_8005B27Cw(s16, s16, s16) __asm__("func_8005B27C");

void func_800552C8(s32 unused) {
    u8 *b = (u8 *) D_800847D0;
    s16 *q;
    s32 t;
    s16 e;
    s32 f;
    s16 v;

    if ((D_800847D0[0] & 0x100) != 0) {
        q = D_800848F8;
        t = q[4] * D_8008480A[0];
        e = t / 32767;
        f = e * q[8];
        v = func_80055750(f / 128);
        func_8005B27Cw(*(s16 *) (b + 0x22), v, v);
    }
}

void func_8005537C(s16 a) {
    s16 v;

    D_800848F8[5] = a;
    if ((D_800847D0[0] & 0x20) != 0) {
        D_800848F8[5] = a - 0x18;
    }
    v = func_80055750(D_800848F8[5]);
    D_800848F8[5] = v;
    D_800848F8[4] = v;
}

extern u32 D_800848FC[];
extern s16 D_80084900[];

void func_800552C8(s32);
void func_8005AF74(s16);
void func_8005AFF4(s16);
void func_8005B070(s16);
void func_8005B16C(s16);

/* This call site passes NO argument. The listing has no argument move before
   `jal func_800552C8` -- $a0 still holds the masked event code from the
   dispatch, and the call's delay slot is spent on the D_80084900 store -- and
   every spelling that passes the event code costs a word: cse follows the
   switch's own `beq` into the arm (the label has one reference), records
   `n == 0x71` there, and attaches it as a REG_EQUAL note that combine
   rematerialises as `addiu $a0, $zero, 0x71`. Passing the literal 0x71 emits
   the same word, so there is no source that passes a value and matches. The
   `__asm__` alias says "prototype-less at this site" without touching the
   declaration the two other call sites in this unit need; it works in the
   original because $a0 happens to hold exactly what the callee reads. */
extern void func_800552C8a(void) __asm__("func_800552C8");

/* `D_800847D0[0] &= ~0x100;` and `&= ~0x1000;` as TWO statements. As one
   expression fold merges the masks into a single `addiu $v1, $zero, -0x1101`
   and the function is a word short; as two, gcc's flow pass drops the first
   store as dead (nothing reads the location in between) and what is left is
   the listing's one `lw`, two `and`s and one `sw`. */
void func_800553D4(s32 n)
{
    u8 *b;

    n &= 0xFF;
    switch (n)
    {
    case 0x71:
        b = (u8 *)D_800847D0;
        if ((D_800847D0[0] & 0x1100) != 0)
        {
            D_80084900[0] = 0;
            func_800552C8a();
            func_8005AF74(*(s16 *)(b + 0x22));
            func_8005AFF4(*(s16 *)(b + 0x22));
        }
        D_800848FC[0] = 0;
        D_800847D0[0] &= ~0x100;
        D_800847D0[0] &= ~0x1000;
        *(u32 *)(b + 4) &= ~2;
        if (*(s16 *)(b + 0x26) != -1)
        {
            func_800550E8();
        }
        break;
    case 0xE1:
        if ((D_800847D0[0] & 0x100) != 0)
        {
            func_8005B070(*(s16 *)((u8 *)D_800847D0 + 0x22));
            D_800847D0[0] = (D_800847D0[0] & ~0x100) | 0x1000;
        }
        break;
    case 0xF1:
        if ((D_800847D0[0] & 0x1000) != 0)
        {
            func_8005B16C(*(s16 *)((u8 *)D_800847D0 + 0x22));
            D_800847D0[0] = (D_800847D0[0] & ~0x1000) | 0x100;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_8005552C);

void func_800555FC(void) {
}

void func_80055604(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_8005560C);

void func_8005BAB0(s32, s32);

/* Both arguments pass straight through, so this compiles to the same bytes
   either way; the widths are the ones func_8005BAB0's own entry and
   func_80054104's call site prove. */
void func_80055730(s32 a, s32 b) {
    func_8005BAB0(a, b);
}

s32 func_80055750(s16 a) {
    if (a < 0) {
        return 0;
    }
    if (a >= 0x80) {
        return 0x7F;
    }
    return a;
}

/* Push onto the D_80084778 ring. The guard is an EARLY RETURN and the success
   path returns its own 0: one variable assigned in both arms gave the target's
   instructions with $v1 and a copy, because a variable live through the join is
   a global allocno. With a return in each arm the whole tail is block-local,
   the 0 goes straight into $v0, and jump2's cross-jumping merges the two `jr`s
   back into the one the listing shows. 7 DIFF.

   The count is re-read after `++` because the store invalidated cse's entry for
   it -- matching-levers section 4 -- so the element written is [count + 1]. */
s32 func_80055778(s32 v) {
    if (D_80084778[0] >= 0x20) {
        return -1;
    }
    D_80084778[0]++;
    D_80084778[D_80084778[0]] = v;
    return 0;
}

void func_800557BC(void) {
    D_80084778[0] = 0;
}

/* Pops the head off the halfword queue at D_80084778 (slot 0 is the count).
   `r = 0;` is a STATEMENT after the clamp, not the declaration's initialiser:
   as an initialiser the zero is materialised at the top of the function and
   the guard's delay slot gets a nop; an early `return 0;` instead costs a `j`
   and a second zero (40 words either way). */
u16 func_800557C8(void) {
    u16 r;
    u16 i;

    if (D_80084778[0] > 0x20) {
        D_80084778[0] = 0x20;
    }
    r = 0;
    if (D_80084778[0] != 0) {
        r = D_80084778[1];
        for (i = 1; i < D_80084778[0]; i++) {
            D_80084778[i] = D_80084778[i + 1];
        }
        D_80084778[0] = D_80084778[0] - 1;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80055864);

s32 func_8003F5D4(void);

/* The four targets are incomplete arrays, so the `lui` lands in a register the
   `beq`'s delay slot can take; as scalars each store spends `lui $at` instead
   and the function is five words longer. */
void func_800558FC(u16 k) {
    s32 v = func_8003F5D4();

    switch (k) {
    case 0x11:
        D_80084758[0] = v;
        break;
    case 0x12:
        D_8008475C[0] = v;
        break;
    case 0x14:
        D_80084760[0] = v;
        break;
    case 0x18:
        D_80084764[0] = v;
        break;
    }
}

void func_80055990(u8 *p) {
    *(u16 *) (p + 0x10) = 0x80;
    *(u32 *) (p + 0xC) = 3;
    *(u32 *) (p + 0x4) = 0;
    *(u16 *) (p + 0x16) = 0xA;
    *(u16 *) (p + 0x18) = 0xA;
}

extern u32 D_80084850[];

void func_80055990(u8 *);
void func_800552C8(s32);
void func_80054D64(s32);

void func_800559B4(void)
{
    u8 *p = (u8 *)D_800847D0;
    u8 *q = (u8 *)D_800848F8;
    u8 *r = D_80084858;
    u8 *c;

    D_800847D0[0] = 0;
    *(u32 *)(p + 0x4) = 0;
    D_800847D0[0] = 0x10;
    *(s16 *)(p + 0x1E) = 0x5A;
    *(s16 *)(p + 0x26) = -1;
    *(u32 *)(p + 0x8) = 0;
    *(u32 *)(p + 0xC) = 0;
    p[0x30] = 0;
    p[0x32] = 0;
    *(u32 *)(p + 0x10) = 0;
    *(u32 *)(p + 0x14) = 0;
    p[0x31] = 0;
    p[0x33] = 0;
    *(u32 *)(p + 0x18) = 0;
    *(s16 *)(p + 0x22) = 0;
    *(s16 *)(p + 0x20) = 0;
    D_80084808[0] = 0x7FFF;
    c = (u8 *)D_80084808;
    *(s16 *)(c + 0x2) = 0x7FFF;
    *(s16 *)(c + 0x4) = 0x7FFF;
    D_80084850[0] = 0;
    func_80055990(q);
    func_80055990(r);
    *(u32 *)D_800848F8 = (u32)func_800552C8;
    *(s16 *)(q + 0x8) = 0x64;
    *(s16 *)(q + 0xA) = 0x64;
    *(s16 *)(q + 0x12) = 0x50;
    *(s16 *)(q + 0x14) = 2;
    *(u32 *)D_80084858 = (u32)func_80054D64;
    *(s16 *)(r + 0x8) = 0x7F;
    *(s16 *)(r + 0xA) = 0x7F;
    *(s16 *)(r + 0x14) = 0x14;
    *(s16 *)(r + 0x12) = 0;
    p[0x28] = 0xC8;
}

/* Three separate statements -- the mask, the base, then the index -- which is
   the order the five address insns come out in (`andi`, `lui`, `addiu`, `sll`,
   `addu`). Any spelling that leaves the index inside the address expression
   emits the shift before the symbol, because fold puts the ADDR_EXPR second:
   3 DIFF through a base local, a sized alias, a struct array and an explicit
   index local. The declarations' order is the rest of it: the offset, the
   count and the counter are initialised before the address. */
void func_80055ADC(u8 *p, u32 b) {
    s32 off = 0x14;
    s32 n = *(s32 *) (p + 0x10);
    s32 i = 0;
    s32 k = b & 0xFFFF;
    u8 *base = D_80084878;
    u32 *q = (u32 *) (base + k * 0x40);
    u32 *d = q;

    for (; i < 0x10; i++) {
        if (i >= n) {
            *d = *q;
        } else {
            u8 *e = p + off;

            *d = (u32) (e + 0x10);
            off += 0x10 + *(s32 *) (e + 4);
        }
        d++;
    }
}

void func_8005A3E0(void);
void func_8005A3D4(void);

/* The CdMix levels are written 2, 0, 3, 1 in both arms, which is the order the
   target emits them in (0, 1, 2, 3 is 8 DIFF at the same word count). */
void func_80055B44(u16 kk) {
    u8 k = kk;
    CdlATV t;

    if (k == 1) {
        func_8005A3E0();
        t.val2 = 0x4F;
        t.val0 = 0x4F;
        t.val3 = 0x4F;
        t.val1 = 0x4F;
        D_800847D0[0] = (D_800847D0[0] & ~0x10) | 0x20;
    } else {
        func_8005A3D4();
        t.val2 = 0x7F;
        t.val0 = 0x7F;
        t.val3 = 0;
        t.val1 = 0;
        D_800847D0[0] = (D_800847D0[0] & ~0x20) | 0x10;
    }
    CdMix(&t);
}

void func_800553D4(s32);
void func_80055C50(s16);

void func_80055BD8(s16 k) {
    if (k == 0 && (D_800847D0[0] & 0x100) != 0) {
        func_800553D4(0x71);
    }
    func_80055C50(k);
    D_800847D0[1] |= 0x1000000 << k;
}

void func_8005AC68(s16);

void func_80055C50(s16 k) {
    u8 *b = (u8 *) D_800847D0;

    if ((D_800847D0[0] & (0x10000 << k)) != 0) {
        if (k == 0 && (D_800847D0[0] & 0x100) != 0) {
            func_800553D4(0x71);
            *(s16 *) (b + 0x26) = -1;
        }
        func_8005AC68(k);
        D_800847D0[0] &= ~(0x10000 << k);
    }
}

void func_80055CEC(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80055CF4);

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80055D84);

void func_80055E6C(void) {
}

void func_80055E74(void) {
}

void func_80055E7C(s32 a, s32 b, s32 c) {
}

/* func_80056098's body one channel over -- every offset is +0x1C from that
   function's, the final divide is by 256 rather than 1024, and the whole thing
   is wrapped in an `else` so the early `p[0x42]++` path falls into the tail
   instead of returning. All four of func_80056098's spellings carry over
   unchanged: `t` declared `s8`, the wave test on the undoubled value, the
   fall-through arm of each if/else written first, and the explicit `v != 0`.

   The tail's own two facts: D_80084918 takes TWO pointer locals, one for the
   `+4` store before the clamps and one for the `+0xA`/`+0xC`/`+0xE` stores
   after them, because cse restarts at the clamps' joins and the listing
   materialises the base twice (one local would live across them in a
   callee-saved register, and this function saves only $ra); and `+8` is
   reached through its OWN symbol D_80084920 as an incomplete array, which is
   the compiler-split `lui`/`sh %lo` the listing has there, while `+0xA` and
   up go through the second local. */
extern s16 D_80084920[];
void func_8005F134(void *);

void func_80055E84(u8 *p)
{
    s32 g;
    s32 n;
    s8 t;
    s32 v;
    s32 u;
    u8 *b;
    u8 *c;
    s32 x;

    g = *(u32 *)(p + 0x4C);
    if (g == 0)
    {
        return;
    }
    if (p[0x42] != p[0x50])
    {
        p[0x42] = p[0x42] + 1;
    }
    else
    {
        if (p[0x44] >= p[0x51])
        {
            *(u32 *)(p + 0x48) = g;
        }
        else
        {
            if (p[0x44] != 0)
            {
                *(u32 *)(p + 0x48) = *(u32 *)(p + 0x48) + *(u32 *)(p + 0x54);
            }
            else
            {
                *(u32 *)(p + 0x48) = *(u32 *)(p + 0x54);
            }
            p[0x44] = p[0x44] + 1;
        }
        p[0x43] = p[0x43] + p[0x52];
        p[0x41] = 0;
        n = *(s8 *)(p + 0x43);
        if (n < 0)
        {
            n = -n;
            u = n * 2;
            t = (n << 25) < 0 ? -u : u;
            v = *(s32 *)(p + 0x48) * t;
            if (v > 0)
            {
                v = -v;
            }
        }
        else
        {
            u = n * 2;
            t = (n << 25) < 0 ? -u : u;
            v = *(s32 *)(p + 0x48) * t;
            if (v < 0)
            {
                v = -v;
            }
        }
        if (v != 0)
        {
            *(u32 *)(p + 0x58) = v / 256;
        }
        else
        {
            *(u32 *)(p + 0x58) = 0;
        }
    }
    if (*(s16 *)(p + 0x6C) != *(s32 *)(p + 0x58))
    {
        *(s16 *)(p + 0x6C) = *(u16 *)(p + 0x58);
        b = D_80084918;
        *(u32 *)(b + 4) = 0xF;
        *(u32 *)D_80084918 = D_80073740[*(u16 *)p];
        x = *(u16 *)(p + 0x10) + *(u32 *)(p + 0x58);
        if (x < 0)
        {
            x = 0;
        }
        else if (x >= 0x4000)
        {
            x = 0x3FFF;
        }
        D_80084920[0] = x;
        x = *(u16 *)(p + 0x12) + *(u32 *)(p + 0x58);
        if (x < 0)
        {
            x = 0;
        }
        else if (x >= 0x4000)
        {
            x = 0x3FFF;
        }
        c = D_80084918;
        *(s16 *)(c + 0xA) = x;
        *(s16 *)(c + 0xC) = 0;
        *(s16 *)(c + 0xE) = 0;
        func_8005F134(c);
    }
}

/* The +0x3C ramp: advance the +0x26 delay, then the +0x28 step, then fold the
   +0x27 phase through a triangle wave and scale by +0x2C. Four spellings:

   * `t` is declared `s8`, not `s32` with an `(s8)` cast at the multiply.
     As an `s32`, `t` and `u` are one quantity and the negation happens in
     place; the listing has `addu $v1, $a1, $zero` in the `bgez` delay slot
     and `negu $v1, $a1` after it, which is two DISTINCT quantities. The
     narrow declaration is what keeps them apart.
   * the wave's test is written on `n`, not on `u`. `u = n * 2;` and then
     `(u & 0x80) != 0` leaves the `andi`, because combine cannot fold the mask
     back through a pseudo with several uses; `(n << 25) < 0` is the fold it
     would have done, and two words.
   * both if/else pairs have the arm the listing FALLS THROUGH written first:
     `p[0x28] >= p[0x35]` (the `sltu` is the taken branch) and the inner
     `p[0x28] != 0`.
   * the final store keeps its explicit `v != 0` test; `v / 1024` alone is the
     three-word `bgez`/`addiu 0x3FF`/`sra` and no zero branch. */
void func_80056098(u8 *p)
{
    s32 g;
    s32 n;
    s8 t;
    s32 v;
    s32 u;

    g = *(u32 *)(p + 0x30);
    if (g == 0)
    {
        return;
    }
    if (p[0x26] != p[0x34])
    {
        p[0x26] = p[0x26] + 1;
        return;
    }
    if (p[0x28] >= p[0x35])
    {
        *(u32 *)(p + 0x2C) = g;
    }
    else
    {
        if (p[0x28] != 0)
        {
            *(u32 *)(p + 0x2C) = *(u32 *)(p + 0x2C) + *(u32 *)(p + 0x38);
        }
        else
        {
            *(u32 *)(p + 0x2C) = *(u32 *)(p + 0x38);
        }
        p[0x28] = p[0x28] + 1;
    }
    p[0x27] = p[0x27] + p[0x36];
    p[0x25] = 0;
    n = *(s8 *)(p + 0x27);
    if (n < 0)
    {
        n = -n;
        u = n * 2;
        t = (n << 25) < 0 ? -u : u;
        v = *(s32 *)(p + 0x2C) * t;
        if (v > 0)
        {
            v = -v;
        }
    }
    else
    {
        u = n * 2;
        t = (n << 25) < 0 ? -u : u;
        v = *(s32 *)(p + 0x2C) * t;
        if (v < 0)
        {
            v = -v;
        }
    }
    if (v != 0)
    {
        *(u32 *)(p + 0x3C) = v / 1024;
    }
    else
    {
        *(u32 *)(p + 0x3C) = 0;
    }
}

extern u16 D_80073738[];
/* An incomplete-array alias: the listing's reads of the index are gcc's own
   `(high)`/`(lo_sum)` pair with the high in a SEPARATE register, which the
   committed 2-byte scalar cannot give -- that form is one insn and maspsx
   splits it using the destination as the temp. */
extern s16 D_80086D50a[] __asm__("D_80086D50");

/* Derives an entity's two scale factors and its +0x18 brightness from the
   active D_80086A40/D_80086C00 pair: the brightness is the slot's +0x1B plus
   the entity's +0x16/+0x17 plus the caller's +0x04, biased by -0xC0 and
   clamped to 0..0x7F (or pinned at 0x40 while D_8007382A is set); the scale is
   a five-term fixed-point product split between the two axes by whether the
   brightness is past 0x40, then scaled by the +0x08/+0x0A pair and the
   entity's own +0x1C, and squared once more when the caller's +0x2C is 0x40 or
   more.

   The two table bases go through LOCALS (`b`, `g`) so that each symbol is
   materialised before the index that uses it -- func_8004A658's idiom -- which
   here also fills the `lh`'s load-delay slot with the second base's `addiu`
   instead of a nop.

   ONE accumulator for the five-term product, not an intermediate `m` and then
   `s`. With two variables the chain's `mflo`s land in $t1, the final `sra`
   reads a register the following `slti` does not write, and reorg is then free
   to pull that `sra` into the branch's delay slot -- which the listing does
   not do, because there the `sra` reads the very register the `slti` clobbers.

   The two axis products are assigned BACK to `x` and `y` rather than into
   fresh locals. The listing reuses x's and y's registers for them, which is
   what gcc does when the old value dies in the same insn that computes the
   new one; fresh locals land in $v1 twice over and serialise the two chains,
   where the listing interleaves them (9 wrong words).

   The `>= 0x40` arm is written FIRST so the other one goes out of line, and
   the `t = sum - 0xC0` assignment comes before the D_8007382A test so it ends
   up in that branch's delay slot. */
void func_800561D8(u8 *p, Unk80084960 *q)
{
    Unk80086A40 *b;
    Unk80086A40 *e;
    Unk80086C00 *g;
    Unk80086C00 *f;
    s32 t;
    s32 s;
    s32 x;
    s32 y;

    b = D_80086A40;
    e = &b[D_80086D50a[0]];
    t = e->unk_1B + p[0x16] + p[0x17] + q->unk_04 - 0xC0;
    if (D_8007382A[0] != 0)
    {
        t = 0x40;
    }
    if (t < 0)
    {
        t = 0;
    }
    if (t >= 0x80)
    {
        t = 0x7F;
    }
    *(s16 *) (p + 0x18) = t;
    s = e->unk_18 * D_80073738[0] * q->unk_14 * q->unk_0C;
    s = (u32) s >> 14;
    s = (s * p[0x14] * p[0x15]) >> 14;
    if (t >= 0x40)
    {
        y = s;
        x = ((0x40 - (t & 0x3F)) * (y << 1)) >> 7;
    }
    else
    {
        x = s;
        y = (t * (x << 1)) >> 7;
    }
    g = D_80086C00;
    f = &g[D_80086D50a[0]];
    x = (x * *(s16 *) &f->unk_08) >> 7;
    y = (y * *(s16 *) &f->unk_0A) >> 7;
    *(s16 *) (p + 0x10) = (x * (p[0x1C] & 0x7F)) >> 7;
    *(s16 *) (p + 0x12) = (y * (p[0x1C] & 0x7F)) >> 7;
    if (q->unk_2C >= 0x40)
    {
        *(s16 *) (p + 0x10) = (*(u16 *) (p + 0x10) * *(u16 *) (p + 0x10)) >> 14;
        *(s16 *) (p + 0x12) = (*(u16 *) (p + 0x12) * *(u16 *) (p + 0x12)) >> 14;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_800563B0);

void func_800563B0(s32, s32, s32);

/* UNPARKED. The entry is indexed off the symbol INSIDE the loop body; as a
   walked `e += 0x78` loop.c adds a second giv biased to `e + 0x12` and uses
   negative displacements, which is what this was parked on. The D_80073734
   bound is read signed (`blez`/`slt`) and re-read by the loop condition, which
   is what keeps it out of a callee-saved register. */
void func_80056408(void) {
    s32 i;

    for (i = 0; i < (s32) D_80073734[0]; i++) {
        u8 *e = D_80085458 + i * 0x78;

        func_800561D8(e, &D_80084960[*(u16 *) (e + 6)]);
        func_800563B0(i, *(u16 *) (e + 0x10), *(u16 *) (e + 0x12));
    }
}


/* The entry pointer is INDEXED off the symbol inside the body, not walked:
   `u8 *e = D_80085458; ... e += 0x78;` makes loop.c add a SECOND giv biased to
   `e + 0x12` and use negative displacements, which is func_80056408's parked
   problem (48 DIFF). And the D_80073734 bound is read SIGNED -- the listing
   has `blez` and `slt`, where the u32 declaration gives `beqz` and `sltu`.
   The parameter is unused; the caller's prototype already says it is there. */
void func_800564A8(s32 unused) {
    s32 i;

    for (i = 0; i < (s32) D_80073734[0]; i++) {
        u8 *e = D_80085458 + i * 0x78;

        if (*(u16 *) (e + 6) < 0x10) {
            func_800561D8(e, &D_80084960[*(u16 *) (e + 6)]);
            func_800563B0(i, *(u16 *) (e + 0x10), *(u16 *) (e + 0x12));
        }
    }
}

extern s32 D_80073734s __asm__("D_80073734");
void func_800563B0(s32, s32, s32);

/* The bound through the SCALAR spelling, so its load is one macro insn that
   maspsx splits afterwards -- as the incomplete array the compiler splits it
   itself and loop.c hoists the `lui` half into a callee-saved register, two
   words longer. Signed, so the guard is `blez` and not `beqz`.
   The entry pointer is computed INSIDE the body, which is what puts the giv's
   initialisation in the preheader unbiased; `p += 0x78` on a pointer declared
   outside biases it to +0x12 (the first field used) instead.
   Both loads are left inside the comparison: in locals, the `lw` is emitted
   first and the fill pass cannot put it in the `lh`'s delay slot (2 DIFF). */
void func_80056558(void) {
    s32 i = 0;

    while (i < D_80073734s) {
        u8 *p = D_80085458 + i * 0x78;

        if (*(s16 *) (p + 0x6C) != *(s32 *) (p + 0x58)) {
            func_800563B0(i, *(u16 *) (p + 0x10) + *(s32 *) (p + 0x58), *(u16 *) (p + 0x12) + *(s32 *) (p + 0x58));
        }
        i++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_800565D8);

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80056654);

/* No local copy of the parameter: `u8 *p = q;` spends a word the target
   leaves as a `nop`. And the negative arm's test is written with the
   DIFFERENCE first (`a - b > -limit`), which is the order the target evaluates
   its three `lh`s in; `-limit < a - b` negates first, 12 DIFF. */
void func_80056800(u8 *p) {
    if (*(u32 *) (p + 8) != 0) {
        if (*(u32 *) (p + 0x34) != 0) {
            if (*(s16 *) (p + 0x3A) + *(s16 *) (p + 0x38) <
                *(s16 *) (p + 0x40)) {
                *(u16 *) (p + 0x3A) = *(u16 *) (p + 0x3A) +
                                      *(u16 *) (p + 0x38);
            } else {
                *(u32 *) (p + 0x34) = 0;
                *(u16 *) (p + 0x3A) = *(u16 *) (p + 0x40);
            }
        } else {
            if (*(s16 *) (p + 0x3A) - *(s16 *) (p + 0x38) >
                -*(s16 *) (p + 0x40)) {
                *(u16 *) (p + 0x3A) = *(u16 *) (p + 0x3A) -
                                      *(u16 *) (p + 0x38);
            } else {
                *(u32 *) (p + 0x34) = 1;
                *(u16 *) (p + 0x3A) = -*(u16 *) (p + 0x40);
            }
        }
    } else {
        *(s16 *) (p + 0x3A) = 0;
    }
}

/* Steps +0x52 towards +0x56 by +0x54, clamping; +0x58 picks the direction.
   Each arm ends in its own `return` -- that is what gives all four of them
   their own `j` to the epilogue (two words short with the else arms falling
   through) -- and the second test is written `x - y > -c`, not `-c < x - y`,
   so the subtraction is emitted before the negation. */
void func_800568B4(u8 *p) {
    if (*(s16 *) (p + 0x50) == 0) {
        return;
    }
    if (*(u32 *) (p + 0x58) != 0) {
        if (*(s16 *) (p + 0x52) + *(s16 *) (p + 0x54) < *(u16 *) (p + 0x56)) {
            *(s16 *) (p + 0x52) = *(s16 *) (p + 0x52) + *(s16 *) (p + 0x54);
            return;
        }
        *(s16 *) (p + 0x52) = *(u16 *) (p + 0x56);
        return;
    }
    if (*(s16 *) (p + 0x52) - *(s16 *) (p + 0x54) > -*(u16 *) (p + 0x56)) {
        *(s16 *) (p + 0x52) = *(s16 *) (p + 0x52) - *(s16 *) (p + 0x54);
        return;
    }
    *(s16 *) (p + 0x52) = -*(u16 *) (p + 0x56);
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80056948);

/* The three-table reset: clears all 0x20 D_80085FA8 entries, then every live
   D_80085458 entry whose +0x06 index is in range (keying its voice off and
   collecting the handles into one mask), then all 0x10 D_80084960 entries,
   and finally keys the collected mask off in one call.

   Right at 138 words on the first compile; three shapes, two of them this
   unit's standing recipe:

   * Each entry pointer is INDEXED off the symbol inside the loop body
     (`e = D_80085FA8 + i * 0x50`), which is what leaves loop.c one unbiased
     giv per loop -- the listing's single `addiu $v1, $v1, 0x50` /
     `addiu $s1, $s1, 0x78` / `addiu $v1, $v1, 0x9C`. Walking a pointer
     declared outside biases the giv to the first field used.

   * D_80084960 is reached through `(u8 *) &D_80084960[i]` -- the struct for
     the stride (see the note on Unk80084960 in structs.h: a byte array lets
     fold migrate the field offset onto the symbol) and a `u8 *` for the
     fields, because this loop writes 28 of them and all but four are still
     inside pads.

   * `m = 0` comes BEFORE the D_80073738 store, which is what puts $s5's save
     and its zeroing ahead of the `lui`; the natural order (the store first)
     is 4 words out, and `i = 0` has to stay last so cse spells it
     `addu $s2, $s5, $zero`. */
void func_80056A08(void)
{
    s32 i;
    u32 m;
    u32 h;

    m = 0;
    D_80073738[0] = 0x7F;
    i = 0;
    do
    {
        u8 *e = D_80085FA8 + i * 0x50;

        *(u32 *) (e + 0x00) = 0;
        *(u32 *) (e + 0x04) = 0;
        *(u32 *) (e + 0x08) = 0;
        *(u32 *) (e + 0x0C) = 0;
        *(u32 *) (e + 0x10) = 0;
        *(u32 *) (e + 0x18) = 0;
        *(u32 *) (e + 0x1C) = 0;
        *(u32 *) (e + 0x20) = 0;
        *(u32 *) (e + 0x24) = 0;
        *(u32 *) (e + 0x28) = 0;
        *(u32 *) (e + 0x2C) = 1;
        *(u32 *) (e + 0x34) = 0;
        *(u32 *) (e + 0x38) = 0;
        *(u32 *) (e + 0x3C) = 0x78;
        *(u32 *) (e + 0x40) = 0;
        e[0x48] = 0;
        e[0x49] = 0xFF;
        e[0x4A] = 0;
        e[0x4C] = 0;
        i++;
    } while (i < 0x20);

    for (i = 0; i < (s32) D_80073734[0]; i++)
    {
        u8 *p = D_80085458 + i * 0x78;

        if (*(u16 *) (p + 6) < 0x10)
        {
            *(s16 *) (p + 0x00) = i;
            *(u16 *) (p + 0x1A) = 0;
            *(u16 *) (p + 0x18) = 0x40;
            p[0x1D] = 0;
            p[0x27] = 0;
            p[0x26] = 0;
            p[0x28] = 0;
            *(u32 *) (p + 0x2C) = 0;
            *(u32 *) (p + 0x30) = 0;
            *(u32 *) (p + 0x3C) = 0;
            p[0x43] = 0;
            p[0x42] = 0;
            p[0x44] = 0;
            *(u32 *) (p + 0x48) = 0;
            *(u32 *) (p + 0x4C) = 0;
            *(u32 *) (p + 0x58) = 0;
            *(u32 *) (p + 0x70) = 0x40;
            *(u32 *) (p + 0x74) = 0x40;
            h = D_80073740[i];
            func_80056DB4(i);
            m |= h;
            func_8005E97C(0, h);
        }
    }

    for (i = 0; i < 0x10; i++)
    {
        u8 *q = (u8 *) &D_80084960[i];

        *(u32 *) (q + 0x0C) = 0x7F;
        *(u32 *) (q + 0x04) = 0x40;
        *(u32 *) (q + 0x18) = 0;
        *(u32 *) (q + 0x1C) = 0x40;
        *(u32 *) (q + 0x14) = 0x7F;
        *(u32 *) (q + 0x20) = 0x7F;
        *(u32 *) (q + 0x24) = 0x7F;
        *(u32 *) (q + 0x08) = 0;
        *(u16 *) (q + 0x3A) = 0;
        *(u32 *) (q + 0x44) = 0;
        *(u16 *) (q + 0x40) = 0;
        *(u32 *) (q + 0x48) = 0;
        *(u32 *) (q + 0x94) = 0;
        q[0x30] = 0;
        q[0x60] = 0;
        q[0x5F] = 0;
        q[0x61] = 0;
        *(u32 *) (q + 0x64) = 0;
        *(u32 *) (q + 0x68) = 0;
        q[0x7B] = 0;
        q[0x7A] = 0;
        q[0x7C] = 0;
        *(u32 *) (q + 0x80) = 0;
        *(u32 *) (q + 0x84) = 0;
        *(u32 *) (q + 0x28) = 0;
        *(u32 *) (q + 0x2C) = 0;
        *(u16 *) (q + 0x50) = 0;
        q[0x98] = 0;
    }
    func_8005E97C(0, m);
}

/* The D_80085458 reset. Four loop facts, three of them the recipe this unit's
   other sweeps use:

   * the counter is `s32`, not `s16`. As `s16` gcc sign-extends it at every use
     and keeps an extra copy for the increment: six words and two more
     callee-saved registers.
   * the bound takes the `__asm__` SCALAR alias, so its load is one macro insn
     that maspsx splits inside the loop. Through the incomplete array loop.c
     hoists the `lui` half into a callee-saved register -- the opposite of what
     func_8005B070 needs for the same symbol, which is why the rule is to read
     the `lui`'s destination per site.
   * the entry pointer is INDEXED inside the body. Walked from a pointer
     declared outside, loop.c keeps the walked pointer AND adds a second giv
     biased to +0x74.

   * `i++` goes AFTER func_8005E97C, not between the two calls. The listing
     has `addu $a0, $zero, $zero` -- func_8005E97C's first argument --
     immediately after func_80056DB4's delay slot, with the three giv updates
     on the far side of it. An argument setup cannot be scheduled earlier than
     its place in the chain, but the giv updates CAN be hoisted across it and
     across the call itself, because they write callee-saved registers the
     call does not clobber. So put the increment after the call and let sched
     pull the updates forward; with it between the calls the four insns come
     out in the other order (4 DIFF at the exact word count). */
extern u8 D_80085014[];
extern s32 D_80073734s __asm__("D_80073734");

void func_80056A08(void);
void func_80056DB4(s32);
void func_8005E97C(s32, s32);

void func_80056C30(void)
{
    u8 *b;
    s32 i;
    u32 m;
    u32 v;

    func_80056A08();
    i = 0;
    m = 0;
    if (D_80073734s > 0)
    {
        do
        {
            u8 *e = D_80085458 + i * 0x78;

            *(s16 *)e = i;
            *(s16 *)(e + 0x1A) = 0;
            *(s16 *)(e + 0x18) = 0x40;
            *(u32 *)(e + 0x70) = 0x40;
            *(u32 *)(e + 0x74) = 0x40;
            v = D_80073740[i];
            func_80056DB4(i);
            func_8005E97C(0, v);
            i++;
            m |= v;
        } while (i < D_80073734s);
    }
    b = D_80085014;
    *(u32 *)(b + 0xC) = 0x7F;
    *(u32 *)(b + 4) = 0x40;
    *(u32 *)(b + 0x18) = 0;
    *(u32 *)(b + 0x1C) = 0x40;
    *(u32 *)(b + 0x14) = 0x7F;
    *(u32 *)(b + 0x20) = 0x7F;
    *(u32 *)(b + 0x24) = 0x7F;
    *(u32 *)(b + 8) = 0;
    *(u32 *)(b + 0x94) = 0;
    *(s16 *)(b + 0x50) = 0;
    func_8005E97C(0, m);
}

void func_80056D3C(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80056D44);

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80056DB4);

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80056E10);

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80057948);

void func_80057A48(void) {
}

extern s16 D_80086D50a[] __asm__("D_80086D50");

/* The two address materialisations interleave -- `lui`, `lui`, `addiu`, `lh`
   -- which they can only do if BOTH are compiler-split pairs. As the scalar
   `D_80086D50`, cc1psx emits `lh $2,D_80086D50` as one macro insn and maspsx
   expands it into an adjacent lui/lh pair that nothing can schedule apart, so
   the pair lands after the other symbol's `addiu`: 2 DIFF that six statement
   orderings could not move. The array alias gives the split form.

   The 0x820 binds to the index term and not to the sum, so the index is its
   own local: written as one expression the constant folds onto the pointer
   instead and the final `addu`/`addiu` come out in the other order. */
u8 *func_80057A50(s32 *p, u16 j) {
    u8 *q = D_80086A40[D_80086D50a[0]].unk_04;
    s32 k = (((*p << 4) + j) << 5) + 0x820;

    return q + k;
}

/* The channel's effect-parameter writer: func_80057A50 resolves the voice
   block for the low nibble of +0x4D, then +0x4C selects which field of it the
   byte at +0x4E goes into, through jtbl_80032F04's sibling jtbl_80032EA4. 23
   entries, bound `sltiu 0x17` with no bias, so cases 0, 13, 14 and 20..22 are
   EMPTY ARMS rather than gaps -- without them the bound would be smaller and
   the table shorter.

   Cases 15 to 19 write a real Psy-Q `SpuReverbAttr` on the stack: the masks
   pair with the union members exactly (SPU_REV_MODE with `mode`,
   SPU_REV_DEPTHL|SPU_REV_DEPTHR with `depth.left`/`depth.right`,
   SPU_REV_FEEDBACK with `feedback`, SPU_REV_DELAYTIME with `delay`), which is
   what puts each store at the listing's displacement; `libspu.h` is already
   in include/common.h. The 16-bit fields are read back as `e[0x10]` (a byte)
   in cases 4 and 5 and as `*(u16 *) (e + 0x10)` in 6 and 7, because the
   listing's loads are `lbu` in the first pair and `lhu` in the second. */
s32 func_8005DA68(void);
void func_8005D88C(s32);
s32 func_8005DA88(SpuReverbAttr *);

void func_80057A94(u8 *p)
{
    SpuReverbAttr r;
    u8 *e;

    e = func_80057A50((s32 *) p, p[0x4D] & 0xF);
    switch (p[0x4C])
    {
    case 0:
        break;
    case 1:
        e[1] = p[0x4E];
        break;
    case 2:
        e[6] = p[0x4E];
        break;
    case 3:
        e[7] = p[0x4E];
        break;
    case 4:
        *(u16 *) (e + 0x10) = e[0x10] | ((0x7F - p[0x4E]) << 8);
        break;
    case 5:
        *(u16 *) (e + 0x10) = (e[0x10] | ((0x7F - p[0x4E]) << 8)) | 0x8000;
        break;
    case 6:
        *(u16 *) (e + 0x10) = (*(u16 *) (e + 0x10) & 0xFF0F) | ((p[0x4E] << 1) & 0xF0);
        break;
    case 7:
        *(u16 *) (e + 0x10) = (p[0x4E] >> 3) | (*(u16 *) (e + 0x10) & 0xFFF0);
        break;
    case 8:
        *(u16 *) (e + 0x12) = (*(u16 *) (e + 0x12) & 0x603F) | ((0x7F - p[0x4E]) << 6);
        break;
    case 9:
        *(u16 *) (e + 0x12) = ((*(u16 *) (e + 0x12) & 0x603F) | ((0x7F - p[0x4E]) << 6)) | 0x8000;
        break;
    case 10:
        *(u16 *) (e + 0x12) = (*(u16 *) (e + 0x12) & 0xFFC0) | ((0x7F - p[0x4E]) >> 3);
        break;
    case 11:
        *(u16 *) (e + 0x12) = ((*(u16 *) (e + 0x12) & 0xFFC0) | ((0x7F - p[0x4E]) >> 3)) | 0x20;
        break;
    case 12:
        if (p[0x4E] < 0x41)
        {
            *(u16 *) (e + 0x12) |= 0x4000;
        }
        else
        {
            *(u16 *) (e + 0x12) &= 0xBFFF;
        }
        break;
    case 13:
    case 14:
        break;
    case 15:
        if (func_8005DA68() == 1)
        {
            func_8005D88C(0);
        }
        if (p[0x4E] != 0)
        {
            r.mask = SPU_REV_MODE;
            r.mode = p[0x4E];
            D_8007382B[0] = p[0x4E];
            func_8005DA88(&r);
            func_8005D88C(1);
        }
        break;
    case 16:
        r.mask = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
        r.depth.left = p[0x4E] << 8;
        r.depth.right = p[0x4E] << 8;
        func_8005DA88(&r);
        func_8005D88C(1);
        break;
    case 17:
        r.mask = SPU_REV_FEEDBACK;
        r.feedback = p[0x4E];
        func_8005DA88(&r);
        break;
    case 18:
    case 19:
        r.mask = SPU_REV_DELAYTIME;
        r.delay = p[0x4E];
        func_8005DA88(&r);
        break;
    case 20:
    case 21:
    case 22:
        break;
    }
}

extern s32 D_80073734a[] __asm__("D_80073734");

void func_80056654(u8 *, s32);
s32 func_8005EB78(s32);
/* No argument: the listing sets up nothing for it, where this unit's own
   `void func_800564A8(s32 unused)` would make the caller load $a0. */
void func_800564A8a(void) __asm__("func_800564A8");

/* The controller-change applier: 126 controller numbers through jtbl_80032F04,
   of which 25 have bodies. Case 1 (modulation) is 135 of the 463 words and
   case 64 (sustain pedal) another 95. The case values and their SOURCE ORDER
   come out of the table itself (index = cc - 1), and case 6 is out of order,
   between 91 and 98 -- the body addresses say so and nothing else does.

   WHAT THE SHAPE IS MADE OF, each measured (the full record, with every
   negative, was docs/near-misses/func_80057D20.c before this landed):

   * `ch` and `d` are `u8`. `ch` ends up in memory at 0x10($sp) -- `sb` once
     at entry, `lbu` at each of its nine uses -- which is the register
     pressure of a function holding s0-s7 and fp; `d` being `u8` is what puts
     an `andi 0xFF` at every use rather than one at entry.
   * THE SEVEN LOOP COUNTERS ARE SEVEN VARIABLES. Sharing one `i` makes a
     single allocno with 46 references over 155 live insns (priority 1.484),
     which outranks the entry pointer and `d` and takes their registers;
     separate counters are 7 refs over 17 insns each (0.824) and put `e` back
     in $s1 and `d` in $s4. That was worth 48 of the 54 words this was stuck
     on. The listing's own register REUSE ($s2 for case 1's loops and case
     64's inner loop) does NOT mean one variable -- disjoint live ranges.
   * The five D_80085458 loops take a BLOCK-SCOPED `u8 *q = D_80085458 + i *
     0x78;` inside the body: a function-scope pointer costs a `move` off the
     giv every iteration, and a walked pointer costs loop.c a second biased
     giv.
   * Case 1's four loops are `n = D_80073734a[0]; i = 0; e->unk_3A = 0;
     if (n > 0) { do ... while (i < D_80073734a[0]); }` -- the bound read
     FIRST so it is not pinned behind the +0x3A store, `i = 0` second so it
     fills the load-delay slot, and the store THIRD so reorg has it for the
     `blez`'s delay slot. A plain `for` with the store ahead of it is two
     words of nops.
   * D_80073734 takes the INCOMPLETE-ARRAY alias here, the opposite of
     func_80057948's choice: the scalar form is one insn of length 2 and
     `eligible_for_delay` refuses it, so every loop keeps a nop instead
     (three words, 272 DIFF).
   * Case 1's arms READ BACK `e->unk_40` and `e->unk_44` just stored; cse
     forwards each to the register holding the stored value, and the read-back
     is also what makes the else arm's divide `divu`, since `e->unk_44` is
     `u32` where `d` would promote to signed int.
   * `d << 1`, never `d * 2`, in all three places: with a `u8 d` the multiply
     materialises the zero-extension as its own `andi`, where the shift lets
     combine fold it into the neighbouring mask.
   * `f1 = 1` sits BEFORE the call in cases 7, 10 and 11. The
     `addiu $a3, $zero, 1` there is NOT a fourth argument to func_80055E7C --
     case 12 has no call at all and still has it; it is reload's register for
     that store, and $a3 is free precisely because the calls stop at $a2.

   THE LAST SIX WORDS WERE A GIV ORDERING, and `r2` below is the fix. In case
   64's inner loop the listing initialises the D_80085458 giv before the
   D_80073740 one and increments them in that order. loop.c's `record_giv`
   PREPENDS to the biv's giv list and the reduction loop walks that list, so
   EMISSION IS THE REVERSE OF CREATION: `r` is the body's first insn, so its
   giv is created first and emitted second, while D_80073740's group is
   represented by its LAST-created member -- the copy move_movables hoists out
   of the do/while, which the loop pass makes last because loops are processed
   innermost first. A SECOND row pointer at OFFSET ZERO, defined after the
   do/while, is created after that hoisted copy, so `combine_givs` makes it
   the group's representative: the two orders swap and the base stays
   unbiased, because the representative's add_val IS the plain base. A walked
   `r` does make it last too, but then the representative is the `+0x1A`
   store and the pointer comes out biased to +0x1A (13 DIFF); writing the
   conditions inline off D_80085458 so `r` is not computed first is 477 words,
   since cse does not share the index chain. */
void func_80057D20(u8 ch, u8 cc, u8 d)
{
    Unk80084960 *e = &D_80084960[ch];
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 f1 = 0;
    s32 f2 = 0;
    s32 k;
    s32 kk;
    s32 n;

    switch (cc)
    {
    case 1:
        if (e->unk_48 < 0x40)
        {
            e->unk_08 = d;
            e->unk_40 = d << 1;
            if (e->unk_44 != 0 && e->unk_40 != 0)
            {
                e->unk_38 = e->unk_40 * 4 / e->unk_44;
            }
            else
            {
                n = D_80073734a[0];
                i1 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i1 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i1++;
                    } while (i1 < D_80073734a[0]);
                }
            }
            if (d == 0 || e->unk_38 == 0)
            {
                n = D_80073734a[0];
                i2 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i2 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i2++;
                    } while (i2 < D_80073734a[0]);
                }
            }
        }
        else
        {
            e->unk_08 = d;
            e->unk_44 = d;
            if (e->unk_44 != 0 && e->unk_40 != 0)
            {
                e->unk_38 = e->unk_40 / e->unk_44 * 2;
            }
            else
            {
                n = D_80073734a[0];
                i3 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i3 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i3++;
                    } while (i3 < D_80073734a[0]);
                }
            }
            if (d == 0 || e->unk_38 == 0)
            {
                n = D_80073734a[0];
                i4 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i4 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i4++;
                    } while (i4 < D_80073734a[0]);
                }
            }
        }
        func_80055E7C(4, ch, e->unk_08);
        break;
    case 2:
        if (e->unk_48 < 0x40)
        {
            e->unk_44 = d;
        }
        else
        {
            e->unk_40 = d << 1;
        }
        break;
    case 3:
        e->unk_48 = d;
        break;
    case 4:
        e->unk_28 = d;
        break;
    case 5:
        e->unk_50 = d;
        break;
    case 7:
        e->unk_0C = d;
        f1 = 1;
        func_80055E7C(3, ch, d);
        break;
    case 10:
        if (d == 0)
        {
            e->unk_04 = 1;
        }
        else
        {
            e->unk_04 = d;
        }
        f1 = 1;
        func_80055E7C(2, ch, e->unk_04);
        break;
    case 11:
        e->unk_14 = d;
        f1 = 1;
        func_80055E7C(5, ch, d);
        break;
    case 12:
        e->unk_2C = d;
        f1 = 1;
        break;
    case 20:
        e->unk_6C = d << 1;
        break;
    case 21:
        e->unk_6E = d;
        break;
    case 22:
        e->unk_68 = d << 4;
        e->unk_64 = d << 4;
        e->unk_6D = 0;
        e->unk_5D = 1;
        break;
    case 23:
        e->unk_6D = d;
        if (d != 0)
        {
            if (e->unk_68 != 0)
            {
                e->unk_70 = e->unk_68 / d;
                if (e->unk_70 == 0)
                {
                    e->unk_70 = 1;
                }
            }
        }
        break;
    case 25:
        e->unk_88 = d << 1;
        break;
    case 26:
        e->unk_8A = d;
        break;
    case 27:
        e->unk_84 = d << 7;
        e->unk_80 = d << 7;
        e->unk_89 = 0;
        e->unk_78 = 1;
        break;
    case 28:
        e->unk_89 = d;
        if (d != 0)
        {
            if (e->unk_84 != 0)
            {
                e->unk_8C = e->unk_84 / d;
                if (e->unk_8C == 0)
                {
                    e->unk_8C = 1;
                }
            }
        }
        break;
    case 30:
        D_80073738[0] = d;
        func_800564A8a();
        break;
    case 64:
        for (kk = 0; kk < D_80073734a[0]; kk++)
        {
            u8 *q = D_80085458 + kk * 0x78;

            if (ch == *(u16 *) (q + 6))
            {
                if (d < 0x40)
                {
                    for (i1 = 0; i1 < D_80073734a[0]; i1++)
                    {
                        u8 *r = D_80085458 + i1 * 0x78;

                        if (*(u16 *) (r + 6) == ch && r[0x1D] != 0 && (r[0x1C] & 0x80) != 0)
                        {
                            f2 |= D_80073740[i1];
                            do
                            {
                                func_8005E97C(0, D_80073740[i1]);
                                k = func_8005EB78(D_80073740[i1]);
                            } while (k != 2 && k != 0);
                            {
                                u8 *r2 = D_80085458 + i1 * 0x78;

                                r2[0x1D] = 0;
                                *(u16 *) (r2 + 0x1A) = 0;
                            }
                        }
                    }
                    q[0x1D] = 0;
                }
                else
                {
                    q[0x1D] = 1;
                }
            }
        }
        if (d < 0x40)
        {
            D_80084960[ch].unk_18 = 0;
        }
        else
        {
            D_80084960[ch].unk_18 = 1;
        }
        break;
    case 91:
        e->unk_30 = d & 0x7F;
        break;
    case 6:
        e->unk_4E = d;
        if (e->unk_4D != 0x14 && e->unk_4D != 0x1E)
        {
            func_80057A94((u8 *) e);
        }
        break;
    case 98:
        e->unk_4C = d;
        break;
    case 99:
        e->unk_4D = d;
        break;
    case 120:
    case 121:
    case 123:
        func_80056A08();
        break;
    case 126:
        e->unk_98 = d;
        break;
    }
    if (f2 != 0)
    {
        func_8005E97C(0, f2);
    }
    if (f1 != 0)
    {
        for (kk = 0; kk < D_80073734a[0]; kk++)
        {
            u8 *q = D_80085458 + kk * 0x78;

            if (ch == *(u16 *) (q + 6))
            {
                if (*(u16 *) (q + 0x1A) != 0)
                {
                    func_800561D8(q, &D_80084960[ch]);
                    func_800563B0(kk, *(u16 *) (q + 0x10), *(u16 *) (q + 0x12));
                }
            }
        }
    }
}

void func_8005845C(u8 i, u8 v) {
    D_80084960[i].unk_00 = v;
}

void func_8005848C(void) {
}

void func_80058494(u8 i, s32 b, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
    func_80055E7C(1, i, v);
}

s32 func_80059BC4(void);

void func_800584E4(void) {
    if (D_800737A4[0] != 0) {
        func_80059BC4();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_80058510);

void func_80058510(void);

/* Arms the root counter 2 event with func_80058510 as the handler. */
void func_800585A0(void) {
    if (D_800737C0[0] == 0) {
        D_800737C0[0] = 1;
        EnterCriticalSection();
        D_80085F9C[0] = OpenEvent(0xF2000002, 2, 0x1000, (void *) func_80058510);
        EnableEvent(D_80085F9C[0]);
        SetRCnt(0xF2000002, 0x1C00, 0x1000);
        StartRCnt(0xF2000002);
        ExitCriticalSection();
        D_80085F98[0] = 0;
        D_800737C0[0] = 0;
    }
}

void func_8006A27C(u32);

void func_8005863C(void) {
    D_800737C0[0] = 1;
    EnterCriticalSection();
    func_8006A27C(0xF2000002);
    DisableEvent(D_80085F9C[0]);
    CloseEvent(D_80085F9C[0]);
    ExitCriticalSection();
    D_80085F98[0] = 0;
    D_800737C0[0] = 0;
}

void func_8006A27C(u32);

void func_800586B0(void) {
    D_800737C0[0] = 1;
    EnterCriticalSection();
    func_8006A27C(0xF2000002);
    ExitCriticalSection();
    D_80085F98[0] = 0;
    D_800737C0[0] = 0;
}

extern u32 D_800869A8[];
void func_800584E4(void);
void func_80056948(void);
void func_8005ACDC(void);

void func_80058700(void) {
    if (D_80085F98[0] == 0) {
        D_80085F98[0] = 1;
        if (D_800737A4[0] != 0) {
            if (D_800869A8[0] == 0x1E0) {
                func_800584E4();
                func_800584E4();
                func_800584E4();
                func_800584E4();
                func_800584E4();
                func_800584E4();
            }
            func_800584E4();
            func_800584E4();
            func_800584E4();
            func_800584E4();
        }
        func_80056948();
        func_8005ACDC();
        D_80085F98[0] = 0;
    }
}

void func_800599B0(void);

void func_800587B4(void) {
    D_80085FA4[0] = 0x80080000;
    D_800869B0[0] = 0xC000;
    func_80056C30();
    func_800599B0();
    D_800737A4[0] = 1;
}

void func_800587F8(void) {
}

s32 func_80058800(u8 *a, u8 *b, s32 n) {
    if (n == 0) {
        return -1;
    }
    while (--n != 0) {
        if (*a != *b) {
            return *a - *b;
        }
        a++;
        b++;
    }
    return *a - *b;
}

s32 func_80058850(u32 i) {
    do {
        if (func_80058800(D_800330FC, (u8 *) (D_80085FA4[0] + i), 4) == 0) {
            return i + 4;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

s32 func_800588C8(u32 i) {
    do {
        if (func_80058800(D_80033104, (u8 *) (D_80085FA4[0] + i), 4) == 0) {
            return i + 4;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

s32 func_80058940(u32 i) {
    do {
        if (func_80058800(D_800737D0, (u8 *) (D_80085FA4[0] + i), 3) == 0) {
            return i + 3;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

/* The byte reader every func_80058xxx call goes through: fetches the byte at
   D_80085FA4[0] + the cursor at +0x00, advances the cursor, and on running
   past D_800869B0[0] sets the +0x2C flag and answers -1.

   THE CURSOR IS DEREFERENCED INLINE IN THE ADDRESS SUM and read a SECOND time
   for the increment, rather than being named in a local. cse forwards the
   second read for free (nothing is stored in between), and what it buys is
   the ORDER OF THE TWO LOADS: the target loads `%lo(D_80085FA4)` first and
   fills its load-delay slot with `lw 0x0($a1)`, while a local gives the
   dereference first. With `off` in a local the deref's insn is ready one
   cycle earlier, sched2's `potential_hazard` promotes it over the `high` of
   the symbol, and the two loads come out swapped -- 19 words either way, 2
   DIFF. Inline, the deref is not ready until the cycle after, and the
   schedule is the listing's.

   The `lim` read must also sit AHEAD of the cursor store, or both load-delay
   slots go empty: 20 words with it after (19 DIFF). Measured on the way, all
   MATCH or close: the globals in locals and the deref inline (2 DIFF), a
   `u8 *` base local (2), both statement orders of the two globals (2 and
   19), the cursor through an `s32 *` local (18 words), and the parameter
   taken as `u8 *` rather than `s32` (MATCH as well -- but `s32` is what the
   unit's own three per-site declarations say, so it is the spelling that
   does not conflict with them; a `u8 *q = (u8 *) a;` local in place of the
   casts is 19 DIFF). */
u32 func_800589B8(s32 a) {
    s32 off;
    u32 lim;
    s32 c;

    c = *(u8 *) (D_80085FA4[0] + *(s32 *) a);
    off = *(s32 *) a + 1;
    lim = D_800869B0[0];
    *(s32 *) a = off;
    if (lim < (u32) off) {
        *(s32 *) (a + 0x2C) = 1;
        return -1;
    }
    return c;
}

/* The 7-bit varint: reads bytes most-significant group first, seven bits at a
   time, while the top bit says another follows. 0 terminates it early.

   ONE variable for the byte, reused by the loop. That is the whole of the
   three words this was short of for two sessions, and it is the conflict
   lever from docs/matching-levers.md rather than anything about the shape:
   the target spends `addu $a0, $v0, $zero` after BOTH calls, which only
   happens if the byte's allocno cannot have $v0, the call's own return
   register. Writing the first byte and the loop's byte as two variables
   gives the first one a live range that contains no other use of $v0, so
   `find_reg` hands it $v0 and reload deletes the copy as a self-move.
   Unifying them imports the loop's conflict: `c << 7` is a block-local
   quantity holding $v0 while the byte is live, so `global_conflicts` ORs
   hard reg 2 into the allocno and both copies become real instructions.

   Measured dead ends for the same three words, all with two variables: a
   copy between them, a `u8` byte (which does buy the loop's copy, because
   the mask folds into the `& 0x7F` and `& 0x80` uses, but puts an
   `andi 0xFF` where the first copy belongs -- the top's `== 0` and `c = w`
   both need all eight bits), a nested `if`, and a `while (1)` with the
   return inside. */
u32 func_80058A04(s32 p)
{
    u32 w;
    u32 c;

    w = func_800589B8(p);
    if (w == 0)
    {
        return 0;
    }
    c = w;
    if ((c & 0x80) == 0)
    {
        return c;
    }
    c &= 0x7F;
    do
    {
        w = func_800589B8(p);
        c = (c << 7) + (w & 0x7F);
    } while ((w & 0x80) != 0);
    return c;
}

u32 func_80058A7C(u32 a, u32 b, u32 c, u32 d) {
    return ((((((a & 0xFF) << 8) + (b & 0xFF)) << 8) + (c & 0xFF)) << 8) | (d & 0xFF);
}

u32 func_80058AA8(u32 a, u32 b) {
    return ((a & 0xFF) << 8) | (b & 0xFF);
}

u32 func_800589B8(s32);
u32 func_80058A7C(u32, u32, u32, u32);

/* Four u8 locals, so each result is masked again at the call. The first call
   needs no argument move -- $a0 still holds the parameter. */
s32 func_80058ABC(u8 *p) {
    u8 a = func_800589B8((s32) p);
    u8 b = func_800589B8((s32) p);
    u8 c = func_800589B8((s32) p);
    u8 d = func_800589B8((s32) p);

    return func_80058A7C(a, b, c, d);
}

u32 func_800589B8(s32);

u16 func_80058B2C(s32 a) {
    u8 x = func_800589B8(a);
    u8 y = func_800589B8(a);

    return func_80058AA8(x, y);
}

extern u32 D_800737C4[];
extern u32 D_800737C8[];
extern u32 D_800737D4[];
extern u32 D_80085FA0[];
extern u32 D_800869B8[];
extern u32 D_800869B4n[] __asm__("D_800869B4");

s32 func_80058850(u32);
s32 func_80058940(u32);
u16 func_80058B2C(s32);

/* The sequencer file open: reads the header for whichever of the three
   formats D_800737C8 says, fills in the track count, the division and the
   first tick offsets, and for format 0 also derives the starting tempo the
   same way func_80058E6C does.

   Four shapes, 179 DIFF down to a match, and three of them came straight off
   advise.py:

   * `q = D_80085FA8;` is a LOCAL holding the base, set before the switch and
     used for every call argument and every +0x1C..+0x24 field; only the two
     `*(u32 *) D_80085FA8 = ...` stores go through the symbol. That is what
     gives the listing its `$s2` (the whole address) alongside `$s0` (the
     `high`, which later gets reused for D_800869B8 and D_800869B0): six
     words, and without it each case materialises the base again.

   * The case bodies are laid out in SOURCE order, so the source order is
     1, 0, 2 -- the listing has format 1's body first. advise.py reports this
     as a MISSING/EXTRA pair of `jal`s twelve words apart; nothing else moves.

   * `(u32) func_80058ABC(q) >> 8`: the helper returns `s32`, so without the
     cast the shift is `sra` where the listing has `srl`. advise.py's
     SIGNEDNESS row.

   * The `>= 0x60` arm of the tail is written FIRST, because the arm that
     falls through in the listing has to come first (its two stores are
     otherwise transposed -- advise.py's EVAL-ORDER row).

   The `addiu $s3, $zero, 0x1` is shared between the switch's `case 1`
   comparison and format 0's `D_800869B4[0] = 1`, which is cse, not a
   deliberate hoist. */
s32 func_80058B70(void)
{
    s32 n;
    u32 i;
    u32 j;
    u32 m;
    u32 k;
    u8 *q;

    D_800737D4[0] = 0;
    i = 0;
    q = D_80085FA8;
    switch ((s32) D_800737C8[0])
    {
    case 1:
        n = func_80058850(0);
        *(u32 *) D_80085FA8 = n;
        if (n == -1)
        {
            return 0;
        }
        D_80085FA0[0] = func_80058ABC(q);
        D_800869B8[0] = func_80058B2C((s32) q) & 0xFFFF;
        D_800869B4n[0] = func_80058B2C((s32) q) & 0xFFFF;
        D_800869A8[0] = func_80058B2C((s32) q) & 0xFFFF;
        if (D_800869B8[0] == 0)
        {
            D_800869B0[0] = 0x10000;
            D_800869B0[0] = func_80058940(0);
            *(u32 *) (q + 0x1C) = D_800869B0[0];
        }
        else
        {
            j = 0;
            D_800869B0[0] = 0x10000;
            while (j < D_800869B4n[0])
            {
                i = func_80058940(i);
                *(u32 *) (q + 0x1C) = i;
                j++;
            }
            D_800869B0[0] = i;
        }
        break;
    case 0:
        *(u32 *) D_80085FA8 = 8;
        D_800869A8[0] = func_80058B2C((s32) q) & 0xFFFF;
        D_800869B8[0] = 0;
        D_800869B4n[0] = 1;
        D_800869B0[0] = 0x10000;
        D_800869B0[0] = func_80058940(0);
        m = (u32) func_80058ABC(q) >> 8;
        *(u32 *) (q + 0x20) = m;
        k = 60000000 / m;
        *(u32 *) (q + 0x24) = k;
        *(u32 *) (q + 0x20) = k;
        *(u32 *) (q + 0x24) = k * 100 / 115;
        if (*(u32 *) (q + 0x24) >= 0x100)
        {
            *(u32 *) (q + 0x24) = 0xFF;
        }
        switch (D_800869A8[0])
        {
        case 0x18:
        case 0x3C:
            *(u32 *) (q + 0x24) >>= 1;
            break;
        case 0x1E:
            *(u32 *) (q + 0x24) >>= 2;
            break;
        }
        func_800589B8((s32) q);
        break;
    case 2:
        D_800869B8[0] = 1;
        D_800869A8[0] = *(u32 *) (D_80085FA4[0] + 8);
        D_800869B4n[0] = *(u32 *) (D_80085FA4[0] + 0xC);
        D_800869B0[0] = *(u32 *) (D_80085FA4[0] + 4);
        break;
    }
    if (D_800869A8[0] >= 0x60)
    {
        D_800737C4[0] = D_800869A8[0];
    }
    else
    {
        D_800737C4[0] = 0;
    }
    D_800737C4[0] = D_800869A8[0];
    return 1;
}

void func_80058E50(s32 *p, s32 v) {
    *p += v;
}

void func_80058E64(void) {
}

extern u32 D_800737C8[];
extern u32 D_800869A8[];
extern u32 D_800869B4n[] __asm__("D_800869B4");

s32 func_80058ABC(u8 *);
void func_80058E50(s32 *, s32);

/* The MIDI meta-event handler. 0x51 is the tempo: it reads three bytes (or
   four through func_80058ABC when the file is format 1), turns them into BPM,
   scales that, clamps it to 0xFF, divides again for the two low frame rates,
   and writes it to every live D_80085FA8 track. 0x2F ends the track; 0x54,
   0x58 and 0x59 just consume their payload bytes; everything else goes
   through the generic skip.

   Five shapes, 142 DIFF down to a match. The first three are the dispatch,
   and they are worth reading together, because the listing's `bltz` is the
   tell:

   * The switch has a case RANGE 0x00..0x0F sharing the default's label -- the
     text meta-events. That is what makes the case list six nodes spanning
     0x00..0x59, and gcc only builds a comparison tree when
     `range > 10 * count` (stmt.c:4817): without it the five scattered cases
     span 42 for a count of 5, so cc1psx emits a JUMP TABLE and the function
     is ten words short with nothing in common with the listing. The range
     node also explains the two low-bound checks: `emit_case_nodes` gives a
     range node `blt low -> default` (the `bltz`, low = 0) and then
     `ble high -> node_label` (the `slti 0x10`, and node_label IS the default
     here), before recursing to the 0x2F leaf.

   * The index is SIGNED: `(s32) (k & 0xFF)`, which is where `slti`/`bltz`
     come from. Unsigned gives `sltiu`/`bltu` throughout.

   * The six bodies are independent; the shared single call at the end and the
     0x54 -> 0x59 chain are jump.c cross-jumping, not source fall-through.

   And two allocation shapes, both read off advise.py:

   * The tempo loop reads its bound AND its value into locals, because the
     loop's stores through `e` could alias `p + 0x24` and the global, so
     neither load is loop-invariant. The bound is read TWICE -- once for the
     guard and once inside -- and cse turns the second into the listing's
     `addu $a2, $v0, $zero`.

   * `i` -- the loop counter -- is reused for the tempo read-back. That is the
     whole last ten words: as its own local the read-back's quantity outranks
     the divide's result and takes $a0, where the listing has the divide in
     $a0 and the read-back in $a1. Reusing a variable that also lives in
     another block imports that block's hard-register conflicts, which is
     advise.py's third ALLOCATION suggestion. */
void func_80058E6C(u8 *p, u32 k)
{
    u32 n;
    u32 i;
    u8 *e;
    u32 m;
    u32 v;

    switch ((s32) (k & 0xFF))
    {
    case 0x2F:
        *(u32 *) (p + 0x2C) = 1;
        func_800589B8((s32) p);
        break;
    case 0x51:
        if (D_800737C8[0] == 1)
        {
            *(u32 *) (p + 0x20) = func_80058ABC(p) & 0xFFFFFF;
        }
        else
        {
            *(u32 *) (p + 0x20) = func_800589B8((s32) p) << 16;
            *(u32 *) (p + 0x20) |= func_800589B8((s32) p) << 8;
            *(u32 *) (p + 0x20) |= func_800589B8((s32) p);
        }
        n = 60000000 / *(u32 *) (p + 0x20);
        *(u32 *) (p + 0x24) = n;
        *(u32 *) (p + 0x20) = n;
        i = *(u32 *) (p + 0x24);
        *(u32 *) (p + 0x24) = i * 100 / 115;
        if (*(u32 *) (p + 0x24) >= 0x100)
        {
            *(u32 *) (p + 0x24) = 0xFF;
        }
        switch (D_800869A8[0])
        {
        case 0x18:
        case 0x3C:
            *(u32 *) (p + 0x24) >>= 1;
            break;
        case 0x1E:
            *(u32 *) (p + 0x24) >>= 2;
            break;
        }
        i = 0;
        v = *(u32 *) (p + 0x24);
        if (D_800869B4n[0] != 0)
        {
            m = D_800869B4n[0];
            do
            {
                e = D_80085FA8 + i * 0x50;
                *(u32 *) (e + 0x24) = v;
                *(u32 *) (e + 0x20) = v;
                i++;
            } while (i < m);
        }
        break;
    case 0x54:
        func_800589B8((s32) p);
        func_800589B8((s32) p);
        func_800589B8((s32) p);
        func_800589B8((s32) p);
        func_800589B8((s32) p);
        func_800589B8((s32) p);
        break;
    case 0x58:
        func_800589B8((s32) p);
        *(s32 *) (p + 0x34) = func_80058ABC(p);
        break;
    case 0x59:
        func_800589B8((s32) p);
        func_800589B8((s32) p);
        func_800589B8((s32) p);
        break;
    case 0x00 ... 0x0F:
    default:
        func_80058E50((s32 *) p, func_80058A04((s32) p));
        break;
    }
}

u32 func_80058A04(s32);
u32 func_800589B8(s32);

/* `func_80058A04(p)` emits nothing -- $a0 already holds p and cse deletes the
   no-op move -- but it is the FOURTH reference to p, which crosses the
   floor_log2 boundary in global-alloc's priority (2*4/11 against 1*3/9) and
   wins p the $s1 the target has. With three references n outranks it and the
   two swap. The increment is written FIRST in the body, which is what puts a
   copy of it in the preheader and the other in the bottom branch's delay
   slot. */
void func_800590E0(s32 p) {
    u32 i = 0;
    u32 n = func_80058A04(p);

    do {
        i++;
        if ((u8) func_800589B8(p) == 0xF7) {
            break;
        }
    } while (i < n);
}

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_8005914C);

INCLUDE_ASM("asm/nonmatchings/unk_2879C", func_8005947C);
