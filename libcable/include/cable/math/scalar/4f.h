#ifndef CBL_MATH_SCALAR_4F_H_
#define CBL_MATH_SCALAR_4F_H_

#include <string.h>
#include <math.h>

#include <cable/porting.h>

#define CBL_4F_SHUFFLE_(x, y, z, w) (((x) << 6) | ((y) << 4) | ((z) << 2) | (w))

typedef enum Cbl4fSwizzle {
    CBL_4F_SWIZZLE_XXXX = CBL_4F_SHUFFLE_(0, 0, 0, 0),
    CBL_4F_SWIZZLE_XXXY = CBL_4F_SHUFFLE_(0, 0, 0, 1),
    CBL_4F_SWIZZLE_XXXZ = CBL_4F_SHUFFLE_(0, 0, 0, 2),
    CBL_4F_SWIZZLE_XXXW = CBL_4F_SHUFFLE_(0, 0, 0, 3),
    CBL_4F_SWIZZLE_XXYX = CBL_4F_SHUFFLE_(0, 0, 1, 0),
    CBL_4F_SWIZZLE_XXYY = CBL_4F_SHUFFLE_(0, 0, 1, 1),
    CBL_4F_SWIZZLE_XXYZ = CBL_4F_SHUFFLE_(0, 0, 1, 2),
    CBL_4F_SWIZZLE_XXYW = CBL_4F_SHUFFLE_(0, 0, 1, 3),
    CBL_4F_SWIZZLE_XXZX = CBL_4F_SHUFFLE_(0, 0, 2, 0),
    CBL_4F_SWIZZLE_XXZY = CBL_4F_SHUFFLE_(0, 0, 2, 1),
    CBL_4F_SWIZZLE_XXZZ = CBL_4F_SHUFFLE_(0, 0, 2, 2),
    CBL_4F_SWIZZLE_XXZW = CBL_4F_SHUFFLE_(0, 0, 2, 3),
    CBL_4F_SWIZZLE_XXWX = CBL_4F_SHUFFLE_(0, 0, 3, 0),
    CBL_4F_SWIZZLE_XXWY = CBL_4F_SHUFFLE_(0, 0, 3, 1),
    CBL_4F_SWIZZLE_XXWZ = CBL_4F_SHUFFLE_(0, 0, 3, 2),
    CBL_4F_SWIZZLE_XXWW = CBL_4F_SHUFFLE_(0, 0, 3, 3),
    CBL_4F_SWIZZLE_XYXX = CBL_4F_SHUFFLE_(0, 1, 0, 0),
    CBL_4F_SWIZZLE_XYXY = CBL_4F_SHUFFLE_(0, 1, 0, 1),
    CBL_4F_SWIZZLE_XYXZ = CBL_4F_SHUFFLE_(0, 1, 0, 2),
    CBL_4F_SWIZZLE_XYXW = CBL_4F_SHUFFLE_(0, 1, 0, 3),
    CBL_4F_SWIZZLE_XYYX = CBL_4F_SHUFFLE_(0, 1, 1, 0),
    CBL_4F_SWIZZLE_XYYY = CBL_4F_SHUFFLE_(0, 1, 1, 1),
    CBL_4F_SWIZZLE_XYYZ = CBL_4F_SHUFFLE_(0, 1, 1, 2),
    CBL_4F_SWIZZLE_XYYW = CBL_4F_SHUFFLE_(0, 1, 1, 3),
    CBL_4F_SWIZZLE_XYZX = CBL_4F_SHUFFLE_(0, 1, 2, 0),
    CBL_4F_SWIZZLE_XYZY = CBL_4F_SHUFFLE_(0, 1, 2, 1),
    CBL_4F_SWIZZLE_XYZZ = CBL_4F_SHUFFLE_(0, 1, 2, 2),
    CBL_4F_SWIZZLE_XYZW = CBL_4F_SHUFFLE_(0, 1, 2, 3),
    CBL_4F_SWIZZLE_XYWX = CBL_4F_SHUFFLE_(0, 1, 3, 0),
    CBL_4F_SWIZZLE_XYWY = CBL_4F_SHUFFLE_(0, 1, 3, 1),
    CBL_4F_SWIZZLE_XYWZ = CBL_4F_SHUFFLE_(0, 1, 3, 2),
    CBL_4F_SWIZZLE_XYWW = CBL_4F_SHUFFLE_(0, 1, 3, 3),
    CBL_4F_SWIZZLE_XZXX = CBL_4F_SHUFFLE_(0, 2, 0, 0),
    CBL_4F_SWIZZLE_XZXY = CBL_4F_SHUFFLE_(0, 2, 0, 1),
    CBL_4F_SWIZZLE_XZXZ = CBL_4F_SHUFFLE_(0, 2, 0, 2),
    CBL_4F_SWIZZLE_XZXW = CBL_4F_SHUFFLE_(0, 2, 0, 3),
    CBL_4F_SWIZZLE_XZYX = CBL_4F_SHUFFLE_(0, 2, 1, 0),
    CBL_4F_SWIZZLE_XZYY = CBL_4F_SHUFFLE_(0, 2, 1, 1),
    CBL_4F_SWIZZLE_XZYZ = CBL_4F_SHUFFLE_(0, 2, 1, 2),
    CBL_4F_SWIZZLE_XZYW = CBL_4F_SHUFFLE_(0, 2, 1, 3),
    CBL_4F_SWIZZLE_XZZX = CBL_4F_SHUFFLE_(0, 2, 2, 0),
    CBL_4F_SWIZZLE_XZZY = CBL_4F_SHUFFLE_(0, 2, 2, 1),
    CBL_4F_SWIZZLE_XZZZ = CBL_4F_SHUFFLE_(0, 2, 2, 2),
    CBL_4F_SWIZZLE_XZZW = CBL_4F_SHUFFLE_(0, 2, 2, 3),
    CBL_4F_SWIZZLE_XZWX = CBL_4F_SHUFFLE_(0, 2, 3, 0),
    CBL_4F_SWIZZLE_XZWY = CBL_4F_SHUFFLE_(0, 2, 3, 1),
    CBL_4F_SWIZZLE_XZWZ = CBL_4F_SHUFFLE_(0, 2, 3, 2),
    CBL_4F_SWIZZLE_XZWW = CBL_4F_SHUFFLE_(0, 2, 3, 3),
    CBL_4F_SWIZZLE_XWXX = CBL_4F_SHUFFLE_(0, 3, 0, 0),
    CBL_4F_SWIZZLE_XWXY = CBL_4F_SHUFFLE_(0, 3, 0, 1),
    CBL_4F_SWIZZLE_XWXZ = CBL_4F_SHUFFLE_(0, 3, 0, 2),
    CBL_4F_SWIZZLE_XWXW = CBL_4F_SHUFFLE_(0, 3, 0, 3),
    CBL_4F_SWIZZLE_XWYX = CBL_4F_SHUFFLE_(0, 3, 1, 0),
    CBL_4F_SWIZZLE_XWYY = CBL_4F_SHUFFLE_(0, 3, 1, 1),
    CBL_4F_SWIZZLE_XWYZ = CBL_4F_SHUFFLE_(0, 3, 1, 2),
    CBL_4F_SWIZZLE_XWYW = CBL_4F_SHUFFLE_(0, 3, 1, 3),
    CBL_4F_SWIZZLE_XWZX = CBL_4F_SHUFFLE_(0, 3, 2, 0),
    CBL_4F_SWIZZLE_XWZY = CBL_4F_SHUFFLE_(0, 3, 2, 1),
    CBL_4F_SWIZZLE_XWZZ = CBL_4F_SHUFFLE_(0, 3, 2, 2),
    CBL_4F_SWIZZLE_XWZW = CBL_4F_SHUFFLE_(0, 3, 2, 3),
    CBL_4F_SWIZZLE_XWWX = CBL_4F_SHUFFLE_(0, 3, 3, 0),
    CBL_4F_SWIZZLE_XWWY = CBL_4F_SHUFFLE_(0, 3, 3, 1),
    CBL_4F_SWIZZLE_XWWZ = CBL_4F_SHUFFLE_(0, 3, 3, 2),
    CBL_4F_SWIZZLE_XWWW = CBL_4F_SHUFFLE_(0, 3, 3, 3),
    CBL_4F_SWIZZLE_YXXX = CBL_4F_SHUFFLE_(1, 0, 0, 0),
    CBL_4F_SWIZZLE_YXXY = CBL_4F_SHUFFLE_(1, 0, 0, 1),
    CBL_4F_SWIZZLE_YXXZ = CBL_4F_SHUFFLE_(1, 0, 0, 2),
    CBL_4F_SWIZZLE_YXXW = CBL_4F_SHUFFLE_(1, 0, 0, 3),
    CBL_4F_SWIZZLE_YXYX = CBL_4F_SHUFFLE_(1, 0, 1, 0),
    CBL_4F_SWIZZLE_YXYY = CBL_4F_SHUFFLE_(1, 0, 1, 1),
    CBL_4F_SWIZZLE_YXYZ = CBL_4F_SHUFFLE_(1, 0, 1, 2),
    CBL_4F_SWIZZLE_YXYW = CBL_4F_SHUFFLE_(1, 0, 1, 3),
    CBL_4F_SWIZZLE_YXZX = CBL_4F_SHUFFLE_(1, 0, 2, 0),
    CBL_4F_SWIZZLE_YXZY = CBL_4F_SHUFFLE_(1, 0, 2, 1),
    CBL_4F_SWIZZLE_YXZZ = CBL_4F_SHUFFLE_(1, 0, 2, 2),
    CBL_4F_SWIZZLE_YXZW = CBL_4F_SHUFFLE_(1, 0, 2, 3),
    CBL_4F_SWIZZLE_YXWX = CBL_4F_SHUFFLE_(1, 0, 3, 0),
    CBL_4F_SWIZZLE_YXWY = CBL_4F_SHUFFLE_(1, 0, 3, 1),
    CBL_4F_SWIZZLE_YXWZ = CBL_4F_SHUFFLE_(1, 0, 3, 2),
    CBL_4F_SWIZZLE_YXWW = CBL_4F_SHUFFLE_(1, 0, 3, 3),
    CBL_4F_SWIZZLE_YYXX = CBL_4F_SHUFFLE_(1, 1, 0, 0),
    CBL_4F_SWIZZLE_YYXY = CBL_4F_SHUFFLE_(1, 1, 0, 1),
    CBL_4F_SWIZZLE_YYXZ = CBL_4F_SHUFFLE_(1, 1, 0, 2),
    CBL_4F_SWIZZLE_YYXW = CBL_4F_SHUFFLE_(1, 1, 0, 3),
    CBL_4F_SWIZZLE_YYYX = CBL_4F_SHUFFLE_(1, 1, 1, 0),
    CBL_4F_SWIZZLE_YYYY = CBL_4F_SHUFFLE_(1, 1, 1, 1),
    CBL_4F_SWIZZLE_YYYZ = CBL_4F_SHUFFLE_(1, 1, 1, 2),
    CBL_4F_SWIZZLE_YYYW = CBL_4F_SHUFFLE_(1, 1, 1, 3),
    CBL_4F_SWIZZLE_YYZX = CBL_4F_SHUFFLE_(1, 1, 2, 0),
    CBL_4F_SWIZZLE_YYZY = CBL_4F_SHUFFLE_(1, 1, 2, 1),
    CBL_4F_SWIZZLE_YYZZ = CBL_4F_SHUFFLE_(1, 1, 2, 2),
    CBL_4F_SWIZZLE_YYZW = CBL_4F_SHUFFLE_(1, 1, 2, 3),
    CBL_4F_SWIZZLE_YYWX = CBL_4F_SHUFFLE_(1, 1, 3, 0),
    CBL_4F_SWIZZLE_YYWY = CBL_4F_SHUFFLE_(1, 1, 3, 1),
    CBL_4F_SWIZZLE_YYWZ = CBL_4F_SHUFFLE_(1, 1, 3, 2),
    CBL_4F_SWIZZLE_YYWW = CBL_4F_SHUFFLE_(1, 1, 3, 3),
    CBL_4F_SWIZZLE_YZXX = CBL_4F_SHUFFLE_(1, 2, 0, 0),
    CBL_4F_SWIZZLE_YZXY = CBL_4F_SHUFFLE_(1, 2, 0, 1),
    CBL_4F_SWIZZLE_YZXZ = CBL_4F_SHUFFLE_(1, 2, 0, 2),
    CBL_4F_SWIZZLE_YZXW = CBL_4F_SHUFFLE_(1, 2, 0, 3),
    CBL_4F_SWIZZLE_YZYX = CBL_4F_SHUFFLE_(1, 2, 1, 0),
    CBL_4F_SWIZZLE_YZYY = CBL_4F_SHUFFLE_(1, 2, 1, 1),
    CBL_4F_SWIZZLE_YZYZ = CBL_4F_SHUFFLE_(1, 2, 1, 2),
    CBL_4F_SWIZZLE_YZYW = CBL_4F_SHUFFLE_(1, 2, 1, 3),
    CBL_4F_SWIZZLE_YZZX = CBL_4F_SHUFFLE_(1, 2, 2, 0),
    CBL_4F_SWIZZLE_YZZY = CBL_4F_SHUFFLE_(1, 2, 2, 1),
    CBL_4F_SWIZZLE_YZZZ = CBL_4F_SHUFFLE_(1, 2, 2, 2),
    CBL_4F_SWIZZLE_YZZW = CBL_4F_SHUFFLE_(1, 2, 2, 3),
    CBL_4F_SWIZZLE_YZWX = CBL_4F_SHUFFLE_(1, 2, 3, 0),
    CBL_4F_SWIZZLE_YZWY = CBL_4F_SHUFFLE_(1, 2, 3, 1),
    CBL_4F_SWIZZLE_YZWZ = CBL_4F_SHUFFLE_(1, 2, 3, 2),
    CBL_4F_SWIZZLE_YZWW = CBL_4F_SHUFFLE_(1, 2, 3, 3),
    CBL_4F_SWIZZLE_YWXX = CBL_4F_SHUFFLE_(1, 3, 0, 0),
    CBL_4F_SWIZZLE_YWXY = CBL_4F_SHUFFLE_(1, 3, 0, 1),
    CBL_4F_SWIZZLE_YWXZ = CBL_4F_SHUFFLE_(1, 3, 0, 2),
    CBL_4F_SWIZZLE_YWXW = CBL_4F_SHUFFLE_(1, 3, 0, 3),
    CBL_4F_SWIZZLE_YWYX = CBL_4F_SHUFFLE_(1, 3, 1, 0),
    CBL_4F_SWIZZLE_YWYY = CBL_4F_SHUFFLE_(1, 3, 1, 1),
    CBL_4F_SWIZZLE_YWYZ = CBL_4F_SHUFFLE_(1, 3, 1, 2),
    CBL_4F_SWIZZLE_YWYW = CBL_4F_SHUFFLE_(1, 3, 1, 3),
    CBL_4F_SWIZZLE_YWZX = CBL_4F_SHUFFLE_(1, 3, 2, 0),
    CBL_4F_SWIZZLE_YWZY = CBL_4F_SHUFFLE_(1, 3, 2, 1),
    CBL_4F_SWIZZLE_YWZZ = CBL_4F_SHUFFLE_(1, 3, 2, 2),
    CBL_4F_SWIZZLE_YWZW = CBL_4F_SHUFFLE_(1, 3, 2, 3),
    CBL_4F_SWIZZLE_YWWX = CBL_4F_SHUFFLE_(1, 3, 3, 0),
    CBL_4F_SWIZZLE_YWWY = CBL_4F_SHUFFLE_(1, 3, 3, 1),
    CBL_4F_SWIZZLE_YWWZ = CBL_4F_SHUFFLE_(1, 3, 3, 2),
    CBL_4F_SWIZZLE_YWWW = CBL_4F_SHUFFLE_(1, 3, 3, 3),
    CBL_4F_SWIZZLE_ZXXX = CBL_4F_SHUFFLE_(2, 0, 0, 0),
    CBL_4F_SWIZZLE_ZXXY = CBL_4F_SHUFFLE_(2, 0, 0, 1),
    CBL_4F_SWIZZLE_ZXXZ = CBL_4F_SHUFFLE_(2, 0, 0, 2),
    CBL_4F_SWIZZLE_ZXXW = CBL_4F_SHUFFLE_(2, 0, 0, 3),
    CBL_4F_SWIZZLE_ZXYX = CBL_4F_SHUFFLE_(2, 0, 1, 0),
    CBL_4F_SWIZZLE_ZXYY = CBL_4F_SHUFFLE_(2, 0, 1, 1),
    CBL_4F_SWIZZLE_ZXYZ = CBL_4F_SHUFFLE_(2, 0, 1, 2),
    CBL_4F_SWIZZLE_ZXYW = CBL_4F_SHUFFLE_(2, 0, 1, 3),
    CBL_4F_SWIZZLE_ZXZX = CBL_4F_SHUFFLE_(2, 0, 2, 0),
    CBL_4F_SWIZZLE_ZXZY = CBL_4F_SHUFFLE_(2, 0, 2, 1),
    CBL_4F_SWIZZLE_ZXZZ = CBL_4F_SHUFFLE_(2, 0, 2, 2),
    CBL_4F_SWIZZLE_ZXZW = CBL_4F_SHUFFLE_(2, 0, 2, 3),
    CBL_4F_SWIZZLE_ZXWX = CBL_4F_SHUFFLE_(2, 0, 3, 0),
    CBL_4F_SWIZZLE_ZXWY = CBL_4F_SHUFFLE_(2, 0, 3, 1),
    CBL_4F_SWIZZLE_ZXWZ = CBL_4F_SHUFFLE_(2, 0, 3, 2),
    CBL_4F_SWIZZLE_ZXWW = CBL_4F_SHUFFLE_(2, 0, 3, 3),
    CBL_4F_SWIZZLE_ZYXX = CBL_4F_SHUFFLE_(2, 1, 0, 0),
    CBL_4F_SWIZZLE_ZYXY = CBL_4F_SHUFFLE_(2, 1, 0, 1),
    CBL_4F_SWIZZLE_ZYXZ = CBL_4F_SHUFFLE_(2, 1, 0, 2),
    CBL_4F_SWIZZLE_ZYXW = CBL_4F_SHUFFLE_(2, 1, 0, 3),
    CBL_4F_SWIZZLE_ZYYX = CBL_4F_SHUFFLE_(2, 1, 1, 0),
    CBL_4F_SWIZZLE_ZYYY = CBL_4F_SHUFFLE_(2, 1, 1, 1),
    CBL_4F_SWIZZLE_ZYYZ = CBL_4F_SHUFFLE_(2, 1, 1, 2),
    CBL_4F_SWIZZLE_ZYYW = CBL_4F_SHUFFLE_(2, 1, 1, 3),
    CBL_4F_SWIZZLE_ZYZX = CBL_4F_SHUFFLE_(2, 1, 2, 0),
    CBL_4F_SWIZZLE_ZYZY = CBL_4F_SHUFFLE_(2, 1, 2, 1),
    CBL_4F_SWIZZLE_ZYZZ = CBL_4F_SHUFFLE_(2, 1, 2, 2),
    CBL_4F_SWIZZLE_ZYZW = CBL_4F_SHUFFLE_(2, 1, 2, 3),
    CBL_4F_SWIZZLE_ZYWX = CBL_4F_SHUFFLE_(2, 1, 3, 0),
    CBL_4F_SWIZZLE_ZYWY = CBL_4F_SHUFFLE_(2, 1, 3, 1),
    CBL_4F_SWIZZLE_ZYWZ = CBL_4F_SHUFFLE_(2, 1, 3, 2),
    CBL_4F_SWIZZLE_ZYWW = CBL_4F_SHUFFLE_(2, 1, 3, 3),
    CBL_4F_SWIZZLE_ZZXX = CBL_4F_SHUFFLE_(2, 2, 0, 0),
    CBL_4F_SWIZZLE_ZZXY = CBL_4F_SHUFFLE_(2, 2, 0, 1),
    CBL_4F_SWIZZLE_ZZXZ = CBL_4F_SHUFFLE_(2, 2, 0, 2),
    CBL_4F_SWIZZLE_ZZXW = CBL_4F_SHUFFLE_(2, 2, 0, 3),
    CBL_4F_SWIZZLE_ZZYX = CBL_4F_SHUFFLE_(2, 2, 1, 0),
    CBL_4F_SWIZZLE_ZZYY = CBL_4F_SHUFFLE_(2, 2, 1, 1),
    CBL_4F_SWIZZLE_ZZYZ = CBL_4F_SHUFFLE_(2, 2, 1, 2),
    CBL_4F_SWIZZLE_ZZYW = CBL_4F_SHUFFLE_(2, 2, 1, 3),
    CBL_4F_SWIZZLE_ZZZX = CBL_4F_SHUFFLE_(2, 2, 2, 0),
    CBL_4F_SWIZZLE_ZZZY = CBL_4F_SHUFFLE_(2, 2, 2, 1),
    CBL_4F_SWIZZLE_ZZZZ = CBL_4F_SHUFFLE_(2, 2, 2, 2),
    CBL_4F_SWIZZLE_ZZZW = CBL_4F_SHUFFLE_(2, 2, 2, 3),
    CBL_4F_SWIZZLE_ZZWX = CBL_4F_SHUFFLE_(2, 2, 3, 0),
    CBL_4F_SWIZZLE_ZZWY = CBL_4F_SHUFFLE_(2, 2, 3, 1),
    CBL_4F_SWIZZLE_ZZWZ = CBL_4F_SHUFFLE_(2, 2, 3, 2),
    CBL_4F_SWIZZLE_ZZWW = CBL_4F_SHUFFLE_(2, 2, 3, 3),
    CBL_4F_SWIZZLE_ZWXX = CBL_4F_SHUFFLE_(2, 3, 0, 0),
    CBL_4F_SWIZZLE_ZWXY = CBL_4F_SHUFFLE_(2, 3, 0, 1),
    CBL_4F_SWIZZLE_ZWXZ = CBL_4F_SHUFFLE_(2, 3, 0, 2),
    CBL_4F_SWIZZLE_ZWXW = CBL_4F_SHUFFLE_(2, 3, 0, 3),
    CBL_4F_SWIZZLE_ZWYX = CBL_4F_SHUFFLE_(2, 3, 1, 0),
    CBL_4F_SWIZZLE_ZWYY = CBL_4F_SHUFFLE_(2, 3, 1, 1),
    CBL_4F_SWIZZLE_ZWYZ = CBL_4F_SHUFFLE_(2, 3, 1, 2),
    CBL_4F_SWIZZLE_ZWYW = CBL_4F_SHUFFLE_(2, 3, 1, 3),
    CBL_4F_SWIZZLE_ZWZX = CBL_4F_SHUFFLE_(2, 3, 2, 0),
    CBL_4F_SWIZZLE_ZWZY = CBL_4F_SHUFFLE_(2, 3, 2, 1),
    CBL_4F_SWIZZLE_ZWZZ = CBL_4F_SHUFFLE_(2, 3, 2, 2),
    CBL_4F_SWIZZLE_ZWZW = CBL_4F_SHUFFLE_(2, 3, 2, 3),
    CBL_4F_SWIZZLE_ZWWX = CBL_4F_SHUFFLE_(2, 3, 3, 0),
    CBL_4F_SWIZZLE_ZWWY = CBL_4F_SHUFFLE_(2, 3, 3, 1),
    CBL_4F_SWIZZLE_ZWWZ = CBL_4F_SHUFFLE_(2, 3, 3, 2),
    CBL_4F_SWIZZLE_ZWWW = CBL_4F_SHUFFLE_(2, 3, 3, 3),
    CBL_4F_SWIZZLE_WXXX = CBL_4F_SHUFFLE_(3, 0, 0, 0),
    CBL_4F_SWIZZLE_WXXY = CBL_4F_SHUFFLE_(3, 0, 0, 1),
    CBL_4F_SWIZZLE_WXXZ = CBL_4F_SHUFFLE_(3, 0, 0, 2),
    CBL_4F_SWIZZLE_WXXW = CBL_4F_SHUFFLE_(3, 0, 0, 3),
    CBL_4F_SWIZZLE_WXYX = CBL_4F_SHUFFLE_(3, 0, 1, 0),
    CBL_4F_SWIZZLE_WXYY = CBL_4F_SHUFFLE_(3, 0, 1, 1),
    CBL_4F_SWIZZLE_WXYZ = CBL_4F_SHUFFLE_(3, 0, 1, 2),
    CBL_4F_SWIZZLE_WXYW = CBL_4F_SHUFFLE_(3, 0, 1, 3),
    CBL_4F_SWIZZLE_WXZX = CBL_4F_SHUFFLE_(3, 0, 2, 0),
    CBL_4F_SWIZZLE_WXZY = CBL_4F_SHUFFLE_(3, 0, 2, 1),
    CBL_4F_SWIZZLE_WXZZ = CBL_4F_SHUFFLE_(3, 0, 2, 2),
    CBL_4F_SWIZZLE_WXZW = CBL_4F_SHUFFLE_(3, 0, 2, 3),
    CBL_4F_SWIZZLE_WXWX = CBL_4F_SHUFFLE_(3, 0, 3, 0),
    CBL_4F_SWIZZLE_WXWY = CBL_4F_SHUFFLE_(3, 0, 3, 1),
    CBL_4F_SWIZZLE_WXWZ = CBL_4F_SHUFFLE_(3, 0, 3, 2),
    CBL_4F_SWIZZLE_WXWW = CBL_4F_SHUFFLE_(3, 0, 3, 3),
    CBL_4F_SWIZZLE_WYXX = CBL_4F_SHUFFLE_(3, 1, 0, 0),
    CBL_4F_SWIZZLE_WYXY = CBL_4F_SHUFFLE_(3, 1, 0, 1),
    CBL_4F_SWIZZLE_WYXZ = CBL_4F_SHUFFLE_(3, 1, 0, 2),
    CBL_4F_SWIZZLE_WYXW = CBL_4F_SHUFFLE_(3, 1, 0, 3),
    CBL_4F_SWIZZLE_WYYX = CBL_4F_SHUFFLE_(3, 1, 1, 0),
    CBL_4F_SWIZZLE_WYYY = CBL_4F_SHUFFLE_(3, 1, 1, 1),
    CBL_4F_SWIZZLE_WYYZ = CBL_4F_SHUFFLE_(3, 1, 1, 2),
    CBL_4F_SWIZZLE_WYYW = CBL_4F_SHUFFLE_(3, 1, 1, 3),
    CBL_4F_SWIZZLE_WYZX = CBL_4F_SHUFFLE_(3, 1, 2, 0),
    CBL_4F_SWIZZLE_WYZY = CBL_4F_SHUFFLE_(3, 1, 2, 1),
    CBL_4F_SWIZZLE_WYZZ = CBL_4F_SHUFFLE_(3, 1, 2, 2),
    CBL_4F_SWIZZLE_WYZW = CBL_4F_SHUFFLE_(3, 1, 2, 3),
    CBL_4F_SWIZZLE_WYWX = CBL_4F_SHUFFLE_(3, 1, 3, 0),
    CBL_4F_SWIZZLE_WYWY = CBL_4F_SHUFFLE_(3, 1, 3, 1),
    CBL_4F_SWIZZLE_WYWZ = CBL_4F_SHUFFLE_(3, 1, 3, 2),
    CBL_4F_SWIZZLE_WYWW = CBL_4F_SHUFFLE_(3, 1, 3, 3),
    CBL_4F_SWIZZLE_WZXX = CBL_4F_SHUFFLE_(3, 2, 0, 0),
    CBL_4F_SWIZZLE_WZXY = CBL_4F_SHUFFLE_(3, 2, 0, 1),
    CBL_4F_SWIZZLE_WZXZ = CBL_4F_SHUFFLE_(3, 2, 0, 2),
    CBL_4F_SWIZZLE_WZXW = CBL_4F_SHUFFLE_(3, 2, 0, 3),
    CBL_4F_SWIZZLE_WZYX = CBL_4F_SHUFFLE_(3, 2, 1, 0),
    CBL_4F_SWIZZLE_WZYY = CBL_4F_SHUFFLE_(3, 2, 1, 1),
    CBL_4F_SWIZZLE_WZYZ = CBL_4F_SHUFFLE_(3, 2, 1, 2),
    CBL_4F_SWIZZLE_WZYW = CBL_4F_SHUFFLE_(3, 2, 1, 3),
    CBL_4F_SWIZZLE_WZZX = CBL_4F_SHUFFLE_(3, 2, 2, 0),
    CBL_4F_SWIZZLE_WZZY = CBL_4F_SHUFFLE_(3, 2, 2, 1),
    CBL_4F_SWIZZLE_WZZZ = CBL_4F_SHUFFLE_(3, 2, 2, 2),
    CBL_4F_SWIZZLE_WZZW = CBL_4F_SHUFFLE_(3, 2, 2, 3),
    CBL_4F_SWIZZLE_WZWX = CBL_4F_SHUFFLE_(3, 2, 3, 0),
    CBL_4F_SWIZZLE_WZWY = CBL_4F_SHUFFLE_(3, 2, 3, 1),
    CBL_4F_SWIZZLE_WZWZ = CBL_4F_SHUFFLE_(3, 2, 3, 2),
    CBL_4F_SWIZZLE_WZWW = CBL_4F_SHUFFLE_(3, 2, 3, 3),
    CBL_4F_SWIZZLE_WWXX = CBL_4F_SHUFFLE_(3, 3, 0, 0),
    CBL_4F_SWIZZLE_WWXY = CBL_4F_SHUFFLE_(3, 3, 0, 1),
    CBL_4F_SWIZZLE_WWXZ = CBL_4F_SHUFFLE_(3, 3, 0, 2),
    CBL_4F_SWIZZLE_WWXW = CBL_4F_SHUFFLE_(3, 3, 0, 3),
    CBL_4F_SWIZZLE_WWYX = CBL_4F_SHUFFLE_(3, 3, 1, 0),
    CBL_4F_SWIZZLE_WWYY = CBL_4F_SHUFFLE_(3, 3, 1, 1),
    CBL_4F_SWIZZLE_WWYZ = CBL_4F_SHUFFLE_(3, 3, 1, 2),
    CBL_4F_SWIZZLE_WWYW = CBL_4F_SHUFFLE_(3, 3, 1, 3),
    CBL_4F_SWIZZLE_WWZX = CBL_4F_SHUFFLE_(3, 3, 2, 0),
    CBL_4F_SWIZZLE_WWZY = CBL_4F_SHUFFLE_(3, 3, 2, 1),
    CBL_4F_SWIZZLE_WWZZ = CBL_4F_SHUFFLE_(3, 3, 2, 2),
    CBL_4F_SWIZZLE_WWZW = CBL_4F_SHUFFLE_(3, 3, 2, 3),
    CBL_4F_SWIZZLE_WWWX = CBL_4F_SHUFFLE_(3, 3, 3, 0),
    CBL_4F_SWIZZLE_WWWY = CBL_4F_SHUFFLE_(3, 3, 3, 1),
    CBL_4F_SWIZZLE_WWWZ = CBL_4F_SHUFFLE_(3, 3, 3, 2),
    CBL_4F_SWIZZLE_WWWW = CBL_4F_SHUFFLE_(3, 3, 3, 3),
} Cbl4fSwizzle;

