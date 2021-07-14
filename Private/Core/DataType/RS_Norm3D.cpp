// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
// Self
#include "Core/DataType/RS_Norm3D.hpp"


namespace Core
{

        Real32
        Norm3D::operator[] (
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
        Norm3D::operator[] (
            const UInt32 idx)
        {
            // 产生如下代码：
            // lea         rax,[rcx+rax*4]
            // - rcx storing this
            // - rax storing idx
            RUNTIME_ASSERT(idx >= 0 && idx < DIMENSION, "Index is out of range");
            return e[idx];
        }

} // namespace Core
