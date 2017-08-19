#ifndef CBL_MATH_SIMD_4F_H_
#define CBL_MATH_SIMD_4F_H_

#include <xmmintrin.h>
#include <math.h>

#include <cable/porting.h>

CBL_EXTERN_BEGIN

typedef enum Cbl4fSwizzle {
    CBL_4F_SWIZZLE_XXXX = _MM_SHUFFLE(0, 0, 0, 0),
    CBL_4F_SWIZZLE_XXXY = _MM_SHUFFLE(0, 0, 0, 1),
    CBL_4F_SWIZZLE_XXXZ = _MM_SHUFFLE(0, 0, 0, 2),
    CBL_4F_SWIZZLE_XXXW = _MM_SHUFFLE(0, 0, 0, 3),
    CBL_4F_SWIZZLE_XXYX = _MM_SHUFFLE(0, 0, 1, 0),
    CBL_4F_SWIZZLE_XXYY = _MM_SHUFFLE(0, 0, 1, 1),
    CBL_4F_SWIZZLE_XXYZ = _MM_SHUFFLE(0, 0, 1, 2),
    CBL_4F_SWIZZLE_XXYW = _MM_SHUFFLE(0, 0, 1, 3),
    CBL_4F_SWIZZLE_XXZX = _MM_SHUFFLE(0, 0, 2, 0),
    CBL_4F_SWIZZLE_XXZY = _MM_SHUFFLE(0, 0, 2, 1),
    CBL_4F_SWIZZLE_XXZZ = _MM_SHUFFLE(0, 0, 2, 2),
    CBL_4F_SWIZZLE_XXZW = _MM_SHUFFLE(0, 0, 2, 3),
    CBL_4F_SWIZZLE_XXWX = _MM_SHUFFLE(0, 0, 3, 0),
    CBL_4F_SWIZZLE_XXWY = _MM_SHUFFLE(0, 0, 3, 1),
    CBL_4F_SWIZZLE_XXWZ = _MM_SHUFFLE(0, 0, 3, 2),
    CBL_4F_SWIZZLE_XXWW = _MM_SHUFFLE(0, 0, 3, 3),
    CBL_4F_SWIZZLE_XYXX = _MM_SHUFFLE(0, 1, 0, 0),
    CBL_4F_SWIZZLE_XYXY = _MM_SHUFFLE(0, 1, 0, 1),
    CBL_4F_SWIZZLE_XYXZ = _MM_SHUFFLE(0, 1, 0, 2),
    CBL_4F_SWIZZLE_XYXW = _MM_SHUFFLE(0, 1, 0, 3),
    CBL_4F_SWIZZLE_XYYX = _MM_SHUFFLE(0, 1, 1, 0),
    CBL_4F_SWIZZLE_XYYY = _MM_SHUFFLE(0, 1, 1, 1),
    CBL_4F_SWIZZLE_XYYZ = _MM_SHUFFLE(0, 1, 1, 2),
    CBL_4F_SWIZZLE_XYYW = _MM_SHUFFLE(0, 1, 1, 3),
    CBL_4F_SWIZZLE_XYZX = _MM_SHUFFLE(0, 1, 2, 0),
    CBL_4F_SWIZZLE_XYZY = _MM_SHUFFLE(0, 1, 2, 1),
    CBL_4F_SWIZZLE_XYZZ = _MM_SHUFFLE(0, 1, 2, 2),
    CBL_4F_SWIZZLE_XYZW = _MM_SHUFFLE(0, 1, 2, 3),
    CBL_4F_SWIZZLE_XYWX = _MM_SHUFFLE(0, 1, 3, 0),
    CBL_4F_SWIZZLE_XYWY = _MM_SHUFFLE(0, 1, 3, 1),
    CBL_4F_SWIZZLE_XYWZ = _MM_SHUFFLE(0, 1, 3, 2),
    CBL_4F_SWIZZLE_XYWW = _MM_SHUFFLE(0, 1, 3, 3),
    CBL_4F_SWIZZLE_XZXX = _MM_SHUFFLE(0, 2, 0, 0),
    CBL_4F_SWIZZLE_XZXY = _MM_SHUFFLE(0, 2, 0, 1),
    CBL_4F_SWIZZLE_XZXZ = _MM_SHUFFLE(0, 2, 0, 2),
    CBL_4F_SWIZZLE_XZXW = _MM_SHUFFLE(0, 2, 0, 3),
    CBL_4F_SWIZZLE_XZYX = _MM_SHUFFLE(0, 2, 1, 0),
    CBL_4F_SWIZZLE_XZYY = _MM_SHUFFLE(0, 2, 1, 1),
    CBL_4F_SWIZZLE_XZYZ = _MM_SHUFFLE(0, 2, 1, 2),
    CBL_4F_SWIZZLE_XZYW = _MM_SHUFFLE(0, 2, 1, 3),
    CBL_4F_SWIZZLE_XZZX = _MM_SHUFFLE(0, 2, 2, 0),
    CBL_4F_SWIZZLE_XZZY = _MM_SHUFFLE(0, 2, 2, 1),
    CBL_4F_SWIZZLE_XZZZ = _MM_SHUFFLE(0, 2, 2, 2),
    CBL_4F_SWIZZLE_XZZW = _MM_SHUFFLE(0, 2, 2, 3),
    CBL_4F_SWIZZLE_XZWX = _MM_SHUFFLE(0, 2, 3, 0),
    CBL_4F_SWIZZLE_XZWY = _MM_SHUFFLE(0, 2, 3, 1),
    CBL_4F_SWIZZLE_XZWZ = _MM_SHUFFLE(0, 2, 3, 2),
    CBL_4F_SWIZZLE_XZWW = _MM_SHUFFLE(0, 2, 3, 3),
    CBL_4F_SWIZZLE_XWXX = _MM_SHUFFLE(0, 3, 0, 0),
    CBL_4F_SWIZZLE_XWXY = _MM_SHUFFLE(0, 3, 0, 1),
    CBL_4F_SWIZZLE_XWXZ = _MM_SHUFFLE(0, 3, 0, 2),
    CBL_4F_SWIZZLE_XWXW = _MM_SHUFFLE(0, 3, 0, 3),
    CBL_4F_SWIZZLE_XWYX = _MM_SHUFFLE(0, 3, 1, 0),
    CBL_4F_SWIZZLE_XWYY = _MM_SHUFFLE(0, 3, 1, 1),
    CBL_4F_SWIZZLE_XWYZ = _MM_SHUFFLE(0, 3, 1, 2),
    CBL_4F_SWIZZLE_XWYW = _MM_SHUFFLE(0, 3, 1, 3),
    CBL_4F_SWIZZLE_XWZX = _MM_SHUFFLE(0, 3, 2, 0),
    CBL_4F_SWIZZLE_XWZY = _MM_SHUFFLE(0, 3, 2, 1),
    CBL_4F_SWIZZLE_XWZZ = _MM_SHUFFLE(0, 3, 2, 2),
    CBL_4F_SWIZZLE_XWZW = _MM_SHUFFLE(0, 3, 2, 3),
    CBL_4F_SWIZZLE_XWWX = _MM_SHUFFLE(0, 3, 3, 0),
    CBL_4F_SWIZZLE_XWWY = _MM_SHUFFLE(0, 3, 3, 1),
    CBL_4F_SWIZZLE_XWWZ = _MM_SHUFFLE(0, 3, 3, 2),
    CBL_4F_SWIZZLE_XWWW = _MM_SHUFFLE(0, 3, 3, 3),
    CBL_4F_SWIZZLE_YXXX = _MM_SHUFFLE(1, 0, 0, 0),
    CBL_4F_SWIZZLE_YXXY = _MM_SHUFFLE(1, 0, 0, 1),
    CBL_4F_SWIZZLE_YXXZ = _MM_SHUFFLE(1, 0, 0, 2),
    CBL_4F_SWIZZLE_YXXW = _MM_SHUFFLE(1, 0, 0, 3),
    CBL_4F_SWIZZLE_YXYX = _MM_SHUFFLE(1, 0, 1, 0),
    CBL_4F_SWIZZLE_YXYY = _MM_SHUFFLE(1, 0, 1, 1),
    CBL_4F_SWIZZLE_YXYZ = _MM_SHUFFLE(1, 0, 1, 2),
    CBL_4F_SWIZZLE_YXYW = _MM_SHUFFLE(1, 0, 1, 3),
    CBL_4F_SWIZZLE_YXZX = _MM_SHUFFLE(1, 0, 2, 0),
    CBL_4F_SWIZZLE_YXZY = _MM_SHUFFLE(1, 0, 2, 1),
    CBL_4F_SWIZZLE_YXZZ = _MM_SHUFFLE(1, 0, 2, 2),
    CBL_4F_SWIZZLE_YXZW = _MM_SHUFFLE(1, 0, 2, 3),
    CBL_4F_SWIZZLE_YXWX = _MM_SHUFFLE(1, 0, 3, 0),
    CBL_4F_SWIZZLE_YXWY = _MM_SHUFFLE(1, 0, 3, 1),
    CBL_4F_SWIZZLE_YXWZ = _MM_SHUFFLE(1, 0, 3, 2),
    CBL_4F_SWIZZLE_YXWW = _MM_SHUFFLE(1, 0, 3, 3),
    CBL_4F_SWIZZLE_YYXX = _MM_SHUFFLE(1, 1, 0, 0),
    CBL_4F_SWIZZLE_YYXY = _MM_SHUFFLE(1, 1, 0, 1),
    CBL_4F_SWIZZLE_YYXZ = _MM_SHUFFLE(1, 1, 0, 2),
    CBL_4F_SWIZZLE_YYXW = _MM_SHUFFLE(1, 1, 0, 3),
    CBL_4F_SWIZZLE_YYYX = _MM_SHUFFLE(1, 1, 1, 0),
    CBL_4F_SWIZZLE_YYYY = _MM_SHUFFLE(1, 1, 1, 1),
    CBL_4F_SWIZZLE_YYYZ = _MM_SHUFFLE(1, 1, 1, 2),
    CBL_4F_SWIZZLE_YYYW = _MM_SHUFFLE(1, 1, 1, 3),
    CBL_4F_SWIZZLE_YYZX = _MM_SHUFFLE(1, 1, 2, 0),
    CBL_4F_SWIZZLE_YYZY = _MM_SHUFFLE(1, 1, 2, 1),
    CBL_4F_SWIZZLE_YYZZ = _MM_SHUFFLE(1, 1, 2, 2),
    CBL_4F_SWIZZLE_YYZW = _MM_SHUFFLE(1, 1, 2, 3),
    CBL_4F_SWIZZLE_YYWX = _MM_SHUFFLE(1, 1, 3, 0),
    CBL_4F_SWIZZLE_YYWY = _MM_SHUFFLE(1, 1, 3, 1),
    CBL_4F_SWIZZLE_YYWZ = _MM_SHUFFLE(1, 1, 3, 2),
    CBL_4F_SWIZZLE_YYWW = _MM_SHUFFLE(1, 1, 3, 3),
    CBL_4F_SWIZZLE_YZXX = _MM_SHUFFLE(1, 2, 0, 0),
    CBL_4F_SWIZZLE_YZXY = _MM_SHUFFLE(1, 2, 0, 1),
    CBL_4F_SWIZZLE_YZXZ = _MM_SHUFFLE(1, 2, 0, 2),
    CBL_4F_SWIZZLE_YZXW = _MM_SHUFFLE(1, 2, 0, 3),
    CBL_4F_SWIZZLE_YZYX = _MM_SHUFFLE(1, 2, 1, 0),
    CBL_4F_SWIZZLE_YZYY = _MM_SHUFFLE(1, 2, 1, 1),
    CBL_4F_SWIZZLE_YZYZ = _MM_SHUFFLE(1, 2, 1, 2),
    CBL_4F_SWIZZLE_YZYW = _MM_SHUFFLE(1, 2, 1, 3),
    CBL_4F_SWIZZLE_YZZX = _MM_SHUFFLE(1, 2, 2, 0),
    CBL_4F_SWIZZLE_YZZY = _MM_SHUFFLE(1, 2, 2, 1),
    CBL_4F_SWIZZLE_YZZZ = _MM_SHUFFLE(1, 2, 2, 2),
    CBL_4F_SWIZZLE_YZZW = _MM_SHUFFLE(1, 2, 2, 3),
    CBL_4F_SWIZZLE_YZWX = _MM_SHUFFLE(1, 2, 3, 0),
    CBL_4F_SWIZZLE_YZWY = _MM_SHUFFLE(1, 2, 3, 1),
    CBL_4F_SWIZZLE_YZWZ = _MM_SHUFFLE(1, 2, 3, 2),
    CBL_4F_SWIZZLE_YZWW = _MM_SHUFFLE(1, 2, 3, 3),
    CBL_4F_SWIZZLE_YWXX = _MM_SHUFFLE(1, 3, 0, 0),
    CBL_4F_SWIZZLE_YWXY = _MM_SHUFFLE(1, 3, 0, 1),
    CBL_4F_SWIZZLE_YWXZ = _MM_SHUFFLE(1, 3, 0, 2),
    CBL_4F_SWIZZLE_YWXW = _MM_SHUFFLE(1, 3, 0, 3),
    CBL_4F_SWIZZLE_YWYX = _MM_SHUFFLE(1, 3, 1, 0),
    CBL_4F_SWIZZLE_YWYY = _MM_SHUFFLE(1, 3, 1, 1),
    CBL_4F_SWIZZLE_YWYZ = _MM_SHUFFLE(1, 3, 1, 2),
    CBL_4F_SWIZZLE_YWYW = _MM_SHUFFLE(1, 3, 1, 3),
    CBL_4F_SWIZZLE_YWZX = _MM_SHUFFLE(1, 3, 2, 0),
    CBL_4F_SWIZZLE_YWZY = _MM_SHUFFLE(1, 3, 2, 1),
    CBL_4F_SWIZZLE_YWZZ = _MM_SHUFFLE(1, 3, 2, 2),
    CBL_4F_SWIZZLE_YWZW = _MM_SHUFFLE(1, 3, 2, 3),
    CBL_4F_SWIZZLE_YWWX = _MM_SHUFFLE(1, 3, 3, 0),
    CBL_4F_SWIZZLE_YWWY = _MM_SHUFFLE(1, 3, 3, 1),
    CBL_4F_SWIZZLE_YWWZ = _MM_SHUFFLE(1, 3, 3, 2),
    CBL_4F_SWIZZLE_YWWW = _MM_SHUFFLE(1, 3, 3, 3),
    CBL_4F_SWIZZLE_ZXXX = _MM_SHUFFLE(2, 0, 0, 0),
    CBL_4F_SWIZZLE_ZXXY = _MM_SHUFFLE(2, 0, 0, 1),
    CBL_4F_SWIZZLE_ZXXZ = _MM_SHUFFLE(2, 0, 0, 2),
    CBL_4F_SWIZZLE_ZXXW = _MM_SHUFFLE(2, 0, 0, 3),
    CBL_4F_SWIZZLE_ZXYX = _MM_SHUFFLE(2, 0, 1, 0),
    CBL_4F_SWIZZLE_ZXYY = _MM_SHUFFLE(2, 0, 1, 1),
    CBL_4F_SWIZZLE_ZXYZ = _MM_SHUFFLE(2, 0, 1, 2),
    CBL_4F_SWIZZLE_ZXYW = _MM_SHUFFLE(2, 0, 1, 3),
    CBL_4F_SWIZZLE_ZXZX = _MM_SHUFFLE(2, 0, 2, 0),
    CBL_4F_SWIZZLE_ZXZY = _MM_SHUFFLE(2, 0, 2, 1),
    CBL_4F_SWIZZLE_ZXZZ = _MM_SHUFFLE(2, 0, 2, 2),
    CBL_4F_SWIZZLE_ZXZW = _MM_SHUFFLE(2, 0, 2, 3),
    CBL_4F_SWIZZLE_ZXWX = _MM_SHUFFLE(2, 0, 3, 0),
    CBL_4F_SWIZZLE_ZXWY = _MM_SHUFFLE(2, 0, 3, 1),
    CBL_4F_SWIZZLE_ZXWZ = _MM_SHUFFLE(2, 0, 3, 2),
    CBL_4F_SWIZZLE_ZXWW = _MM_SHUFFLE(2, 0, 3, 3),
    CBL_4F_SWIZZLE_ZYXX = _MM_SHUFFLE(2, 1, 0, 0),
    CBL_4F_SWIZZLE_ZYXY = _MM_SHUFFLE(2, 1, 0, 1),
    CBL_4F_SWIZZLE_ZYXZ = _MM_SHUFFLE(2, 1, 0, 2),
    CBL_4F_SWIZZLE_ZYXW = _MM_SHUFFLE(2, 1, 0, 3),
    CBL_4F_SWIZZLE_ZYYX = _MM_SHUFFLE(2, 1, 1, 0),
    CBL_4F_SWIZZLE_ZYYY = _MM_SHUFFLE(2, 1, 1, 1),
    CBL_4F_SWIZZLE_ZYYZ = _MM_SHUFFLE(2, 1, 1, 2),
    CBL_4F_SWIZZLE_ZYYW = _MM_SHUFFLE(2, 1, 1, 3),
    CBL_4F_SWIZZLE_ZYZX = _MM_SHUFFLE(2, 1, 2, 0),
    CBL_4F_SWIZZLE_ZYZY = _MM_SHUFFLE(2, 1, 2, 1),
    CBL_4F_SWIZZLE_ZYZZ = _MM_SHUFFLE(2, 1, 2, 2),
    CBL_4F_SWIZZLE_ZYZW = _MM_SHUFFLE(2, 1, 2, 3),
    CBL_4F_SWIZZLE_ZYWX = _MM_SHUFFLE(2, 1, 3, 0),
    CBL_4F_SWIZZLE_ZYWY = _MM_SHUFFLE(2, 1, 3, 1),
    CBL_4F_SWIZZLE_ZYWZ = _MM_SHUFFLE(2, 1, 3, 2),
    CBL_4F_SWIZZLE_ZYWW = _MM_SHUFFLE(2, 1, 3, 3),
    CBL_4F_SWIZZLE_ZZXX = _MM_SHUFFLE(2, 2, 0, 0),
    CBL_4F_SWIZZLE_ZZXY = _MM_SHUFFLE(2, 2, 0, 1),
    CBL_4F_SWIZZLE_ZZXZ = _MM_SHUFFLE(2, 2, 0, 2),
    CBL_4F_SWIZZLE_ZZXW = _MM_SHUFFLE(2, 2, 0, 3),
    CBL_4F_SWIZZLE_ZZYX = _MM_SHUFFLE(2, 2, 1, 0),
    CBL_4F_SWIZZLE_ZZYY = _MM_SHUFFLE(2, 2, 1, 1),
    CBL_4F_SWIZZLE_ZZYZ = _MM_SHUFFLE(2, 2, 1, 2),
    CBL_4F_SWIZZLE_ZZYW = _MM_SHUFFLE(2, 2, 1, 3),
    CBL_4F_SWIZZLE_ZZZX = _MM_SHUFFLE(2, 2, 2, 0),
    CBL_4F_SWIZZLE_ZZZY = _MM_SHUFFLE(2, 2, 2, 1),
    CBL_4F_SWIZZLE_ZZZZ = _MM_SHUFFLE(2, 2, 2, 2),
    CBL_4F_SWIZZLE_ZZZW = _MM_SHUFFLE(2, 2, 2, 3),
    CBL_4F_SWIZZLE_ZZWX = _MM_SHUFFLE(2, 2, 3, 0),
    CBL_4F_SWIZZLE_ZZWY = _MM_SHUFFLE(2, 2, 3, 1),
    CBL_4F_SWIZZLE_ZZWZ = _MM_SHUFFLE(2, 2, 3, 2),
    CBL_4F_SWIZZLE_ZZWW = _MM_SHUFFLE(2, 2, 3, 3),
    CBL_4F_SWIZZLE_ZWXX = _MM_SHUFFLE(2, 3, 0, 0),
    CBL_4F_SWIZZLE_ZWXY = _MM_SHUFFLE(2, 3, 0, 1),
    CBL_4F_SWIZZLE_ZWXZ = _MM_SHUFFLE(2, 3, 0, 2),
    CBL_4F_SWIZZLE_ZWXW = _MM_SHUFFLE(2, 3, 0, 3),
    CBL_4F_SWIZZLE_ZWYX = _MM_SHUFFLE(2, 3, 1, 0),
    CBL_4F_SWIZZLE_ZWYY = _MM_SHUFFLE(2, 3, 1, 1),
    CBL_4F_SWIZZLE_ZWYZ = _MM_SHUFFLE(2, 3, 1, 2),
    CBL_4F_SWIZZLE_ZWYW = _MM_SHUFFLE(2, 3, 1, 3),
    CBL_4F_SWIZZLE_ZWZX = _MM_SHUFFLE(2, 3, 2, 0),
    CBL_4F_SWIZZLE_ZWZY = _MM_SHUFFLE(2, 3, 2, 1),
    CBL_4F_SWIZZLE_ZWZZ = _MM_SHUFFLE(2, 3, 2, 2),
    CBL_4F_SWIZZLE_ZWZW = _MM_SHUFFLE(2, 3, 2, 3),
    CBL_4F_SWIZZLE_ZWWX = _MM_SHUFFLE(2, 3, 3, 0),
    CBL_4F_SWIZZLE_ZWWY = _MM_SHUFFLE(2, 3, 3, 1),
    CBL_4F_SWIZZLE_ZWWZ = _MM_SHUFFLE(2, 3, 3, 2),
    CBL_4F_SWIZZLE_ZWWW = _MM_SHUFFLE(2, 3, 3, 3),
    CBL_4F_SWIZZLE_WXXX = _MM_SHUFFLE(3, 0, 0, 0),
    CBL_4F_SWIZZLE_WXXY = _MM_SHUFFLE(3, 0, 0, 1),
    CBL_4F_SWIZZLE_WXXZ = _MM_SHUFFLE(3, 0, 0, 2),
    CBL_4F_SWIZZLE_WXXW = _MM_SHUFFLE(3, 0, 0, 3),
    CBL_4F_SWIZZLE_WXYX = _MM_SHUFFLE(3, 0, 1, 0),
    CBL_4F_SWIZZLE_WXYY = _MM_SHUFFLE(3, 0, 1, 1),
    CBL_4F_SWIZZLE_WXYZ = _MM_SHUFFLE(3, 0, 1, 2),
    CBL_4F_SWIZZLE_WXYW = _MM_SHUFFLE(3, 0, 1, 3),
    CBL_4F_SWIZZLE_WXZX = _MM_SHUFFLE(3, 0, 2, 0),
    CBL_4F_SWIZZLE_WXZY = _MM_SHUFFLE(3, 0, 2, 1),
    CBL_4F_SWIZZLE_WXZZ = _MM_SHUFFLE(3, 0, 2, 2),
    CBL_4F_SWIZZLE_WXZW = _MM_SHUFFLE(3, 0, 2, 3),
    CBL_4F_SWIZZLE_WXWX = _MM_SHUFFLE(3, 0, 3, 0),
    CBL_4F_SWIZZLE_WXWY = _MM_SHUFFLE(3, 0, 3, 1),
    CBL_4F_SWIZZLE_WXWZ = _MM_SHUFFLE(3, 0, 3, 2),
    CBL_4F_SWIZZLE_WXWW = _MM_SHUFFLE(3, 0, 3, 3),
    CBL_4F_SWIZZLE_WYXX = _MM_SHUFFLE(3, 1, 0, 0),
    CBL_4F_SWIZZLE_WYXY = _MM_SHUFFLE(3, 1, 0, 1),
    CBL_4F_SWIZZLE_WYXZ = _MM_SHUFFLE(3, 1, 0, 2),
    CBL_4F_SWIZZLE_WYXW = _MM_SHUFFLE(3, 1, 0, 3),
    CBL_4F_SWIZZLE_WYYX = _MM_SHUFFLE(3, 1, 1, 0),
    CBL_4F_SWIZZLE_WYYY = _MM_SHUFFLE(3, 1, 1, 1),
    CBL_4F_SWIZZLE_WYYZ = _MM_SHUFFLE(3, 1, 1, 2),
    CBL_4F_SWIZZLE_WYYW = _MM_SHUFFLE(3, 1, 1, 3),
    CBL_4F_SWIZZLE_WYZX = _MM_SHUFFLE(3, 1, 2, 0),
    CBL_4F_SWIZZLE_WYZY = _MM_SHUFFLE(3, 1, 2, 1),
    CBL_4F_SWIZZLE_WYZZ = _MM_SHUFFLE(3, 1, 2, 2),
    CBL_4F_SWIZZLE_WYZW = _MM_SHUFFLE(3, 1, 2, 3),
    CBL_4F_SWIZZLE_WYWX = _MM_SHUFFLE(3, 1, 3, 0),
    CBL_4F_SWIZZLE_WYWY = _MM_SHUFFLE(3, 1, 3, 1),
    CBL_4F_SWIZZLE_WYWZ = _MM_SHUFFLE(3, 1, 3, 2),
    CBL_4F_SWIZZLE_WYWW = _MM_SHUFFLE(3, 1, 3, 3),
    CBL_4F_SWIZZLE_WZXX = _MM_SHUFFLE(3, 2, 0, 0),
    CBL_4F_SWIZZLE_WZXY = _MM_SHUFFLE(3, 2, 0, 1),
    CBL_4F_SWIZZLE_WZXZ = _MM_SHUFFLE(3, 2, 0, 2),
    CBL_4F_SWIZZLE_WZXW = _MM_SHUFFLE(3, 2, 0, 3),
    CBL_4F_SWIZZLE_WZYX = _MM_SHUFFLE(3, 2, 1, 0),
    CBL_4F_SWIZZLE_WZYY = _MM_SHUFFLE(3, 2, 1, 1),
    CBL_4F_SWIZZLE_WZYZ = _MM_SHUFFLE(3, 2, 1, 2),
    CBL_4F_SWIZZLE_WZYW = _MM_SHUFFLE(3, 2, 1, 3),
    CBL_4F_SWIZZLE_WZZX = _MM_SHUFFLE(3, 2, 2, 0),
    CBL_4F_SWIZZLE_WZZY = _MM_SHUFFLE(3, 2, 2, 1),
    CBL_4F_SWIZZLE_WZZZ = _MM_SHUFFLE(3, 2, 2, 2),
    CBL_4F_SWIZZLE_WZZW = _MM_SHUFFLE(3, 2, 2, 3),
    CBL_4F_SWIZZLE_WZWX = _MM_SHUFFLE(3, 2, 3, 0),
    CBL_4F_SWIZZLE_WZWY = _MM_SHUFFLE(3, 2, 3, 1),
    CBL_4F_SWIZZLE_WZWZ = _MM_SHUFFLE(3, 2, 3, 2),
    CBL_4F_SWIZZLE_WZWW = _MM_SHUFFLE(3, 2, 3, 3),
    CBL_4F_SWIZZLE_WWXX = _MM_SHUFFLE(3, 3, 0, 0),
    CBL_4F_SWIZZLE_WWXY = _MM_SHUFFLE(3, 3, 0, 1),
    CBL_4F_SWIZZLE_WWXZ = _MM_SHUFFLE(3, 3, 0, 2),
    CBL_4F_SWIZZLE_WWXW = _MM_SHUFFLE(3, 3, 0, 3),
    CBL_4F_SWIZZLE_WWYX = _MM_SHUFFLE(3, 3, 1, 0),
    CBL_4F_SWIZZLE_WWYY = _MM_SHUFFLE(3, 3, 1, 1),
    CBL_4F_SWIZZLE_WWYZ = _MM_SHUFFLE(3, 3, 1, 2),
    CBL_4F_SWIZZLE_WWYW = _MM_SHUFFLE(3, 3, 1, 3),
    CBL_4F_SWIZZLE_WWZX = _MM_SHUFFLE(3, 3, 2, 0),
    CBL_4F_SWIZZLE_WWZY = _MM_SHUFFLE(3, 3, 2, 1),
    CBL_4F_SWIZZLE_WWZZ = _MM_SHUFFLE(3, 3, 2, 2),
    CBL_4F_SWIZZLE_WWZW = _MM_SHUFFLE(3, 3, 2, 3),
    CBL_4F_SWIZZLE_WWWX = _MM_SHUFFLE(3, 3, 3, 0),
    CBL_4F_SWIZZLE_WWWY = _MM_SHUFFLE(3, 3, 3, 1),
    CBL_4F_SWIZZLE_WWWZ = _MM_SHUFFLE(3, 3, 3, 2),
    CBL_4F_SWIZZLE_WWWW = _MM_SHUFFLE(3, 3, 3, 3),
} Cbl4fSwizzle;

