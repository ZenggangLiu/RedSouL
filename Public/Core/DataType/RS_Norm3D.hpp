#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // NOTE:
    // Norm3D定义为POD/Aggregate类型：
    // - NOT constructor, NO copy constructor, NO operator=()
    // - The default opeartor=() will use memcpy() to do byte by byte data copy
    //
    // 法线向量：单位长度，与切面垂直
    struct alignas(16) Norm3D
    {
        static constexpr UInt32 DIMENSION = 3;

        union
        {
            struct
            {
                Real32  x;
                Real32  y;
                Real32  z;
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
    };


    // x轴
    constexpr Norm3D X_AXIS     = Norm3D{1, 0, 0};
    // y轴
    constexpr Norm3D Y_AXIS     = Norm3D{0, 1, 0};
    // z轴
    constexpr Norm3D Z_AXIS     = Norm3D{0, 0, 1};

} // namespace Core
