#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

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

        // 创建原点
        Pnt3D ();

        Pnt3D (
            const Real32 x,
            const Real32 y,
            const Real32 z);
    };


    // 原点：< 0, 0, 0 >
//    constexpr Pnt3D ORIGIN(0, 0, 0);

} // namespace Core