#undef CBL_4F_SHUFFLE_

typedef struct Cbl4f {
    union {
        struct {
            float x_, y_, z_, w_;
        };
        float arr_[4];
    };
} Cbl4f;

CBL_INLINE Cbl4f cbl4fNew(float x, float y, float z, float w) {
    return (Cbl4f) { .x_ = x, .y_ = y, .z_ = z, .w_ = w };
}

CBL_INLINE Cbl4f cbl4fZero() {
    return cbl4fNew(0.0f, 0.0f, 0.0f, 0.0f);
}

CBL_INLINE bool cbl4fEqual(Cbl4f lhs, Cbl4f rhs) {
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_ && lhs.w_ == rhs.w_;
}

CBL_INLINE bool cbl4fLessThan(Cbl4f lhs, Cbl4f rhs) {
    return lhs.x_ < rhs.x_ && lhs.y_ < rhs.y_ && lhs.z_ < rhs.z_ && lhs.w_ < rhs.w_;
}

CBL_INLINE bool cbl4fGreaterThan(Cbl4f lhs, Cbl4f rhs) {
    return lhs.x_ > rhs.x_ && lhs.y_ > rhs.y_ && lhs.z_ > rhs.z_ && lhs.w_ > rhs.w_;
}

CBL_INLINE Cbl4f cbl4fMap(Cbl4f v4f, float (*fn)(float)) {
    return cbl4fNew(fn(v4f.x_), fn(v4f.y_), fn(v4f.z_), fn(v4f.w_));
}

