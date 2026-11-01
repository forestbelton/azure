#include "common.h"

/* The second half of the carved unit unk_1C540, split off at func_800595C0.
   The split is forced by the jump tables: a translation unit emits ONE
   contiguous `.rodata`, and this unit owns two separated runs of tables --
   jtbl_80032E14 and jtbl_80032EA4 at 0x80032E14, which belong to
   func_80054F9C and func_80057A94 in csrc/unk_1C540.c, and jtbl_8003310C at
   0x8003310C, which belongs to func_800595C0 below. Two runs need two
   objects. See the jump-table section of docs/open-matching-problems.md.

   Everything above the first function is copied verbatim from the head of
   csrc/unk_1C540.c plus every file-scope declaration that stood above this
   point in the undivided file. The `.size` records in particular are
   file-scope and change every access to those symbols in whichever unit
   they appear, so both halves carry the whole list -- dropping one here
   would move this half's loads off $gp. */

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

s32 func_80055750(s16);

void func_8005B27Cw(s16, s16, s16) __asm__("func_8005B27C");

extern s16 D_80084900[];

void func_8005AF74(s16);

void func_8005AFF4(s16);

void func_8005B070(s16);

void func_8005B16C(s16);

extern void func_800552C8a(void) __asm__("func_800552C8");

void func_8005BAB0(s32, s32);

s32 func_8003F5D4(void);

void func_80055990(u8 *);

void func_8005A3E0(void);

void func_8005A3D4(void);

void func_8005AC68(s16);

extern s16 D_80084920[];

void func_8005F134(void *);

extern u16 D_80073738[];

extern s16 D_80086D50a[] __asm__("D_80086D50");

void func_800563B0(s32, s32, s32);

extern s32 D_80073734s __asm__("D_80073734");

extern u8 D_80085014[];

void func_80056A08(void);

void func_80056DB4(s32);

void func_8005E97C(s32, s32);

s32 func_80059BC4(void);

void func_80058510(void);

void func_8006A27C(u32);

extern u32 D_800869A8[];

void func_800584E4(void);

void func_80056948(void);

void func_8005ACDC(void);

s32 func_800599B0(void);

u32 func_800589B8(s32);

u32 func_80058A7C(u32, u32, u32, u32);

extern u32 D_800737C4[];

extern u32 D_800737C8[];

extern u32 D_800737D4[];

extern u32 D_80085FA0[];

extern u32 D_800869B8[];

extern u32 D_800869B4n[] __asm__("D_800869B4");

s32 func_80058850(u32);

s32 func_80058940(u32);

u16 func_80058B2C(s32);

s32 func_80058ABC(u8 *);

void func_80058E50(s32 *, s32);

u32 func_80058A04(s32);


u32 func_800589B8(s32);
void func_80056E10(u8, s32, s32);
void func_80057948(u8, s32, s32);
void func_8005845C(u8, u8);
void func_80058494(u8, s32, u8);
void func_8005914C(Unk80085FA8 *, u8, s32, s32);

/* The event dispatcher: reads one byte, and if its top bit is clear treats it
   as running status, otherwise dispatches on the low seven bits through
   jtbl_8003310C. 58 entries covering 0x46..0x7F, of which seven have bodies;
   the other 51 share the default. The table also records the SOURCE order of
   the arms -- 0x4A, 0x4B, 0x47, 0x48, 0x49, 0x46, 0x7F, default -- which is
   not numeric and cannot be recovered any other way.

   The load-bearing shape is that the byte is TWO variables in every block: a
   `u32 w` local to that block for the immediate uses, and the `u8 c` that
   carries it to the common tail. Spelled with one variable, or with two that
   are both `u32`, cse unifies them and the function is four words short.

   * What keeps the pair apart is the `u8`. cse's `insert` only calls
     `make_regs_eqv` when it finds a REG of the SAME MODE in the source's
     equivalence class, and `c = w` with `c` a `u8` is
     `(set (reg:QI c) (subreg:QI (reg:SI w)))` -- no QImode REG in the class,
     so no quantity merge, so `canon_reg` never rewrites `w`'s later uses to
     `c`. Two `u32`s make it a plain SImode copy, `make_regs_eqv` runs, and
     because `c`'s last reference (the tail) is past the end of every
     extended block that contains the copy it becomes the canonical register:
     the uses move to `c`, `w` dies at the copy, and combine folds
     `(set w $v0)` into it. MIPS emits a QImode reg-reg move as `addu`, so
     the dodge costs nothing. `b = a` at the top is the same device.

   * The temps are declared in each case's OWN BLOCK, so each is a separate
     pseudo with `REG_BASIC_BLOCK` set to that arm. local-alloc then ties
     each to $v0, the call's return register, and the `(set w $v0)` insn
     disappears -- which is why the arms read `$v0` for `w & 0x7F` and spend
     only the `addu $s2, $v0, $zero`. One function-scope temp instead is a
     global allocno, gets ONE hard register ($a2 measured), and every arm
     pays a second copy. Case 0x47 is the exception that makes the word count
     122: the `lui` for D_800869B4 and the loop's `sltu` both want $v0, so
     its temp falls to $v1 and `addu $v1, $v0, $zero` is a real insn.

   * The return goes through a local -- `r = c & 0x80; return r != 0;`. As
     one expression the tree handed to `do_store_flag` is a BIT_AND_EXPR
     against zero with a power-of-two mask, which it turns into a shift
     (`srl 7` + `andi 1`); through the local the comparison is on a VAR_DECL
     and expands as `andi` + `sltu`, the two insns the listing splits across
     .L80059780 and .L80059784.

   * `a` stays `u32`: the target copies the call result straight into $s0,
     and a `u8` there would mask it. The running body writes `w & 0x7F`
     twice, once for the test and once for the argument, and cse folds the
     pair into the single `andi $a2, $v0, 0x7F` whose result survives the
     call in $a2.

   verify.py masks the two halfwords that reference jtbl_8003310C, so only
   `make check` can see this land. The table itself is placed by a segment of
   its own in splat/slus_006.14.yaml -- see the comment there, and the
   jump-table section of docs/open-matching-problems.md for the mechanism.
   That placement holds only while this is unk_1C540's one switch function in
   C: the unit's other three tables belong 0x2F8 bytes earlier, so landing
   func_80054F9C, func_80057A94 or func_80057D20 means splitting the unit. */
u8 func_800595C0(Unk80085FA8 *p)
{
    u32 a;
    u8 b;
    u8 c;
    u32 i;
    u32 n;
    Unk80085FA8 *e;
    u32 r;

    a = func_800589B8((s32)p);
    b = a;
    if ((a & 0x80) == 0)
    {
        u32 w = func_800589B8((s32)p);

        c = w;
        if ((w & 0x7F) != 0)
        {
            func_80056E10(p->unk_4C, a & 0x7F, w & 0x7F);
        }
        else
        {
            func_80057948(p->unk_4C, a & 0x7F, 0);
        }
        p->unk_4A = b;
        r = c & 0x80;
        return r != 0;
    }
    switch (a & 0x7F)
    {
    case 0x4A:
        func_80057948(p->unk_4C, p->unk_4A, 0);
        return 0;
    case 0x4B:
        func_80057948(p->unk_4C, p->unk_4A, 0);
        return 1;
    case 0x47:
    {
        u32 w = func_800589B8((s32)p);

        c = w;
        i = 0;
        n = D_800869B4n[0];
        if (n != 0)
        {
            u32 v = (w & 0x7F) * 2 + 2;

            e = D_80085FA8s;
            do
            {
                e->unk_24 = v;
                i++;
                e++;
            } while (i < n);
        }
        break;
    }
    case 0x48:
    {
        u32 w = func_800589B8((s32)p);

        c = w;
        func_80058494(p->unk_4C, 0, w & 0x7F);
        break;
    }
    case 0x49:
    {
        u32 w = func_800589B8((s32)p);

        c = w;
        func_8005845C(p->unk_4C, w & 0x7F);
        break;
    }
    case 0x46:
    {
        u32 w = func_800589B8((s32)p);

        c = w;
        p->unk_4C = w & 0xF;
        break;
    }
    case 0x7F:
    {
        u32 w = func_800589B8((s32)p);

        c = w;
        p->unk_2C = 1;
        break;
    }
    default:
    {
        u32 w = func_800589B8((s32)p);

        c = w;
        func_8005914C(p, (u8)(p->unk_4C + 0xB0), b & 0x7F, w & 0x7F);
        break;
    }
    }
    r = c & 0x80;
    return r != 0;
}

s32 func_800588C8(u32);
s32 func_80058ABC(u8 *);

/* No local for the second result: the target stores it straight out of $v0 and
   only then copies it to $a1, because the `return 0` needs $v0. Reading
   `*(u32 *) (p + 0x18)` back for the sum is free -- cse disambiguates it from
   the +0x38 store -- while `*(u32 *) p` is re-read for real, the call having
   invalidated it. */
s32 func_800597A8(u8 *p) {
    s32 v = func_800588C8(*(u32 *) p);

    *(u32 *) p = v;
    if (v == -1) {
        return 1;
    }
    *(u32 *) (p + 0x18) = func_80058ABC(p);
    *(u32 *) (p + 0x38) = *(u32 *) p;
    *(u32 *) (p + 0x1C) = *(u32 *) p + *(u32 *) (p + 0x18);
    return 0;
}

/* Advances the +0x10 accumulator by the +0x40 step at a rate set by the video
   mode in D_800869A8, then splits the result into a remainder at +0x40 and a
   quotient at +0x10. Ten case values in six bodies.

   Each body stores the mask STRAIGHT INTO +0x40 and then READS +0x10 AGAIN
   for the shift, rather than holding the sum in a local. That re-read is the
   `addu $v1, $v0, $zero` in every arm of the listing, and it is worth nine
   words: cse's `invalidate_memory` drops every pointer-based load after any
   store through a pointer with no offset check at all, so the second read
   survives expand as a real `lw`, which pins the first `sw 0x10` in place
   (sched1 cannot sink a store that a live load depends on) and leaves jump2
   only the two final stores to cross-jump. Hold the sum in a local instead
   and the store sinks to the end of each arm, jump2 cross-jumps it into the
   shared tail too, and every arm is a word short: 94 of 103, 69 DIFF. See
   docs/matching-levers.md. */
void func_80059814(Unk80085FA8 *p)
{
    switch (D_800869A8[0])
    {
    case 0x30:
        p->unk_10 = p->unk_10 * 10;
        p->unk_10 = p->unk_10 + p->unk_40;
        p->unk_40 = p->unk_10 & 3;
        p->unk_10 = p->unk_10 >> 2;
        break;
    case 0x60:
        p->unk_10 = p->unk_10 * 5;
        p->unk_10 = p->unk_10 + p->unk_40;
        p->unk_40 = p->unk_10 & 3;
        p->unk_10 = p->unk_10 >> 2;
        break;
    case 0xC0:
    case 0xF0:
        p->unk_10 = p->unk_10 + p->unk_40;
        p->unk_40 = p->unk_10 & 1;
        p->unk_10 = p->unk_10 >> 1;
        break;
    case 0x120:
    case 0x168:
        p->unk_10 = p->unk_10 / 3;
        break;
    case 0x180:
    case 0x1E0:
        p->unk_10 = p->unk_10 + p->unk_40;
        p->unk_40 = p->unk_10 & 3;
        p->unk_10 = p->unk_10 >> 2;
        break;
    case 0x300:
    case 0x3C0:
        p->unk_10 = p->unk_10 + p->unk_40;
        p->unk_40 = p->unk_10 & 7;
        p->unk_10 = p->unk_10 >> 3;
        break;
    }
}


