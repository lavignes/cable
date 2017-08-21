#ifndef CBL_MATH_MAT4F_H_
#define CBL_MATH_MAT4F_H_

#include <cable/math/4f.h>

CBL_EXTERN_BEGIN

typedef struct CblMat4f {
    Cbl4f x, y, z, w;
} CblMat4f;

static inline CblMat4f cblMat4fNew(Cbl4f x, Cbl4f y, Cbl4f z, Cbl4f w) {
    return (CblMat4f) {x, y, z, w};
}

static inline void cblMat4fIdentity(CblMat4f *m4f) {
    *m4f = (CblMat4f) {
            cbl4fNew(1.0f, 0.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, 1.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, 1.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, 0.0f, 1.0f)
    };
}

static inline void cblMat4fMul4f(const CblMat4f *lhs, const Cbl4f *rhs, Cbl4f *out) {
    *out = cbl4fAdd(cbl4fMul(lhs->x, cbl4fSplatX(*rhs)),
                    cbl4fAdd(cbl4fMul(lhs->y, cbl4fSplatY(*rhs)),
                             cbl4fAdd(cbl4fMul(lhs->z, cbl4fSplatZ(*rhs)),
                                      cbl4fMul(lhs->w, cbl4fSplatW(*rhs)))));
}

static inline void cblMat4fAdd(const CblMat4f *lhs, const CblMat4f *rhs, CblMat4f *out) {
    *out = (CblMat4f) {
            cbl4fAdd(lhs->x, rhs->x),
            cbl4fAdd(lhs->y, rhs->y),
            cbl4fAdd(lhs->z, rhs->z),
            cbl4fAdd(lhs->w, rhs->w)
    };
}

static inline void cblMat4fSub(const CblMat4f *lhs, const CblMat4f *rhs, CblMat4f *out) {
    *out = (CblMat4f) {
            cbl4fSub(lhs->x, rhs->x),
            cbl4fSub(lhs->y, rhs->y),
            cbl4fSub(lhs->z, rhs->z),
            cbl4fSub(lhs->w, rhs->w)
    };
}

static inline void cblMat4fMul(const CblMat4f *lhs, const CblMat4f *rhs, CblMat4f *out) {
    *out = (CblMat4f) {
            cbl4fMul(lhs->x, rhs->x),
            cbl4fMul(lhs->y, rhs->y),
            cbl4fMul(lhs->z, rhs->z),
            cbl4fMul(lhs->w, rhs->w)
    };
}

static inline void cblMat4fDiv(const CblMat4f *lhs, const CblMat4f *rhs, CblMat4f *out) {
    *out = (CblMat4f) {
            cbl4fDiv(lhs->x, rhs->x),
            cbl4fDiv(lhs->y, rhs->y),
            cbl4fDiv(lhs->z, rhs->z),
            cbl4fDiv(lhs->w, rhs->w)
    };
}

static inline void cblMat4fPerspective(CblMat4f *m4f, float fovy, float aspect, float znear, float zfar) {
    const float tau = 6.28318530717958647692f;
    const float dz = zfar - znear;
    const float cot = tanf(tau - fovy * 0.5f);
    *m4f = (CblMat4f) {
            cbl4fNew(cot / aspect, 0.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, cot, 0.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, -(zfar + znear) / dz, -1.0f),
            cbl4fNew(0.0f, 0.0f, -2.0f * znear * zfar / dz, 0.0f)
    };
}

static inline void cblMat4fOrtho(CblMat4f *m4f, float left, float right,
                                 float bottom, float top, float znear, float zfar) {
    const float dx = right - left;
    const float dy = top - bottom;
    const float dz = zfar - znear;
    *m4f = (CblMat4f) {
            cbl4fNew(2.0f / dx, 0.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, 2.0f / dy, 0.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, -2.0f / dz, 0.0f),
            cbl4fNew(-(right + left) / dx, -(top + bottom) / dy,
                    -(zfar + znear) / dz, 1.0f)
    };
}

static inline void cblMat4fTranslation(CblMat4f *m4f, float x, float y, float z) {
    *m4f = (CblMat4f) {
            cbl4fNew(1.0f, 0.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, 1.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, 1.0f, 0.0f),
            cbl4fNew(x, y, z, 1.0f)
    };
}

static inline void cblMat4fScale(CblMat4f *m4f, float x, float y, float z) {
    *m4f = (CblMat4f) {
            cbl4fNew(x, 0.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, y, 0.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, z, 0.0f),
            cbl4fNew(0.0f, 0.0f, 0.0f, 1.0f)
    };
}

static inline void cblMat4fRotateX(CblMat4f *m4f, float radians) {
    const float s = sinf(radians);
    const float c = cosf(radians);
    *m4f = (CblMat4f) {
            cbl4fNew(1.0f, 0.0f, 0.0f, 0.0f),
            cbl4fNew(0.0f, c, -s, 0.0f),
            cbl4fNew(0.0f, s, c, 0.0f),
            cbl4fNew(0.0f, 0.0f, 0.0f, 1.0f)
    };
}

static inline void cblMat4fRotateY(CblMat4f *m4f, float radians) {
    const float s = sinf(radians);
    const float c = cosf(radians);
    *m4f = (CblMat4f) {
            cbl4fNew(c, 0.0f, s, 0.0f),
            cbl4fNew(0.0f, 1.0f, 0.0f, 0.0f),
            cbl4fNew(-s, 0.0f, c, 0.0f),
            cbl4fNew(0.0f, 0.0f, 0.0f, 1.0f)
    };
}

static inline void cblMat4fRotateZ(CblMat4f *m4f, float radians) {
    const float s = sinf(radians);
    const float c = cosf(radians);
    *m4f = (CblMat4f) {
            cbl4fNew(c, -s, 0.0f, 0.0f),
            cbl4fNew(s, c, 0.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, 1.0f, 0.0f),
            cbl4fNew(0.0f, 0.0f, 0.0f, 1.0f)
    };
}

static inline void cblMat4fTranspose(CblMat4f *m4f) {
    const Cbl4f dx = cbl4fNew(cbl4fGetX(m4f->x), cbl4fGetX(m4f->y),
                              cbl4fGetX(m4f->z), cbl4fGetX(m4f->w));
    const Cbl4f dy = cbl4fNew(cbl4fGetY(m4f->x), cbl4fGetY(m4f->y),
                              cbl4fGetY(m4f->z), cbl4fGetY(m4f->w));
    const Cbl4f dz = cbl4fNew(cbl4fGetZ(m4f->x), cbl4fGetZ(m4f->y),
                              cbl4fGetZ(m4f->z), cbl4fGetZ(m4f->w));
    const Cbl4f dw = cbl4fNew(cbl4fGetW(m4f->x), cbl4fGetW(m4f->y),
                              cbl4fGetW(m4f->z), cbl4fGetW(m4f->w));
    *m4f = (CblMat4f) {dx, dy, dz, dw};
}

CBL_EXTERN_END

#endif // CBL_MATH_MAT4F_H_