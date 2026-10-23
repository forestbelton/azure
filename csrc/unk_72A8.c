#include "common.h"

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033AA8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033AE8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033B2C);

void func_80033AE8(s32);

void func_80033B78(s16 a) {
    func_80033AE8(a);
}

void func_80033AA8(s32);

void func_80033B9C(s16 a) {
    func_80033AA8(a);
}

void func_80033B2C(s32);

void func_80033BC0(s16 a) {
    func_80033B2C(a);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033BE4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033C1C);

void func_80033C84(u32 *p) {
    *p = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033C8C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033CD8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033D08);

s32 func_80033D38(u32 *p) {
    return p[-3] != 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033D44);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033D54);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800345B8);

void func_80034E40(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80034E48);

void func_80034EB4(u8 *p) {
    p[0] = 0;
    p[1] = 0;
    *(u32 *) (p + 0x34) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80034EC4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80034EF8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80034F58);

void func_80034F88(u32 *p, u32 v) {
    p[13] = v;
    p[14] = v;
    p[66] = (u32) &p[50];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80034F9C);

s32 func_80034FD0(u32 *p) {
    return p[4] != (u32) &p[2];
}

s32 func_80034FE4(u32 *p) {
    return p[39] != (u32) func_80038A00;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80034FFC);

void func_80035060(u8 *p, u32 v) {
    p[3] = v;
}

void func_80035068(u8 *p, u8 *n) {
    *(u8 **) (n + 0x4) = p + 0x8;
    *(u8 **) (n + 0x8) = *(u8 **) (p + 0x10);
    *(u8 **) (*(u8 **) (p + 0x10) + 0x4) = n;
    *(u8 **) (p + 0x10) = n;
}

void func_80035090(u32 *p) {
    ((u32 *) p[1])[2] = p[2];
    ((u32 *) p[2])[1] = p[1];
}

void func_800350B0(u8 *a, u8 *p) {
    *(u8 **) (p + 0x54) = p;
    func_80035068(a, p + 0x54);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800350D4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035164);

void func_80035164(s16 *, s32, s32);

void func_80035208(s32 a) {
    func_80035164(D_80082A38, a, 2);
}

void func_80035164(s16 *, s32, s32);

void func_80035234(s32 a) {
    func_80035164(D_80082A38, a, 1);
}

void func_80035164(s16 *, s32, s32);

void func_80035260(s32 a) {
    func_80035164(D_80082A38, a, 0);
}

void func_8003528C(u32 v) {
    func_80034F88((u32 *) D_80082A38, v);
}

void func_80034F9C(s16 *);

void func_800352B4(void) {
    func_80034F9C(D_80082A38);
}

void func_800352D8(void) {
    func_80034FD0((u32 *) D_80082A38);
}

void func_80034FFC(s16 *);

void func_800352FC(void) {
    func_80034FFC(D_80082A38);
}

void func_80035320(u32 v) {
    func_80035060((u8 *) D_80082A38, v);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035348);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035378);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800353F4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035484);

void func_800355EC(u8 *p) {
    (*(void (**)(u8 *)) (p + 0x68))(p);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035614);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003566C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003571C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035760);

void func_800357D0(void);

void func_800357B0(u8 *p) {
    if (*(u16 *) (p + 0x50) == 0x101) {
        *(u32 *) (p + 0x68) = (u32) func_800357D0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800357D0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035820);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035888);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800359FC);

void func_80035AAC(u32 *p) {
    p[0x1A] = (u32) func_80035AF8;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035ABC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035AF8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035BE0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035C44);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035C80);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035CE4);

void func_80035D38(void) {
}

void func_80035D40(u32 v) {
    D_8006ADB8[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035D4C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035DBC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035E58);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003605C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036110);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036154);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036210);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003626C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800362A0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800362D0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036350);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036434);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036484);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800364BC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800364EC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036574);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003661C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003666C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800366F4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036880);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800368D0);

void func_800368F8(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036900);

void func_80036980(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036988);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800369C8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036A70);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036B10);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036C44);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036C7C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036D4C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036ED4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036F24);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037030);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037090);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003719C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037394);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800374F4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037534);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800375C0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037714);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037C7C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037D50);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037D9C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037E58);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037F70);

void func_80038000(u8 v) {
    D_80082B4B[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003800C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038128);

void func_80038A10(void);

void func_800381A4(u8 *p) {
    u16 *c = (u16 *) (p + 0x18);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800381D0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038240);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038288);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800382F0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003832C);

void func_80038A10(void);

void func_80038368(u8 *p) {
    u16 *c = (u16 *) (p + 0x18);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038394);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800383D4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038408);

void func_80038464(u8 *p) {
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038478;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038478);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800384A8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800384F8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038538);

void func_80038570(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038588;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038588);

void func_800385D0(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_800385E8;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800385E8);

