#pragma once


// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


// 最大的32位整数
#ifndef MAXIMUM_UINT32
#define MAXIMUM_UINT32                      ((UInt32)-1)
#endif // #ifndef MAXIMUM_UINT32


// 最大的64位整数
#ifndef MAXIMUM_UINT64
#define MAXIMUM_UINT64                      ((UInt64)-1)
#endif // #ifndef MAXIMUM_UINT64


// 浮点数相等的Tolerance
#ifndef EPSILON
// 由于相邻的两个32位浮点数的最小数值差为：0.00000006：6e-8
// 所以我们使用0.000001：1e-6
#define EPSILON                             1.0e-6
#endif // #ifndef EPSILON


// 近似0的角度
#ifndef APPROXIMATE_ZERO_DEGREE
// 0.001 degree
#define APPROXIMATE_ZERO_DEGREE             1.0e-3
#endif // #ifndef APPROXIMATE_ZERO_DEGREE


// 近似0的弧度
#ifndef APPROXIMATE_ZERO_RADIAN
// 0.001 degree
#define APPROXIMATE_ZERO_RADIAN             1.745329251994e-5
#endif // #ifndef APPROXIMATE_ZERO_RADIAN


// 近似0的角度的Sine的数值
#ifndef SINE_APPROXIMATE_ZERO
// sine(0.001 degree) = 0.000017453292519
#define SINE_APPROXIMATE_ZERO               1.745329251906e-5
#endif // #ifndef SINE_APPROXIMATE_ZERO


// PI
#ifndef ONE_PI
#define ONE_PI                              3.141592653589
#endif // #ifndef ONE_PI


// 二PI
#ifndef TWO_PI
#define TWO_PI                              (2 * ONE_PI)
#endif // #ifndef TWO_PI


// 三PI
#ifndef THREE_PI
#define THREE_PI                            (3 * ONE_PI)
#endif // #ifndef THREE_PI


// 四PI
#ifndef FOUR_PI
#define FOUR_PI                             (4 * ONE_PI)
#endif // #ifndef FOUR_PI


// PI的平方
#ifndef SQUARED_PI
#define SQUARED_PI                          (ONE_PI * ONE_PI)
#endif // #ifndef SQUARED_PI


// 二分之一PI
#ifndef HALF_PI
#define HALF_PI                             (0.5 * ONE_PI)
#endif // #ifndef HALF_PI


// PI的导数: 1/PI
#ifndef RECIPROCAL_PI
#define RECIPROCAL_PI                       (1 / ONE_PI)
#endif // #ifndef RECIPROCAL_PI


// 从角度转换到弧度时使用的系数: PI/180
#ifndef FACTOR_DEGREE_TO_RADIAN
#define FACTOR_DEGREE_TO_RADIAN             (ONE_PI / 180)
#endif // #ifndef FACTOR_DEGREE_TO_RADIAN


// 从弧度变换到角度时使用的系数: 180/PI
#ifndef FACTOR_RADIAN_TO_DEGREE
#define FACTOR_RADIAN_TO_DEGREE             (180 * RECIPROCAL_PI)
#endif // #ifndef FACTOR_RADIAN_TO_DEGREE


// 将角度转换为弧度
#ifndef DEGREE_TO_RADIAN
#define DEGREE_TO_RADIAN(degrees)           (degrees * FACTOR_DEGREE_TO_RADIAN)
#endif // #ifndef DEGREE_TO_RADIAN


// 将弧度转换为角度
#ifndef RADIAN_TO_DEGREE
#define RADIAN_TO_DEGREE(radians)           (radians * FACTOR_RADIAN_TO_DEGREE)
#endif // #ifndef RADIAN_TO_DEGREE



//--------------------------------------------------------------------------------------------------
// 16 BITS FLOATING POINT NUMBER
//--------------------------------------------------------------------------------------------------
// MARK: - 16位浮点数

// 16位最小的Normalized的浮点数的Bit Pattern
#ifndef MINIMUM_POSITIVE_REAL16
// 0.00006103515625
#define MINIMUM_POSITIVE_REAL16             0x0400
#endif // #ifndef MINIMUM_POSITIVE_REAL16


// 16位最小的De-Normalized的浮点数的Bit Pattern
#ifndef MINIMUM_POSITIVE_REAL16_DENOM
// 0.000000059604644775390625
#define MINIMUM_POSITIVE_REAL16_DENOM       0x0001
#endif // #ifndef MINIMUM_POSITIVE_REAL16_DENOM


