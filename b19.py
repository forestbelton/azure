A = "unk_72A8"
PROTO = "void func_80038A10(void);\n\n"
CANDS = [
("func_80038240", A, """s32 func_80038240(s32 a) {
    if (a == 0) {
        return 0;
    }
    if (a == 1) {
        func_80053DA8(0xB4);
        return 0;
    }
    func_80053DA8(0x74);
    return 1;
}"""),
("func_80038588", A, PROTO + """void func_80038588(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_800385E8", A, PROTO + """void func_800385E8(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_80038648", A, PROTO + """void func_80038648(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_8003ACC0", A, """u8 *func_8003ACC0(u8 *p, u16 n) {
    /* The PARAMETER is the moving pointer and the saved copy is what gets
       returned, which is the `addu $a2, $a0, $zero` at the top; the other way
       round swaps those two registers through the whole function. */
    u8 *r = p;
    u8 *b = r + (n - 1);
    u8 x;
    u8 y;

    while (p < b) {
        x = *p;
        y = *b;
        *b-- = x;
        *p++ = y;
    }
    return r;
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
   iteration, which is the loop in the listing -- nothing in the source is
   unrolled. */
void func_8003BD34(void) {
    *(Unk8006ADBC *) D_8006ADBC = *(Unk8006ADBC *) D_8006ADEC;
}"""),
("func_800494B4", "unk_1C540", """s32 func_800B8C90(void);

void func_800494B4(s32 p) {
    u8 *q;

    /* One call after the join, not a call in each arm: in each arm the
       argument setup for p is duplicated and the jal's delay slot goes empty
       (14 DIFF). */
    if (func_800B8C90() != 0) {
        q = D_80080B34;
    } else {
        q = D_80080B40;
    }
    func_80049374(p, q);
}"""),
]
