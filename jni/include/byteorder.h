/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2016 Apr 22
 ************************************************/
#ifndef __BYTEORDER_H_
#define __BYTEORDER_H_

#include <zl/types.h>

#define ___constant_swab16(x) ((u16)(             \
            (((u16)(x) & (u16)0x00ffU) << 8) |          \
            (((u16)(x) & (u16)0xff00U) >> 8)))

#define ___constant_swab32(x) ((u32)(             \
            (((u32)(x) & (u32)0x000000ffUL) << 24) |        \
            (((u32)(x) & (u32)0x0000ff00UL) <<  8) |        \
            (((u32)(x) & (u32)0x00ff0000UL) >>  8) |        \
            (((u32)(x) & (u32)0xff000000UL) >> 24)))

#define ___constant_swab64(x) ((u64)(             \
            (((u64)(x) & (u64)0x00000000000000ffULL) << 56) |   \
            (((u64)(x) & (u64)0x000000000000ff00ULL) << 40) |   \
            (((u64)(x) & (u64)0x0000000000ff0000ULL) << 24) |   \
            (((u64)(x) & (u64)0x00000000ff000000ULL) <<  8) |   \
            (((u64)(x) & (u64)0x000000ff00000000ULL) >>  8) |   \
            (((u64)(x) & (u64)0x0000ff0000000000ULL) >> 24) |   \
            (((u64)(x) & (u64)0x00ff000000000000ULL) >> 40) |   \
            (((u64)(x) & (u64)0xff00000000000000ULL) >> 56)))

#ifdef  __BIG_ENDIAN

#define cpu_to_le64(v) ___constant_swab64(v)
#define le64_to_cpu(v) ___constant_swab64(v)
#define cpu_to_le32(v) ___constant_swab32(v)
#define le32_to_cpu(v) ___constant_swab32(v)
#define cpu_to_le16(v) ___constant_swab16(v)
#define le32_to_cpu(v) ___constant_swab16(v)

#else

#define cpu_to_le64(v) (v)
#define le64_to_cpu(v) (v)
#define cpu_to_le32(v) (v)
#define le32_to_cpu(v) (v)
#define cpu_to_le16(v) (v)
#define le16_to_cpu(v) (v)
#endif

#endif

