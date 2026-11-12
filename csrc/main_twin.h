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
s32 *func_8003C480(s32 a);                           /* func_8003FC64 */
s32 *func_8003C714(s32 a, u8 *b, s32 c);             /* func_8003FE78 */
s32 func_80056F14(s32 a);                            /* func_80049E6C */
void func_80058368(s32 p, s16 *r);                   /* func_80049D80 */
/* THIS BLOCK LINKED ITS OWN COPY OF THE C LIBRARY AND THE BIOS DIRECTORY
   CALLS, at addresses that are not the resident ones. The live source names
   them (strcpy, firstfile2, ...) and those names resolve to the RESIDENT
   engine, which links silently and is wrong -- see the alias note in
   csrc/ovl_main_80400000.c. Spell them by address here. */
char *func_8007BF50(char *d, char *s);               /* strcat */
char *func_8007BF80(char *d, const char *s);         /* strcpy */
long func_8007C9D8(long event);                      /* EnableEvent */
struct DIRENTRY *func_8007CAB8(u8 *name, struct DIRENTRY *e);  /* firstfile2 */
struct DIRENTRY *func_8007CAC8(struct DIRENTRY *e);  /* nextfile */
u_short func_80078570(int tp, int abr, int x, int y); /* GetTPage */
u_short func_80078640(int x, int y);                 /* GetClut */
void func_8007BEF0();                                /* exit, psyq/include/stdlib.h */
int func_8007C040(char *fmt, ...);                   /* printf */
long func_8007C998(unsigned long, long, long, long (*func)()); /* OpenEvent */
long func_8007C9A8(long);                            /* CloseEvent */
long func_8007C9C8(long);                            /* TestEvent */
int func_8007CA38(void);                             /* EnterCriticalSection */
void func_8007CA48(void);                            /* ExitCriticalSection */
long func_804009C0(void);                            /* HandleCardEOF */
long func_804009D4(void);                            /* HandleCardError */
long func_804009E8(void);                            /* HandleCardTimeout */
long func_804009FC(void);                            /* HandleCardNew */
void func_80400A10(void);                            /* open_card_events */
s32 func_804010F0(void);                             /* on_sw_card_eof */
s32 func_804012B0(void);                             /* on_card_new */
s32 func_804013F8(s32 card_slot);                    /* check_card_slot */
void func_804014E4(void);                            /* reset_sw_events */
void func_80401578(void);                            /* reset_hw_events */
s32 func_804016D0(void);                             /* func_8002168C */
void func_804018FC(void);                            /* print_card_status */
s32 func_804019B4(void);                             /* count_card_files */
s32 func_80401B30(s32 card_slot);                     /* format_card */
s32 func_80401E2C(s32 n);                            /* load_save_slot_info */
s32 func_80402084(s32 a, u8 *b);                     /* write_save_file */
s32 func_80402154(s32 a, u8 *b);                     /* read_save_file, which takes
                                                        THREE here in the live build --
                                                        this listing reads $a0 and $a1
                                                        and never $a2 */
