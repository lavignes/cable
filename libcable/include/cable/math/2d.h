#ifndef CBL_MATH_2D_H_
#define CBL_MATH_2D_H_

#ifdef __SSE__
#include <cable/math/simd/2d.h>
#else
#include <cable/math/scalar/2d.h>
#endif

CBL_EXTERN_BEGIN

CBL_INLINE Cbl2d cbl2dDot(Cbl2d lhs, Cbl2d rhs) {
    const Cbl2d mul = cbl2dMul(lhs, rhs);
    return cbl2dAdd(cbl2dSplatX(mul), cbl2dSplatY(mul));
}

CBL_INLINE Cbl2d cbl2dLength(Cbl2d v2d) {
    return cbl2dSqrt(cbl2dDot(v2d, v2d));
}

CBL_INLINE Cbl2d cbl2dLengthFast(Cbl2d v2d) {
    return cbl2dSqrtFast(cbl2dDot(v2d, v2d));
}

CBL_INLINE Cbl2d cbl2dSquareLength(Cbl2d v2d) {
    return cbl2dDot(v2d, v2d);
}

CBL_INLINE Cbl2d cbl2dNormalize(Cbl2d v2d) {
    return cbl2dMul(v2d, cbl2dRecipSqrt(cbl2dDot(v2d, v2d)));
}

CBL_EXTERN_END

#endif // CBL_MATH_2D_H_