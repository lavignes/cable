#ifndef CBL_MATH_2D_H_
#define CBL_MATH_2D_H_

#ifdef __SSE__
#include <cable/math/simd/2d.h>
#else
#include <cable/math/scalar/2d.h>
#endif

CBL_EXTERN_BEGIN

CBL_EXTERN_END

#endif // CBL_MATH_2D_H_