// 16位浮点数1的Bit Pattern
#ifndef ONE_REAL16
// 1
#define ONE_REAL16                          0x3C00
#endif // #ifndef ONE_REAL16


// 16位最大的小于1的浮点数一的Bit Pattern
#ifndef MAXIMUM_REAL16_LESS_THAN_ONE
// 0.99951171875
#define MAXIMUM_REAL16_LESS_THAN_ONE        0x3BFF
#endif // #ifndef MAXIMUM_REAL16_LESS_THAN_ONE


// 16位最大的的浮点数的Bit Pattern
#ifndef MAXIMUM_REAL16
// 65504
#define MAXIMUM_REAL16                      0x7BFF
#endif // #ifndef MAXIMUM_REAL16



//--------------------------------------------------------------------------------------------------
// 32 BITS FLOATING POINT NUMBER
//--------------------------------------------------------------------------------------------------
// MARK: - 32位浮点数

// 32位最小的Normalized的浮点数的Bit Pattern: +2^-126
// NOTE: 不可使用此数来确定数值的间隙：
// 由于数值的分布间隙有Exponent来确定。
// 例如：数值1附近的间隔为1.19209290e-07， 而0.1附近的间隔为7.45058060e-09	
// 数值1的间隙16倍大
#ifndef MINIMUM_POSITIVE_REAL32
// 0x00800000
#define MINIMUM_POSITIVE_REAL32             1.175494351e-38f
#endif // #ifndef MINIMUM_POSITIVE_REAL32


// 32位最小的De-Normalized的浮点数的Bit Pattern
#ifndef MINIMUM_POSITIVE_REAL32_DENOM
// 0x00000001
#define MINIMUM_POSITIVE_REAL32_DENOM       401298464e-45f
#endif // #ifndef MINIMUM_POSITIVE_REAL32_DENOM


// 32位浮点数1的Bit Pattern
#ifndef ONE_REAL32
// 1
#define ONE_REAL32                          0x3F800000
#endif // #ifndef ONE_REAL32

// 32位最大的小于1的浮点数一的Bit Pattern
#ifndef MAXIMUM_REAL32_LESS_THAN_ONE
// 0x3F7FFFFF
#define MAXIMUM_REAL32_LESS_THAN_ONE        0.999999940f
#endif // #ifndef MAXIMUM_REAL32_LESS_THAN_ONE


// 32位最大的的浮点数的Bit Pattern
#ifndef MAXIMUM_REAL32
// 0x7F7FFFFF
#define MAXIMUM_REAL32                      3.402823466e+38f
#endif // #ifndef MAXIMUM_REAL32



//--------------------------------------------------------------------------------------------------
// 64 BITS FLOATING POINT NUMBER
//--------------------------------------------------------------------------------------------------
// MARK: - 64位浮点数

// 64位最小的Normalized的浮点数的Bit Pattern
#ifndef MINIMUM_POSITIVE_REAL64
// 0x0010000000000000
#define MINIMUM_POSITIVE_REAL64             2.225073858507e-308
#endif // #ifndef MINIMUM_POSITIVE_REAL64


// 64位最小的De-Normalized的浮点数的Bit Pattern
#ifndef MINIMUM_POSITIVE_REAL64_DENOM
// 0x0000000000000001
#define MINIMUM_POSITIVE_REAL64_DENOM       4.940656458412e-324
#endif // #ifndef MINIMUM_POSITIVE_REAL64_DENOM


// 64位浮点数1的Bit Pattern
#ifndef ONE_REAL64
// 1
#define ONE_REAL64                          0x3FF0000000000000
#endif // #ifndef ONE_REAL64


// 64位最大的小于1的浮点数一的Bit Pattern
#ifndef MAXIMUM_REAL64_LESS_THAN_ONE
// 0x3FEFFFFFFFFFFFFF
#define MAXIMUM_REAL64_LESS_THAN_ONE        0.99999999999999989
#endif // #ifndef MAXIMUM_REAL64_LESS_THAN_ONE


// 64位最大的的浮点数的Bit Pattern
#ifndef MAXIMUM_REAL64
// 0x7FEFFFFFFFFFFFFF
#define MAXIMUM_REAL64                      1.797693134862e+308
#endif // #ifndef MAXIMUM_REAL64
