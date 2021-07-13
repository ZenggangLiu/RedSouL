// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
// Self
#include "Core/DataType/RS_Vec3DHelper.hpp"


namespace Core
{

    Vec3D
    make (
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

} // namespace Core
