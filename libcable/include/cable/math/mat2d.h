#ifndef CBL_MATH_MAT2D_H_
#define CBL_MATH_MAT2D_H_

#include <cable/math/2d.h>

CBL_EXTERN_BEGIN

typedef struct CblMat2d {
    Cbl2d x, y;
} CblMat2d;

CBL_INLINE CblMat2d cblMat2dNew(Cbl2d x, Cbl2d y) {
    return (CblMat2d) {x, y};
}

CBL_INLINE void cblMat2dIdentity(CblMat2d *m2d) {
    *m2d = (CblMat2d) {
            cbl2dNew(1.0, 0.0f),
            cbl2dNew(0.0, 1.0)
    };
}

CBL_INLINE void cblMat2dMul2d(const CblMat2d *lhs, const Cbl2d *rhs, Cbl2d *out) {
    *out = cbl2dAdd(cbl2dMul(lhs->x, cbl2dSplatX(*rhs)),
                    cbl2dMul(lhs->y, cbl2dSplatY(*rhs)));
}

CBL_INLINE void cblMat2dAdd(const CblMat2d *lhs, const CblMat2d *rhs, CblMat2d *out) {
    *out = (CblMat2d) {
            cbl2dAdd(lhs->x, rhs->x),
            cbl2dAdd(lhs->y, rhs->y)
    };
}

CBL_INLINE void cblMat2dSub(const CblMat2d *lhs, const CblMat2d *rhs, CblMat2d *out) {
    *out = (CblMat2d) {
            cbl2dSub(lhs->x, rhs->x),
            cbl2dSub(lhs->y, rhs->y),
    };
}

CBL_INLINE void cblMat2dMul(const CblMat2d *lhs, const CblMat2d *rhs, CblMat2d *out) {
    *out = (CblMat2d) {
            cbl2dMul(lhs->x, rhs->x),
            cbl2dMul(lhs->y, rhs->y)
    };
}

CBL_INLINE void cblMat2dDiv(const CblMat2d *lhs, const CblMat2d *rhs, CblMat2d *out) {
    *out = (CblMat2d) {
            cbl2dDiv(lhs->x, rhs->x),
            cbl2dDiv(lhs->y, rhs->y)
    };
}

CBL_INLINE void cblMat2dScale(CblMat2d *m4f, double x, double y) {
    *m4f = (CblMat2d) {
            cbl2dNew(x, 0.0),
            cbl2dNew(0.0, y)
    };
}

CBL_INLINE void cblMat2dRotate(CblMat2d *m4f, double radians) {
    const double s = sin(radians);
    const double c = cos(radians);
    *m4f = (CblMat2d) {
            cbl2dNew(c, s),
            cbl2dNew(-s, c)
    };
}

CBL_INLINE void cblMat2dShear(CblMat2d *m4f, double x, double y) {
    *m4f = (CblMat2d) {
            cbl2dNew(1.0, x),
            cbl2dNew(y, 1.0)
    };
}

CBL_EXTERN_END

#endif // CBL_MATH_MAT4F_H_