typedef __m128 Cbl4f;

typedef union {
    Cbl4f v4f_;
    float arr_[4];
    struct {
        float x_, y_, z_, w_;
    };
} Cbl4fSIMDUnion_;

CBL_INLINE Cbl4f cbl4fNew(float x, float y, float z, float w) {
    return _mm_setr_ps(x, y, z, w);
}

CBL_INLINE Cbl4f cbl4fZero() {
    return _mm_setzero_ps();
}

CBL_INLINE bool cbl4fEqual(Cbl4f lhs, Cbl4f rhs) {
    return 0x0F == _mm_movemask_ps(_mm_cmpeq_ps(lhs, rhs));
}

CBL_INLINE bool cbl4fLessThan(Cbl4f lhs, Cbl4f rhs) {
    return 0x0F == _mm_movemask_ps(_mm_cmplt_ps(lhs, rhs));
}

CBL_INLINE bool cbl4fGreaterThan(Cbl4f lhs, Cbl4f rhs) {
    return 0x0F == _mm_movemask_ps(_mm_cmpgt_ps(lhs, rhs));
}

CBL_INLINE Cbl4f cbl4fMap(Cbl4f v4f, float (*fn)(float)) {
    Cbl4fSIMDUnion_ u = {v4f};
    return cbl4fNew(fn(u.x_), fn(u.y_), fn(u.z_), fn(u.w_));
}

