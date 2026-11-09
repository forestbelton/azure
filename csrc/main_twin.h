#ifndef OVERLAY_MAIN_TWIN_H_
#define OVERLAY_MAIN_TWIN_H_

/* Declarations for main.bin's DEBUG TWIN at 0x80400000 -- the 0x80020000 card
 * overlay's source built at -G8 with its asserts compiled in, linked against a
 * different engine build, dead on retail and in the container regardless.
 *
 * WHY THIS IS A SEPARATE HEADER FROM main.h, AND WHY IT INCLUDES IT. Both
 * blocks sit in ONE container, so a symbol name cannot resolve to two
 * addresses: every object the twin touches is a DIFFERENT symbol from the live
 * overlay's, and there is no constant offset to derive one from the other
 * (card_slot is D_800287CC live and D_804094EC here, a delta of 0x3E0D20,
 * while the two data bases differ by 0x3E1320). The TYPES, though, are the
 * same types -- it is the same source -- so this includes main.h for them
 * rather than duplicating 200 lines of struct definitions. main.h's own
 * declarations come along unused, which is harmless: none of its names collide
 * with the ones here.
 *
 * Every correspondence below is a `tools/decomp/twinalign.py --symmap` entry,
 * not a guess. That map is derived from the aligned pairs: the Nth symbol
 * reference in a live function is the Nth in its twin, so each pair votes.
 * The x-counts in the comments are how many pairs voted.
 *
 * KEEPING THE TWO SOURCES SEPARATE IS DELIBERATE AND TEMPORARY. The twin's
 * bodies are moved over one at a time and verified against this block; merging
 * them back onto one source is a decision to make once the whole block is
 * reproduced and we know whether the 44 functions at <60% similarity are
 * asserts or an EARLIER REVISION of the source.
 */

#include "main.h"

/* --- functions, in address order --- */
s32 func_80047358(s32 a, u8 *fmt, s32 n);            /* func_800491F4 */
void func_8004DCCC(s32 *p);                          /* func_8004CB2C */
void func_800514A8(u8 *p, s32 a);                    /* func_8004E264 */
u8 *func_80051B50(u8 *out, u8 *s, s32 a);            /* func_8004DA74 */
void func_80051DE4(u8 *p, s32 a);                    /* func_8004E884 */
void func_80051FBC(s32 a);                           /* func_8004E994 */
void func_80058368(s32 p, s16 *r);                   /* func_80049D80 */
void func_8007BEF0();                                /* exit, psyq/include/stdlib.h */
int func_8007C040(char *fmt, ...);                   /* printf */
long func_8007C998(unsigned long, long, long, long (*func)()); /* OpenEvent */
long func_8007C9A8(long);                            /* CloseEvent */
long func_8007C9C8(long);                            /* TestEvent */
int func_8007CA38(void);                             /* EnterCriticalSection */
void func_8007CA48(void);                            /* ExitCriticalSection */
void func_80400A10(void);                            /* func_80020A80 */
s32 func_804013F8(s32 card_slot);                    /* check_card_slot */
void func_804014E4(void);                            /* reset_sw_events */
void func_80401578(void);                            /* reset_hw_events */
s32 func_804016D0(void);                             /* func_8002168C */
s32 func_80401E2C(s32 n);                            /* func_80021D54 */
s32 func_80402084(s32 a, u8 *b);                     /* func_80021F18 */
s32 func_80402154(s32 a, u8 *b, s32 c);              /* func_80021FF0 */
void func_80402214(void);                            /* func_800220DC */
s32 func_80402268(void);                             /* func_80022138 */
s32 func_804022C8(s32 idx);                          /* func_8002219C */
void func_80402974(UnkCardPair *p, u32 *a, u32 *b);  /* func_800226E0 */
u8 *func_80402CD4(u8 *p, s32 *q);                    /* func_80022A30 */
void func_80402EA4(s32 *p, s32 a, s32 b, s32 c, s32 d); /* func_80022C48 */
s32 func_80402F84(s32 a, s32 id, s32 kind, s32 p);   /* func_80022D60 */
void func_804030F4(s32 a);                           /* func_80022E9C */
void func_8040320C(s32 a);                           /* func_80022FB4 */
void func_80403234(s32 a);                           /* func_80022FDC */
void func_804032D4(s32 a);                           /* func_8002307C */
s32 func_8040343C(s32 flag);                         /* func_800231E4 */
void func_80403698(s32 *p);                          /* func_800233D0 */
void func_804037F4(u8 *p);                           /* func_8002352C */
void func_804039F8(UnkCardPair *p, u32 *a, u32 *b);  /* func_80023730 */
void func_80403D24(s32 *p);                          /* func_80023A00 */
void func_80403D7C(s32 *p, s32 v);                   /* func_80023A50 */
void func_80403DA0(UnkCardPair *p, u32 *a, u32 *b, s32 n); /* func_80023A78 */
void func_80404178(s32 *p);                          /* func_80023E14 */
s32 func_80404364(s32 a, s32 n);                     /* func_80023FF0 */
void func_80404570(s32 *p);                          /* func_80024184 */
void func_804045C8(s32 *p, s32 flag);                /* func_800241D4 */
void func_80404618(s32 *p, s32 flag);                /* func_80024224 */
void func_80404668(s32 *p);                          /* func_80024274 */
s32 func_80404C84(s32 *p, s32 *q);                   /* func_8004F418 */
void func_80404DBC(s32 a);                           /* func_8004F52C */
u8 *func_80404DEC(u8 *p, s32 w, s32 h);              /* func_800242D8 */
void func_80405624(UnkCardPair *p, u32 *a, u32 *b);  /* func_80024BB4 */
void func_804059A8(s32 *p);                          /* func_80024EEC */
void func_80405A00(s32 *p, s32 a, s32 b);            /* func_80024F3C */
void func_80405A3C(s32 *p);                          /* func_80024F7C */
void func_80405AE8(s32 *p);                          /* func_80025030 */
void func_80405C44(u8 *p);                           /* func_8002519C */
void func_80405CB8(s32 *p);                          /* func_80025230 */
void func_80405DE0(s32 *p);                          /* func_80025398 */
void func_80406368(s32 *p);                          /* func_8002593C */
s32 *func_80406678(s32 *p, s32 a);                   /* func_80025C7C */
void func_80406678w(s32 *p) __asm__("func_80406678");   /* ONE argument at func_80407E08's call site, where the
    * func_8040701C beside it takes two and the live source passes two to both.
    * The one-argument call cannot be written through the real prototype, so it
    * goes through an __asm__ alias -- the same device as main.h's
    * func_80024C48w and csrc/gprel_72A8.c's func_80033AE8w. */
