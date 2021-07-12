#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // NOTE:
    // Pnt3D定义为POD/Aggregate类型：
    // - NOT constructor, NO copy constructor, NO operator=()
    // - The default opeartor=() will use memcpy() to do byte by byte data copy
    //
    struct alignas(16) Pnt3D
    {
        static constexpr UInt32 DIMENSION = 3;

        /// elements
        union
        {
            struct
            {
                Real32 x;
                Real32 y;
                Real32 z;
            };
            Real32 e[DIMENSION];
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


    // 原点：< 0, 0, 0 >
    constexpr Pnt3D ORIGIN = Pnt3D{0, 0, 0};

} // namespace Core