CBL_INLINE Cbl4f cbl4fLoad(const float *arr) {
    return _mm_load_ps(arr);
}

CBL_INLINE void cbl4fStore(Cbl4f v4f, float *arr) {
    _mm_storeu_ps(arr, v4f);
}

CBL_INLINE float cbl4fGetX(Cbl4f v4f) {
    Cbl4fSIMDUnion_ u = {v4f};
    return u.x_;
}

CBL_INLINE float cbl4fGetY(Cbl4f v4f) {
    Cbl4fSIMDUnion_ u = {v4f};
    return u.y_;
}

CBL_INLINE float cbl4fGetZ(Cbl4f v4f) {
    Cbl4fSIMDUnion_ u = {v4f};
    return u.z_;
}

CBL_INLINE float cbl4fGetW(Cbl4f v4f) {
    Cbl4fSIMDUnion_ u = {v4f};
    return u.w_;
}

#define cbl4fSwizzle(v4f, swizzle) (_mm_shuffle_ps((v4f), (v4f), (swizzle)))

CBL_INLINE Cbl4f cbl4fSplat(float f) {
    return _mm_set1_ps(f);
}

CBL_INLINE Cbl4f cbl4fSplatX(Cbl4f v4f) {
    return cbl4fSwizzle(v4f, CBL_4F_SWIZZLE_XXXX);
}

