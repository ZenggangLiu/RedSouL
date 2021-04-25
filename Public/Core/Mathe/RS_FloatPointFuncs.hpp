#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/Mathe/RS_MatheDefs.hpp"


namespace Core
{

    struct FloatPoint
    {
        // 获得给定16位浮点数的Bit Pattern
        static
        UInt16
        toUInt16 (
            const Real16 data);

        // 获得给定32位浮点数的Bit Pattern
        static
        UInt32
        toUInt32 (
            const Real32 data);

        // 获得给定64位浮点数的Bit Pattern
        static
        UInt64
        toUInt64 (
            const Real64 data);

        /* 此次的函数没有进行测试
        // 检测给定的两个浮点数A,B是否近似相等
        //
        // @param[in] epsilon
        //      容忍程度
        static
        Bool
        equals (
            const Real32 a,
            const Real32 b,
            const Real32 epsilon = (Real32)EPSILON);

        static
        Bool
        equals (
            const Real64 a,
            const Real64 b,
            const Real64 epsilon = EPSILON);

        // 使用16Bit Pattern创建一个浮点数
        static
        Real16
        fromUInt16 (
            const UInt16 bit_pattern);*/

        // 使用32Bit Pattern创建一个浮点数
        static
        Real32
        fromUInt32 (
            const UInt32 bit_pattern);

        // 使用64Bit Pattern创建一个浮点数
        static
        Real64
        fromUInt64 (
            const UInt64 bit_pattern);

    /* 此次的函数没有进行测试
        // 检测给定的浮点数是否为无穷: 正/负无穷
        static
        Bool
        isInfinite (
            const Real16 data);

        static
        Bool
        isInfinite (
            const Real32 data);

        static
        Bool
        isInfinite (
            const Real64 data);

        // 检查给定的浮点数是非为NaN: Not a Number
        static
        Bool
        isNan (
            const Real16 data);

        static
        Bool
        isNan (
            const Real32 data);

        static
        Bool
        isNan (
            const Real64 data);

        // 检测给定的浮点数是否为负无穷
        static
        Bool
        isNegativeInfinite (
            const Real16 data);

        static
        Bool
        isNegativeInfinite (
            const Real32 data);

        static
        Bool
        isNegativeInfinite (
            const Real64 data);

        // 检测给定的浮点数是否为负零
        static
        Bool
        isNegativeZero (
            const Real16 data);

        static
        Bool
        isNegativeZero (
            const Real32 data);

        static
        Bool
        isNegativeZero (
            const Real64 data);

        // 检测给定的浮点数是否为正无穷
        static
        Bool
        isPositiveInfinite (
            const Real16 data);

        static
        Bool
        isPositiveInfinite (
            const Real32 data);

        static
        Bool
        isPositiveInfinite (
            const Real64 data);

        // 检测给定的浮点数是否为正零
        static
        Bool
        isPositiveZero (
            const Real16 data);

        static
        Bool
        isPositiveZero (
            const Real32 data);

        static
        Bool
        isPositiveZero (
            const Real64 data);

        // 检测给定的浮点数是否为零: 正/负零
        static
        Bool
        isZero (
            const Real16 data);

        static
        Bool
        isZero (
            const Real32 data);

        static
        Bool
        isZero (
            const Real64 data);

        // 获得给定浮点数的下一个数值
        static
        Real32
        nextFloat (
            const Real32 data);

        static
        Real64
        nextFloat (
            const Real64 data);

        // 获得给定浮点数的上一个数值
        static
        Real32
        prevFloat (
            const Real32 data);

        static
        Real64
        prevFloat (
            const Real64 data);*/
    };

} // namespace Core
