#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_Pnt3D.hpp"
#include "Core/Collision/RS_Ray3D.hpp"


namespace Core
{

    struct CollisionRec;


    //         ^ Y
    //         |    / Z
    //   P + * * * /
    //    * \  |  / *
    //  *    \ | /    *
    //  *      C------*-----> X
    //  *         r   *
    //    *         *
    //       * * *
    // 球体的Equaltion为：|P-C|^2 - r^2 = 0
    // 球体的方程可改写为：(P-C) dot (P-C) - r*r = 0
    //
    struct Sphere
    {
        // 中心点C
        Pnt3D   center;
        // 半径r
        Real32  radius;
        // 最小的认为相交的t的数值：t在[epsilon, +INF]
        Real32  epsilon;

        // 检查此球体是否与给定Ray相交
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