CBL_INLINE Cbl4f cbl4fSplatY(Cbl4f v4f) {
    return cbl4fSwizzle(v4f, CBL_4F_SWIZZLE_YYYY);
}

CBL_INLINE Cbl4f cbl4fSplatZ(Cbl4f v4f) {
    return cbl4fSwizzle(v4f, CBL_4F_SWIZZLE_ZZZZ);
}

CBL_INLINE Cbl4f cbl4fSplatW(Cbl4f v4f) {
    return cbl4fSwizzle(v4f, CBL_4F_SWIZZLE_WWWW);
}

CBL_INLINE Cbl4f cbl4fAdd(Cbl4f lhs, Cbl4f rhs) {
    return _mm_add_ps(lhs, rhs);
}

CBL_INLINE Cbl4f cbl4fSub(Cbl4f lhs, Cbl4f rhs) {
    return _mm_sub_ps(lhs, rhs);
}

CBL_INLINE Cbl4f cbl4fMul(Cbl4f lhs, Cbl4f rhs) {
    return _mm_mul_ps(lhs, rhs);
}

CBL_INLINE Cbl4f cbl4fDiv(Cbl4f lhs, Cbl4f rhs) {
    return _mm_div_ps(lhs, rhs);
}

CBL_INLINE Cbl4f cbl4fRecip(Cbl4f v4f) {
    return _mm_div_ps(cbl4fSplat(1.0f), v4f);
}