void func_80402214(void);                            /* refresh_card_info */
s32 func_80402268(void);                             /* get_free_blocks */
s32 func_80402290(void);                             /* count_used_save_slots */
s32 func_804022C8(s32 idx);                          /* erase_save_file */
void func_80402974(UnkCardPair *p, u32 *a, u32 *b);  /* func_800226E0 */
u8 *func_80402CD4(u8 *p, s32 *q);                    /* func_80022A30 */
void func_80402EA4(s32 *p, s32 a, s32 b, s32 c, s32 d); /* func_80022C48 */
s32 func_80402F84(s32 a, s32 id, s32 kind, s32 p);   /* open_card_message */
void func_804030F4(s32 a);                           /* msg_no_card */
void func_8040311C(s32 a, s32 p);                    /* msg_please_wait */
void func_80403144(s32 a);                           /* msg_overwrite_failed */
void func_8040320C(s32 a);                           /* msg_not_enough_blocks_overwrite */
void func_80403234(s32 a);                           /* msg_not_enough_blocks */
void func_8040325C(s32 a);                           /* msg_load_failed */
void func_80403284(s32 a);                           /* msg_format_failed */
void func_804032AC(s32 a);                           /* msg_format_done */
void func_804032D4(s32 a);                           /* msg_card_damaged */
void func_804032FC(s32 a);                           /* msg_no_saved_game */
void func_8040334C(s32 a);                           /* msg_copy_failed */
void func_80403374(s32 a);                           /* msg_cannot_overwrite */
void func_804033C4(s32 a);                           /* msg_delete_failed */
s32 func_8040343C(s32 flag);                         /* func_800231E4 */
void func_80403698(s32 *p);                        /* func_800233D0 */
void func_804037F4(u8 *p);                           /* func_8002352C */
void func_804039F8(UnkCardPair *p, u32 *a, u32 *b);  /* func_80023730 */
s32 *func_80403BC4(s32 *p);                          /* func_800238D4 */
void func_80403D24(s32 *p);                          /* func_80023A00 */
void func_80403D7C(s32 *p, s32 v);                   /* func_80023A50 */
void func_80403DA0(UnkCardPair *p, u32 *a, u32 *b, s32 n); /* func_80023A78 */
void func_80403E54(s32 *p);                        /* func_80023B18 */
void func_804040C8(s32 *p);                          /* func_80023D64 */
void func_80404178(s32 *p);                          /* func_80023E14 */
s32 func_80404364(s32 a, s32 n);                     /* func_80023FF0 */
void func_80404570(s32 *p);                          /* func_80024184 */
void func_804045C8(s32 *p, s32 flag);                /* func_800241D4 */
void func_80404618(s32 *p, s32 flag);                /* func_80024224 */
void func_80404668(s32 *p);                          /* func_80024274 */
void func_80404708(u8 *p, u32 *a, u32 *b);           /* func_8004EEFC */
u8 *func_804047B0(u8 *p);                            /* func_8004EF90 */
u8 *func_80404818(u8 *p);                            /* func_8004EFF4 */
u8 *func_80404888(u8 *p);                            /* func_8004F068 */
void func_804048F8(u8 *p);                           /* func_8004F0DC */
u8 *func_80404A3C(u8 *p);                            /* func_8004F1E8 */
void func_80404B10(u8 *p);                           /* func_8004F2BC */
void func_80404C3C(u32 *p, u32 v);                   /* func_8004F3D0 */
s32 func_80404C84(s32 *p, s32 *q);                   /* func_8004F418 */
void func_80404DBC(u8 *p);                           /* func_8004F52C */
u8 *func_80404DEC(u8 *p, s32 w, s32 h);              /* func_800242D8 */
void func_80405624(UnkCardPair *p, u32 *a, u32 *b);  /* func_80024BB4 */
s32 *func_804057F0(s32 *p, s32 a);                  /* func_80024D58 */
void func_804059A8(s32 *p);                          /* func_80024EEC */
void func_80405A00(s32 *p, s32 a);                    /* func_80024F3C, which takes THREE -- this
                                                        build reads only $a0/$a1, see its listing */
void func_80405A3C(s32 *p);                          /* func_80024F7C */
void func_80405AE8(s32 *p);                          /* func_80025030 */
void func_80405B98(s32 *p);                          /* func_800250E8 */
void func_80405C44(u8 *p);                           /* func_8002519C */
void func_80405C4C(s32 *p);                          /* func_800251A4 */
void func_80405CB8(s32 *p);                          /* func_80025230 */
void func_80405D54(s32 *p);                          /* func_800252CC */
void func_80405DE0(s32 *p);                          /* func_80025398 */
void func_80406150(s32 *p);                          /* func_800253E8-family, paired by alignment */
void func_804061C4(s32 *p);                          /* func_80025788 */
void func_80406274(s32 *p);                          /* func_80025848 */
void func_804062F4(s32 *p);                          /* func_800258C8 */
void func_80406368(s32 *p);                          /* func_8002593C */
void func_804065CC(s32 *p);                          /* func_80025BD4 */
s32 *func_80406678(s32 *p, s32 a);                   /* func_80025C7C */
void func_80406678w(s32 *p) __asm__("func_80406678");
void func_80406720(s32 *p);                          /* func_80025D34 */
void func_804067BC(s32 *p);                          /* func_80025DC8 */
void func_804069FC(s32 *p);                          /* func_800261C0 */
void func_80406AA8(s32 *p);                          /* func_800262BC */
void func_80406BEC(s32 *p);                          /* func_80026410 */
void func_80406DA0(s32 *p);                          /* func_800265B8 */
void func_80406F70(s32 *p);                          /* func_800267C0 */
s32 *func_8040701C(s32 *p, s32 a);                   /* func_80026868 */
void func_804070C4(s32 *p);                          /* func_80026920 */
void func_80407160(s32 *p);                          /* func_800269B4 */
void func_804072CC(u8 *p, s32 *b, s32 *c);           /* func_80026B18 */
void func_804073B0(UnkCardPair *p, u32 *a, u32 *b);  /* func_80026C00 */
void func_80407458(UnkCardGrp044 *c);                /* func_80026C94 */
void func_804074C4(UnkCardGrp044 *c);                /* func_80026CF0 */
void func_80407558(UnkCardGrp044 *c);             /* func_80026D78 */
s32 *func_804076C8(s32 *p, s32 a);                   /* func_80026ED0 */
void func_8040789C(UnkCardGrp0D4 *c);             /* func_80027018 */
void func_804078D8(u8 *p);                           /* func_80027054 */
void func_80407A44(UnkCardPair *p, u32 *a, u32 *b);  /* func_800271C0 */
s32 *func_80407C10(s32 *p);                          /* func_80027364 */
void func_80407D38(s32 *p);                          /* func_80027458 */
void func_80407D90(s32 *p);                          /* func_800274A8 */
/* ONE argument at func_80407E08's call site, where the func_8040701C beside it takes two; same device as main.h's func_80024C48w. */
void func_80407E64(s32 *p);                          /* func_80027584 */
void func_80407F68(s32 *p);                          /* func_80027684 */
void func_804081AC(s32 *p);                          /* func_8002789C */
void func_804083A0(s32 *p);                          /* func_80027A9C */
void func_804084DC(s32 *p);                          /* func_80027C90 */
long func_80408654(long drv);                        /* _card_wait */
long func_80408664(long chan);                       /* _card_info */
long func_80408674(long chan);                       /* _card_load */
s32 *func_804086BC(s32 a, s32 b);                    /* func_80027AFC */

