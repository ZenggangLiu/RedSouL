#pragma once


#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    struct EndianHelper
    {
        // 检测当前CPU是否使用Big Endianness
        static
        Bool
        isBigEndian ();

        // 交换16位整数的Endianness: 0xAABB <--> 0xBBAA
        static
        SInt16
        swapEndian (
            const SInt16 data);

        // 交换32位整数的Endianness：0xAABBCCDD <--> 0xDDCCBBAA
        static
        SInt32
        swapEndian (
            const SInt32 data);

        // 交换64位整数的Endianness：0xAABBCCDDEEFFGGHH <--> 0xHHGGFFEEDDCCBBAA
        static
        SInt64
        swapEndian (
            const SInt64 data);


        // --- OS specific methods START --- //
        // Implementations are in RS_EndiannessHelper_Appl/Wind/Andr.???
        // 交换16位整数的Endianness: 0xAABB <--> 0xBBAA
        static
        UInt16
        swapEndian(
            const UInt16 data);

        // 交换32位整数的Endianness：0xAABBCCDD <--> 0xDDCCBBAA
        static
        UInt32
        swapEndian(
            const UInt32 data);

        // 交换64位整数的Endianness：0xAABBCCDDEEFFGGHH <--> 0xHHGGFFEEDDCCBBAA
        static
        UInt64
        swapEndian(
            const UInt64 data);
        // --- OS specific methods END --- //
    };

} // namespace Core
