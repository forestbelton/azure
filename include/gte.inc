.macro AVSZ3
    c2 0x158002d
.endm

.macro AVSZ4
    c2 0x168002e
.endm

.macro MVMVA a b c d e
    .if \a == 1 && \b == 0 && \c == 0 && \d == 0 && \e == 0
        c2 0x480012
    .elseif \a == 1 && \b == 0 && \c == 0 && \d == 3 && \e == 0
        c2 0x486012
    .else
        .abort "invalid arg!"
    .endif
.endm

.macro NCLIP
    c2 0x1400006
.endm

.macro RTPS
    c2 0x0180001
.endm

.macro RTPT
    c2 0x0280030
.endm

.macro SQR arg
    .if \arg == 0
        c2 0x0a00428
    .elseif \arg == 1
        c2 0x0a80428
    .else
        .abort "invalid arg!"
    .endif
.endm
