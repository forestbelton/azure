/* func_80057A94 (unk_1C540) -- 163 of 163 words, MATCH. NOT COMPILED, and the
 * reason is placement, not codegen: see docs/near-misses/func_80054F9C.c for
 * the three-table run this belongs to.
 *
 * The controller-change applier: looks the channel's voice record up through
 * func_80057A50 and then dispatches on the controller number at +0x4C, 23
 * cases through jtbl_80032EA4. Cases 1..12 poke the record's +0x01, +0x06,
 * +0x07 and the two attribute halfwords at +0x10 and +0x12; cases 15..19 set
 * the SPU reverb instead.
 *
 * MATCHED ON THE FIRST COMPILE. What had to be read rather than guessed:
 *
 * * THE STACK TEMP IS A REAL PSY-Q `SpuReverbAttr`, and the witness is the
 *   mask each arm writes against the field it writes next to it: case 15 sets
 *   mask 0x01 and +0x04, case 16 sets 0x06 and the two halfwords at +0x08 and
 *   +0x0A, case 17 sets 0x10 and +0x10, cases 18/19 set 0x08 and +0x0C. Those
 *   are SPU_REV_MODE / DEPTHL|DEPTHR / FEEDBACK / DELAYTIME against
 *   `mode` / `depth.left`, `depth.right` / `feedback` / `delay` exactly.
 *   libspu.h is already in include/common.h (func_8005C2C0 put it there for
 *   SpuVoiceAttr), so the type needs no new include. func_8005DA88 is
 *   therefore SpuSetReverbModeParam, func_8005D88C SpuSetReverb and
 *   func_8005DA68 the reverb-state read.
 *
 * * THE EMPTY CASES ARE LOAD-BEARING. The range check is `sltiu 0x17` with NO
 *   bias subtracted, so the table's low value is 0 and its high is 22 -- which
 *   means `case 0:` and `case 22:` are both in the source with empty bodies,
 *   even though neither does anything. Without them the table would span only
 *   1..19 and the bound would be 0x14. 13, 14, 20 and 21 are written out too;
 *   they cost nothing, since every empty case shares the end label.
 *
 * * The OR operand ORDER is visible where two arms cross-join. Cases 6 and 7
 *   share one `or`/`sh` tail: case 6 is `(hw & 0xFF0F) | ((d << 1) & 0xF0)`
 *   and case 7 is `(d >> 3) | (hw & 0xFFF0)` -- masked value first in one and
 *   second in the other, which is what the listing's operand registers say.
 *   Cases 8 and 10 share theirs the same way. Cases 9 and 11 are the same
 *   expressions with a trailing `| 0x8000` and `| 0x20`, written out in full
 *   because they do NOT share.
 *
 * * `0x7F - p[0x4E]` is a signed int, which is why case 10's shift is `sra`.
 *   p[0x4E] is re-read at every use -- three times in case 15, twice in case
 *   16 -- rather than held in a local.
 *
 * * func_80057A50 takes the record pointer AND the masked channel; $a0 is the
 *   incoming `p` passed straight through, so the call is
 *   `func_80057A50((s32 *) p, p[0x4D] & 0xF)`.
 */

#include "common.h"

u8 *func_80057A50(s32 *, u16);
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