void func_80038630(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038648;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038648);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038690);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038708);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003875C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800387D0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800389B4);

void func_80038A00(void) {
}

void func_80038A08(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038A10);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038AB8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038C40);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038CB8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038F48);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800390D0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039148);

void func_8003919C(void) {
}

void func_800391A4(void) {
}

void func_800391AC(void) {
}

void func_800391B4(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800391BC);

void func_800391EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x28] = *(*q)++;
}

void func_80039208(u8 *p) {
    p[0x2E] = 2;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039214);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039258);

void func_8003929C(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800392A4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003931C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039470);

void func_80039560(void) {
}

void func_80039568(u32 *p) {
    p[4] = (u32) func_80038464;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039578);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800395B0);

void func_800395E8(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800395F0);

void func_80039640(u8 *p) {
    u32 **q = (u32 **) (p + 0x7C);

    *(u32 *) (p + 0x1C) = *--(*q);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003965C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039694);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039730);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039760);

void func_800397A0(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800397A8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039814);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039884);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800398BC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800399EC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039A08);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039A88);

void func_80039ACC(u8 *p) {
    *(u32 *) (p + 0x10) = (u32) func_80039AE8;
    *(s32 *) (p + 0x1C) -= 1;
}

void func_80039AE8(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039AF0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039B28);

void func_80039B64(void) {
}

void func_80039B6C(void) {
}

void func_80039B74(void) {
}

void func_80039B7C(void) {
}

void func_80039B84(u32 *p) {
    p[4] = (u32) func_80038A08;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039B94);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039BBC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039BE4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039C0C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039C44);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039C74);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039D10);

void func_80039D40(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039D48);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039DBC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039E1C);

void func_80039ED0(u32 *p) {
    p[4] = 0;
}

void func_80039ED8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x88] = *(*q)++;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039EF4);

void func_80039F28(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + *(*q)++ * 4 + 0x48) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039F4C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039F80);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039FC0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A01C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A060);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A10C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A1A4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A248);

void func_8003A288(u32 *p) {
    p[7]++;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A29C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A2E0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A374);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A3E8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A468);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A4B4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A500);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A54C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A5B4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A61C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A684);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A6EC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A754);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A7BC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A7FC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A858);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A904);

void func_8003A9B8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;

    *q = *(u8 **) (t + i * 4 + 0x48);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A9E4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AA34);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AA84);

u32 func_8003AAC4(void) {
    return D_80083168[0];
}

u32 func_8003AAD4(void) {
    return D_80083170[0];
}

void func_8003AAE4(u16 a, u16 b) {
    D_80082A38[86] = a;
    D_80082A38[87] = b;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AAF8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AB14);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AB44);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AC1C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003ACC0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AD08);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003ADB4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003ADF4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AE28);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AE60);

void func_8003AEA8(u32 *p, s32 n) {
    for (; n > 0; n--) {
        *p++ = 0;
    }
}

void func_800A4928(void);

void func_8003AEC8(void) {
    func_800A4928();
}

void func_8003AEE8(void) {
}

void func_8003AEF0(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AEF8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AF58);

void func_8003AF8C(u32 *p) {
    p[5] = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AF94);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AFE0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B034);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B06C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B0B4);

void func_8003AF58(u8 *, u8 *);

void func_8003B188(void) {
    func_8003AF58(D_800717D0, D_8006AE1C);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B1B4);

void func_8003AF58(u8 *, u8 *);

void func_8003B1E4(void) {
    func_8003AF58(D_800717D0, D_8006AE34);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B210);

void func_8003AF58(u8 *, u8 *);

void func_8003B238(void) {
    func_8003AF58(D_800717D0, D_8006AE4C);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B264);

void func_8003AF58(u8 *, u8 *);

void func_8003B298(void) {
    func_8003AF58(D_800717D0, D_8006AE64);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B2C4);

void func_8003AF58(u8 *, u8 *);

void func_8003B2F8(void) {
    func_8003AF58(D_800717D0, D_8006AE7C);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B324);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B35C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B400);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B42C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B470);

void func_8003B644(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B64C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B714);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B7C8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B820);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B8DC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B92C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B988);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B9B8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B9E8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BA24);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BA60);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BA9C);

void func_8003B9E8(void);

void func_8003BAD8(void) {
    func_8003B9E8();
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BAF8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BD34);

void func_8003BD7C(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BD84);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BE1C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BFE4);

void func_8003C064(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C06C);

void func_8003C0A4(u8 *p, u32 *q) {
    if (q[2] != 0) {
        p[4] = 1;
    }
}

void func_8003C0C0(u8 *p) {
    p[4] = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C0C8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C128);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C160);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C24C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C2B8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C2F8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C364);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C3B4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C450);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C488);

void func_8003C4C0(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C4C8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C520);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C5A4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C5F0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C634);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C67C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C6F8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C758);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C7D8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C890);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C8E4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C920);