CBL_INLINE Cbl4f cbl4fLoad(float *arr) {
    return cbl4fNew(arr[0], arr[1], arr[2], arr[3]);
}

CBL_INLINE void cbl4fStore(Cbl4f v4f, float *arr) {
    memcpy(arr, &v4f, sizeof(Cbl4f));
}

CBL_INLINE float cbl4fGetX(Cbl4f v4f) {
    return v4f.x_;
}

CBL_INLINE float cbl4fGetY(Cbl4f v4f) {
    return v4f.y_;
}

CBL_INLINE float cbl4fGetZ(Cbl4f v4f) {
    return v4f.z_;
}

CBL_INLINE float cbl4fGetW(Cbl4f v4f) {
    return v4f.w_;
}

CBL_INLINE Cbl4f cbl4fSwizzle(Cbl4f v4f, Cbl4fSwizzle swizzle) {
    return cbl4fNew(v4f.arr_[(swizzle & 0xC0) >> 6],
                    v4f.arr_[(swizzle & 0x30) >> 4],
                    v4f.arr_[(swizzle & 0x0C) >> 2],
                    v4f.arr_[(swizzle & 0x03)]);
}

CBL_INLINE Cbl4f cbl4fSplat(float f) {
    return cbl4fNew(f, f, f, f);
}

