A = "unk_72A8"
CANDS = [
("func_80038240", A, """s32 func_80038240(s32 a) {
    s32 r = 0;

    if (a != 0) {
        if (a == 1) {
            func_80053DA8(0xB4);
            r = 0;
        } else {
            func_80053DA8(0x74);
            r = 1;
        }
    }
    return r;
}"""),
("func_80038588", A, """void func_80038A10(void);

void func_80038588(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_800385E8", A, """void func_800385E8(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_80038648", A, """void func_80038648(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_8003ACC0", A, """u8 *func_8003ACC0(u8 *p, u16 n) {
    u8 *a = p;
    u8 *b = p + (n - 1);
    u8 x;
    u8 y;

    while (a < b) {
        x = *a;
        y = *b;
        *b-- = x;
        *a++ = y;
    }
    return p;
}"""),
("func_8003AE60", A, """void func_8003AE60(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b) {
    s16 rect[4];

    rect[0] = x;
    rect[1] = y;
    rect[2] = w;
    rect[3] = h;
    ClearImage(rect, r, g, b);
}"""),
("func_8003B06C", A, """void func_8003C758(u8 *);

void func_8003B06C(void) {
    func_8003C758(D_80080F40);
    func_8003C758(D_80081010);
    func_8003C758(D_80080F60);
}"""),
("func_8003BD34", A, """typedef struct Unk8006ADBC {
    u32 unk_00[12];
} Unk8006ADBC;

/* A 0x30-byte struct assignment: cc1psx's block move does four words per
   iteration, which is the loop in the listing rather than anything the source
   unrolled. */
void func_8003BD34(void) {
    *(Unk8006ADBC *) D_8006ADBC = *(Unk8006ADBC *) D_8006ADEC;
}"""),
("func_80048FBC", "unk_1C540", """void func_80048FBC(u32 *p, s32 n) {
    s32 i = func_80048F8C(p, n);

    p[i] = func_8004B404(0x104);
}"""),
("func_800494B4", "unk_1C540", """s32 func_800B8C90(void);

void func_800494B4(s32 p) {
    if (func_800B8C90() != 0) {
        func_80049374(p, D_80080B34);
    } else {
        func_80049374(p, D_80080B40);
    }
}"""),
]
