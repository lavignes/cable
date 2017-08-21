#ifndef CBL_MATH_SIMD_2D_H_
#define CBL_MATH_SIMD_2D_H_

#include <xmmintrin.h>
#include <math.h>

#include <cable/porting.h>

CBL_EXTERN_BEGIN

typedef enum Cbl2dSwizzle {
    CBL_2D_SWIZZLE_XX = _MM_SHUFFLE2(0, 0),
    CBL_2D_SWIZZLE_XY = _MM_SHUFFLE2(0, 1),
    CBL_2D_SWIZZLE_YX = _MM_SHUFFLE2(1, 0),
    CBL_2D_SWIZZLE_YY = _MM_SHUFFLE2(1, 1),
} Cbl2dSwizzle;

typedef __m128d Cbl2d;

typedef union {
    Cbl2d v2d_;
    double arr_[2];
    struct {
        double x_, y_;
    };
} Cbl2dSIMDUnion_;

static inline Cbl2d cbl2dNew(double x, double y) {
    return _mm_setr_pd(x, y);
}

static inline Cbl2d cbl2dZero() {
    return _mm_setzero_pd();
}

static inline bool cbl2dEqual(Cbl2d lhs, Cbl2d rhs) {
    return 0x0F == _mm_movemask_pd(_mm_cmpeq_pd(lhs, rhs));
}

static inline bool cbl2dLessThan(Cbl2d lhs, Cbl2d rhs) {
    return 0x0F == _mm_movemask_pd(_mm_cmplt_pd(lhs, rhs));
}

static inline bool cbl2dGreaterThan(Cbl2d lhs, Cbl2d rhs) {
    return 0x0F == _mm_movemask_pd(_mm_cmpgt_pd(lhs, rhs));
}

static inline Cbl2d cbl2dMap(Cbl2d v2d, double (*fn)(double)) {
    Cbl2dSIMDUnion_ u = {v2d};
    return cbl2dNew(fn(u.x_), fn(u.y_));
}

static inline Cbl2d cbl2dLoad(const double *arr) {
    return _mm_load_pd(arr);
}

static inline void cbl2dStore(Cbl2d v2d, double *arr) {
    _mm_storeu_pd(arr, v2d);
}

static inline double cbl2dGetX(Cbl2d v2d) {
    Cbl2dSIMDUnion_ u = {v2d};
    return u.x_;
}

static inline double cbl2dGetY(Cbl2d v2d) {
    Cbl2dSIMDUnion_ u = {v2d};
    return u.y_;
}

#define cbl2dSwizzle(v2d, swizzle) (_mm_shuffle_pd((v2d), (v2d), (swizzle)))

static inline Cbl2d cbl2dSplat(double d) {
    return _mm_set1_pd(d);
}

static inline Cbl2d cbl2dSplatX(Cbl2d v2d) {
    return cbl2dSwizzle(v2d, CBL_2D_SWIZZLE_XX);
}

static inline Cbl2d cbl2dSplatY(Cbl2d v2d) {
    return cbl2dSwizzle(v2d, CBL_2D_SWIZZLE_YY);
}

static inline Cbl2d cbl2dAdd(Cbl2d lhs, Cbl2d rhs) {
    return _mm_add_pd(lhs, rhs);
}

static inline Cbl2d cbl2dSub(Cbl2d lhs, Cbl2d rhs) {
    return _mm_sub_pd(lhs, rhs);
}

static inline Cbl2d cbl2dMul(Cbl2d lhs, Cbl2d rhs) {
    return _mm_mul_pd(lhs, rhs);
}

static inline Cbl2d cbl2dDiv(Cbl2d lhs, Cbl2d rhs) {
    return _mm_div_pd(lhs, rhs);
}

static inline Cbl2d cbl2dRecip(Cbl2d v2d) {
    return _mm_div_pd(cbl2dSplat(1.0f), v2d);
}

static inline Cbl2d cbl2dSqrt(Cbl2d v2d) {
    Cbl2dSIMDUnion_ u = {v2d};
    return cbl2dNew(sqrt(u.x_), sqrt(u.y_));
}

static inline Cbl2d cbl2dSqrtFast(Cbl2d v2d) {
    return _mm_sqrt_pd(v2d);
}

static inline Cbl2d cbl2dRecipSqrt(Cbl2d v2d) {
    return cbl2dRecip(cbl2dSqrt(v2d));
}

static inline double cbl2dCross(Cbl2d lhs, Cbl2d rhs) {
    Cbl2dSIMDUnion_ u1 = {lhs};
    Cbl2dSIMDUnion_ u2 = {rhs};
    return u1.x_ * u2.y_ - u1.y_ * u2.x_;
}

static inline Cbl2d cbl2dMin(Cbl2d lhs, Cbl2d rhs) {
    return _mm_min_pd(lhs, rhs);
}

static inline Cbl2d cbl2dMax(Cbl2d lhs, Cbl2d rhs) {
    return _mm_max_pd(lhs, rhs);
}

static inline Cbl2d cbl2dAbs(Cbl2d v2d) {
    // removes sign bit
    return _mm_andnot_pd(_mm_castsi128_pd(_mm_set1_epi64x(0x8000000000000000)), v2d);
}

static inline Cbl2d cbl2dNeg(Cbl2d v2d) {
    // flips sign bit
    return _mm_xor_pd(_mm_castsi128_pd(_mm_set1_epi64x(0x8000000000000000)), v2d);
}

CBL_EXTERN_END

#endif // CBL_MATH_SIMD_2D_H_