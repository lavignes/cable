#ifndef CBL_MATH_4F_H_
#define CBL_MATH_4F_H_

#ifdef __SSE__
#include <cable/math/simd/4f.h>
#else
#include <cable/math/scalar/4f.h>
#endif

CBL_EXTERN_BEGIN

static inline Cbl4f cbl4fDot2(Cbl4f lhs, Cbl4f rhs) {
    const Cbl4f mul = cbl4fMul(lhs, rhs);
    return cbl4fAdd(cbl4fSplatX(mul), cbl4fSplatY(mul));
}

static inline Cbl4f cbl4fDot3(Cbl4f lhs, Cbl4f rhs) {
    const Cbl4f mul = cbl4fMul(lhs, rhs);
    return cbl4fAdd(cbl4fAdd(cbl4fSplatX(mul), cbl4fSplatY(mul)),
                    cbl4fSplatZ(mul));
}

static inline Cbl4f cbl4fDot(Cbl4f lhs, Cbl4f rhs) {
    const Cbl4f mul = cbl4fMul(lhs, rhs);
    return cbl4fAdd(cbl4fAdd(cbl4fAdd(cbl4fSplatX(mul),
                                      cbl4fSplatY(mul)),
                             cbl4fSplatZ(mul)),
                    cbl4fSplatW(mul));
}

static inline Cbl4f cbl4fLength2(Cbl4f v4f) {
    return cbl4fSqrt(cbl4fDot2(v4f, v4f));
}

static inline Cbl4f cbl4fLength3(Cbl4f v4f) {
    return cbl4fSqrt(cbl4fDot3(v4f, v4f));
}

static inline Cbl4f cbl4fLength(Cbl4f v4f) {
    return cbl4fSqrt(cbl4fDot(v4f, v4f));
}

static inline Cbl4f cbl4fLength2Fast(Cbl4f v4f) {
    return cbl4fSqrtFast(cbl4fDot2(v4f, v4f));
}

static inline Cbl4f cbl4fLength3Fast(Cbl4f v4f) {
    return cbl4fSqrtFast(cbl4fDot3(v4f, v4f));
}

static inline Cbl4f cbl4fLengthFast(Cbl4f v4f) {
    return cbl4fSqrtFast(cbl4fDot(v4f, v4f));
}

static inline Cbl4f cbl4fSquareLength2(Cbl4f v4f) {
    return cbl4fDot2(v4f, v4f);
}

static inline Cbl4f cbl4fSquareLength3(Cbl4f v4f) {
    return cbl4fDot3(v4f, v4f);
}

static inline Cbl4f cbl4fSquareLength(Cbl4f v4f) {
    return cbl4fDot(v4f, v4f);
}

static inline Cbl4f cbl4fNormalize2(Cbl4f v4f) {
    return cbl4fMul(v4f, cbl4fRecipSqrt(cbl4fDot2(v4f, v4f)));
}

static inline Cbl4f cbl4fNormalize3(Cbl4f v4f) {
    return cbl4fMul(v4f, cbl4fRecipSqrt(cbl4fDot3(v4f, v4f)));
}

static inline Cbl4f cbl4fNormalize(Cbl4f v4f) {
    return cbl4fMul(v4f, cbl4fRecipSqrt(cbl4fDot(v4f, v4f)));
}

static inline Cbl4f cbl4fNormalize2Fast(Cbl4f v4f) {
    return cbl4fMul(v4f, cbl4fRecipSqrtFast(cbl4fDot2(v4f, v4f)));
}

static inline Cbl4f cbl4fNormalize3Fast(Cbl4f v4f) {
    return cbl4fMul(v4f, cbl4fRecipSqrtFast(cbl4fDot3(v4f, v4f)));
}

static inline Cbl4f cbl4fNormalizeFast(Cbl4f v4f) {
    return cbl4fMul(v4f, cbl4fRecipSqrtFast(cbl4fDot(v4f, v4f)));
}

static inline Cbl4f cbl4fSurfNorm(Cbl4f v4f1, Cbl4f v4f2, Cbl4f v4f3) {
    return cbl4fCross(cbl4fSub(v4f2, v4f1), cbl4fSub(v4f3, v4f1));
}

CBL_EXTERN_END

#endif // CBL_MATH_4F_H_