/* --- variables, in address order --- */
extern u32 D_8002D690;                               /* D_8002E5D8[0] in the shipped build */
/* The ARRAY spelling of the 4-word template, for func_80404B84's loop, which
 * keeps it in a base register; func_80404708 reads the same words as the four
 * scalars above. Per-site, exactly as main.h's D_800814A0_a. */
extern u32 D_8002D690_a[] __asm__("D_8002D690");
extern u32 D_8002D694;                               /* D_8002E5D8[1] */
extern u32 D_8002D698;                               /* D_8002E5D8[2] */
extern u32 D_8002D69C;                               /* D_8002E5D8[3] */
extern u32 D_8002D6A0;                               /* D_8002E5E8[0] */
/* and the ARRAY spelling of the 3-word template, for the five functions that
 * copy both templates into a caller-supplied buffer -- the live source reads
 * them as `u32 *s = D_8002E5E8;` and so must this, or the seven absolute
 * loads come out hoisted into four registers instead of reusing $v0. */
extern u32 D_8002D6A0_a[] __asm__("D_8002D6A0");
extern u32 D_8002D6A4;                               /* D_8002E5E8[1] */
extern u32 D_8002D6A8;                               /* D_8002E5E8[2] */
                             /* D_8002E5D8, the 4-word GPU template, in the OTHER engine build */
                             /* D_8002E5E8, the 3-word one */
extern u8 D_80084110[];                              /* D_80027F7C */
extern u8 D_800841DC[];                              /* D_80079054 */
extern u8 D_8008423C[];                              /* D_800790B4 */
extern u8 D_8008429C[];                              /* D_80079114 */
extern u8 D_8008432C[];                              /* D_800791A4 */
extern u32 D_8008DAB4;                               /* D_800814A0, declared in include/structs.h, not main.h */
extern u32 D_8008DAB4_a[1] __asm__("D_8008DAB4");    /* the array spelling of the same object; see main.h's note on D_800814A0_a -- the lever is PER SITE */
extern u8 D_80082FF4[];                              /* D_80077E84 */
extern u8 D_80083060[];                              /* D_80077EF0 */
extern u8 D_8008306C[];                              /* D_80077EFC */
extern u8 D_80083834[];                              /* D_800786DC */
extern u8 D_80083840[];                              /* D_800786E8 */
extern s32 D_8009DDD8[][32];                         /* D_80083E98, the 0x80-byte save-slot row */
extern s32 D_8009EB58[];                             /* card_free_blocks, D_80084120 */
extern s32 D_80136140[];                             /* card_file_count, D_80084128 -- and note the
                                                        ENGINE BUILD is different here: this one is
                                                        nowhere near the resident 0x80084128 */

