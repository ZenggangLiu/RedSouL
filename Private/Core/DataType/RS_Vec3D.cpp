// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/Mathe/RS_FloatPointFuncs.hpp"
#include "Core/Mathe/RS_MatheFuncs.hpp"
#include "core/DataType/RS_Vec3DHelper.hpp"
// Self
#include "Core/DataType/RS_Vec3D.hpp"


namespace Core
{

        Real32
        Vec3D::operator[] (
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
        Vec3D::operator[] (
            const UInt32 idx)
        {
            // 产生如下代码：
            // lea         rax,[rcx+rax*4]
            // - rcx storing this
            // - rax storing idx
            RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
            return e[idx];
        }


        Real32
        Vec3D::dot (
            const Vec3D v2) const
        {
            // v1 dot v2 = |v1|*|v2|*cos(theta), theta 为v1, v2之间的夹角
            // dot product properties:
            // - (u dot v) = (v dot u)
            // - (s*u dot v) = s*(u dot v)
            // - (u dot (v + w)) = (u dot v) + (u dot w)
            return x*v2.x + y*v2.y + z*v2.z;
        }


        Vec3D
        Vec3D::cross (
            const Vec3D v2) const
        {
            // 使用Real64来避免浮点数错误：
            // - 例如计算两个相似的数值的差
            const Real64 _ax = x, _ay = y, _az = z;
            const Real64 _bx = v2.x, _by = v2.y, _bz = v2.z;
            const Real64 _x_val = _ay*_bz - _az*_by;
            const Real64 _y_val = _az*_bx - _ax*_bz;
            const Real64 _z_val = _ax*_by - _ay*_bx;

            return Vec3DHelper::make(_x_val, _y_val, _z_val);
        }


        void
        Vec3D::normalize ()
        {
            const Real32 _rec_length = Mathe::reciprocal(length());
            x *= _rec_length;
            y *= _rec_length;
            z *= _rec_length;
        }


        Real32
        Vec3D::length () const
        {
            return Mathe::squaredRoot(length_squared());
        }


        Real32
        Vec3D::length_squared () const
        {
            // dot(v, v)
            return dot(*this);
        }

} // namespace Core