extern u32 D_800737C4[];
extern u32 D_800737C8[];
/* An incomplete-array alias: this function's reads of the count have their
   `lui` scheduled apart from the `lw` -- one lands in a branch delay slot --
   which the scalar `extern u32 D_800869B4` cannot do, since that form is a
   single insn until maspsx splits it. */
extern u32 D_800869B4n[] __asm__("D_800869B4");

void func_80058B70(void);

/* The sequencer start: clears D_800737C4, picks the format from the first byte
   of the data at D_80085FA4 (0x70 -> 0, 0x4B -> 2, otherwise 1), runs
   func_80058B70, then initialises all D_800869B4 of the D_80085FA8 track
   entries -- one way for formats 0 and 1, another for format 2, where each
   track also gets a running offset advanced by its own length halfword.

   SEVEN THINGS ARE LOAD-BEARING. The last one is the whole of format 2's
   read-back pattern and was worth the final word; the six before it are the
   126 DIFF this came down from.

   * `switch ((s32) D_800737C8[0])` with the CAST. The symbol is `u32` in this
     unit (func_80059BC4 needs that) and unsigned gives one `sltiu` range
     check where the listing has `bltz`, `slti 0x2` and `beq 0x2` -- gcc's
     signed balanced tree over cases 0, 1 and 2.
   * EACH ENTRY POINTER IS INDEXED INSIDE THE LOOP BODY
     (`e = D_80085FA8 + i * 0x50`). Walked as `e += 0x50` loop.c adds a second
     giv biased to `e + 0x40` and rewrites half the fields as negative
     displacements off it; that was 40 words.
   * FORMAT 2's LENGTH HALFWORD IS ALSO INDEXED, off a base read before the
     loop (`b = (u8 *) D_80085FA4[0];`). The base has to be a local because
     the loop calls func_80058A04, so the load is not loop-invariant, and the
     index has to be an index because that is what puts the giv's
     initialisation in the preheader.
   * `f = D_800737C4[0];` IS READ INTO A LOCAL between the three byte stores
     and the three word stores of the shared tail. A store through `e` and a
     load of a global are a real dependence to sched, so left inside
     `if (D_800737C4[0] != 0)` after the stores the load cannot reach a delay
     slot and both loops end with stalls.
   * FORMAT 2's +0x04, +0x38 and +0x1C ARE READ-BACKS, not re-spellings of `n`
     and `len`. That is where the listing's three reg-to-reg moves come from.
     Spelled as `n` and `len` directly the function loses three words.
   * THE TWO STORES OF THE READ-BACK SHARE ONE SOURCE-LEVEL READ -- `y`, read
     once and stored to +0x04 and +0x38. Two separate reads of +0x00 give the
     +0x38 store a register of its own and that is one differing word (this
     function sat at exactly that for two days).
   * AND THE +0x1C SUM RE-READS +0x00, NOT +0x38, with the +0x18 store BETWEEN
     the +0x04 and +0x38 stores. Both halves are about which loads survive cse,
     and the rule behind them is worth stating because it is not obvious:

       A LOAD cse EQUATES BECOMES A COPY BEFORE COMBINE, WHICH FOLDS IT AWAY;
       A LOAD THAT SURVIVES cse BECOMES A COPY IN RELOAD, TOO LATE TO FOLD.

     So every one of the listing's three moves is a load that cse did NOT
     equate, carrying a REG_EQUIV note from local-alloc's `update_equiv_regs`
     and turned into a register copy by reload. cse's memory table is flushed
     by every store here, so a load survives exactly when a store stands
     between it and the store that last wrote its field. `y`'s +0x00 read is
     covered by the +0x24/+0x20 stores; the sum's +0x18 read by the +0x38
     store; and the sum's first operand has to read +0x00 -- covered by three
     stores -- because reading +0x38 puts it immediately after the store that
     writes +0x38, where cse equates it and combine folds the move.
     Reading +0x04 instead is 13 DIFF and reading +0x38 is 132 words. */
s32 func_800599B0(void)
{
    u8 *e;
    u8 *b;
    u32 i;
    u32 n;
    u32 y;
    u32 v;
    u32 len;
    u32 f;

    D_800737C4[0] = 0;
    D_800737C8[0] = 1;
    n = 0;
    if (*(u8 *) D_80085FA4[0] == 0x70)
    {
        D_800737C8[0] = 0;
    }
    else if (*(u8 *) D_80085FA4[0] == 0x4B)
    {
        D_800737C8[0] = 2;
    }
    func_80058B70();
    switch ((s32) D_800737C8[0])
    {
    case 0:
    case 1:
        i = 0;
        if (D_800869B4n[0] != 0)
        {
            do
            {
                e = D_80085FA8 + i * 0x50;
                if (D_800737C8[0] == 1)
                {
                    *(u32 *) (e + 0x00) = n + 1;
                    *(u32 *) (e + 0x24) = 0x68;
                    *(u32 *) (e + 0x20) = 0x68;
                    func_800597A8(e);
                    n = *(u32 *) (e + 0x00);
                }
                *(u32 *) (e + 0x04) = *(u32 *) (e + 0x00);
                v = func_80058A04((s32) e);
                e[0x48] = 0;
                e[0x4A] = 0;
                e[0x49] = 0;
                f = D_800737C4[0];
                *(u32 *) (e + 0x10) = v;
                *(u32 *) (e + 0x2C) = 0;
                *(u32 *) (e + 0x40) = 0;
                if (f != 0)
                {
                    func_80059814((Unk80085FA8 *) e);
                }
                i++;
            } while (i < D_800869B4n[0]);
        }
        break;
    case 2:
        n = 0x50;
        b = (u8 *) D_80085FA4[0];
        i = 0;
        if (D_800869B4n[0] != 0)
        {
            do
            {
                e = D_80085FA8 + i * 0x50;
                *(u32 *) (e + 0x00) = n;
                len = *(u16 *) (b + i * 2 + 0x10);
                *(u32 *) (e + 0x24) = 0x68;
                *(u32 *) (e + 0x20) = 0x68;
                y = *(u32 *) (e + 0x00);
                *(u32 *) (e + 0x04) = y;
                *(u32 *) (e + 0x18) = len;
                *(u32 *) (e + 0x38) = y;
                *(u32 *) (e + 0x1C) = *(u32 *) (e + 0x00) + *(u32 *) (e + 0x18);
                n += len;
                v = func_80058A04((s32) e);
                e[0x48] = 0;
                e[0x4A] = 0;
                e[0x49] = 0;
                f = D_800737C4[0];
                *(u32 *) (e + 0x10) = v;
                *(u32 *) (e + 0x2C) = 0;
                *(u32 *) (e + 0x40) = 0;
                if (f != 0)
                {
                    func_80059814((Unk80085FA8 *) e);
                }
                i++;
            } while (i < D_800869B4n[0]);
        }
        break;
    }
    return 0;
}

void func_8005947C(Unk80085FA8 *);
u8 func_800595C0(Unk80085FA8 *);
void func_80059814(Unk80085FA8 *);

/* Per-entry tick: advances each live D_80085FA8 entry's +0x3C accumulator by
   its +0x24 step and, on each wrap past 0x100, either counts +0x10 down or --
   when it is already zero -- runs the command pump until something sets it.

   The inner pump is written with `goto`, and that is the whole point: the C
   front end only emits NOTE_INSN_LOOP_BEG for a real loop statement, so
   loop.c never sees a goto loop and never hoists its invariants. Written as
   `for (;;)` with `continue`, loop-invariant motion lifts
   `(high D_800737C8)` into the preheader, which puts the loop's LABEL after
   the `lui` instead of before it -- and then reorg cannot move the
   fall-through's `lui %hi(D_800737C4)` into the first back edge's delay slot,
   because the opposite thread now starts with the `lw` that READS that
   register. It copies the loop's own `lui` in instead, and the function is a
   word long. With `goto` the label precedes the `lui`, the opposite thread
   starts with an insn that only WRITES the register, and the move goes
   through.

   D_800737C8 has to be the incomplete-array form for the same reason: a
   4-byte scalar is small data at -G8, one insn until maspsx splits it, so its
   `lui` could not be stolen into the second back edge's slot at all.

   `e = &D_80085FA8s[i]` indexed, not a walked `e++`: the pointer is passed to
   every call, so a source-level increment would be a biv that cannot be
   eliminated AND loop.c would reduce the field addresses into a second
   register. Indexed, the giv is the one `addiu $s0, $s0, 0x50` the listing
   has. */
s32 func_80059BC4(void)
{
    Unk80085FA8 *e;
    u32 i;
    u32 v;

    i = 0;
    if (D_800869B4n[0] != 0)
    {
        do
        {
            e = &D_80085FA8s[i];
            if (e->unk_2C == 0)
            {
                v = e->unk_3C + *(u16 *) &e->unk_24;
                e->unk_3C = v;
                if (v >= 0x100)
                {
                    e->unk_3C = v & 0xFF;
                    if (e->unk_10 == 0)
                    {
                    top:
                        if (D_800737C8[0] != 2)
                        {
                            func_8005947C(e);
                            if (e->unk_2C != 0)
                            {
                                goto out;
                            }
                            e->unk_10 = func_80058A04((s32) e);
                        }
                        else if ((u8) func_800595C0(e) == 0)
                        {
                            if (e->unk_2C != 0)
                            {
                                goto out;
                            }
                            e->unk_10 = func_80058A04((s32) e);
                        }
                        else if (e->unk_2C != 0)
                        {
                            goto out;
                        }
                        if (e->unk_10 == 0)
                        {
                            goto top;
                        }
                        if (D_800737C4[0] != 0)
                        {
                            func_80059814(e);
                        }
                        if (e->unk_10 == 0)
                        {
                            goto top;
                        }
                    out:
                        if (e->unk_10 != 0)
                        {
                            e->unk_10 = e->unk_10 - 1;
                        }
                    }
                    else
                    {
                        e->unk_10 = e->unk_10 - 1;
                    }
                }
            }
            i++;
        } while (i < D_800869B4n[0]);
    }
    return 0;
}

void func_80059D1C(void) {
    u32 i = 0;

    if (D_800869B4 != 0) {
        do {
            D_80085FA8s[i].unk_2C = 1;
            D_80085FA8s[i].unk_00 = 0;
            i++;
        } while (i < D_800869B4);
    }
}

s32 func_80059D60(void) {
    u32 i;

    for (i = 0; i < D_800869B4; i++) {
        if (D_80085FA8s[i].unk_2C == 0) {
            return 1;
        }
    }
    return 3;
}