extern s32 D_8009E390;                               /* D_80084118; read off this block's own listing, not a symmap vote -- its pair is assert-bearing */
extern const char D_80400004[40];                    /* ASSERT_FMT */
extern const UnkFnTab5 D_80400054;                   /* gSwCardEventHandlers */
extern const UnkFnTab5 D_80400068;                   /* gHwCardEventHandlers */
extern const char D_8040002C[];                      /* FILENAME_C_SERVER, "c_server.c" */
extern const char D_804000F4[];                      /* D_80020084, the filename suffix */
extern u8 D_804005F0[8];                              /* D_800282B0 */
/* MEM_CARD_NAMES, DECLARED INCOMPLETE ON PURPOSE. The object really is 8
   bytes -- D_80408AA8 below is the next one -- but 8 bytes is the -G8
   small-data path, which makes the address ONE insn, after which cse folds
   func_804019B4's conditional `+4` into a fresh SYM+4 instead of adding to
   the live register. The target adds, so the original declaration cannot
   have carried the size. The live block is -G0, where the declared size
   changes nothing, which is why csrc/main.h can say `[2]` and why this -G8
   build is the only place the difference shows. */
extern const char *D_80408AA0[];                     /* MEM_CARD_NAMES */
extern char D_80408AA8[16];                          /* MEM_CARD_PREFIX */
extern const char *D_80408AB8[6];                    /* MEM_CARD_FILENAMES */
extern u8 D_80408C84[8];                              /* D_80027F60 */
/* FOUR SIZED-ALIAS DEVICES USED TO LIVE HERE and are gone as of 2026-11-11.
   Three were `extern u8 func_XXXXXXXX_a[8] __asm__("func_XXXXXXXX")`, giving
   a FUNCTION a declared size so its address took the -G8 small-data path and
   came out as one insn -- the only way, under 4.1's compiler, to stop reorg
   stealing the `lui` into a branch delay slot where this block keeps a nop.
   The fourth was `D_80408C8C_a[1]`, the same trick on a table whose real size
   is 16. All four were EMULATING a compiler without split addresses, one site
   at a time. This unit now has one (tools/psx272gcc), so the plain spellings
   match and the lies are deleted. */
extern void func_80406590(s32 *p);                   /* func_80025B9C AND func_80025B60:
                                                        this build uses ONE handler where the
                                                        live source has two. func_804065CC. */
extern void func_80406F34(s32 *p);                   /* func_80026784 AND func_80026748: the
                                                        same one-handler-for-two as above,
                                                        on func_80406F70. */

extern u8 *D_80408C8C[4];                            /* D_800282BC */
extern u8 D_80408C9C[];                              /* D_80027F68 */
extern u8 *D_80408CB0[];                             /* D_800282A4 */
extern u8 D_80408CB8[];                              /* D_80027FC4 */
extern u32 D_80408CC4;                               /* D_80071660 */
extern u32 D_80408CC8;                             /* D_80071664 */
extern u32 D_80408CCC;                             /* D_80071668 */
extern u32 D_80408CD0;                               /* D_8007166C */
extern u32 D_80408CD4;                             /* D_80071670 */
extern u32 D_80408CD8;                             /* D_80071674 */
extern u32 D_80408CDC;                               /* D_80071678 */
extern u32 D_80408CE0;                               /* D_8007167C, read as a SCALAR here */
extern u32 D_80408CE4;                               /* D_80071680, likewise */
extern u8 D_80408CE8[8];                              /* D_800717BC, include/structs.h */
extern u8 D_80408CF0[4];                              /* D_80027FD0 */
extern u8 D_80408D28[];                              /* D_80027FF8 */
extern u8 D_80408D54[];                              /* D_8002807C */
extern u8 D_80408D5C[];                              /* D_80028084 */
extern u8 D_80408D64[8];                              /* D_8002808C */
extern u8 D_80408D6C[8];                             /* D_800282CC */
extern u8 D_80408D74[];                              /* D_80028094 */
extern struct DIRENTRY D_80409290[15];               /* card_dir */
extern u8 D_8040C628[];                              /* D_8002B7E0 */
extern long D_80409270;                              /* gSwCardEOFEvent, D_80028550 */
extern long D_80409274;                              /* gSwCardErrorEvent, D_80028554 */
extern long D_80409278;                              /* gSwCardTimeoutEvent, D_80028558 */
extern long D_8040927C;                              /* gSwCardNewEvent, D_8002855C */
extern long D_80409280;                              /* gHwCardEOFEvent, D_80028560 */
extern long D_80409284;                              /* gHwCardErrorEvent, D_80028564 */
extern long D_80409288;                              /* gHwCardTimeoutEvent, D_80028568 */
extern long D_8040928C;                              /* gHwCardNewEvent, D_8002856C */
extern s32 D_804094E8;                               /* card_st, D_800287C8 */
extern s32 D_804094EC;                               /* card_slot, D_800287CC */
extern s32 D_80409500;                               /* card_evflg, D_800287E0 */
extern CARD_STATE D_80409504;                        /* gCardState, D_800287E4 */

#define dbg_card_slot D_804094EC

#endif
