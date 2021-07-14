#pragma once


// System headers
// std::abs(),  std::sqrt(), std::sin(),   std::cos()
// std::asin(), std::acos(), std::atan2(), std::floor()
#include <cmath>
#include <tuple>
// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // MARK: - 函数定义
    struct Mathe
    {

        // 求给定数值的绝对值
        static
        UInt32
        absolute (
            const SInt32 val);

        static
        Real32
        absolute (
            const Real32 val);

        // 求给定数值的绝对值
        static
        Real64
        absolute (
            const Real64 val);

        // 求给定数值的倒数：1/v
        static
        Real32
        reciprocal (
           const Real32 val);

        static
        Real64
        reciprocal (
           const Real64 val);

        // 求32位浮点数的方根
        // 同std::sqrt()相同精度，但是快34%
        static
        Real32
        squared_root (
            const Real32 val);

        // 求32位浮点数的方根：快速版
        // 比std::sqrt()快71%: Quake 3 的方法
        static
        Real32
        squared_root_fast (
            const Real32 val);

        static
        Real64
        squared_root (
            const Real64 val);

        /* 此函数没有进行测试
        // 求ArcCosine
        //
        // @param[in] cos_val
        //      cosine的数值: [-1, +1]
        // @return
        //      相应的角度: [0, PI]
        // NOTE: 此函数的误差为小于等于+/-6.7e-5
        static
        Real32
        arccosine (
            const Real32 cos_val);

        static
        Real64
        arccosine (
            const Real64 cos_val);

        // 求ArcSine
        //
        // @param[in] sin_val
        //      sine的数值: [-1, +1]
        // @return
        //      相应的角度: [-PI/2, +PI/2]
        static
        Real32
        arcsine (
            const Real32 sin_val);

        static
        Real64
        arcsine (
            const Real64 sin_val);

        // 使用渐进方法来求平均值:
        // - A_{n+1} = A_{n} + \frac{v_{n+1}-A_{n}}{n+1}
        //
        // @param[in] curent_avg
        //      当前的平均值: A_{n}
        // @param[in] new_sample
        //      新的采样数据: v_{n+1}
        // @param[in] total_samples
        //      当前一共进行的采样数目: n+1
        static
        Real32
        average (
            const Real32 curent_avg,
            const Real32 new_sample,
            const UInt32 total_samples);

        static
        Real64
        average (
            const Real64 curent_avg,
            const Real64 new_sample,
            const UInt32 total_samples);
         */
        // Clamps给定的数值到给定的区间: [min, max]
        template < typename T >
        INLINE_FUNCTION
        static
        T
        clamp (
           const T val,
           const T min,
           const T max);
        /*
        // 求给定弧度的Cosine值
        //
        // uses sine() to do the calculation:
        // maximal error: 0.00109060109
        // average error: 0.000505
        // @param[in] angle_in_rads
        //     The angle expressed in radians
        static
        Real32
        cosine (
            const Real32 angle_in_rads);

        static
        Real64
        cosine (
            const Real64 angle_in_rads);

        // 求a/b的商的Round Up
        template < typename T >
        INLINE_FUNCTION
        static
        T
        divideUp (
            const T a,
            const T b);

        // 求给定数值的小数部分
        static
        Real32
        fraction (
            const Real32 val);

        static
        Real64
        fraction (
            const Real64 val);
         */
        // 求A，B数值中的最大值
        template < typename T >
        INLINE_FUNCTION
        static
        T
        maximum (
           const T a,
           const T b);

        // 求A，B数值中的最大值和最小值
        //
        // @return
        //      (min, max)
        template < typename T >
        INLINE_FUNCTION
        static
        std::tuple<T, T>
        maxMinimum (
            const T a,
            const T b);

        // 求A，B数值中的最小值
        template < typename T >
        INLINE_FUNCTION
        static
        T
        minimum (
            const T a,
            const T b);

        /* 此次的函数没有进行测试
        // 使用B的倍数来表示A: RoundUp(a/b) * b
        template < typename T >
        INLINE_FUNCTION
        static
        T
        multipleUp (
            const T a,
            const T b);

        // 求给定弧度的Sine值
        // 最大误差：0.00109060109
        // 平均误差：0.000505
        //
        // @param[in] angle_in_rads
        //      角度的弧度表示: [0, 2PI]
        static
        Real32
        sine (
            const Real32 angle_in_rads);

        static
        Real64
        sine (
            const Real64 angle_in_rads);

        // 交换A与B
        template < typename T >
        INLINE_FUNCTION
        static
        void
        swap (
            T & a,
            T & b);*/
    };

} // namespace Core


// MARK: - 函数实现
namespace Core
{

    template < typename T >
    INLINE_FUNCTION
    T
    Mathe::clamp (
        const T val,
        const T min,
        const T max)
    {
        return minimum(maximum(val, min), max);
    }

    /* 此次的函数没有进行测试
    // SInt8/16/32/64, UInt8/16/32/64
    template < typename T >
    INLINE_FUNCTION
    T
    Mathe::divideUp (
        const T a,
        const T b)
    {
        // (a + b-1)/b = (a-1)/b + 1
        return (a - (T)1)/b + (T)1;
    }


    // Real32
    template <>
    INLINE_FUNCTION
    Real32
    Mathe::divideUp (
        const Real32 a,
        const Real32 b)
    {
        return (Real32)divideUp((UInt32)a, (UInt32)b);
    }*/


    template < typename T >
    INLINE_FUNCTION
    T
    Mathe::maximum (
        const T a,
        const T b)
    {
        return a > b ? a : b;
    }


    template < typename T >
    INLINE_FUNCTION
    std::tuple<T, T>
    Mathe::maxMinimum (
        const T a,
        const T b)
    {
        return a < b ? std::make_tuple(a, b) : std::make_tuple(b, a);
    }


    template < typename T >
    INLINE_FUNCTION
    T
    Mathe::minimum (
        const T a,
        const T b)
    {
        return a < b ? a : b;
    }


    /* 此次的函数没有进行测试
    template < typename T >
    INLINE_FUNCTION
    T
    Mathe::multipleUp (
        const T a,
        const T b)
    {
        return ((a + b - (T)1) / b) * b;
    }


    template < typename T >
    INLINE_FUNCTION
    void
    Mathe::swap (
        T &a,
        T &b)
    {
        const T _temp = a;

        a = b;
        b = _temp;
    }*/

} // namespace Core
