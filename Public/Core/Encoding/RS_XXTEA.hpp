#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // Usage: 包含RS_Codedc.hpp，并使用XXTEA_ENCODE()或者XXTEA_DECODE()
    // NOTE: DECLARE_XXTEA_CODEC在RS_Codec.hpp中定义
#if ((API_BUILD == 1) || (DECLARE_XXTEA_CODEC == 1))
    struct XXTEA
    {
        // 编码给定数据中的内容
        //
        // @param[in] key
        //      编码时使用的Key
        // @param[in] key_size
        //      Key的字节数目
        //      NOTE：最长的Key为16字节(128位)
        // @param[in] data_ptr
        //      原始数据的指针
        // @param[in] data_size
        //      原始数据的长度(以UInt32来衡量)
        //      NOTE：最短的数据长度为2
        static
        void
        encode (
            const UInt8 *const key,
            const UInt32       key_size,
            UInt32 *const      data_ptr,
            const UInt32       data_size);

        // 解码给定数据中的内容
        //
        // @param[in] key
        //      解码时使用的Key
        // @param[in] key_size
        //      Key的字节数目
        //      NOTE：最长的Key为16字节(128位)
        // @param[in] data_ptr
        //      编码数据的指针
        // @param[in] data_size
        //      编码数据的长度(以UInt32来衡量)
        //      NOTE：最短的数据长度为2
        static
        void
        decode (
            const UInt8 *const key,
            const UInt32       key_size,
            UInt32 *const      data_ptr,
            const UInt32       data_size);
    };
#endif // #if ((API_BUILD == 1) || (DECLARE_XXTEA_CODEC == 1))

} // namespace Core
