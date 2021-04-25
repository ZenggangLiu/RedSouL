#pragma once


// System headers
// Lib headers


// 定义XXTEA编码/解码函数
#if !defined(DECLARE_XXTEA_CODEC)
#define DECLARE_XXTEA_CODEC             1

#include "Core/Encoding/RS_XXTEA.hpp"
// 编码给定数据中的内容
//
// @param[in] key
//      编码时使用的Key
// @param[in] key_size
//      Key的字节数目
//      NOTE：最长的Key为16字节(128位)
// @param[in] data_ptr
//      原始数据的指针
//      NOTE: 最短的数据长度为2
#define XXTEA_ENCODE(key, key_size, data_ptr)                                                      \
    Core::XXTEA::encode(key, key_size, data_ptr, ARRAY_ITEM_COUNT(data_ptr));

// 解码给定数据中的内容
//
// @param[in] key
//      解码时使用的Key
// @param[in] key_size
//      Key的字节数目
//      NOTE：最长的Key为16字节(128位)
// @param[in] data_ptr
//      编码数据的指针
//      NOTE：最短的数据长度为2
#define XXTEA_DECODE(key, key_size, data_ptr)                                                      \
    Core::XXTEA::decode(key, key_size, data_ptr, ARRAY_ITEM_COUNT(data_ptr));

#endif // #if !defined(DECLARE_XXTEA_CODEC)

