// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
// Self
#include "Core/DataOperation/RS_BitOperationHelper.hpp"


namespace Core
{

    UInt64
    BitOperationHelper::makeBitsSetRange (
        const UInt8 start_pos,
        const UInt8 end_pos)
    {
        RUNTIME_ASSERT(start_pos <= 63 && end_pos <= 63 && start_pos <= end_pos,
                       "position is out of range");
        if (end_pos < 63)
        {
            // ((1ull << end_pos) - (1ull << start_pos))无法设置最高位(第63位)为1
            return (1ull << (end_pos + 1)) - (1ull << start_pos);
        }
        else
        {
            return ((1ull << end_pos) - (1ull << start_pos)) | 0x8000000000000000ull;
        }
    }


    Bool
    BitOperationHelper::isBitSet32 (
        const UInt32 data,
        const UInt8  pos)
    {
        return data & (1u << pos);
    }


    Bool
    BitOperationHelper::isBitSet64 (
        const UInt64 data,
        const UInt8  pos)
    {
        return data & (1ull << pos);
    }

} // namespace Core
