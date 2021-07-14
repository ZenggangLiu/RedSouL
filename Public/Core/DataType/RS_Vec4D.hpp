#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // NOTE:
    // Vec4D定义为POD/Aggregate类型：
    // - NOT constructor, NO copy constructor, NO operator=()
    // - The default opeartor=() will use memcpy() to do byte by byte data copy
    //
    struct alignas(16) Vec4D
    {
        static constexpr UInt32 DIMENSION = 4;

        union
        {
            struct
            {
                Real32  x;
                Real32  y;
                Real32  z;
                Real32  w;
            };
            Real32      e[DIMENSION];
        };

        // 获得给定索引的数值
        Real32
        operator[] (
            const UInt32 idx) const;

        // 获得给定索引的数值
        Real32&
        operator[] (
            const UInt32 idx);

        // normalize此向量
        void
        normalize ();

        // 计算此向量的长度
        Real32
        length () const;

        // 计算此向量的长度的平方
        Real32
        length_squared () const;
    };


    // 零向量
    constexpr Vec4D ZERO_4D     = Vec4D{0,  0,  0, 0};
    // 一向量
    constexpr Vec4D ONE_4D      = Vec4D{1,  1,  1, 1};

} // namespace Core
