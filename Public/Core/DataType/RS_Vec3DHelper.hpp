#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_Pnt3D.hpp"
#include "Core/DataType/RS_Vec3D.hpp"


namespace Core
{

    struct Vec3DHelper
    {

        // 使用提供的坐标生产一个向量
        static
        Vec3D
        make (
            const Real32 x,
            const Real32 y,
            const Real32 z);

        // 求从点P1到P2的向量
        static
        Vec3D
        fromP1ToP2 (
            const Pnt3D p1,
            const Pnt3D p2);

        // 计算v1 dot v2
        static
        Real32
        dot (
            const Vec3D v1,
            const Vec3D v2);

        // 计算v1 cross v2
        static
        Vec3D
        cross (
            const Vec3D v1,
            const Vec3D v2);

        static
        Vec3D
        normalized (
            const Vec3D v);
    };

} // namespace Core