CBL_INLINE Cbl4f cbl4fRecipFast(Cbl4f v4f) {
    return _mm_rcp_ps(v4f);
}

CBL_INLINE Cbl4f cbl4fSqrt(Cbl4f v4f) {
    Cbl4fSIMDUnion_ u = {v4f};
    return cbl4fNew(sqrtf(u.x_), sqrtf(u.y_), sqrtf(u.z_), sqrtf(u.w_));
}

CBL_INLINE Cbl4f cbl4fSqrtFast(Cbl4f v4f) {
    return _mm_sqrt_ps(v4f);
}

CBL_INLINE Cbl4f cbl4fRecipSqrt(Cbl4f v4f) {
    return cbl4fRecip(cbl4fSqrt(v4f));
}

CBL_INLINE Cbl4f cbl4fRecipSqrtFast(Cbl4f v4f) {
    return _mm_rsqrt_ps(v4f);
}

CBL_INLINE Cbl4f cbl4fCross(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fSub(cbl4fMul(cbl4fSwizzle(lhs, CBL_4F_SWIZZLE_WXZY),
                             cbl4fSwizzle(rhs, CBL_4F_SWIZZLE_WYXZ)),
                    cbl4fMul(cbl4fSwizzle(lhs, CBL_4F_SWIZZLE_WYXZ),
                             cbl4fSwizzle(rhs, CBL_4F_SWIZZLE_WXZY)));
}

CBL_INLINE Cbl4f cbl4fMin(Cbl4f lhs, Cbl4f rhs) {
    return _mm_min_ps(lhs, rhs);
}

CBL_INLINE Cbl4f cbl4fMax(Cbl4f lhs, Cbl4f rhs) {
    return _mm_max_ps(lhs, rhs);
}

CBL_INLINE Cbl4f cbl4fAbs(Cbl4f v4f) {
    // removes sign bit
    return _mm_andnot_ps(_mm_castsi128_ps(_mm_set1_epi32(0x80000000)), v4f);
}

CBL_INLINE Cbl4f cbl4fNeg(Cbl4f v4f) {
    // flips sign bit
    return _mm_xor_ps(_mm_castsi128_ps(_mm_set1_epi32(0x80000000)), v4f);
}

CBL_EXTERN_END

#endif // CBL_MATH_SIMD_4F_H_