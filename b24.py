B = "unk_1C540"
CANDS = [
("func_800498A0", B, """typedef struct Unk498A0 {
    u8 unk_00[8];
} Unk498A0;

u8 func_800498A0(u8 *p) {
    u8 t[8];

    /* A byte-aligned 8-byte struct assignment: two lwl/lwr and two swl/swr
       pairs, not four word moves. */
    *(Unk498A0 *) t = *(Unk498A0 *) D_80080B20;
    return t[*(u32 *) (p + 0x14) & 7];
}"""),
("func_8004A0C0", B, """u32 func_8004A0C0(u32 m) {
    u32 r = 0;
    s32 i;
    u8 *p = D_800712FC;

    for (i = 0; i < 0xD; i++) {
        if ((*(u32 *) (p + 4) & m) != 0) {
            r = *(u32 *) p;
            break;
        }
        p += 8;
    }
    return r;
}"""),
("func_8004D75C", B, """typedef struct Unk4D75C {
    u32 unk_00[0x11];
} Unk4D75C;

/* 0x44 bytes is 17 words, so cc1psx's block move emits the odd word first and
   then a four-words-per-iteration loop over the other sixteen. */
void func_8004D75C(u8 *d, u8 *s) {
    *(Unk4D75C *) d = *(Unk4D75C *) s;
    *(u32 *) s = 0;
}"""),
("func_8004E5E8", B, """void func_8004E4C0(s32, u16, s32, s32);
u8 *func_8004E478(u8 *, u16);

u8 *func_8004E5E8(s32 a, u16 n, u8 *dst) {
    func_8004E4C0(a, n, (s32) dst, 0x30);
    func_8004E478(dst, n);
    return dst;
}"""),
("func_8004ED5C", B, """void func_8004EC20(void);
void func_8004ED00(void);

void func_8004ED5C(u8 *p) {
    if (func_80044144(1, 0x1E, (u32) func_8004EC20, (u32) p) != 0) {
        *(u32 *) (p - 0x10) = (u32) func_8004ED00;
    }
}"""),
("func_8004F2BC", B, """void func_8004EF90(u8 *);
void func_8004EFF4(u8 *);
void func_8004F068(u8 *);
void func_8004F1E8(u8 *);
void func_8004F0DC(u8 *);

void func_8004F2BC(u8 *p) {
    func_8004EF90(p + 0x8);
    func_8004EFF4(p + 0x80);
    func_8004F068(p + 0xF8);
    func_8004F1E8(p + 0x170);
    func_8004F0DC(p);
}"""),
("func_8004FFA8", B, """s32 func_8004FF20(u8 *);
void func_80050EA8(u8 *);

s32 func_8004FFA8(u8 *p) {
    s32 r = func_8004FF20(p);

    if (r != 0) {
        *(u32 *) (p - 0x10) = *(u32 *) (p + 4);
        func_80050EA8(p - 0x20);
    }
    return r;
}"""),
("func_8005405C", B, """s32 func_8005405C(s16 i) {
    if ((D_800847D0[1] & (0x1000000 << i)) != 0) {
        return 0;
    }
    if ((D_800847D0[0] & (0x10000 << i)) != 0) {
        return 1;
    }
    return 0;
}"""),
("func_800589B8", B, """u32 func_800589B8(s32 p) {
    u32 off = *(u32 *) p;
    u8 b = *(u8 *) (D_80085FA4[0] + off);
    u32 next = off + 1;

    *(u32 *) p = next;
    if (D_800869B0[0] < next) {
        *(u32 *) (p + 0x2C) = 1;
        return -1;
    }
    return b;
}"""),
("func_80059D60", B, """s32 func_80059D60(void) {
    u32 n = D_800869B4;
    u32 i;

    for (i = 0; i < n; i++) {
        if (D_80085FA8s[i].unk_2C == 0) {
            return 1;
        }
    }
    return 3;
}"""),
]
