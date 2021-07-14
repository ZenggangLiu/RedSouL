// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
// Self
#include "Core/DataType/RS_Vec3DHelper.hpp"


namespace Core
{

    Vec3D
    Vec3DHelper::make (
        const Real32 x,
        const Real32 y,
        const Real32 z)
    {
        Vec3D _new_vec3d;
        _new_vec3d.x = x;
        _new_vec3d.y = y;
        _new_vec3d.z = z;

        return _new_vec3d;
    }


    Vec3D
    Vec3DHelper::fromP1ToP2 (
        const Pnt3D p1,
        const Pnt3D p2)
    {
        Vec3D _new_vec3d;
        _new_vec3d.x = p2.x - p1.x;
        _new_vec3d.y = p2.y - p1.y;
        _new_vec3d.z = p2.z - p1.z;
        return _new_vec3d;
    }


    Real32
    Vec3DHelper::dot (
        const Vec3D v1,
        const Vec3D v2)
    {
        // v1 dot v2 = |v1|*|v2|*cos(theta), theta为v1, v2之间的夹角
        // dot product properties:
        // - (u dot v) = (v dot u)
        // - (s*u dot v) = s*(u dot v)
        // - (u dot (v + w)) = (u dot v) + (u dot w)
        return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    }


    Vec3D
    Vec3DHelper::cross (
        const Vec3D v1,
        const Vec3D v2)
    {
        // |v1 x v2| = |v1|*|v2|*sin(theta), theta为v1, v2之间的夹角
        // |v1|为v1向量的长度
        //
        // 对于一个以v1, v2为边的四边形：
        //     +------------+
        // v1 /|           /
        //   / | h        /
        //  +--+---------+
        //        v2
        // 它的面积为：|v2| * h with h := |v1|*sin(theta)
        // 因此这个四边形的面积为|v1 x v2|：v1与v2的cross product的绝对值

        const Real32 _x_comp = (v1.y * v2.z) - (v1.z * v2.y);
        const Real32 _y_comp = (v1.z * v2.x) - (v1.x * v2.z);
        const Real32 _z_comp = (v1.x * v2.y) - (v1.y * v2.x);
        return Vec3DHelper::make(_x_comp, _y_comp, _z_comp);
        /*
        --- GPU没有Real64支持
        // 使用Real64来避免浮点数错误：
        // - 例如计算两个相似的数值的差
        const Real64 _x_comp = (v1.y * v2.z) - (v1.z * v2.y);
        const Real64 _y_comp = (v1.z * v2.x) - (v1.x * v2.z);
        const Real64 _z_comp = (v1.x * v2.y) - (v1.y * v2.x);
        return Vec3DHelper::make((Real32)_x_comp, (Real32)_y_comp, (Real32)_z_comp);
        */
    }


    Vec3D
    Vec3DHelper::normalized (
        const Vec3D v)
    {
        Vec3D _normalized = Vec3DHelper::make(v.x, v.y, v.z);
        _normalized.normalize();
        return _normalized;
    }

} // namespace Core
