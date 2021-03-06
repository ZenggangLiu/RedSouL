// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
// Self
#include "Core/DataOperation/RS_EndiannessHelper.hpp"


namespace Core
{

    // 初始化一个UInt32：0x04030201. 如果内存字节存储顺序：
    // - 0x01 0x02 0x03 0x04则为Little Endianness
    // - 0x04 0x03 0x02 0x01则为Big Endianness
    static const
    union
    {
        UInt32 ui32Data;
        UInt8  ui8Data[4];
    } ENDIANESS_CHECKING_DATA = { 0x04030201 };


    Bool
    EndianHelper::isBigEndian ()
    {
        return (ENDIANESS_CHECKING_DATA.ui8Data[0] == 0x04);
    }

} // namespace Core
