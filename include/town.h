#ifndef TOWN_H
#define TOWN_H

/* Recovered identifiers for the town's person state machine.
 *
 * THESE NAMES ARE THE ORIGINAL SOURCE'S.  town.bin carries two symbol tables
 * -- `struct { char name[32]; s32 value; }`, at file 0x80C000 (678 records)
 * and 0x811FFC (2,909) -- that the development build's flag viewer used, and
 * they are still on the retail disc.  docs/town-identifiers.md has the
 * extraction, the two checks that fix the record's field order, and the
 * evidence that these particular values are the indices of the two dispatch
 * tables below.  `tools/scan-container.py --symbols` dumps all 3,587.
 *
 * The short version, which func_800C3988 states in its own bounds:
 *
 *     t = person->state;
 *     if (t >= 2 && t < DFLT_END_PSN_DM)  D_800D5084[t](...);
 *     else if (t >= DFLT_END_PSN_DM)      actor->unk_5C[t - DFLT_END_PSN_DM](...);
 *
 * so states 2..21 are the SHARED set named by PSN_DM_*, DFLT_END_PSN_DM is
 * one past the last of them, and each actor's own states carry on from there
 * -- which is why every SSTP_<CHARACTER>_* enumeration in the table starts at
 * 22.  The player's table is not split that way: D_800D0508 is one 54-entry
 * table holding the shared states at 2..21 and SSTP_PLT_DM_* at 22..53.
 *
 * The HANDLER names in csrc/ovl_town*.c are derived from these, not recovered:
 * `psn_dm_homepos` is "the D_800D5084 entry PSN_DM_HOMEPOS selects".  The
 * state names are the evidence; the handler names are the bookkeeping.
 */

/* The shared person states -- D_800D5084[2..21], and the first 22 entries of
   every actor-specific table built to the same shape. */
#define PSN_DM_KANASIBA      1
#define PSN_DM_HOMEPOS       2
#define PSN_DM_TALK          3
#define PSN_DM_TALK2         4
#define PSN_DM_NORTHWAY      5
#define PSN_DM_SOUTHWAY      6
#define PSN_DM_WESTWAY       7
#define PSN_DM_EASTWAY       8
#define PSN_DM_NORTHWK       9
#define PSN_DM_SOUTHWK       10
#define PSN_DM_WESTWK        11
#define PSN_DM_EASTWK        12
#define PSN_DM_JMP           13
#define PSN_DM_GOTGT         14
#define PSN_DM_PLWAY         15
#define PSN_DM_TGTWAY        16
#define PSN_DM_DISP_OFF      17
#define PSN_DM_DISP_ON       18
#define PSN_DM_DEL           19
#define PSN_DM_ANG_ORG       20
#define PSN_DM_POS_ORG       21
#define DFLT_END_PSN_DM      22

/* The player's own states, D_800D0508[22..53]. 32 contiguous values, which is
   exactly what that table has room for above the shared set. */
#define SSTP_PLT_DM_SLEEP            22
#define SSTP_PLT_DM_KICKED_BFR       23
#define SSTP_PLT_DM_STANDUP          24
#define SSTP_PLT_DM_BIKU_BFR         25
#define SSTP_PLT_DM_MAMTALK          26
#define SSTP_PLT_DM_MAMTALKEND       27
#define SSTP_PLT_DM_YODA_MV          28
#define SSTP_PLT_DM_YODA_STDWN       29
#define SSTP_PLT_DM_YODA_POKA        30
#define SSTP_PLT_DM_YODA_STDUP       31
#define SSTP_PLT_DM_YUKA_HOSEI       32
#define SSTP_PLT_DM_MOGURA_POS_SET   33
#define SSTP_PLT_DM_EAT              34
#define SSTP_PLT_DM_DEAD             35
#define SSTP_PLT_DM_CARRYUP          36
#define SSTP_PLT_DM_CARRYWAYTGT      37
#define SSTP_PLT_DM_CARRYWALKTGT     38
#define SSTP_PLT_DM_KICKED           39
#define SSTP_PLT_DM_SWIM_BFR         40
#define SSTP_PLT_DM_SWIM             41
#define SSTP_PLT_DM_SWIM_AFTR        42
#define SSTP_PLT_DM_JMPUP_POS_SET    43
#define SSTP_PLT_DM_BIKU             44
#define SSTP_PLT_DM_NICPAN           45
#define SSTP_PLT_DM_NOW_GETITEM      46
#define SSTP_PLT_DM_KAO_UP           47
#define SSTP_PLT_DM_KAO_DOWN         48
#define SSTP_PLT_DM_UDAUDA           49
#define SSTP_PLT_DM_BATH             50
#define SSTP_PLT_DM_BATH_TURN        51
#define SSTP_PLT_DM_DAMAGE0          52
#define SSTP_PLT_DM_DAMAGE1          53

/* A parallel enumeration with its own numbering: the animation, not the state.
   Nothing in the landed C is known to index a table with it yet. */
#define ANM_PSN_DM_KANASIBA      1
#define ANM_PSN_DM_HOMEPOS       2
#define ANM_PSN_DM_TALK          3
#define ANM_PSN_DM_TALK2         4
#define ANM_PSN_DM_WAY           5
#define ANM_PSN_DM_WK            6
#define ANM_PSN_DM_JMP           7
#define ANM_PSN_DM_JMPCNT        8
#define ANM_PSN_DM_TGTPOS        9

#endif /* TOWN_H */