void func_804067BC(s32 *p);                          /* func_80025DC8 */
void func_804069FC(s32 *p);                          /* func_800261C0 */
void func_80406AA8(s32 *p);                          /* func_800262BC */
void func_80406BEC(s32 *p);                          /* func_80026410 */
void func_80406DA0(s32 *p);                          /* func_800265B8 */
s32 *func_8040701C(s32 *p, s32 a);                   /* func_80026868 */
void func_804072CC(u8 *p, s32 *b, s32 *c);           /* func_80026B18 */
void func_804073B0(UnkCardPair *p, u32 *a, u32 *b);  /* func_80026C00 */
void func_80407558(s32 *p);                          /* func_80026D78 */
s32 *func_804076C8(s32 *p, s32 a);                   /* func_80026ED0 */
void func_8040789C(s32 *p);                          /* func_80027018 */
void func_804078D8(u8 *p);                           /* func_80027054 */
void func_80407A44(UnkCardPair *p, u32 *a, u32 *b);  /* func_800271C0 */
void func_80407D38(s32 *p);                          /* func_80027458 */
void func_80407D90(s32 *p);                          /* func_800274A8 */
void func_80407E64(s32 *p);                          /* func_80027584 */
void func_80407F68(s32 *p);                          /* func_80027684 */
void func_804081AC(s32 *p);                          /* func_8002789C */
void func_804084DC(s32 *p);                          /* func_80027C90 */
long func_80408654(long drv);                        /* _card_wait */
long func_80408664(long chan);                       /* _card_info */
long func_80408674(long chan);                       /* _card_load */

/* --- variables, in address order --- */
extern u8 D_80084110[];                              /* D_80027F7C */
extern u8 D_800841DC[];                              /* D_80079054 */
extern u8 D_8008423C[];                              /* D_800790B4 */
extern u8 D_8008429C[];                              /* D_80079114 */
extern u8 D_8008432C[];                              /* D_800791A4 */
extern u32 D_8008DAB4;                               /* D_800814A0, declared in include/structs.h, not main.h */
extern u32 D_8008DAB4_a[1] __asm__("D_8008DAB4");    /* the array spelling of the same object; see main.h's note on D_800814A0_a -- the lever is PER SITE */
extern s32 D_8009E390;                               /* D_80084118; read off this block's own listing, not a symmap vote -- its pair is assert-bearing */
extern const char D_80400004[40];                    /* ASSERT_FMT */
extern const char D_8040002C[];                      /* FILENAME_C_SERVER, "c_server.c" */
extern u8 D_804005F0[];                              /* D_800282B0 */
extern u8 D_80408C84[];                              /* D_80027F60 */
extern u8 *D_80408C8C[];                             /* D_800282BC */
extern u8 D_80408C9C[];                              /* D_80027F68 */
extern u8 *D_80408CB0[];                             /* D_800282A4 */
extern u8 D_80408CB8[];                              /* D_80027FC4 */
extern u8 D_80408CF0[];                              /* D_80027FD0 */
extern u8 D_80408D28[];                              /* D_80027FF8 */
extern u8 D_80408D64[];                              /* D_8002808C */
extern u8 D_80408D6C[];                              /* D_800282CC */
extern u8 D_80408D74[];                              /* D_80028094 */
extern long D_80409270;                              /* gSwCardEOFEvent, D_80028550 */
extern long D_80409274;                              /* gSwCardErrorEvent, D_80028554 */
extern long D_80409278;                              /* gSwCardTimeoutEvent, D_80028558 */
extern long D_8040927C;                              /* gSwCardNewEvent, D_8002855C */
extern s32 D_80409280;                               /* D_80028560 */
extern s32 D_80409284;                               /* D_80028564 */
extern s32 D_80409288;                               /* D_80028568 */
extern s32 D_8040928C;                               /* D_8002856C */
extern s32 D_804094E8;                               /* D_800287C8 */
extern s32 D_804094EC;                               /* card_slot, D_800287CC */
extern s32 D_80409500;                               /* D_800287E0 */
extern CARD_STATE D_80409504;                        /* gCardState, D_800287E4 */

#define dbg_card_slot D_804094EC

#endif
