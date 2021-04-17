#pragma once


#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    struct BitOperationHelper
    {
        // 将从start_pos到end_pos之间的所有位都设置为1
        // LSB --> MSB
        // 0   --> 31
        //
        // @param[in] start_pos
        //      起始位置(zero based, LSB为第0位)
        // @param[in] end_pos
        //      终止位置(zero based, LSB为第0位)
        // @return
        //      64bits pattern: 将从start_pos到end_pos之间的所有位都设置为1
        static
        UInt64
        makeBitsSetRange (
            const UInt8 start_pos,
            const UInt8 end_pos);

        // 检查指定的位置是否为1
        //
        // @param[in] data
        //      32位整数
        // @param[in] pos
        //      数位(zero based)
        // @return
        //      TRUE,   如果指定位置为1
        //      FALSE,  如果指定位置为0
        static
        Bool
        isBitSet32 (
            const UInt32 data,
            const UInt8  pos);

        // Checks if the given bit is set in the given 64bits integer
        static
        Bool
        isBitSet64 (
            const UInt64 data,
            const UInt8  pos);


        // --- OS specific methods START --- //
        // 在给定的32位整数中寻找第一个为1的位置(zero based, LSB为第0位)
        // LSB --> MSB
        // 0   --> 31
        //
        // @param[in] data
        //      32位整数
        // @return
        //      zero-based的位置,  如果此整数不为0
        //      -1,               如果此整数为0
        static
        SInt32
        findFSBitPos32 (
            const UInt32 data);

        // 在给定的64位整数中寻找第一个为1的位置(zero based, LSB为第0位)
        // LSB --> MSB
        // 0   --> 63
        //
        // @param[in] data
        //      64位整数
        // @return
        //      zero-based的位置,  如果此整数不为0
        //      -1,               如果此整数为0
        static
        SInt32 findFSBitPos64 (
            const UInt64 data);

        // 在给定的32位整数中寻找最后一个为1的位置(zero based, LSB为第0位)
        // LSB --> MSB
        // 0   --> 31
        //
        // @param[in] data
        //      32位整数
        // @return
        //      zero-based的位置,  如果此整数不为0
        //      -1,               如果此整数为0
        static
        SInt32
        findLSBitPos32 (
            const UInt32 data);

        // 在给定的64位整数中寻找最后一个为1的位置(zero based, LSB为第0位)
        // LSB --> MSB
        // 0   --> 63
        //
        // @param[in] data
        //      64位整数
        // @return
        //      zero-based的位置,  如果此整数不为0
        //      -1,               如果此整数为0
        static
        SInt32
        findLSBitPos64 (
            const UInt64 data);
        // --- OS specific methods END --- //
    };

} // namespace Core
