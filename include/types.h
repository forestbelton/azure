#ifndef TYPES_H
#define TYPES_H

/* Fixed-width names used throughout the decompilation and by m2c's output.
 * Deliberately not PSY-Q's <sys/types.h>: that is only on the include path
 * once a translation unit actually needs the SDK headers, and its u_char /
 * u_long spellings do not match what m2c emits. */

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef short               s16;
typedef int                 s32;
typedef long long           s64;

typedef float               f32;
typedef double              f64;

typedef volatile unsigned char  vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int   vu32;

#ifndef NULL
#define NULL ((void *) 0)
#endif

#endif /* TYPES_H */
