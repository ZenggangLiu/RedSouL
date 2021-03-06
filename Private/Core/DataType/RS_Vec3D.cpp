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
            return Mathe::squared_root(length_squared());
        }


        Real32
        Vec3D::length_squared () const
        {
            // dot(v, v)
            return Vec3DHelper::dot(*this, *this);
        }

} // namespace Core
