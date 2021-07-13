#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_Pnt3D.hpp"


namespace Core
{

    struct Pnt3DHelper
    {
        // 使用提供的坐标生产一个点
        static
        Pnt3D
        make (
            const Real32 x,
            const Real32 y,
            const Real32 z);

        // 计算P1到P2之间得距离
        static
        Real32
        distance (
            const Pnt3D p1,
            const Pnt3D p2);

        // 计算P1到P2之间得距离平方
        static
        Real32
        distance_squared (
            const Pnt3D p1,
            const Pnt3D p2);

    };

} // namespace Core
