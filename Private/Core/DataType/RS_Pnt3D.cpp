// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/Mathe/RS_FloatPointFuncs.hpp"
#include "Core/Mathe/RS_MatheFuncs.hpp"
// Self
#include "Core/DataType/RS_Pnt3D.hpp"


namespace Core
{

        Real32
        Pnt3D::operator[] (
            const UInt32 idx) const
        {
            // 产生如下代码：
            // movss       xmm0,dword ptr [rcx+rax*4]
            // - rcx storing this
            // - rax storing idx
            RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
            return e[idx];
        }


        Real32&
        Pnt3D::operator[] (
            const UInt32 idx)
        {
            // 产生如下代码：
            // lea         rax,[rcx+rax*4]
            // - rcx storing this
            // - rax storing idx
            RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
            return e[idx];
        }


        Bool
        Pnt3D::operator== (
            const Pnt3D p) const
        {
            return FloatPoint::equal(x, p.x)    &&
                   FloatPoint::equal(y, p.y)    &&
                   FloatPoint::equal(z, p.z);
        }


        Bool
        Pnt3D::operator!= (
            const Pnt3D p) const
        {
            return !operator==(p);
        }


        Pnt3D
        Pnt3D::operator+ (
            const Pnt3D p) const
        {
            return Pnt3D{x + p.x, y + p.y, z + p.z};
        }


        Pnt3D
        Pnt3D::operator+ (
            const Vec3D v) const
        {
            return Pnt3D{x + v.x, y + v.y, z + v.z};
        }


        Pnt3D
        Pnt3D::operator- (
            const Vec3D v) const
        {
            return Pnt3D{ x - v.x, y - v.y, z - v.z };
        }


        Pnt3D
        Pnt3D::operator* (
            const Real32 s) const
        {
            return Pnt3D{s*x , s*y, s*z};
        }


        Pnt3D
        Pnt3D::operator/ (
            const Real32 s) const
        {
            const Real32 _reciprocal = Mathe::reciprocal(s);
            return Pnt3D{_reciprocal*x , _reciprocal*y, _reciprocal*z};
        }

} // namespace Core