/* Sorts the D_800869C0 slot table ascending by each slot's offset. Counts the
   live slots first -- stop at a zero offset, cap sixteen -- then a bubble sort
   comparing slot `i` against slot `j` for `j` from `n - 1` down to `i + 1`,
   swapping the whole eight-byte pair through a stack temp, which is the entire
   frame.

   FOUR SHAPES ARE LOAD-BEARING and all four are SYMPTOMs -- matching artefacts
   rather than anything the original author would have had to think about:

     * the bases go through LOCALS with the index written first,
       `(u32 *) (i * 8 + (u32) c)` and not `c + i * 8`, which is the same idiom
       func_8005A1D0 below already uses;
     * the two loops need SEPARATE base locals, because with one the second
       loop reuses the first's register and the second `lui`/`addiu` pair goes
       missing;
     * the second loop's base is materialised INSIDE the outer body, so loop.c
       hoists it to the preheader, which is AFTER the `blez` guard. Declared
       between the loops it lands before the guard and reorg then steals
       `i = 0` into the first loop's `beq` delay slot;
     * the comparison is `*(s32 *) <d> > *(s32 *) s`, not `s < d` -- same test,
       but the operand named first is loaded first.

   AND THE ROW POINTER IS WRITTEN OUT IN FULL rather than kept in a `u32 *d`
   local, which is the whole of the last eight words. The listing walks
   `&base[i]` as a strength-reduced giv in $t1 and copies it into $a1 inside
   the outer body; that copy is what loop.c emits for a giv it reduces but
   cannot REPLACE. Whether the giv happens at all is `benefit` surviving above
   zero (loop.c:3786-3823), and the subtraction that kills it here,
   `benefit -= copy_cost`, is guarded by `REG_USERVAR_P (v->dest_reg)`
   (loop.c:3799). A named `d` is a user variable, pays the copy_cost, and ends
   at 4 - 2 - 2 = 0, so the giv is dropped and the address is recomputed from
   `i` every outer iteration. Spelt inline, the pointer is a pseudo cse1
   created, REG_USERVAR_P is false, the subtraction is skipped, benefit is
   4 - 2 = 2, and the giv appears -- together with the copy, because it is
   still not replaceable. The freed delay slot at the outer back edge then
   takes the giv's `addiu $t1, $t1, 0x8` instead of a duplicated `n - 1`. */
void func_80059DAC(void)
{
    u8 *b = (u8 *)D_800869C0;
    s32 n;
    s32 i;

    for (n = 0; n < 0x10; n++)
    {
        if (*(u32 *)(n * 8 + (u32)b) == 0)
        {
            break;
        }
    }
    for (i = 0; i < n; i++)
    {
        u8 *c = (u8 *)D_800869C0;
        s32 j;

        for (j = n - 1; i < j; j--)
        {
            u32 *s = (u32 *)(j * 8 + (u32)c);
            u32 t[2];

            if (*(s32 *)((u32 *)(i * 8 + (u32)c)) > *(s32 *)s)
            {
                t[1] = ((u32 *)(i * 8 + (u32)c))[1];
                t[0] = ((u32 *)(i * 8 + (u32)c))[0];
                ((u32 *)(i * 8 + (u32)c))[1] = s[1];
                ((u32 *)(i * 8 + (u32)c))[0] = s[0];
                s[1] = t[1];
                s[0] = t[0];
            }
        }
    }
}

/* Claims a D_800869C0 slot -- a {offset, length} pair per slot, sixteen of
   them -- for a run of `size` bytes and returns its offset, or -1 when the
   run would reach past 0x80000 - D_80073830[D_8007382B[0]]. The slot table
   empty is its own arm, allocating at 0x1010; otherwise the first slot with a
   zero length is taken. Running off the end of the table is not an error: the
   0x1010 that `base` still holds is what comes back.

   `p` IS SET ONCE and the rows are reached as `p[i * 2]`, not by walking
   `p += 2`. Both halves of the listing's preheader want that:

     * strength reduction turns the two row references into ONE giv -- the
       `addiu $a0, $a0, 0x8` at the bottom -- where `p += 2` plus `p[0]`/`p[1]`
       leaves the user's own pointer and `D_800869C0[i * 2]` spelt out gives
       two givs that combine_givs does not merge;
     * a pointer set once is a pseudo with `reg_n_sets == 1`, so sched1's
       birthing-insn rule raises `p = D_800869C0` above the hoisted `lui
       $a3, 0x8` and emits the constant FIRST. With `p += 2` the pointer is
       set twice, the two tie on priority, RTL order wins, and the constant
       comes out one slot late -- which also costs the registers, because
       `$a3` is then free to carry both values in turn ($a3 for the table
       address, reused for 0x80000) instead of the listing's $t0 and $a3. */
