// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/DataType/RS_Vec3DHelper.hpp"
// Self
#include "Core/DataType/RS_Pnt3DHelper.hpp"


namespace Core
{

    Pnt3D
    Pnt3DHelper::make (
        const Real32 x,
        const Real32 y,
        const Real32 z)
    {
        Pnt3D _new_pnt3d;
        _new_pnt3d.x = x;
        _new_pnt3d.y = y;
        _new_pnt3d.z = z;

        return _new_pnt3d;
    }


    Real32
    Pnt3DHelper::distance (
        const Pnt3D p1,
        const Pnt3D p2)
    {
        // 计算从P1到P2的向量：P1 ---> P2
        const Vec3D _diff = Vec3DHelper::fromP1ToP2(p1, p2);
        return _diff.length();
    }


    Real32
    Pnt3DHelper::distance_squared (
        const Pnt3D p1,
        const Pnt3D p2)
    {
        // 计算从P1到P2的向量：P1 ---> P2
        const Vec3D _diff = Vec3DHelper::fromP1ToP2(p1, p2);
        return _diff.length_squared();
    }

} // namespace Core
