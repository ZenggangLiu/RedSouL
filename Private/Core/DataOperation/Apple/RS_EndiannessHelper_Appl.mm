// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <libkern/OSByteOrder.h>
// Lib headers
// Self
#include "Core/DataOperation/RS_EndiannessHelper.hpp"


namespace Core
{

    UInt16
    EndianHelper::swapEndian16 (
        const UInt16 data)
    {
        return OSSwapInt16(data);
    }


    UInt32
    EndianHelper::swapEndian32 (
        const UInt32 data)
    {
        return OSSwapInt32(data);
    }


    UInt64
    EndianHelper::swapEndian64 (
        const UInt64 data)
    {
        return OSSwapInt64(data);
    }

} // namespace Core

#endif // #if defined(__APPLE__)
