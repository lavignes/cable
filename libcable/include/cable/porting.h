#ifndef CBL_PORTING_H_
#define CBL_PORTING_H_

// Grab the bulk of the c11 primitives
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <uchar.h>

// etc.
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
#define CBL_EXTERN_BEGIN extern "C" {
#define CBL_EXTERN_END }
#else
#define CBL_EXTERN_BEGIN
#define CBL_EXTERN_END
#endif

CBL_EXTERN_BEGIN

#ifndef __GNUC__
#define __has_attribute(...) 0
#endif

#if __has_attribute(always_inline)
#define CBL_INLINE inline __attribute__((always_inline))
#else
#define CBL_INLINE inline
#endif

#define CBL_NOT_FOUND SIZE_MAX

typedef enum CblCmp CblCmp;
enum CblCmp {
    CBL_CMP_EQUAL = 0,
    CBL_CMP_GREATER = 1,
    CBL_CMP_LESSER = -1,
};

CBL_INLINE size_t cblUSizeMin(size_t lhs, size_t rhs) {
    return (lhs < rhs) ? lhs : rhs;
}

CBL_INLINE size_t cblUSizeMax(size_t lhs, size_t rhs) {
    return (lhs > rhs) ? lhs : rhs;
}

typedef struct CblRange {
    size_t location;
    size_t length;
} CblRange;

CBL_INLINE CblRange cblRangeNew(size_t location, size_t length) {
    return (CblRange){location, length};
}

typedef enum CblByteOrder {
    CBL_BYTE_ORDER_UNKNOWN,
    CBL_BYTE_ORDER_LITTLE_ENDIAN,
    CBL_BYTE_ORDER_BIG_ENDIAN
} CblByteOrder;

CBL_INLINE CblByteOrder cblByteOrderGet() {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return CBL_BYTE_ORDER_LITTLE_ENDIAN;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return CBL_BYTE_ORDER_BIG_ENDIAN;
#else
    return CBL_BYTE_ORDER_UNKNOWN;
#endif
}

CBL_INLINE uint16_t cblByteOrderSwap16(uint16_t swap) {
    return ((swap << 8) & (uint16_t)0xFF00) | ((swap >> 8) & (uint16_t)0xFF);
}

CBL_INLINE uint16_t cblByteOrderSwap16BigToHost(uint16_t swap) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap16(swap);
#endif
}

CBL_INLINE uint16_t cblByteOrderSwap16HostToBig(uint16_t swap) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap16(swap);
#endif
}

CBL_INLINE uint16_t cblByteOrderSwap16LittleToHost(uint16_t swap) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap16(swap);
#endif
}

CBL_INLINE uint16_t cblByteOrderSwap16HostToLittle(uint16_t swap) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap16(swap);
#endif
}

CBL_INLINE uint32_t cblByteOrderSwap32(uint32_t swap) {
    return ((swap & 0xFF) << 24) | ((swap & 0xFF00) << 8) | ((swap >> 8) & 0xFF00) | ((swap >> 24) & 0xFF);
}

CBL_INLINE uint32_t cblByteOrderSwap32BigToHost(uint32_t swap) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap32(swap);
#endif
}

CBL_INLINE uint32_t cblByteOrderSwap32HostToBig(uint32_t swap) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap32(swap);
#endif
}

CBL_INLINE uint32_t cblByteOrderSwap32LittleToHost(uint32_t swap) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap32(swap);
#endif
}

CBL_INLINE uint32_t cblByteOrderSwap32HostToLittle(uint32_t swap) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap32(swap);
#endif
}

CBL_INLINE uint64_t cblByteOrderSwap64(uint64_t swap) {
    union {
        uint64_t _u64;
        uint32_t _u32s[2];
    } tmp, result;
    tmp._u64 = swap;
    result._u32s[0] = cblByteOrderSwap32(tmp._u32s[1]);
    result._u32s[1] = cblByteOrderSwap32(tmp._u32s[0]);
    return result._u64;
}

CBL_INLINE uint64_t cblByteOrderSwap64BigToHost(uint64_t swap) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap64(swap);
#endif
}

CBL_INLINE uint64_t cblByteOrderSwap64HostToBig(uint64_t swap) {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap64(swap);
#endif
}

CBL_INLINE uint64_t cblByteOrderSwap64LittleToHost(uint64_t swap) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap64(swap);
#endif
}

CBL_INLINE uint64_t cblByteOrderSwap64HostToLittle(uint64_t swap) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return swap;
#else
    return cblByteOrderSwap64(swap);
#endif
}

#define cblReturnIf(expr, value) { if ((expr)) return (value); }
#define cblBailIf(expr) { if ((expr)) return; }
#define cblReturnUnless(expr, value) { if (!(expr)) return (value); }
#define cblBailUnless(expr) { if (!(expr)) return; }

CBL_EXTERN_END

#endif // CBL_PORTING_H_