// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/Collision/RS_CollisionRec.hpp"
#include "Core/DataType/RS_Vec3DHelper.hpp"
#include "Core/Mathe/RS_MatheFuncs.hpp"
// Self
#include "Core/Geometry/RS_Sphere.hpp"


namespace Core
{

    Bool
    Sphere::hit (
        const Ray3D    ray,
        Real32 &       min_t,
        CollisionRec & hit) const
    {
        // Ray    := O + t*d
        // Sphere := (P-C) dot (P-C) - r*r = 0
        //
        // 所有如果相交:
        // (O + t*d - C) dot (O + t*d - C) - r*r = 0
        // 如果使用(a + b) dot (c + d) --> (a dot c) + (a dot d) + (b dot c) + (b dot d)
        // --> (d dot d)t*t + 2[(O-C) dot d]t + (O-C) dot (O-C) - r*r = 0
        // 此为一个t的二次方程：
        //         - b +/- sqrt(b*b - 4ac)
        // --> t = -----------------------
        //                  2a
        // where
        // a := d dot d
        // b := 2[(O-C) dot d]
        // c := (O-C) dot (O-C) - r*r
        //
        // NOTE:
        // 当b*b - 4ac = 0时候，t将只有一个数值：Ray与球体切向相交。
        // 此时的t := -b/2a 
        //
        // C --> O: O - C
        const Vec3D _o_c = Vec3DHelper::fromP1ToP2(center, ray.org);
        // a := d dot d
        const Real32 _a_value = ray.dir.length_squared();
        // b := 2[(O-C) dot d]
        const Real32 _b_value = 2 * Vec3DHelper::dot(_o_c, ray.dir);
        // c := (O-C) dot (O-C) - r*r
        const Real32 _c_value = _o_c.length_squared() - radius*radius;
        // b*b - 4ac
        const Real32 _disc = (_b_value * _b_value) - (4 * _a_value * _c_value);
        if (_disc < 0)
        {
            return false;
        }
        else
        {
            // e := sqrt(b*b - 4ac)
            const Real32 _e_value =  Mathe::squared_root(_disc);
            // denom := 2a
            const Real32 _denom = 2 * _a_value;
            //
            //       b + sqrt(b*b - 4ac)
            // t = - -----------------------
            //                2a
            //
            const Real32 _hit_t = -(_b_value + _e_value) / _denom;
            if ((_hit_t > epsilon) && (_hit_t < min_t))
            {
                hit.normal = normal;
                hit.t = _hit_t;
                min_t = _hit_t;
                tmin = t;
                sr.normal = (temp + t * ray.d) / radius;
                sr.local_hit_point = ray.o + t * ray.d;
                return true;
            }

            //     sqrt(b*b - 4ac) - b
            // t = -----------------------
            //              2a
            t = (_e_value - _b_value) / _denom;
            if ((_hit_t > epsilon) && (_hit_t < min_t))
            {
                hit.normal = normal;
                hit.t = _hit_t;
                min_t = _hit_t;
                tmin = t;
                sr.normal = (temp + t * ray.d) / radius;
                sr.local_hit_point = ray.o + t * ray.d;
                return true;
            }
        }
        return false;
    }

} // namespace Core