s32 func_80059E94(u32 size)
{
    u32 base;
    u32 *p;
    s32 i;

    base = 0x1010;
    if (D_800869C0[0] == 0)
    {
        if (base + size < 0x80000 - D_80073830[D_8007382B[0]])
        {
            D_800869C0[0] = base;
            D_800869C0[1] = size;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        i = 0;
        p = D_800869C0;
        for (; i < 0x10; i++)
        {
            if (p[i * 2 + 1] == 0)
            {
                base = p[i * 2];
                if (base + size < 0x80000 - D_80073830[D_8007382B[0]])
                {
                    p[i * 2] = base + size;
                    p[i * 2 + 1] = size;
                    break;
                }
                return -1;
            }
        }
    }
    func_80059DAC();
    return base;
}

/* Claims a D_800869C0 slot for the run [p, p + n) at a CALLER-CHOSEN offset
   `p`, keeping the table sorted by offset, and returns `p`, or -1 when the
   run is out of range, overlaps a neighbour, or the table is full. Row 0 is
   special-cased (the empty table, and a run below the first row); otherwise
   the walked cursor `q` finds the row at or above `p`.

   Everything about the shape below was read off the listing and the
   compiler's own dumps; each item names the pass that wants it.

   1. THE TAIL IS INSIDE THE LOOP, ONCE PER SCAN, AND THE FUNCTION ENDS IN
      THE CALL. loop.c's find_and_verify_loops (loop.c:2222; the move at
      :2331-2427) takes every block that ends in a jump out of the loop and
      is hopped over by a conditional jump, and drops it after the first
      BARRIER at the jump target's loop depth found walking BACK from that
      target (:2390-2393). The hit and new-slot blocks jump to the return
      label, so they land after the empty arm's barrier -- the target's
      .L8005A018/.L8005A028 -- only if nothing with a barrier sits between
      the loop and the epilogue: the `if (i == lim) return -1; break;`
      tails are in the loop body and the loop falls into
      `func_80059DAC(); return p;`. Each scan's store block then lands after
      its own tail's barrier (scan 1) or after the new-slot test's
      `return -1` (scan 2), which is where the listing has them.

   2. NO CALL IN THE LOOP. A CALL_INSN sets unknown_address_altered
      (loop.c:2199-2203) and no MEM is invariant after that; the target
      hoists the `lbu` of D_8007382B into the preheader.

   3. `lim`. Written as the constant, the two `(set tmp 16)` are movables
      that combine_movables (loop.c:1245-1285) adds together and move_movables
      (:1630-1633) hoists -- and the 3 it takes off the threshold (:1719) is
      what leaves the `high D_8007382B` behind. A user variable assigned in
      two blocks fails all three admission clauses (loop.c:695-700:
      maybe_never, REG_USERVAR_P, reg_in_basic_block_p) and stays put.

   4. STRUCT ROWS AND A NON-AGGREGATE BYTE READ. With store blocks in the
      loop, the `lbu` is invariant only if true_dependence (sched.c:817-838)
      clears it against every store: the stores must be MEM_IN_STRUCT_P
      (COMPONENT_REF, expr.c:4890) and the byte read must NOT be
      (`D_8007382B[0]` is, expr.c:4567-4576 marks any aggregate reference).
      `*(u8 *) (u32) D_8007382B` is the read that is not. Measured:
      `D_8007382B[0]` here is 146 words / 112 DIFF.

   5. THE SCANS INDEX THE TABLE BY THE COUNTER WITH NO POINTER LOCAL. The
      row address is then a compiler-temp giv, and loop.c:3798-3800 only
      charges copy_cost to a REG_USERVAR_P giv, so it is reduced (benefit
      4 - add_cost 2) even though its store block has been moved out of the
      loop and makes it non-replaceable; the `tmp = giv` copy loop.c emits
      is what cse2 then sorts out per scan. Scan 1's block sits AFTER the
      loop: cse2's TAKEN pass (store block inside, cse_basic_block_end past
      it) leaves the reduced register canonical and the stores on it, the
      NOT_TAKEN pass (end at the back-branch) makes the copy canonical
      (make_regs_eqv, cse.c:850-857) and puts the load and the increment on
      it -- `addu $v1,$a0 / lw 4($v1) / addiu $a0,$v1,8 / sw 0($a0)`. Scan
      2's block sits BEFORE the loop, the copy's first reference precedes
      the block (clause 1b) in both passes, the reduced register stays
      canonical and the copy dies: one register. A pointer local cannot do
      this: cse1 retargets `u = X; v = u` into `v = X` (cse.c:7452-7476)
      and the user variable it leaves is either not reduced or not split.
      The lo_sum for the row address is also born inside the scan, after
      `c < p + n`, which is why the preheader copies come out high, plus,
      lo_sum; a pointer local initialised in the arm shares the `c` load's
      lo_sum temp and hoists it ahead of the plus (145/9 at best).

   6. `e2`. The `slt` compares against a RECOMPUTED `p + n` (`addu
      $v0,$s0,$a1` at 8005A150) while the two other sites use the hoisted
      copy. A user variable with a dead earlier set (`e2 = 0` before the
      loop, deleted by flow) moves regno_first_uid off the in-loop set, so
      reg_in_basic_block_p's first test (loop.c) fails and the set stays.
      Without `e2 = 0` it hoists: 144 words / 37 DIFF. Reassigning `end`
      instead changes the entry block (148/118); reusing `c` moves 20
      registers (146/53). */
typedef struct
{
    u32 offset;
    u32 length;
} Slot;

s32 func_80059F8C(u32 p, u32 n)
{
    s32 i;
    u32 b;
    u32 c;
    u32 end;
    u32 e2;
    s32 lim;
    Slot *q;
    Slot *r;

    if ((u32) (p - 0x1010) > 0x7EFEF)
    {
        return -1;
    }
    e2 = 0;
    end = p + n;
    if (end > 0x7FFFF)
    {
        return -1;
    }
    q = (Slot *) D_800869C0;
    if (D_800869C0[0] == 0)
    {
        if (end < 0x80000 - D_80073830[*(u8 *) (u32) D_8007382B])
        {
            D_800869C0[0] = p;
            D_800869C0[1] = n;
            func_80059DAC();
            return p;
        }
        return -1;
    }
    for (i = 0; i < 0x10; i++, q++)
    {
        if (i == 0)
        {
            /* named so the load stays in the loop: scan_loop's clause (1)
               refuses a user-variable set past the `if (i == 0)` branch */
            c = D_800869C0[0];
            if (p < c)
            {
                if (c < p + n)
                {
                    return -1;
                }
                while (i < 0x10)
                {
                    if (((Slot *) D_800869C0)[i].length == 0)
                    {
                        ((Slot *) D_800869C0)[i].offset = p;
                        ((Slot *) D_800869C0)[i].length = n;
                        break;
                    }
                    i++;
                }
                lim = 0x10;
                if (i == lim)
                {
                    return -1;
                }
                break;
            }
        }
        else if (p < q->offset)
        {
            continue;
        }
        b = q->offset;
        if (b == p)
        {
            q->offset = p;
            q->length = n;
            return p;
        }
        /* index-first through a cast keeps row i+1 off row i's giv */
        r = (Slot *) ((i + 1) * 8 + (u32) D_800869C0);
        if (r->length == 0)
        {
            if (p + n < 0x80000 - D_80073830[*(u8 *) (u32) D_8007382B])
            {
                r->offset = p;
                r->length = n;
                func_80059DAC();
                return p;
            }
            return -1;
        }
        if (b < p && p < r->offset)
        {
            if (p < b + q->length)
            {
                return -1;
            }
            e2 = p + n;
            if ((s32) r->offset < (s32) e2)
            {
                return -1;
            }
            while (i < 0x10)
            {
                if (((Slot *) D_800869C0)[i].length == 0)
                {
                    ((Slot *) D_800869C0)[i].offset = p;
                    ((Slot *) D_800869C0)[i].length = n;
                    break;
                }
                i++;
            }
            lim = 0x10;
            if (i == lim)
            {
                return -1;
            }
            break;
        }
    }
    func_80059DAC();
    return p;
}

/* UNPARKED. Both loops index off a base LOCAL with the index written first --
   `(u8 *) (i * 8 + (u32) b)`, not `b + i * 8` -- which is what gives the
   target's `addu $v0, $v0, $a2` operand order and stops loop.c walking the
   inner row pointer. `s32 i = 0;` is declared BEFORE `b` so its `addu $v1,
   $zero, $zero` comes out ahead of the base's lui/addiu. */
void func_8005A1D0(u32 k) {
    s32 i = 0;
    u8 *b = (u8 *) D_800869C0;

    for (; i < 0x10; i++) {
        if (*(u32 *) (i * 8 + (u32) b) == k) {
            while (i < 0x10) {
                u8 *d = (u8 *) (i * 8 + (u32) b);
                u8 *s = (u8 *) ((i + 1) * 8 + (u32) b);
                u32 v;

                *(u32 *) (d + 4) = *(u32 *) (s + 4);
                v = *(u32 *) s;
                *(u32 *) d = v;
                if (v == 0) {
                    break;
                }
                i++;
            }
            func_80059DAC();
            return;
        }
    }
}

/* Resets both tables. D_800869C0 is a pair of words per slot, cleared +4
   before +0. */
void func_8005A26C(void) {
    Unk80086A40 *p;
    Unk80086C00 *q;
    u32 *r;
    s32 i;

    p = D_80086A40;
    for (i = 0; i < 0x10; i++) {
        p[i].unk_00 = -1;
        p[i].unk_19 = 0x7F;
        p[i].unk_1A = 0x7F;
        p[i].unk_1B = 0x40;
    }
    q = D_80086C00;
    r = D_800869C0;
    for (i = 0; i < 0x10; i++) {
        q[i].unk_00 = -1;
        q[i].unk_02 = 0;
        q[i].unk_0A = 0x7F;
        q[i].unk_08 = 0x7F;
        r[i * 2 + 1] = 0;
        r[i * 2] = 0;
    }
    D_80086D4C[0] = 0x1000;
}

void func_8005D838(s32, u8 *);

void func_8005A2FC(void) {
    func_8005CA70();
    func_80056C30();
    func_8005A26C();
    func_8005D838(0x10, D_80086CC0);
}

void func_800585A0(void);

void func_8005A33C(void) {
    /* Nested ifs, not `> 0 && < 4`: written with && the two bounds combine
       into one `addiu -1` / `sltiu 3` range check (13 DIFF). */
    if ((s32) D_80086D4C[0] > 0) {
        if ((s32) D_80086D4C[0] < 4) {
            func_800585A0();
        }
    }
    D_80073824[0] = 1;
}

void func_8005A33C(void);

void func_8005A37C(void) {
    func_8005A33C();
}

void func_8005A39C(u32 v) {
    D_80086D4C[0] = v;
}

void func_80058700(void);

void func_8005A3A8(void) {
    if (D_80073824[0] != 0) {
        func_80058700();
    }
}

void func_8005A3D4(void) {
    D_8007382A[0] = 0;
}

void func_8005A3E0(void) {
    D_8007382A[0] = 1;
}

s32 func_8005A3F0(u32 a) {
    a &= 0xFF;
    if (a >= 0x19) {
        return 0xFF;
    }
    if (a == 0) {
        return 0xFF;
    }
    D_80073734[0] = a;
    return a;
}

void func_8005A420(void) {
}

void func_8005863C(void);
void func_8005A1D0(u32);
void func_8005A26C(void);

/* Nested ifs for the 0 < x < 4 test, as in func_8005A33C: with `&&` the two
   bounds combine into one range check. */
void func_8005A428(void) {
    Unk80086A40 *p;
    s32 i;

    func_80056C30();
    if ((s32) D_80086D4C[0] > 0) {
        if ((s32) D_80086D4C[0] < 4) {
            func_8005863C();
        }
    }
    p = D_80086A40;
    for (i = 0; i < 8; i++) {
        if (p[i].unk_00 >= 0) {
            func_8005A1D0(p[i].unk_10);
        }
    }
    func_8005A26C();
}

void func_80056C30(void);
void func_8005A26C(void);
void func_8005D7BC(void);

void func_8005A4B8(void) {
    func_80056C30();
    func_8005A26C();
    func_8005D7BC();
}

void func_8005EDA0(s16 *);

void func_8005A4E8(u8 a, u8 b, u8 c) {
    u32 t[10];

    if (a == 0) {
        if (b == 0) {
            t[0] = 0x200;
            t[6] = c;
        } else {
            t[0] = 0x100;
            t[5] = c;
        }
    } else {
        if (b == 0) {
            t[0] = 0x2000;
            t[9] = c;
        } else {
            t[0] = 0x1000;
            t[8] = c;
        }
    }
    func_8005EDA0((s16 *) t);
}

void func_8005EDA0(s16 *);

/* The two shifts are hoisted out of the arms, and as `s32` locals: that is
   what sign-extends them (`sll 16` + `sra 8`). Written inside the arms, each
   store needs only the low half and gcc emits a bare `sll 8` twice over. */
void func_8005A56C(s16 f, s16 x, s16 y) {
    s16 t[20];
    s32 u = x << 8;
    s32 v = y << 8;

    if (f == 0) {
        *(s32 *) &t[0] = 0xC0;
        t[8] = u;
        t[9] = v;
    } else {
        *(s32 *) &t[0] = 0xC00;
        t[14] = u;
        t[15] = v;
    }
    func_8005EDA0(t);
}

void func_8005EDA0(s16 *);

void func_8005A5C8(s16 x, s16 y) {
    s16 t[20];

    *(s32 *) &t[0] = 0xF;
    t[2] = x << 7;
    t[3] = y << 7;
    t[4] = 0;
    t[5] = 0;
    func_8005EDA0(t);
}

extern s32 D_8007382C[];

/* The scan is an INFINITE loop with both exits inside it, not a `do`/`while`
   with the `return -1` after it. The NOTE_INSN_LOOP_END then lands after that
   return rather than before it, and that one note is the whole 13 DIFF: it
   defers `sw $zero, %lo(D_8007382C)($v0)` three sched1 slots, past the raw
   copy of `id`, so $v0 stays live and the copy falls to $v1. $v0 is then free
   for the entry test's -1 -- and because `lui $v0, %hi(D_80086A40)` clobbers
   it immediately after the `bne`, reload's find_equiv_reg no longer sees a
   register holding -1 and re-materialises `addiu $a0, $zero, -0x1` in the
   preheader instead of copying it. The `do`/`while` spelling keeps -1 in $a2
   across the whole scan and pays a `move` for it.
   `while (1)` with a `break` and the return outside is 13 DIFF again; the
   loop rewritten as `while (i < 0x10)` or `for (i = 0; i < 0x10; i++)` is
   also 13. `id = id << 16; id = id >> 16;` is the in-place sign-extension
   (section 5) and `j = id` has to be the s16 truncation, both already here. */
s32 func_8005A608(u8 *p, s32 id)
{
    s16 i;
    s16 j;
    s32 k;
    Unk80086A40 *e;
    s32 h;

    D_8007382C[0] = 0;
    i = 0;
    j = id;
    id = id << 16;
    id = id >> 16;
    if (id == -1)
    {
        for (;;)
        {
            if (D_80086A40[i].unk_00 == -1)
            {
                goto found;
            }
            i++;
            if (i >= 0x10)
            {
                return -1;
            }
        }
    }
    i = j;
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_00 = i;
    e->unk_04 = p;
    e->unk_08 = (*(u16 *) (p + 0x12) << 9) + 0xA20;
    e->unk_14 = *(u32 *) (p + 0xC) - e->unk_08;
    e->unk_18 = p[0x18];
    e->unk_1B = p[0x19];
    h = func_80059E94(e->unk_14);
    e->unk_10 = h;
    if (h == -1)
    {
        return -1;
    }
    return k;
}

/* func_80059F8C is now defined in THIS unit (above) with this signature;
   the declaration is kept as the record of how it was settled. Two parks used to
   disagree with it and no longer do: measured 2026-10-30, func_80059F8C's own
   park is 101 DIFF whether its second parameter is `s32` or `u32`, and
   func_8005A90C is 2 DIFF whether it declares this callee `(s32, u32)` or
   `(u32, u32)`, so the spelling below is free at all three sites and both
   parks now use it. Nothing has to be reconciled here at landing. Changing it
   is still a change to this function's output and needs re-measuring. */
s32 func_80059F8C(u32, u32);

/* The three-argument twin of func_8005A608 above: same scan, same note
   placement (the `for (;;)` with both exits inside it is worth 16 of the 20
   words -- see that function's comment), plus a bound check on the supplied
   `id` and a second copy of `p`.

   Two spellings carry the last 4 words.

   `id` is an `s16` PARAMETER here, not an `s32` sign-extended in place. The
   raw `addu $v1, $a1, $zero` for `j = id` has to come out BEFORE the
   `sll`/`sra` pair, and section 5's rule puts a narrow parameter's
   sign-extension at its first INT use -- which is the `== -1` test, i.e.
   after the copy. func_8005A608's in-place `id = id << 16; id = id >> 16;`
   extends first and is 4 DIFF; `s16 id` with those shifts kept as well is
   95 DIFF, and a separate `t = id << 16; t = t >> 16;` is 93. The caller in
   unk_2879C.c already declares the parameter `s16`.

   `q = p` has to be written AFTER `j = id`. The copy's RTL position is the
   assignment's, so `u8 *q = p;` as a declaration initialiser -- or the
   assignment anywhere ahead of `j = id` -- emits `sw $s3, 0x1C($sp)` and
   `addu $s3, $s4, $zero` one slot early, 3 DIFF. Written after `j = id` the
   pair lands between that copy and the `sll`. The second pointer is not
   optional: using `p` for both the stored value and the +0x12/+0xC/+0x18
   loads is 97 DIFF, and giving `q` the store and `p` the loads is 6.
   `D_8007382C[0] = 0;` ahead of `i = 0;` (which is the order func_8005A608
   wants) is 7 DIFF here. */
s32 func_8005A778(u8 *p, s16 id, u32 h)
{
    u8 *q;
    s16 i;
    s16 j;
    s32 k;
    Unk80086A40 *e;
    s32 r;

    i = 0;
    D_8007382C[0] = 0;
    j = id;
    q = p;
    if (id == -1)
    {
        for (;;)
        {
            if (D_80086A40[i].unk_00 == -1)
            {
                goto found;
            }
            i++;
            if (i >= 0x10)
            {
                return -1;
            }
        }
    }
    i = j;
    if (i >= 0x10)
    {
        return -1;
    }
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_00 = i;
    e->unk_04 = p;
    e->unk_08 = (*(u16 *) (q + 0x12) << 9) + 0xA20;
    e->unk_10 = h;
    e->unk_14 = *(u32 *) (q + 0xC) - e->unk_08;
    e->unk_18 = q[0x18];
    e->unk_1B = q[0x19];
    r = func_80059F8C(h, e->unk_14);
    e->unk_10 = r;
    if (r == -1)
    {
        return -1;
    }
    return k;
}

u32 func_8005ECA0(u32);
u32 func_8005EC40(u8 *, u32);

/* The four-argument sibling of func_8005A608 and func_8005A778 above: same
   scan with both exits inside the `for (;;)`, same in-place sign extension of
   `id`. Three things carry the rest, each measured on the one before it:
   the `do { } while (0);` after the unk_14 store is a block boundary that keeps
   the call's argument copies below the pointer computation, which is what
   makes b and c overlap e and costs the fifth and sixth callee-saved
   registers the listing spends (99 words / 87 DIFF without it, 103 / 13 with);
   the func_8005ECA0 result goes back into the dead parameter b, not a fresh
   local (13 -> 11); and the DEVICE at the stores, which is the last 2 words
   and is explained where it sits. The `(u32, u32)` prototype for
   func_80059F8C is the unit's own, above. */
s32 func_8005A90C(u8 *p, s32 b, u32 c, s32 id)
{
    s16 i;
    s16 j;
    s32 k;
    Unk80086A40 *e;
    s32 r;

    D_8007382C[0] = 0;
    i = 0;
    j = id;
    id = id << 16;
    id = id >> 16;
    if (id == -1)
    {
        for (;;)
        {
            if (D_80086A40[i].unk_00 == -1)
            {
                goto found;
            }
            i++;
            if (i >= 0x10)
            {
                return -1;
            }
        }
    }
    i = j;
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_14 = c;
    do
    {
    } while (0);
    {
        /* DEVICE: the call's second argument copied into $a1 HERE, by a
           register variable pinned to $5, plus an empty asm that reads and
           rewrites that register. Worth the last 2 of 103 words; without it
           the function is 103/103 and 2 DIFF with `addu $a0` and `addu $a1`
           the other way round, and nothing in the call can fix that, because
           expand_call emits the $a0 copy before the $a1 copy for every call
           (calls.c:1850, no PUSH_ARGS_REVERSED on mips) and sched1 breaks
           their tie on INSN_LUID (sched.c:2428), which keeps that order.
           Emitted at the declaration, the $a1 copy is the first insn after
           the loop notes above, so sched_analyze_insn (sched.c:2058) makes it
           the fence that everything after depends on: it sits right after the
           unk_14 store, the $a0 copy lands after it, and the call's own
           `$a1 = $a1` is deleted as a no-op by flow (flow.c:957). The asm is
           needed because cse prices a user-variable hard register at 0
           against a pseudo's 1 (cse.c:477-485) and would otherwise rewrite
           `e->unk_08 = c` to store $a1 -- 1 DIFF; its set of $a1 ends that
           equivalence and its empty body emits nothing.
           Ruled out, all 103/103: the pinned copy without the asm, 1 DIFF
           (`sw $a1,0x8`); `__asm__ volatile("")` instead of the asm, 1 DIFF;
           the copy before the `do {} while (0)`, 11 DIFF (it floats to the
           block top); no loop at all, 11 DIFF. A plain `register u32 zz = c;`
           with the same asm also MATCHES, by a different route (reload
           materialises the copy for the asm's matching constraint); the
           pinned form is the one whose dump was read. Silent under -Wall; no
           uninitialised read. */
        register u32 zz __asm__("$5") = c;
        __asm__("" : "=r"(zz) : "0"(zz));
        e->unk_18 = 0x7F;
        e->unk_00 = i;
        e->unk_04 = p;
        e->unk_08 = c;
        e->unk_1B = 0x40;
        e->unk_10 = b;
        r = func_80059F8C(b, zz);
    }
    e->unk_10 = r;
    if (r == -1)
    {
        return -1;
    }
    b = func_8005ECA0(r);
    if (func_8005EC40(p, e->unk_14) != e->unk_14)
    {
        return -1;
    }
    *(u32 *) e->pad_0C = (u32) p;
    e->unk_10 = b;
    return k;
}

/* The table base through a pointer local initialised in its declaration, so
   its lui/addiu comes out BEFORE the index's sign extension and the final
   `addu` has the operands the target's way round (8 DIFF indexing the symbol
   directly). */
s32 func_8005AAA8(u8 *p, s16 k) {
    Unk80086A40 *t = D_80086A40;
    u8 *e = (u8 *) &t[k];
    s16 n = *(s16 *) e;
    u32 r;

    if (n != k) {
        return -1;
    }
    r = func_8005ECA0(*(u32 *) (e + 0x10));
    if (func_8005EC40(p, *(u32 *) (e + 0x14)) != *(u32 *) (e + 0x14)) {
        return -1;
    }
    *(u32 *) (e + 0xC) = (u32) p;
    *(u32 *) (e + 0x10) = r;
    return n;
}

extern s32 D_8007382C[];

u32 func_8005ECA0(u32);
u32 func_8005EC40(u8 *, u32);

s32 func_8005AB50(u8 *p, u32 n, s16 k)
{
    Unk80086A40 *t = D_80086A40;
    u8 *e = (u8 *)&t[k];
    s16 id = *(s16 *)e;
    u32 avail;
    s32 cur;
    s32 lim;

    if (id != k)
    {
        return -1;
    }
    func_8005ECA0(*(u32 *)(e + 0x10) + D_8007382C[0]);
    avail = *(u32 *)(e + 0x14) - D_8007382C[0];
    if (avail < n)
    {
        n = avail;
    }
    if (func_8005EC40(p, n) != n)
    {
        return -1;
    }
    cur = D_8007382C[0];
    lim = *(s32 *)(e + 0x14);
    cur += n;
    D_8007382C[0] = cur;
    if (cur >= lim)
    {
        return id;
    }
    return -2;
}

s32 func_8005ECF8(s32);

s16 func_8005AC30(s16 a) {
    /* Two returns of the same call, not `func_8005ECF8(a != 0)`, which
       store-flags into one `sltu`. And the arm written FIRST ends up SECOND in
       memory: crossjumping merges the two tails and leaves the second arm as
       the fall-through, so `a == 0` first is what gives the target's `beqz`. */
    if (a == 0) {
        return func_8005ECF8(0);
    } else {
        return func_8005ECF8(1);
    }
}

void func_8005A1D0(u32);

/* The base read into a local first, so its lui/addiu come out ahead of the
   28-stride index chain (8 DIFF the other way round). The -1 is one constant
   shared by the compare and the store, which is why it sits in a callee-saved
   register across the call. */
void func_8005AC68(s16 i) {
    Unk80086A40 *b = D_80086A40;
    Unk80086A40 *p = &b[i];

    if (p->unk_00 != -1) {
        func_8005A1D0(p->unk_10);
        p->unk_00 = -1;
    }
}

void func_8005ACD0(u16 v) {
    D_80073828[0] = v;
}

void func_8005E97C(s32, s32);
s32 func_8005EB78(u32);

/* Ages every live D_80085458 entry whose D_80073740 handle func_8005EB78
   reports in state 3: the +0x1A counter is bumped until it reaches 2, and on
   the second pass the voice is stopped (func_8005E97C, then spun on
   func_8005EB78 until it reports 2 or 0) and the counter cleared. The handles
   stopped this way are OR-ed into one mask and stopped once more at the end.

   Same two idioms as func_8005BAB0: the entry pointer is INDEXED inside the
   body so loop.c keeps one giv with +0x1A as its displacement, and
   D_80073740[i] is indexed rather than walked.

   The guard is TWO NESTED IFS with `i = 0` between them, not one `&&` chain.
   Written as a chain, `i = 0` lands after the second branch and reorg fills
   that branch's delay slot with the `lui %hi(D_80085458)` of the first giv
   instead of the `i = 0` copy -- 3 DIFF, and no statement order inside the
   loop moves it. Split, the copy sits before the branch and reorg sinks it.

   Both globals are read through a SIGNED cast: D_80073828 is `lh`, and the
   bound is `slt`. */
void func_8005ACDC(void)
{
    s32 m;
    s32 i;
    s32 k;
    u32 v;

    m = 0;
    if ((s16)D_80073828[0] == 0)
    {
        i = 0;
        if (m < (s32)D_80073734[0])
        {
            do
            {
                u8 *e = D_80085458 + i * 0x78;

                if (*(u16 *)(e + 0x1A) != 0)
                {
                    if (func_8005EB78(D_80073740[i]) == 3)
                    {
                        v = *(u16 *)(e + 0x1A);
                        if (v >= 2)
                        {
                            u32 *h = &D_80073740[i];

                            m |= D_80073740[i];
                            do
                            {
                                func_8005E97C(0, h[0]);
                                k = func_8005EB78(h[0]);
                            } while (k != 2 && k != 0);
                            {
                                u8 *e2 = D_80085458 + i * 0x78;

                                *(u16 *)(e2 + 0x1A) = 0;
                            }
                        }
                        else
                        {
                            *(u16 *)(e + 0x1A) = v + 1;
                        }
                    }
                }
                i++;
            } while (i < (s32)D_80073734[0]);
        }
    }
    if (m != 0)
    {
        func_8005E97C(0, m);
    }
}

/* The three magics are 'pQSE', 'MTdh' and 'KDT ' little-endian: the sequence
   header check, then the first free slot in D_80086C00 takes the pointer. */
s16 func_8005AE08(u32 *p, s16 n) {
    s32 i;

    if (*p != 0x53455170 && *p != 0x6468544D && *p != 0x2054444B) {
        return -1;
    }
    for (i = 0; i < 8; i++) {
        if (D_80086C00[i].unk_00 == -1) {
            D_80086C00[i].unk_04 = (u32) p;
            D_80086C00[i].unk_00 = n;
            D_80086C00[i].unk_02 = 2;
            return i;
        }
    }
    return -1;
}

extern s16 D_80086D48[];

void func_80056A08(void);
s32 func_800599B0(void);

void func_8005AE90(s16 k, u8 b, s16 c)
{
    Unk80086C00 *e;
    Unk80086C00 *t;

    if (k == -1)
    {
        return;
    }
    func_80056A08();
    t = D_80086C00;
    e = (Unk80086C00 *)(k * 12 + (u32)t);
    if (e->unk_00 == -1)
    {
        return;
    }
    D_80086D50a[0] = *(u16 *)e;
    D_80086D48[0] = c;
    D_800869B0[0] = 0x10000;
    D_80085FA4[0] = e->unk_04;
    func_800599B0();
    if (b == 0)
    {
        D_800737A4[0] = 0;
        D_80085F98[0] = 1;
        e->unk_02 = 4;
    }
    else
    {
        D_800737A4[0] = 1;
        D_80085F98[0] = 0;
        e->unk_02 = 1;
    }
}

void func_80059D1C(void);
void func_80056A08(void);

void func_8005AF74(s16 i) {
    Unk80086C00 *b;
    Unk80086C00 *e;

    D_80085F98[0] = 1;
    D_800737A4[0] = 0;
    func_80059D1C();
    func_80056A08();
    b = D_80086C00;
    e = &b[i];
    e->unk_0A = 0x7F;
    e->unk_08 = 0x7F;
    e->unk_02 = 2;
    D_80085F98[0] = 0;
}

void func_80059D1C(void);
void func_80056A08(void);

/* The entry base read into a local after the calls, so its lui/addiu come out
   ahead of the 0xC-stride index chain (13 DIFF the other way round). */
void func_8005AFF4(s16 i) {
    Unk80086C00 *b;
    Unk80086C00 *e;

    D_80085F98[0] = 1;
    func_80059D1C();
    func_80056A08();
    b = D_80086C00;
    e = &b[i];
    e->unk_00 = -1;
    e->unk_02 = 0;
    e->unk_0A = 0x7F;
    e->unk_08 = 0x7F;
    D_80085F98[0] = 0;
}

void func_8005F134(void *);

/* The D_80085458 sweep. Four facts, each worth two words because a hoisted
   invariant drags a callee-saved save and restore with it:

   * the entry pointer is computed INSIDE the body. Walked from a pointer
     declared outside, loop.c biases the giv to +0x1A -- the first field used
     -- and the two loads come out with negative displacements. Same recipe as
     func_80056558 above.
   * the bound is read in the loop CONDITION, not into a local.
   * the bound takes the INCOMPLETE ARRAY spelling. That is the OPPOSITE of
     func_80056558, which needs the scalar alias for the same symbol: here the
     listing has `lui $v0` / `lw $v1, %lo(...)($v0)`, the compiler-split form
     with a separate address temp, where the scalar gives the assembler
     macro's one-register form. Read the `lui`'s destination per site.
   * the stack primitive is an `Unk5F134SP`, not `u8 buf[0x40]` with casts.
     Struct accesses carry MEM_IN_STRUCT_P, which makes every varying-address
     load depend on them, and the body then comes out in strict source order.
     Through a byte array sched1 interleaves the loads with the stores.

   The store order matters with it: +0x04, then +0x00 from the table, then the
   two halfwords. With +0x00 written last the table load moves two slots
   earlier and takes a second register (4 DIFF). */
void func_8005B070(s16 n)
{
    Unk5F134SP buf;
    s32 i = 0;

    D_80085F98[0] = 1;
    while (i < (s32)D_80073734[0])
    {
        u8 *e = D_80085458 + i * 0x78;

        if (*(u16 *)(e + 6) < 0x10 && *(u16 *)(e + 0x1A) != 0)
        {
            buf.unk_04 = 3;
            buf.unk_00 = D_80073740[i];
            buf.unk_08 = 0;
            buf.unk_0A = 0;
            func_8005F134(&buf);
        }
        i++;
    }
    D_80086C00[n].unk_02 = 4;
    D_80085F98[0] = 0;
    D_800737A4[0] = 0;
}

/* func_8005B070's sweep with the two halfwords taken from the entry at +0x10
   and +0x12 instead of zeroed, and a different tail: +0x02 gets 1 and
   D_800737A4 gets 1, in the other order. The same four facts carry it; see
   above. */
void func_8005B16C(s16 n)
{
    Unk5F134SP buf;
    s32 i = 0;

    D_80085F98[0] = 1;
    while (i < (s32)D_80073734[0])
    {
        u8 *e = D_80085458 + i * 0x78;

        if (*(u16 *)(e + 6) < 0x10 && *(u16 *)(e + 0x1A) != 0)
        {
            buf.unk_04 = 3;
            buf.unk_00 = D_80073740[i];
            buf.unk_08 = *(u16 *)(e + 0x10);
            buf.unk_0A = *(u16 *)(e + 0x12);
            func_8005F134(&buf);
        }
        i++;
    }
    D_80086C00[n].unk_02 = 1;
    D_800737A4[0] = 1;
    D_80085F98[0] = 0;
}

void func_800564A8(s32);

void func_8005B27C(s16 i, u8 a, u8 b) {
    D_80086C00[i].unk_08 = a & 0x7F;
    D_80086C00[i].unk_0A = b & 0x7F;
    func_800564A8(i);
}

void func_8005B2C8(s16 i, s32 b, u16 *x, u16 *y) {
    *x = D_80086C00[i].unk_08;
    *y = D_80086C00[i].unk_0A;
}

void func_8005ACDC(void);

void func_8005B300(void) {
    func_8005ACDC();
}

void func_8005E450(s32);
void func_8005D88C(s32);

void func_8005B320(void) {
    func_8005E450(1);
    func_8005D88C(1);
}

void func_8005D88C(s32);
void func_8005E450(s32);

void func_8005B348(void) {
    D_8007382B[0] = 0;
    func_8005D88C(0);
    func_8005E450(0);
}

s32 func_8005DA88(void *);

s16 func_8005B378(s16 a) {
    s32 t[6];

    t[0] = 1;
    t[1] = a;
    if (func_8005DA88(t) != 0) {
        return -1;
    }
    D_8007382B[0] = a;
    return a;
}

s32 func_8005DA88(void *);

void func_8005B3D8(s32 x, s32 y) {
    s16 t[12];

    *(s32 *) &t[0] = 6;
    /* `(x << 16) >> 8`, not `(s16) x << 8`: the two shifts cannot be combined
       (the arithmetic right shift fills from bit 15, so the result is not
       x << 8), which is why the target spends both. Any spelling that means
       "sign-extend then shift" collapses to one `sll` because the value is
       stored as a halfword. */
    t[4] = (x << 16) >> 8;
    t[5] = (y << 16) >> 8;
    func_8005DA88(t);
}

void func_8005E97C(s32, s32);

void func_8005B418(s32 a) {
    s32 i = 0;
    s32 r = 0;

    /* The bound is read from the global IN the loop condition, not hoisted
       into a local: loop-invariant motion then lifts that read into the
       preheader as a COPY of the guard's read, which is the target's
       `addu $a2, $v0, $zero`. With a local it is one read and one register,
       a word short. The `(s32)` is what makes the tests `blez`/`slt`. */
    for (; i < (s32) D_80073734[0]; i++) {
        r |= D_80073740[i];
    }
    func_8005E97C(0, r);
}

s32 func_8005B470(s16 i, u8 *dst) {
    u8 *src;
    u32 j;

    if (D_80086A40[i].unk_00 == -1) {
        return -1;
    }
    src = D_80086A40[i].unk_04;
    for (j = 0; j < 0x20; j++) {
        *dst = *src;
        src++;
        dst++;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_2CDC0", func_8005B4D0);

void func_80056D44(s32, u8 *);
void func_8005E97C(s32, s32);

/* func_80057948's sibling: stops every D_80085458 entry whose +0x06 is at
   least 0x10 and whose +0x0A, +0x5C and +0x04 match the two arguments' high
   byte, high byte and low seven bits, OR-ing their D_80073740 handles
   together and stopping the accumulation at the end. D_80085F98 is held at 1
   across it.

   Same two idioms as func_80057948: the entry pointer is INDEXED inside the
   body (`D_80085458 + i * 0x78`) so loop.c keeps one giv with the field
   offsets as displacements, and `D_80073740[i]` is indexed rather than
   walked. The five tests are one `&&` chain, which is what gives the five
   branches to the shared skip label; the fields differ so nothing combines
   into a range check.

   The two identical func_80056D44/func_8005E97C pairs and the two
   `m |= D_80073740[i]` are in the listing exactly as written here. */
void func_8005BAB0(s32 a, s32 b)
{
    s32 i = 0;
    s32 m = 0;

    D_80085F98[0] = 1;
    if ((s32) D_80073734[0] > 0)
    {
        do
        {
            u8 *e = D_80085458 + i * 0x78;

            if (*(u16 *) (e + 0x1A) != 0 && *(u16 *) (e + 6) >= 0x10 &&
                *(u16 *) (e + 0xA) == (b >> 8) && *(u16 *) (e + 0x5C) == (a >> 8) &&
                *(u16 *) (e + 4) == (a & 0x7F))
            {
                m |= D_80073740[i];
                *(u16 *) (e + 0x1A) = 0;
                *(u16 *) (e + 0xA) = 0;
                func_80056D44(i, e);
                func_8005E97C(0, D_80073740[i]);
                func_80056D44(i, e);
                func_8005E97C(0, D_80073740[i]);
                m |= D_80073740[i];
            }
            i++;
        } while (i < (s32) D_80073734[0]);
    }
    func_8005E97C(0, m);
    D_80085F98[0] = 0;
}

void func_8005E4A0(s32, u32);
/* The unit declares func_8005EC0C with ONE argument (its own landing, below)
   and calls it so. This function passes three: one argument measured 102 DIFF
   against the matched body. The alias keeps both views and emits the same
   `jal func_8005EC0C`. */
void func_8005EC0C3(SpuVoiceAttr *, s32, s32) __asm__("func_8005EC0C");

/* Starts a note on channel `ch0`: looks the program and key up in bank
   `bank`'s data, waits for the channel's voice to go idle, sums the sample
   table to find the wave address, fills the D_80085458 entry and an
   SpuVoiceAttr, pans the volume, and hands the attr to func_8005EC0C.
   Returns the channel, or -1 if the channel or velocity range is out.

   The load-bearing spellings, each measured on docs/near-misses before it
   was folded in (the park is gone; the numbers are what reverting cost):
   - D_80085458 + ch * 0x78 is recomputed three times, each with its OWN base
     local (s3 / s / s2) and its OWN result local (v0 / v / v2): 57 words.
   - `l = sum * 8;` as its own statement into `l`, which already has a range:
     puts the shift in the lw delay slot (7 words; `sum *= 8` or inlining it
     into attr.addr emits it in the wrong slot).
   - `d = e[0xD];` hoisted above the halfword stores: 15 words.
   - halfword store order 0x00, 0x04, 0x08, 0x0A, 0x06, 0x1A (unique minimum
     of all 720 orders); loop setup `i = 0; sum = 0; n = ...; w = ...;`.
   - `sum += *w; w++; i++;` -- the bump before the counter, or the loop needs
     a nop in the lhu delay slot (334 words).
   - `b + (((s16) prog * 0x10 + key) * 0x20 + 0x820)`: 0x820 joins the index
     before `b` is added.
   - `u` and `t` are two locals, one per ADSR halfword.
   - `attr.note = pit;` sits after attr.volume.right so its `sh $s7` is what
     sched2 drops into the `lhu 0x12` delay slot; 42 other positions measured
     2 to 271.
   The two `__asm__` statements are devices; see docs/matching-levers.md,
   "A DEAD SET IN A TWO-OUTPUT ASM: a conflict for global-alloc and a second
   death for local-alloc, with no insn and no fence". */
s16 func_8005BBFC(s16 ch0, s16 bank, s16 prog, s16 key, s16 vel, s16 fine, s16 volx, s16 panx)
{
    SpuVoiceAttr attr;
    s32 ch;
    u8 *b;
    u8 *q;
    u8 *e;
    u8 *v;
    u8 *v0;
    u8 *v2;
    u16 *w;
    s32 pit;
    s32 i;
    s32 n;
    s32 sum;
    s32 k;
    s32 x;
    s32 a;
    s32 l;
    s32 r;
    u16 t;
    u16 u;
    u8 d;
    u8 *s;
    u8 *s2;
    u8 *s3;
    s32 g;

    ch = ch0;
    D_80085F98[0] = 1;
    if ((s32) D_80073734[0] - 1 < ch)
    {
        ch = -1;
    }
    if (ch != -1)
    {
        b = D_80086A40[bank].unk_04;
        q = b + ((s16) prog * 0x10 + 0x20);
        pit = (vel << 8) + fine;
        e = b + (((s16) prog * 0x10 + key) * 0x20 + 0x820);
        if (vel >= e[6] && e[7] >= vel)
        {
            func_80056DB4(ch);
            do
            {
                func_8005E97C(0, D_80073740[ch]);
                k = func_8005EB78(D_80073740[ch]);
            } while (k != 2 && k != 0);
            i = 0;
            sum = 0;
            n = *(s16 *) (e + 0x16);
            w = (u16 *) (D_80086A40[bank].unk_04 + (*(u16 *) (b + 0x12) << 9) + 0x820);
            if (n > 0)
            {
                do
                {
                    sum += *w;
                    w++;
                    i++;
                } while (i < n);
            }

            l = sum * 8;
            attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_VOLMODEL | SPU_VOICE_VOLMODER |
                        SPU_VOICE_NOTE | SPU_VOICE_SAMPLE_NOTE | SPU_VOICE_WDSA |
                        SPU_VOICE_ADSR_AMODE | SPU_VOICE_LSAX | SPU_VOICE_ADSR_ADSR1 |
                        SPU_VOICE_ADSR_ADSR2;
            attr.volmode.left = 0;
            attr.volmode.right = 0;
            attr.voice = D_80073740[ch];
            attr.addr = D_80086A40[bank].unk_10 + l;

            s3 = D_80085458;
            v0 = s3 + ch * 0x78;
            u = *(u16 *) (e + 0x10);
            attr.adsr1 = u;
            *(u32 *) (v0 + 0x60) = u;
            t = *(u16 *) (e + 0x12);
            attr.adsr2 = t;
            *(u32 *) (v0 + 0x64) = t;
            if ((*(u16 *) (e + 0x10) & 0x80) != 0)
            {
                attr.a_mode = SPU_VOICE_EXPIncN;
            }
            else
            {
                attr.a_mode = SPU_VOICE_LINEARIncN;
            }
            attr.loop_addr = attr.addr;
            s = D_80085458;
            v = s + ch * 0x78;
            v[0x22] = e[4];
            v[0x23] = e[5];
            v[0x21] = e[0xC];
            d = e[0xD];
            *(s16 *) (v + 0x00) = ch;
            *(u16 *) (v + 0x04) = prog;
            *(s16 *) (v + 0x08) = key;
            *(s16 *) (v + 0x0A) = pit >> 8;
            *(s16 *) (v + 0x06) = 0x11;
            *(s16 *) (v + 0x1A) = 1;
            v[0x20] = d;
            v[0x14] = q[1];
            v[0x16] = q[4];
            v[0x15] = e[2];
            v[0x17] = e[3];
            *(s16 *) (v + 0x5C) = bank;
            x = D_80086A40[bank].unk_1B + v[0x16] + v[0x17] - 0x80;
            if (x < 0)
            {
                x = 0;
            }
            if (x >= 0x80)
            {
                x = 0x7F;
            }
            *(s16 *) (v + 0x18) = x;
            g = D_80086A40[bank].unk_18;
            a = g * v[0x14] * v[0x15] >> 7;
            /* DEVICE: a second, dead set of `g` in its own block, in an asm
             * that emits nothing and is kept because its other output (`x`,
             * tied to itself) is used. The dead set gets a REG_UNUSED death
             * (flow.c:2094-2101), so `g` "dies in 2 places" and local-alloc
             * leaves it to global-alloc (local-alloc.c:472), which allocates
             * it AFTER `x` (priority 1.0 against 1.565) into $a0. Left to
             * local-alloc it takes $v1 ahead of `x`, `x` falls to $a0, and
             * the arms below cannot come out right: 15 DIFF without this.
             * The tie target is free (`l` matches, `x` matches; `r` is
             * uninitialised here and gives 15) and so is the block (either
             * arm matches too). Not volatile, so sched does not fence on it
             * (sched.c:1957). No -Wall warning. */
            __asm__("" : "=r"(g), "=r"(x) : "1"(x));
            if (x >= 0x40)
            {
                r = a;
                /* DEVICE: a dead set of `a` after the copy, in an asm that
                 * emits nothing; its other output ties `r` to itself so flow
                 * keeps the insn (flow.c:2094-2101). global_conflicts records
                 * the set against everything live -- `r` here, `l` in the
                 * else arm (global.c:729, then the REG_UNUSED death at :765-767)
                 * -- so `a` CONFLICTS with both
                 * and expand_preferences (global.c:781-828) does not pass it
                 * the $a1/$a2 the two copies would otherwise hand it; with no
                 * usable preference find_reg takes the lowest free register
                 * (global.c:951-980; mips.h has no REG_ALLOC_ORDER), $a0.
                 * Without the pair `a` shares $a1 with `l`, the else-arm copy
                 * is free, and the body is one word short: 108. One arm alone
                 * is 117 / 108. `__asm__ volatile("" : "=r"(a));` (a fence)
                 * also matches; a single-output non-volatile asm is deleted
                 * as dead (no change). Legal spellings measured: a dead
                 * `a = 0;` after the call 21 (cse makes `a` canonical and the
                 * shifts read it, cse.c:853-857); `r = x * (a << 1) >> 7`
                 * no change (cse rewrites `a` to `l`); an identical-arm
                 * `if (a)` at the join 104-122; `a = unk_18; a = a * ...`
                 * 39/41; one-argument func_8005EC0C 102. No -Wall warning. */
                __asm__("" : "=r"(a), "=r"(r) : "1"(r));
                l = (0x40 - (x & 0x3F)) * (r << 1) >> 7;
            }
            else
            {
                l = a;
                /* DEVICE: the else arm's half of the pair above. */
                __asm__("" : "=r"(a), "=r"(l) : "1"(l));
                r = x * (l << 1) >> 7;
            }

            l = l * D_80086A40[bank].unk_18 >> 7;
            r = r * D_80086A40[bank].unk_18 >> 7;
            s2 = D_80085458;
            v2 = s2 + ch * 0x78;
            *(s16 *) (v2 + 0x10) = l * (s16) volx >> 7;
            *(s16 *) (v2 + 0x12) = r * (s16) panx >> 7;
            attr.volume.left = *(s16 *) (v2 + 0x10);
            attr.volume.right = *(u16 *) (v2 + 0x12);
            attr.note = pit;
            if (e[5] != 0)
            {
                attr.sample_note = (*(e - 0x1C) << 8) | (0x7F - e[5]);
            }
            else
            {
                attr.sample_note = e[5] | (e[4] << 8);
            }
            func_8005EC0C3(&attr, l, r);
            if ((e[1] & 4) != 0)
            {
                func_8005E4A0(1, D_80073740[ch]);
            }
            else
            {
                func_8005E4A0(0, D_80073740[ch]);
            }
        }
        else
        {
            ch = -1;
        }
    }
    D_80085F98[0] = 0;
    return ch;
}

s32 func_8005EB78(u32);

/* Finds a free slot -- first a D_80073740 handle func_8005EB78 reports idle,
   then a D_80085458 entry with a zero +0x1A -- and hands it plus all seven
   arguments to func_8005BBFC, returning -1 if neither sweep found one.
   D_80085F98 is held at 1 across the whole thing.

   The first sweep is a `goto` loop inside a `do { } while (0)`, and that pair
   is the lever. The listing wants BOTH halves of loop.c's behaviour split
   apart: `lui $s1, %hi(D_80073734)` hoisted out of the sweep (so only the
   `lw %lo(...)($s1)` is inside it), but the element address NOT
   strength-reduced -- `sll $v0, $s0, 2` / `addu $v0, $v0, $s2` / `lw` every
   iteration. A real `do`/`while` gives both: the giv's benefit is 4, one
   `add_cost` over the cut, so loop.c reduces it to a walking pointer and the
   function is two words short. A bare `goto` loop gives neither: with no
   NOTE_INSN_LOOP_BEG there is nothing to hoist from, the `lui` stays inside
   the sweep, the register pressure drops and the `c` parameter no longer
   spills to 0x20($sp) -- two words short again, differently.

   `do { } while (0)` round the goto loop gives exactly one of them. The
   wrapper is a loop STATEMENT, so loop.c scans the region and hoists the
   `(high D_80073734)` into the entry block; the back edge inside it is a
   `goto`, so there is no biv and nothing to reduce. The extra callee-saved
   register that the hoist occupies is what pushes `c` to its stack home.

   The SECOND sweep is a plain `do`/`while`, because there the listing does
   want the giv: `addiu $a0, $a0, 0x78` with +0x1A as the displacement, which
   needs the 0x78-stride base in a register (matching-levers section 9).

   `i = 0` is the function's first statement so that its `addu $s0, $zero,
   $zero` pairs with `sw $s0` ahead of the stack parameters' loads. */
s16 func_8005C130(s16 a, s16 b, s16 c, s16 d, s16 e, s16 f, s16 g)
{
    u32 *t;
    u8 *q;
    s32 i;

    i = 0;
    D_80085F98[0] = 1;
    t = D_80073740;
    do
    {
    scan:
        if (func_8005EB78(*(u32 *) (i * 4 + (u32) t)) == 0)
        {
            goto call;
        }
        i++;
        if (i <= (s32) D_80073734[0] - 1)
        {
            goto scan;
        }
        i = -1;
    } while (0);
call:
    if (i == -1)
    {
        q = D_80085458;
        i = 0;
        do
        {
            if (*(u16 *) (q + i * 0x78 + 0x1A) == 0)
            {
                goto call2;
            }
            i++;
        } while (i <= (s32) D_80073734[0] - 1);
        i = -1;
    call2:
        if (i == -1)
        {
            goto out;
        }
    }
    i = func_8005BBFC(i, a, b, c, d, e, f, g);
out:
    D_80085F98[0] = 0;
    return i;
}

void func_8005EC0C(SpuVoiceAttr *);

/* Starts one voice: picks the D_80085458 entry for channel `b`, fills its
   twenty-odd fields, copies the volumes and the note into an SpuVoiceAttr and
   hands that to func_8005EC0C.

   THREE PARAMETERS ARE NARROWED TO SUB-WORD TYPES, and that is the whole
   reason this matches. With all four of a, b, c, d declared s32 they are
   copied out of $a0..$a3 by a CONTIGUOUS RUN of hard-register moves at the top
   of the function, and schedule_block pins that whole run out of sched1
   (sched.c:3186, "At the start of a function, before reload has run, don't
   delay getting parameters from hard registers into pseudo registers" -- it
   walks the run setting INSN_REF_COUNT to 1 and advancing HEAD past it).
   Pinned, they stay first, and the lui/lhu/li group that belongs ahead of them
   in the listing can never get there: 10 words, every instruction already
   right.

   A sub-word parameter still ARRIVES promoted in SImode, so gcc keeps a raw
   SImode pseudo for it and emits the narrowing separately, after every
   parameter has left its hard register. For the first one the narrowing is a
   TRUNCATION whose source is a SUBREG OF A PSEUDO -- which is exactly what the
   prefix loop's source test rejects, so the run ends there. In the sched1 dump
   the pinned insns are 4 (a's raw copy, later deleted: it dies in the entry
   block and takes $a0) and 8 (b), while insn 6, the truncation
   `(set (reg/v:HI 80) (subreg:HI (reg:SI 81) 0))`, has a priority like any
   schedulable insn. The three narrowings then land after the lui/lhu/li group
   in parameter order -- `move $s5,$a0`, `move $s4,$a2`, `move $s3,$a3` -- which
   is the listing.

   Two more things fall out of the same shape: cse rewrites the sign-extension
   of `a` onto the RAW pseudo, which dies in the entry block and so extends in
   place in $a0 (the listing's `sll $a0`/`sra $a0`), and `d != 0` keeps its
   `andi $s3,0xFFFF` because a narrowed parameter is not assumed already
   zero-extended the way one arriving through a prototype is.

   The DECLARATION STYLE is not load-bearing -- measured: prototyped with these
   same types also matches, and converting a definition to old-style while
   keeping its types changes nothing in any of the 38 multi-parameter park
   files. It is written old-style for a different reason: a narrow prototype in
   scope rewrites a later caller's sll/sra into a truncation (the hazard
   func_80033AE8's carved-sweep row hit, solved there with an __asm__ alias),
   and an old-style definition declares no prototype.

   Three field shapes also paid for, each measured:
     * `q = D_80085458; p = q + b * 0x78;` -- a pointer VARIABLE for the base.
       `D_80085458 + b * 0x78` has an ADDR_EXPR operand, which is TREE_CONSTANT,
       so fold swaps it right and the index chain expands first; sched1 then
       lifts the index shifts into the shadow of the `lw 0x10` that loads
       st.addr and local-alloc gives p $v1 instead of $v0 -- 26 words.
     * `st.volume.left = *(u16 *) (p + 0x10);` -- read BACK from the entry, not
       `f << 7` spelled again, so cse turns the load into the move the listing
       has. st.note is `e` because +0x12 is NOT forwarded and keeps its `lhu`.
     * the four constant bytes in CHANNEL-INTERLEAVED order 0x14, 0x16, 0x15,
       0x17 -- 16 words. It decides which constant sched1 hoists: with 0x7F's
       pseudo born first it reaches the st.addr load's shadow, crosses the 0x11
       and 0x40 pseudos in $v1 and takes $a0, which frees $v1 for `f << 7`. */

s16 func_8005C2C0(a, b, c, d, e, f, g)
s16 a;
s32 b;
u16 c;
u16 d;
u16 e;
u16 f;
u16 g;
{
    SpuVoiceAttr st;
    u8 *q;
    u8 *p;
    s32 t;

    D_80085F98[0] = 1;
    if (a != -1 && b < 0x18)
    {
        func_80056DB4(b);
        do
        {
            func_8005E97C(0, D_80073740[b]);
            t = func_8005EB78(D_80073740[b]);
        } while (t != 2 && t != 0);

        st.mask = 0x1FFEF;
        st.a_mode = 1;
        st.s_mode = 1;
        st.r_mode = 3;
        st.ar = 0;
        st.dr = 0;
        st.sr = 0;
        st.rr = 0;
        st.sl = 0;
        st.volmode.left = 0;
        st.volmode.right = 0;
        st.voice = D_80073740[b];
        st.addr = D_80086A40[a].unk_10;

        q = D_80085458;
        p = q + b * 0x78;
        *(u16 *)(p + 0x06) = 0x11;
        *(u8 *)(p + 0x14) = 0x7F;
        *(u8 *)(p + 0x16) = 0x40;
        *(u8 *)(p + 0x15) = 0x7F;
        *(u8 *)(p + 0x17) = 0x40;
        *(u16 *)(p + 0x10) = f << 7;
        *(u16 *)(p + 0x00) = b;
        *(u16 *)(p + 0x1A) = 1;
        *(u8 *)(p + 0x22) = c;
        *(u8 *)(p + 0x23) = d;
        *(u16 *)(p + 0x0A) = e;
        *(u16 *)(p + 0x5C) = a;
        *(u16 *)(p + 0x12) = g << 7;

        st.volume.left = *(u16 *)(p + 0x10);
        st.volume.right = *(u16 *)(p + 0x12);
        st.note = e;
        if (d != 0)
        {
            st.sample_note = ((c - 1) << 8) | (0x7F - d);
        }
        else
        {
            st.sample_note = d | (c << 8);
        }
        func_8005EC0C(&st);
    }
    D_80085F98[0] = 0;
    return b;
}

/* Two spellings are load-bearing here, both measured against the 15 DIFF the
   plain form sits at:

     * `u32 *f = D_80085F98;` used for the `= 1` store ONLY, with the two exit
       stores spelled through the symbol. The pointer local puts the symbol's
       `(high)` at the TOP of the entry block instead of beside its store, so
       it conflicts with $a0's incoming `n` and with $v1's D_80085458 address
       and takes $a1 -- the listing's hoisted `lui $a1` at 8005C4D4. combine
       folds the `%lo` back into the store, so the local costs no instruction
       and the `addiu` it would otherwise need never appears. Writing the exit
       stores through `f` as well is 47 DIFF; dropping the local is 55 DIFF at
       63 words.
     * `func_80056D44(n, e)` -- TWO arguments, which is the signature this unit
       already declares at the top. The third `e` that m2c infers from $a2
       being live is a phantom `$a2 = $a2`: reload deletes it, but sched2 still
       schedules it, it takes cycle T-10, and `lw $s1` then loses T-12 to
       `addu $a1,$a2,$zero` and comes out one insn late. That alone is the
       remaining 2 DIFF. */

s32 func_8005C4D0(s16 n)
{
    u32 *f = D_80085F98;
    u8 *b = D_80085458;
    u8 *e;
    u32 v;
    s32 r;
    u16 h;

    e = b + n * 0x78;
    h = *(u16 *)(e + 0x1A);
    f[0] = 1;
    if (h == 0)
    {
        goto fail;
    }
    if (*(u16 *)(e + 6) < 0x10)
    {
        goto fail;
    }
    v = D_80073740[n];
    *(u16 *)(e + 0x1A) = 0;
    *(u16 *)(e + 0xA) = 0;
    func_80056D44(n, e);
    func_80056DB4(n);
    func_8005E97C(0, D_80073740[n]);
    do
    {
        func_8005E97C(0, v);
        r = func_8005EB78(v);
        if (r == 2)
        {
            goto ok;
        }
    } while (r != 0);
    goto ok;
fail:
    D_80085F98[0] = 0;
    return -1;
ok:
    D_80085F98[0] = 0;
    return 0;
}

s32 func_80059D60(void);

/* The base into a local first so its lui/addiu precede the 0xC-stride index
   chain, and the return RE-INDEXES rather than reusing `e`. Unk80086C00.unk_02
   is signed: the target reads it with `lh`. */
s16 func_8005C5D0(s16 i) {
    Unk80086C00 *b = D_80086C00;
    Unk80086C00 *e = &b[i];
    s32 r;

    if (e->unk_02 == 1) {
        r = func_80059D60();
        if (r == 3) {
            e->unk_02 = r;
        }
    }
    return D_80086C00[i].unk_02;
}

void func_8005F134(void *);

s32 func_8005C668(s16 i, s16 a, s16 b) {
    u32 t[0x10];

    t[1] = 3;
    t[0] = D_80073740[i];
    *(u16 *) ((u8 *) t + 8) = a;
    *(u16 *) ((u8 *) t + 0xA) = b;
    func_8005F134(t);
    return 0;
}

void func_8005F134(void *);

/* An `s16` block here, where func_8005C668's is `u32` with casts for the two
   halfwords: the masked stores have to be ARRAY_REFs on the DECL so that
   MEM_IN_STRUCT_P is set on them. Through a cast they are plain INDIRECT_REFs,
   sched1's alias calls answer differently and the `andi`/`sll` pair for the
   first halfword floats to the top of the function (11 DIFF, same words). */
s32 func_8005C6B4(s16 i, s16 a, s16 b) {
    s16 t[0x20];

    *(u32 *) &t[2] = 3;
    *(u32 *) &t[0] = D_80073740[i];
    t[4] = (a & 0x7F) << 7;
    t[5] = (b & 0x7F) << 7;
    func_8005F134(t);
    return 0;
}

void func_8005FA34(u32 *);

/* A u32 block, not a u8 one: as `u8 t[0x40]` with `*(u32 *)` casts the two
   stores and the table load come out in the other order (3 DIFF). */
s32 func_8005C710(s16 i, s16 *a, s16 *b) {
    u32 t[0x10];

    t[1] = 0;
    t[0] = D_80073740[i];
    func_8005FA34(t);
    *a = *(u16 *) ((u8 *) t + 8);
    *b = *(u16 *) ((u8 *) t + 0xA);
    return 0;
}

void func_8005FA34(u32 *);

s32 func_8005C784(s16 i, s16 *a, s16 *b) {
    u32 t[0x10];

    t[1] = 0;
    t[0] = D_80073740[i];
    func_8005FA34(t);
    *a = (s16) *(u16 *) ((u8 *) t + 8) >> 7;
    *b = (s16) *(u16 *) ((u8 *) t + 0xA) >> 7;
    return 0;
}

u32 func_8005C808(void) {
    return D_80085FCC[0];
}

void func_8005C818(s32 a, u32 b) {
    u32 i = 0;

    if (D_800869B4 != 0) {
        do {
            D_80085FA8s[i].unk_24 = b;
            D_80085FA8s[i].unk_20 = b;
            i++;
        } while (i < D_800869B4);
    }
}

s32 func_8005C858(s32 a, s16 i) {
    return D_80084960[i].unk_0C;
}

void func_80057D20(s16, s32, u32);

void func_8005C88C(s32 arg0, s16 b, u32 c) {
    func_80057D20(b, 7, c & 0x7F);
}

s32 func_8005C8B8(s32 a, s16 i) {
    return D_80084960[i].unk_04;
}

void func_80057D20(s16, s32, u32);

void func_8005C8EC(s32 arg0, s16 b, u32 c) {
    func_80057D20(b, 0xA, c & 0x7F);
}

s32 func_8005C918(s32 a, s16 i) {
    return D_80084960[i].unk_1C;
}

void func_8005C94C(s32 a, s16 i, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
}

void func_8005C980(void) {
}

void func_8005C988(void) {
}

void func_8005C990(void) {
}

void func_8005C998(void) {
}

void func_8005C9A0(void) {
}