CBL_INLINE Cbl4f cbl4fSplatX(Cbl4f v4f) {
    return cbl4fSplat(v4f.x_);
}

CBL_INLINE Cbl4f cbl4fSplatY(Cbl4f v4f) {
    return cbl4fSplat(v4f.y_);
}

CBL_INLINE Cbl4f cbl4fSplatZ(Cbl4f v4f) {
    return cbl4fSplat(v4f.z_);
}

CBL_INLINE Cbl4f cbl4fSplatW(Cbl4f v4f) {
    return cbl4fSplat(v4f.w_);
}

CBL_INLINE Cbl4f cbl4fAdd(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fNew(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_, lhs.w_ + rhs.w_);
}

CBL_INLINE Cbl4f cbl4fSub(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fNew(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_, lhs.w_ - rhs.w_);
}

CBL_INLINE Cbl4f cbl4fMul(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fNew(lhs.x_ * rhs.x_, lhs.y_ * rhs.y_, lhs.z_ * rhs.z_, lhs.w_ * rhs.w_);
}

CBL_INLINE Cbl4f cbl4fDiv(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fNew(lhs.x_ / rhs.x_, lhs.y_ / rhs.y_, lhs.z_ / rhs.z_, lhs.w_ / rhs.w_);
}

CBL_INLINE Cbl4f cbl4fRecip(Cbl4f v4f) {
    return cbl4fNew(1.0f / v4f.x_, 1.0f / v4f.y_, 1.0f / v4f.z_, 1.0f / v4f.w_);
}

