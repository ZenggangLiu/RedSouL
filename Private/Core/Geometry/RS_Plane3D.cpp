// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/Collision/RS_CollisionRec.hpp"
#include "Core/DataType/RS_Norm3DHelper.hpp"
// Self
#include "Core/Geometry/RS_Plane3D.hpp"


namespace Core
{

    Bool
    Plane3D::hit (
        const Ray3D    ray,
        Real32 &       min_t,
        CollisionRec & hit) const
    {
        // Ray   := O + t*d
        // Plane := P dot n - V dot n = 0
        //
        // 所有如果相交:
        // (O + t*d) dot n - V dot n = 0
        // --> O dot n - V dot n + t*(d dot n) = 0
        // --> t*(d dot n) = V dot n - O dot n
        // --> t = (-neg_distance - O dot n)/(d dot n)
        //
        const Real32 _o_dot_n = Norm3DHelper::dot(*(Vec3D*)&ray.org, normal);
        const Real32 _d_dot_n = Norm3DHelper::dot(          ray.dir, normal);
        // NOTE: 如果(d dot n)近似等于0，则t将为正无穷。此时逻辑依旧成立
        const Real32 _hit_t   = (-neg_distance - _o_dot_n) / _d_dot_n;
        if ((_hit_t > epsilon) && (_hit_t < min_t))
        {
            hit.normal = normal;
            hit.t = _hit_t;
            min_t = _hit_t;
            return true;
        }
        else
        {
            return false;
        }
    }

} // namespace Core
