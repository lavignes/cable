#ifndef CBL_MATH_SCALAR_2D_H_
#define CBL_MATH_SCALAR_2D_H_

#include <math.h>
#include <string.h>

#include <cable/porting.h>

CBL_EXTERN_BEGIN

#define CBL_2D_SHUFFLE_(x, y) (((x) << 1) | (y))

typedef enum Cbl2dSwizzle {
    CBL_2D_SWIZZLE_XX = CBL_2D_SHUFFLE_(0, 0),
    CBL_2D_SWIZZLE_XY = CBL_2D_SHUFFLE_(0, 1),
    CBL_2D_SWIZZLE_YX = CBL_2D_SHUFFLE_(1, 0),
    CBL_2D_SWIZZLE_YY = CBL_2D_SHUFFLE_(1, 1),
} Cbl2dSwizzle;

#undef CBL_2D_SHUFFLE_

typedef struct Cbl2d {
    union {
        struct {
            double x_, y_;
        };
        double arr_[2];
    };
} Cbl2d;

CBL_INLINE Cbl2d cbl2dNew(double x, double y) {
    return (Cbl2d) { .x_ = x, .y_ = y };
}

CBL_INLINE Cbl2d cbl2dZero() {
    return (Cbl2d) { .x_ = 0.0, .y_ = 0.0 };
}

CBL_INLINE bool cbl2dEqual(Cbl2d lhs, Cbl2d rhs) {
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}

CBL_INLINE bool cbl2dLessThan(Cbl2d lhs, Cbl2d rhs) {
    return lhs.x_ < rhs.x_ && lhs.y_ < rhs.y_;
}

CBL_INLINE bool cbl2dGreaterThan(Cbl2d lhs, Cbl2d rhs) {
    return lhs.x_ > rhs.x_ && lhs.y_ > rhs.y_;
}

CBL_INLINE Cbl2d cbl2dMap(Cbl2d v2d, double (*fn)(double)) {
    return cbl2dNew(fn(v2d.x_), fn(v2d.y_));
}

CBL_INLINE Cbl2d cbl2dLoad(const double *arr) {
    return cbl2dNew(arr[0], arr[1]);
}

CBL_INLINE void cbl2dStore(Cbl2d v2d, double *arr) {
    memcpy(arr, &v2d, sizeof(Cbl2d));
}

CBL_INLINE double cbl2dGetX(Cbl2d v2d) {
    return v2d.x_;
}

CBL_INLINE double cbl2dGetY(Cbl2d v2d) {
    return v2d.y_;
}

CBL_INLINE Cbl2d cbl2dSwizzle(Cbl2d v2d, Cbl2dSwizzle swizzle) {
    return cbl2dNew(v2d.arr_[(swizzle & 0x2) >> 1],
                    v2d.arr_[(swizzle & 0x1)]);
}
CBL_INLINE Cbl2d cbl2dSplat(double d) {
    return cbl2dNew(d, d);
}

CBL_INLINE Cbl2d cbl2dSplatX(Cbl2d v2d) {
    return cbl2dSplat(v2d.x_);
}

CBL_INLINE Cbl2d cbl2dSplatY(Cbl2d v2d) {
    return cbl2dSplat(v2d.y_);
}

CBL_INLINE Cbl2d cbl2dAdd(Cbl2d lhs, Cbl2d rhs) {
    return cbl2dNew(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_);
}

CBL_INLINE Cbl2d cbl2dSub(Cbl2d lhs, Cbl2d rhs) {
    return cbl2dNew(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_);
}

CBL_INLINE Cbl2d cbl2dMul(Cbl2d lhs, Cbl2d rhs) {
    return cbl2dNew(lhs.x_ * rhs.x_, lhs.y_ * rhs.y_);
}

CBL_INLINE Cbl2d cbl2dDiv(Cbl2d lhs, Cbl2d rhs) {
    return cbl2dNew(lhs.x_ / rhs.x_, lhs.y_ / rhs.y_);
}

CBL_INLINE Cbl2d cbl2dRecip(Cbl2d v2d) {
    return cbl2dNew(1.0 / v2d.x_, 1.0 / v2d.y_);
}

CBL_INLINE Cbl2d cbl2dSqrt(Cbl2d v2d) {
    return cbl2dNew(sqrt(v2d.x_), sqrt(v2d.x_));
}

CBL_INLINE Cbl2d cbl2dSqrtFast(Cbl2d v2d) {
    return cbl2dSqrt(v2d);
}

CBL_INLINE Cbl2d cbl2dRecipSqrt(Cbl2d v2d) {
    return cbl2dRecip(cbl2dSqrt(v2d));
}

CBL_INLINE double cbl2dCross(Cbl2d lhs, Cbl2d rhs) {
    return lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_;
}

CBL_INLINE Cbl2d cbl2dMin(Cbl2d lhs, Cbl2d rhs) {
    return cbl2dNew(fmin(lhs.x_, rhs.x_), fmin(lhs.y_, rhs.y_));
}

CBL_INLINE Cbl2d cbl2dMax(Cbl2d lhs, Cbl2d rhs) {
    return cbl2dNew(fmax(lhs.x_, rhs.x_), fmax(lhs.y_, rhs.y_));
}

CBL_INLINE Cbl2d cbl2dAbs(Cbl2d v2d) {
    return cbl2dNew(fabs(v2d.x_), fabs(v2d.y_));
}

CBL_INLINE Cbl2d cbl2dNeg(Cbl2d v2d) {
    return cbl2dNew(-v2d.x_, -v2d.y_);
}

CBL_EXTERN_END

#endif // CBL_MATH_SCALAR_2D_H_