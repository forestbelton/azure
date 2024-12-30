# https://github.com/ethteck/splat/wiki/General-Workflow/c520ca34534be5f500a377594cc6903ef01616d8#assembly-macros
.macro glabel label
    .global \label
    .type \label, @function
    \label:
.endm

.macro dlabel label
    .global \label
    \label:
.endm

.macro jlabel label
    .global \label
    \label:
.endm

.include "gte.inc"
