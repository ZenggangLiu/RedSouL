#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_Norm3D.hpp"
#include "Core/DataType/RS_Vec3D.hpp"


namespace Core
{

    struct Norm3DHelper
    {

        // 使用提供的坐标生产一个法线向量
        static
        Norm3D
        make (
            const Real32 x,
            const Real32 y,
            const Real32 z);

        // 计算n dot v
        static
        Real32
        dot (
            const Norm3D n,
            const Vec3D  v);

        // 计算v dot n
        static
        Real32
        dot (
            const Vec3D  v,
            const Norm3D n);

        // 计算n1 cross n2
        static
        Norm3D
        cross (
            const Norm3D n1,
            const Norm3D n2);
    };

} // namespace Core
