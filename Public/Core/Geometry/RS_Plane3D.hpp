#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_Norm3D.hpp"
#include "Core/DataType/RS_Vec4D.hpp"
#include "Core/Collision/RS_Ray3D.hpp"


namespace Core
{

    struct CollisionRec;


    //             ^ n
    //             |
    //        +----|----------+
    //       /     |         /
    //      /  *P  |        /
    //     /       V       /
    //    /               /
    //   /               /
    //  +---------------+
    // 平面的Equation为：(P-V) dot n = 0, 其中P为平面上任意一点， V为平面上的一个特定点
    // 平面方程可以改写为：(Px, Py, Pz, 1) dot (nx, ny, nz, -V dot n) = 0
    //
    struct Plane3D
    {
        union
        {
            struct
            {
                // 平面的法线
                Norm3D  normal;
                // -(V dot n)
                Real32  neg_distance;
            };

            // 平面方程系数向量
            Vec4D       coefficient;
        };

        // 最小的认为相交的t的数值：t在[epsilon, +INF]
        Real32          epsilon;

        // 检查此平面是否与给定Ray相交
        //
        // @param[in]  ray
        // @param[in]  min_t
        //      当前最小的t
        // @param[out] hit
        //      如果有Hit， collision信息
        Bool
        hit (
            const Ray3D    ray,
            Real32 &       min_t,
            CollisionRec & hit) const;
    };

} // namespace Core
