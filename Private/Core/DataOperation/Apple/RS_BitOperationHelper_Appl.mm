// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
// Lib headers
// Self
#include "Core/DataOperation/RS_BitOperationHelper.hpp"


namespace Core
{

    SInt32
    BitOperationHelper::findFSBitPos32 (
        const UInt32 data)
    {
        // ffs()返回0， 如果输入的数据为0
        // 否则one based的位置
        return ffs((SInt32)data) - 1;
    }


    SInt32
    BitOperationHelper::findFSBitPos64 (
        const UInt64 data)
    {
        // ffsll()返回0， 如果输入的数据为0
        // 否则one based的位置
        return ffsll((SInt64)data) - 1;
    }


    SInt32
    BitOperationHelper::findLSBitPos32 (
        const UInt32 data)
    {
        // fls()返回0， 如果输入的数据为0
        // 否则one based的位置
        return fls((SInt32)data) - 1;
    }


    SInt32
    BitOperationHelper::findLSBitPos64 (
        const UInt64 data)
    {
        // flsll()返回0， 如果输入的数据为0
        // 否则one based的位置
        return flsll((SInt64)data) - 1;
    }

} // namespace Core

#endif // #if defined(__APPLE__)