CBL_INLINE Cbl4f cbl4fRecipFast(Cbl4f v4f) {
    return cbl4fRecip(v4f);
}

CBL_INLINE Cbl4f cbl4fSqrt(Cbl4f v4f) {
    return cbl4fNew(sqrtf(v4f.x_), sqrtf(v4f.y_), sqrtf(v4f.z_), sqrtf(v4f.w_));
}

CBL_INLINE Cbl4f cbl4fSqrtFast(Cbl4f v4f) {
    return cbl4fSqrt(v4f);
}

CBL_INLINE Cbl4f cbl4fRecipSqrt(Cbl4f v4f) {
    return cbl4fRecip(cbl4fSqrt(v4f));
}

CBL_INLINE Cbl4f cbl4fRecipSqrtFast(Cbl4f v4f) {
    return cbl4fRecipSqrt(v4f);
}

CBL_INLINE Cbl4f cbl4fCross(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fNew(lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
                    lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
                    lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_, 0.0f);
}

CBL_INLINE Cbl4f cbl4fMin(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fNew(fminf(lhs.x_, rhs.x_), fminf(lhs.y_, rhs.y_),
                    fminf(lhs.z_, rhs.z_), fminf(lhs.w_, rhs.w_));
}

CBL_INLINE Cbl4f cbl4fMax(Cbl4f lhs, Cbl4f rhs) {
    return cbl4fNew(fmaxf(lhs.x_, rhs.x_), fmaxf(lhs.y_, rhs.y_),
                    fmaxf(lhs.z_, rhs.z_), fmaxf(lhs.w_, rhs.w_));
}

CBL_INLINE Cbl4f cbl4fAbs(Cbl4f v4f) {
    return cbl4fNew(fabsf(v4f.x_), fabsf(v4f.y_), fabsf(v4f.z_), fabsf(v4f.w_));
}

CBL_INLINE Cbl4f cbl4fNeq(Cbl4f v4f) {
    return cbl4fNew(-v4f.x_, -v4f.y_, -v4f.z_, -v4f.w_);
}

#endif // CBL_MATH_SCALAR_4F_H_