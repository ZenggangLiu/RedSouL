#pragma once


// Lib headers
// Common
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // 计算一个32位整数的32位Hash
    UInt32
    hash32 (
        const UInt32 key);

    // 计算一个64位整数的32位Hash
    UInt32
    hash32 (
        const UInt64 key);

    // --- Hash Function(on buffer and seed) checking Result --- //
    // --- Hash Collision Check on 2^32 different uint32 Numbers --- //
    // Collision rate:
    // - hash32: (xxHash64 --> cast to 32) has NO collisions
    // - xxHash64 or CityHash64:           has NO collision
    // - std::hash<uint32>:                has 3 collisions
    // - std::hash<uint64>:                has NO collision
    //
    // Performance:
    // xxHash64:                6.86 secs, 1.5 times slower than CityHash64
    // CityHash64:              4.67 secs
    // std::hash<uint32>:       5.11 secs
    // std::hash<uint64>:       10.30 secs, 1.5 times slower than xxHash64
    //
    //
    // --- Hash Collision Check on 589660 files in Documents Folder --- //
    //
    // Collision rate:
    // - hash32: (xxHash64 --> cast to 32) has 32 collisions
    // - xxHash64 or CityHash64:           has NO collision
    // - std::hash<std::wstring>:          has 41 collisions
    //
    //
    // Performance:
    // xxHash64:                0.036573 sec
    // CityHash64:              0.041216 sec, 1.13 time slower than xxHash64
    // std::hash<std::wstring>: 0.142131 sec, 3.89 time slower than xxHash64
    // MurmurHash128:           0.055775 sec
    //

    // 计算一个Buffer的32位Hash
    //
    // @param[in] seed
    //      The randomized value used to get a randomized hash code for the same data input
    // @param[in] buffer
    //      The data buffer
    // @param[in] buffer_len
    //      The length of data buffer, which MUST be a multiple of 4.
    //      NOTE: If key_len is NOT a multiple of 4, it will be rounded down and
    //            all rest values will be discarded.
    // @return
    //     The 32bits hash code
    UInt32
    hash32 (
        const UInt32       seed,
        const UInt8 *const buffer,
        const SInt32       buffer_len);


    // 计算一个Bufer的64位Hash
    //
    // @param[in] seed
    //      The randomized value used to get a randomized hash code for the same data input
    // @param[in] buffer
    //      The data buffer
    // @param[in] buffer_len
    //      The length of data buffer, which MUST be a multiple of 8.
    //      NOTE: If key_len is NOT a multiple of 8, it will be rounded down and
    //            all rest values will be discarded.
    // @return
    //      The 64bits hash code
    UInt64
    hash64 (
        const UInt32       seed,
        const UInt8 *const buffer,
        const SInt32       buffer_len);


    // 计算一个Bufer的128位Hash
    //
    // @param[in] seed
    //      The randomized value used to get a randomized hash code for the same data input
    // @param[in] buffer
    //      The data buffer
    // @param[in] buffer_len
    //      The length of data buffer, which MUST be a multiple of 16.
    //      NOTE: If key_len is NOT a multiple of 16, it will be rounded down and
    //            all rest values will be discarded.
    // @param[out] low
    //      The lower 64bits part of the 128bit hash code
    // @param[out] high
    //      The higher 64bits part of the 128bits hash code
    void
    hash128 (
        const UInt32       seed,
        const UInt8 *const buffer,
        const SInt32       buffer_len,
        UInt64 &           low,
        UInt64 &           high);

} // namespace Core
