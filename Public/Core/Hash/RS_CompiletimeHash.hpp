#pragma once


#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{
    // 在constexpr定义中只可以使用如下操作:
    // - conditional operator: cond ? A : B
    // - 递归函数
    // - 不能使用CAST。所以所有函数都使用const char *const
    //   由于UTF8定义为unsigned char. 但是text literal的类型为const char[].
    //   如果使用cast，我们将得到如下错误：
    //   cast that performs the conversions of a reinterpret_cast is not allowed
    //   in a constant expression

    constexpr UInt32 MURMUR3_C1 = 0xcc9e2d51;
    constexpr UInt32 MURMUR3_C2 = 0x1b873593;

    constexpr UInt32
    murmur3_add (
        const UInt32 x,
        const SInt32 add)
    {
        return x + add;
    }


    constexpr UInt32
    murmur3_getblock32 (
        const char *const buffer,
        const SInt32      i)
    {
        return UInt32(buffer[i*4+0])        |
               UInt32(buffer[i*4+1]) << 8   |
               UInt32(buffer[i*4+2]) << 16  |
               UInt32(buffer[i*4+3]) << 24;
    }


    constexpr UInt32
    murmur3_mult (
        const UInt32 x,
        const SInt32 mult)
    {
        return x * mult;
    }


    constexpr UInt32
    murmur3_rotl32 (
        const UInt32 x,
        const SInt8 r )
    {
        return (x << r) | (x >> (32 - r));
    }

    constexpr UInt32
    murmur3_downshift_xor (
        const UInt32 x,
        const SInt32 downshift)
    {
        return x ^ (x >> downshift);
    }


    // v1 ^ v2
    constexpr UInt32
    murmur3_xor (
        const UInt32 v1,
        const SInt32 v2)
    {
        return v1 ^ v2;
    }


    constexpr UInt32
    murmur3_fmix32 (
         const UInt32 x)
    {
        return murmur3_downshift_xor(
                              murmur3_mult(
                                    murmur3_downshift_xor(
                                                   murmur3_mult(
                                                         murmur3_downshift_xor(x, 16),
                                                         0x85ebca6b),
                                                   13),
                                    0xc2b2ae35),
                              16);
    }


    // 在编译时获得字符串的长度
    constexpr UInt32
    static_text_length (
        const char *const data,
        const UInt32      idx = 0)
    {
        return (data[idx] == '\0') ? idx : static_text_length(data, idx+1);
    }


    constexpr UInt32
    murmur3_body (
       const UInt32 k1,
       const UInt32 block)
    {
        return murmur3_add(
                    murmur3_mult(
                          murmur3_rotl32(
                                  murmur3_xor(k1,
                                       murmur3_mult(
                                             murmur3_rotl32(
                                                     murmur3_mult(
                                                           block,
                                                           MURMUR3_C1),
                                                     15),
                                             MURMUR3_C2)
                                       ),
                                  13),
                          5),
                    0xe6546b64);
    }


    constexpr UInt32
    murmur3_body_rec (
        const UInt32      k1,
        const char *const blocks,
        const SInt32      i)
    {
        return ((i < 0) ?
            murmur3_body_rec(murmur3_body(k1, murmur3_getblock32(blocks, i)), blocks, i + 1) : k1);
    }


    constexpr UInt32
    murmur3_tail1 (
         const UInt32      k1,
         const char *const tail)
    {
        return murmur3_mult(murmur3_rotl32(murmur3_mult(murmur3_xor(k1, tail[0]),
                                   MURMUR3_C1),
                             15),
                     MURMUR3_C2);
    }


    constexpr UInt32
    murmur3_tail (
        const UInt32      k1,
        const char *const tail,
        const SInt32      buffer_len)
    {
        return ((buffer_len & 3) == 3) ?    // case 3:
            murmur3_xor(k1, murmur3_tail1(murmur3_xor(tail[2] << 16, tail[1] << 8), tail)) :
            ((buffer_len & 3) == 2) ?       // case 2:
                murmur3_xor(k1, murmur3_tail1(tail[1] << 8, tail)) :
                ((buffer_len & 3) == 1) ?   // case 1:
                    murmur3_xor(k1, murmur3_tail1(0, tail)) :
                    k1;
    }


    constexpr UInt32
    murmur3_final (
        const UInt32 tail,
        const SInt32 buffer_len)
    {
        // h1 ^= buffer_len;
        // fmix32(h1);
        return murmur3_fmix32(murmur3_xor(tail, buffer_len));
    }


    // 编译的时候计算一个字符串32位Hash
    constexpr UInt32
    static_murmurhash3_x86_32 (
        const UInt32      seed,
        const char *const buffer,
        const SInt32      buffer_len)
    {
        return murmur3_final(murmur3_tail(murmur3_body_rec(seed,
                                      (buffer + (buffer_len/4)*4),
                                      -1*(buffer_len/4)),
                            (buffer + ((buffer_len/4)*4)),
                            buffer_len),
                      buffer_len);
    }
}


// NOTE: constexpr函数是否在编译时候进行计算，取决于他的使用方式：
// 1. 编译时进行计算：初始化一个常量(const, constexpr)，或者初始化Non-Type Template Argument
// 2. 运行时进行计算：初始化其它非常量

// 所以为了迫使编译器计算Hash，我们定义如下一个变量：
template < UInt32 Cal_Hash_Value >
struct CalcHash32 { static constexpr UInt32 value = Cal_Hash_Value; };
#if !defined(COMPILE_TIME_HASH)
    #define COMPILE_TIME_HASH(seed, text)                                                          \
    CalcHash32<Core::static_murmurhash3_x86_32(seed, text, Core::static_text_length(text))>::value
#endif // #if !defined(COMPILE_TIME_HASH)
