#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_Pnt3D.hpp"
#include "Core/DataType/RS_Vec3D.hpp"


namespace Core
{

    // 3D Ray: P(t) := O + t*D, t为任意数值[-INF, +INF]
    struct Ray3D
    {
        // 起始点O
        Pnt3D org;
        // 方向向量D
        Vec3D dir;
    };

} // namespace Core
