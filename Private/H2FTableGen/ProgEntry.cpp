// System headers
#include <iostream>                     // cout
#include <vector>
// Lib headers
#include "Core/DataOperation/RS_BitOperationHelper.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/FileSystem/RS_TextFileWriter.hpp"
#include "H2FConversionTable.hpp"


// 16位-->32位浮点数转换表格中的最大列数
static const UInt32 MAX_H2F_TABLE_COLUMN_COUNT = 6;
// 32位-->16位浮点数转换表格中的最大列数
static const UInt32 MAX_F2H_TABLE_COLUMN_COUNT = 10;


using namespace Core;
using namespace std;


//--------------------------------------------------------------------------------------------------
// TABLE GENERATING FUNCS
//--------------------------------------------------------------------------------------------------
// MARK: - 生成16位浮点数<-->32位浮点数转换表格

// --- Generates Tables to convert Real16 to Real32 --- //
//
// 1. +ZERO: 0x0000 --> 0x0000 0000
// 2. -ZERO: 0x8000 --> 0x8000 0000
// 3. +infinity/+NaN: Hex: [0x7C00, 0x7FFF], Dec: [31744, 32767]
//    (0xFF << 23) | (mantissa << 13) = 0x7F80 0000 | (mantissa << 13)
// 4. -infinity/-NaN: Hex: [0xFC00, 0xFFFF], Dec: [64512, 65535]
//    0x8000 0000 | (0xFF << 23) | (mantissa << 13) = 0xFF80 000 | (mantissa << 13)
// 5. +normalised numbers: Hex: [0x0400, 0x7BFF], Dec: [1024, 31743]
//    (((exponent >> 10) - 15 + 127) << 23) | (mantissa << 13)
// 6. -normalised numbers: Hex: [0x8400, 0xFBFF], Dec: [33792, 64511]
//    0x8000 0000 | (((exponent >> 10) - 15 + 127) << 23) | (mantissa << 13)
// 7. +un-normalised numbers: Hex: [0x0001, 0x03FF], Dec: [1, 1023]
//    -un-normalised numbers: Hex: [0x8400, 0xFBFF], Dec: [33792, 64511]
//    HAVE to normalise the current number:
//    + find the first 1 bit from MSB --> LSB
//    + shift the mantissa to MSB
//    + adjust the exponent accordingly
//    + apply the sign
//
// 32位浮点数的Patern使用如下的方法从16位的数值产生：
// Real32 = Base[S(Real16)|E(Real16)] | Mod[Offset[E(Real16)]+mantissa(Real16)]
// where:
// - S(Real16) is the sign of Real16 and E(Real16) is the exponent of the Real16
// - Mantissa table has 2048 items
// Base table: indexed using the (sign | exponent) of Real16: [0, 0xFFFF]
// - sign | exponent = 0: +ZERO/+Un-normalized numbers: 0, its mod table contains Real32's pattern
// - sign | exponent <- [1, 30]: +normalised numbers:
//   (((exponent >> 10) - 15 + 127) << 23)
//   for instance:
//   + sign | exponent =  1: (1-15+127)  << 23 = 0x3880 0000
//   + sign | exponent = 30: (30-15+127) << 23 = 0x4000 0000
// - sign | exponent = 31: +infinity/+NaN: 0x7F80 0000
// - sign | exponent = 32: -ZERO/-Un-normalized numbers: 0x8000 0000,
//   its mod table contains Real32's pattern
// - sign | exponent <- [33, 62]: -normalised numbers:
//   0x8000 0000 | (((exponent >> 10) - 15 + 127) << 23)
// - sign | exponent = 63: -infinity/-NaN: 0xFF80 0000
// Offset table: indexed using the exponent part of Real16
// - exponent = 0: ZERO/Un-normalised numbers: 0
// - exponent <- [1, 31]: 1024
// Mod table: indexed using the value of mantissa shifted by the offset value
// - +ZERO/-ZERO: 0
// - [1, 1023]: Real32 pattern for the +/-un-normalised numbers
// - [1024, 2047]: i << 13, where i:[0, 3FF]
//
// Base table: 使用16位浮点数的sign(S)以及exponent part(E)来检索
std::vector<UInt32> H2F_BASE_TABLE;
// Offset table: 使用16位浮点数的exponent part(E)来检索: [0, 31]
std::vector<UInt32> H2F_OFFSET_TABLE;
// Mod table: 使用如下方法进行检索: Mod[Offset[exponent(Real16)] + mantissa(Real16)]
std::vector<UInt32> H2F_MOD_TABLE;


// 生成一个16位浮点数到32位的转换表：H2F_CONVERT_TABLE[]
static
void
genOneTable (
    TextFileWriter & io)
{
    // 15 (msb)
    // |
    // | 14  10
    // | |   |
    // | |   | 9        0 (lsb)
    // | |   | |        |
    // X XXXXX XXXXXXXXXX
    // |1| 5 | | <--10->|
    //  s  e        m
    // s: sign bit       : 1bit
    // e: exponential bit: 5bits: the real exponential = e - 15: [-15, +16]
    // m: mantissa bit: 10bits
    //
    // positive numbers: Hex: [0x0000, 0x7FFF], Dec: [    0, 32767]
    // negative numbers: Hex: [0x8000, 0xFFFF], Dec: [32768, 65535]
    //
    // special patterns:
    // 1. +ZERO: Hex: 0x0000, Dec: 0
    //      sign:     0
    //    exponent: 0
    //    mantissa: 0
    //    value: 2^(-15) * (0*2^0 + 0)
    // 2. +Un-normalised numbers: 0.xyz: Hex: [0x0001, 0x03FF], Dec: [1, 1023]
    //    sign:     0
    //    exponent: 0
    //    mantissa: [0x0001, 0x3FF]
    //    value: 2^(-14)*(0 + ...): NOTE: 2^(-14) NOT 2^(-15)
    //    for instance:
    //    - the minimal positive Non-normalized number: 0x0001:
    //      2^(-14)*(0*2^0 + 1*2^(-10)) = 0.000000059604644775390625
    // 3. +normalised numbers: 1.xyz: Hex: [0x0400, 0x7BFF], Dec: [1024, 31743]
    //    sign:     0
    //    exponent: [1, 30]
    //    mantissa: [0x0000, 0x3FF]
    //    value: 2^(-14)*(1 + ...)
    //    for instance:
    //    - the minimal normalised positive number: 0x4000, exponent = 1
    //      2^(-14)*(1*2^0 + 0) = 0.00006103515625
    //    - ONE: 0x3C00, exponent = 15
    //      2^0*(1*2^0 + 0) = 1
    //    - the maximal normalised positive number: 0x7BFF, exponent = 30
    //      2^(15)*(1*2^0 + 1*2^-1 + ... 1 * 2^(-10)) = 2^(15) * 1.9990234375 = 65504
    // 4. +infinity: Hex: 0x7C00, Dec: 31744
    //    exponent: 31
    //    mantissa: 0
    // 5. +NaN: Hex: [0x7C01, 0x7FFF], Dec: [31745, 32767]
    //    exponent: 31
    //    mantissa: [0x0001, 0x3FF]
    // 6. -ZERO: Hex: 0x8000, Dec: 32768
    //      sign:     1
    //      exponent: 0
    //    mantissa: 0
    //    value: -1*2^(-15) * (0*2^0 + 0)
    // 7. -Un-normalised numbers: -0.xyz: Hex: [0x8001, 0x83FF], Dec: [32769, 33791]
    //      sign:     1
    //    exponent: 0
    //    mantissa: [0x0001, 0x3FF]
    //    value: -1*2^(-14)*(0 + ...): NOTE: 2^(-14) NOT 2^(-15)
    // 8. -normalised numbers: -1.xyz: Hex: [0x8400, 0xFBFF], Dec: [33792, 64511]
    //    sign:     1
    //      exponent: [1, 30]
    //    mantissa: [0x0000, 0x3FF]
    //    value: -1*2^(-14)*(1 + ...)
    // 9. -infinity: Hex: 0xFC00, Dec: 64512
    //    sign:     1
    //    exponent: 31
    //    mantissa: 0
    // 10.-NaN: Hex: [0xFC01, 0xFFFF], Dec: [64513, 65535]
    //    sign:     1
    //    exponent: 31
    //    mantissa: [0x0001, 0x3FF]

    // converts Real16 to Real32
    // 1. +ZERO: 0x0000 --> 0x0000 0000
    // 2. -ZERO: 0x8000 --> 0x8000 0000
    // 3. +infinity/+NaN: Hex: [0x7C00, 0x7FFF], Dec: [31744, 32767]
    //    (0xFF << 23) | (mantissa << 13) = 0x7F80 0000 | (mantissa << 13)
    // 4. -infinity/-NaN: Hex: [0xFC00, 0xFFFF], Dec: [64512, 65535]
    //    0x8000 0000 | (0xFF << 23) | (mantissa << 13) = 0xFF80 000 | (mantissa << 13)
    // 5. +normalised numbers: Hex: [0x0400, 0x7BFF], Dec: [1024, 31743]
    //    (((exponent >> 10) - 15 + 127) << 23) | (mantissa << 13)
    // 6. -normalised numbers: Hex: [0x8400, 0xFBFF], Dec: [33792, 64511]
    //    0x8000 0000 | (((exponent >> 10) - 15 + 127) << 23) | (mantissa << 13)
    // 7. +un-normalised numbers: Hex: [0x0001, 0x03FF], Dec: [1, 1023]
    //    -un-normalised numbers: Hex: [0x8400, 0xFBFF], Dec: [33792, 64511]
    //    HAVE to normalise the current number:
    //    + find the first 1 bit from MSB --> LSB
    //    + shift the mantissa to MSB
    //    + adjust the exponent accordingly
    //    + apply the sign

    // converts all Real16 numbers: Hex: [0, 0xFFFF], Dec: [0, 65535]
    char buffer[64];
    UInt32 current_column_idx = 0;
    io << "#pragma once\n";
    io << "\n\n";
    io << "// System headers\n";
    io << "// Lib headers\n";
    io << "#include \"Common/TypeDefs.hpp\"\n";
    io << "\n\n";
    io << "// Conversion table: indexed using Real16 number: Hex: [0x0000, 0xFFFF], Dec: [0, 65535]\n";
    io << "// Real32 = H2F_CONVERT_TABLE[Real16]\n";
    io << "uint32 H2F_CONVERT_TABLE[] = \n";
    io << "{\n";
    io << "    ";
    for (UInt32 i = 0; i <= 0xFFFF; ++i)
    {
        // +ZERO: 0x0000 --> 0x0000 0000
        if (i == 0)
        {
            io << "0x00000000, ";
            current_column_idx++;
        }
        // -ZERO: 0x8000 --> 0x8000 0000
        else if (i == 0x8000)
        {
            if (current_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
            {
                io << "\n";
                io << "    ";
                current_column_idx = 0;
            }

            io << "0x80000000, ";
            current_column_idx++;
        }
        // +infinity/+NaN: Hex: [0x7C00, 0x7FFF], Dec: [31744, 32767]
        // 0x7F80 0000 | (mantissa << 13)
        // -infinity/-NaN: Hex: [0xFC00, 0xFFFF], Dec: [64512, 65535]
        // 0xFF80 000 | (mantissa << 13)
        // (sign << 16) | (mantissa << 13)
        else if ((i >= 0x7C00 && i <= 0x7FFF) || i >= 0xFC00)
        {
            if (current_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
            {
                io << "\n";
                io << "    ";
                current_column_idx = 0;
            }

            const UInt32 sign     = (i & 0x8000) << 16;
            const UInt32 exponent = 0xFF << 23;
            const UInt32 mantissa = (i & 0x3FF) << 13;
            const UInt32 f32 = sign | exponent | mantissa;
            snprintf(buffer, sizeof(buffer), "0x%08X", f32);
            io << buffer << ", ";
            current_column_idx++;
        }
        // +normalised numbers: Hex: [0x0400, 0x7BFF], Dec: [1024, 31743]
        // (((exponent >> 10) - 15 + 127) << 23) | (mantissa << 13)
        // -normalised numbers: Hex: [0x8400, 0xFBFF], Dec: [33792, 64511]
        // 0x8000 0000 | (((exponent >> 10) - 15 + 127) << 23) | (mantissa << 13)
        // (sign << 16) | (((exponent >> 10) - 15 + 127) << 23) | (mantissa << 13)
        else if ((i >= 0x0400 && i <= 0x7BFF) ||
                 (i >= 0x8400 && i <= 0xFBFF))
        {
            if (current_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
            {
                io << "\n";
                io << "    ";
                current_column_idx = 0;
            }

            const UInt32 sign     = (i & 0x8000) << 16;
            const UInt32 exponent = (((i & 0x7FFF) >> 10) - 15 + 127) << 23;
            const UInt32 mantissa = (i & 0x3FF) << 13;
            const UInt32 f32 = sign | exponent | mantissa;
            snprintf(buffer, sizeof(buffer), "0x%08X", f32);
            io << buffer << ", ";
            current_column_idx++;
        }
        // +un-normalised numbers: Hex: [0x0001, 0x03FF], Dec: [1, 1023]
        // -un-normalised numbers: Hex: [0x8400, 0xFBFF], Dec: [33792, 64511]
        //    HAVE to normalise the current number:
        //    + find the first 1 bit from MSB --> LSB
        //    + shift the mantissa to MSB
        //    + adjust the exponent accordingly
        //    + apply the sign
        // NOTE: for +/-un-normalized numbers: 2^(-14) NOT 2^(-15)
        else
        {
            if (current_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
            {
                io << "\n";
                io << "    ";
                current_column_idx = 0;
            }

            // --- Some un-normalized Numbers --- //
            // [0 00000 0000000001, 0 00000 1111111111]: [0x0001, 0x03FF]: [1, 1023]
            // 0 00000 0000000001: 2^(-14)*(0*2^0 + 1*2^(-10)): 0.000000059604644775390625
            // 0 01100111 00000000000000000000000: 2^(-24)
            // 0 00000 0000000001 --> 0 01100111 00000000000000000000000: 0x001 --> 0x3380 0000

            // 0 00000 0000000010: 2^(-14)*(0*2^0 + 1*2^(-9)): 0.00000011920928955078125
            // 0 01101000 00000000000000000000000: 2^(-23)
            // 0 00000 0000000010 --> 0 01101000 00000000000000000000000: 0x002 --> 0x3400 0000

            // 0 00000 0000000011: 2^(-14)*(0*2^0 + 1*2^(-9) + 1*2^(-10)):     0.000000178813934326171875
            // 0 01101000 10000000000000000000000: 2^(-23)*(1*2^0 + 1*2^(-1)): 0.000000178813934326171875
            // 0 00000 0000000011 --> 0 01101000 10000000000000000000000: 0x003 --> 0x3440 0000

            // 0 00000 0000000100: 2^(-14)*(0*2^0 + 1*2^(-8)): 0.0000002384185791015625
            // 0 01101001 00000000000000000000000: 2^(-22):    0.0000002384185791015625
            // 0 00000 0000000100 --> 0 01101001 00000000000000000000000: 0x004 --> 0x3480 0000

            // 0 00000 0000000101: 2^(-14)*(0*2^0 + 1*2^(-8) + 1*2^(-10)):     0.000000298023223876953125
            // 0 01101001 01000000000000000000000: 2^(-22)*(1*2^0 + 1*2^(-2)): 0.000000298023223876953125
            // 0 00000 0000000101 --> 0 01101001 01000000000000000000000: 0x005 --> 0x34A0 0000

            // 0 00000 0000000110: 2^(-14)*(0*2^0 + 1*2^(-8) + 1*2^(-9)):     0.00000035762786865234375
            // 0 01101001 10000000000000000000000: 2^(-22)*(1*2^0 + 1*2(-1)): 0.00000035762786865234375
            // 0 00000 0000000100 --> 0 01101001 10000000000000000000000: 0x006 --> 0x34C0 0000

            // 0 00000 0000000111: 2^(-14)*(0*2^0 + 1*2^(-8) + 1*2^(-9) + 1*2^(-10)):    0.000000417232513427734375
            // 0 01101001 11000000000000000000000: 2^(-22)*(1*2^0 + 1*2(-1) + 1*2^(-2)): 0.000000417232513427734375
            // 0 00000 0000000101 --> 0 01101001 11000000000000000000000: 0x007 --> 0x34E0 0000
            //                        X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX

            // 0 00000 0000001000: 2^(-14)*(0*2^0 + 1*2^(-7)):    0.000000476837158203125: 0x008 --> 0x3500 0000
            // 0 00000 0000001001: 2^(-14)*(0*2^0 + 1*2^(-7) + 2^(-10)): 0.000000536441802978515625 : 0x009 --> 0x3510 0000


            // we have to normalise a decimal number in binary format:
            // (0.00010001001)_2 --> 2^(-4)(1.0001001)_2
            // 0 00000 (0000000110): = 2^(-14)*(0.0000000110)_2 = 2^(-14)*2^(-8)*(1.10)_2
            // 2^(-22) *(1.10)_2  = 0 01101001 10000000000000000000000
            //                      0 01101001 10000000000000000000000
            //                      X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
            // gets the mantissa of the given Real16
            // 0 00000 1111111111
            // X XXXXX XXXXXXXXXX
            const UInt32 mantissa_half = i & 0x3FF;
            // finds the position of the last set bit from LSB
            const SInt32 last_set_bit = BitOperationHelper::findLSBitPos32(mantissa_half);
            // calculates the bits to be shifted to the left
            const SInt32 shift_count = 10 - last_set_bit;
            // calculates exponent: -14 - shift + 127
            const SInt32 exponent = 127 - 14 - shift_count;
            // calculates mantissa of Real32:
            // shift out the last set bit: 0.0000100001 ---> 1.0001000000 shift 5 left
            // --> mask out the shift 1 with 0x3FF: 1.0001000000 & 0.1111111111 --> 0001000000
            // --> shift the result left 13: 0001000000 << 13 --> 10000000000000000000
            const UInt32 mantissa = ((mantissa_half << shift_count) & 0x3FF) << 13;
            // creates F32: X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
            const UInt32 sign = (i & 0x8000) << 16;
            const UInt32 f32 = sign | (exponent << 23) | mantissa;
            snprintf(buffer, sizeof(buffer), "0x%08X", f32);
            io << buffer << ", ";
            current_column_idx++;
        }
    }
    io << "\n";
    io << "};\n";
    io << "\n";
}


// 生成多个表格来实现16位, 32位浮点数之间转换
static
void
genH2FBaseTable ()
{
    // Base table: 使用16位浮点数的sign(S)以及exponent part(E)来检索

    H2F_BASE_TABLE.clear();

    // S = 0 and E = 0: 表示+ZERO/+Un-normalized的数值，我们存储0. MOD table将包括32位浮点数的Pattern
    H2F_BASE_TABLE.push_back(0x00000000);
    // S = 0 and E = 1, 2, 3, ..., 30: 表示+normalised数值: (E-15+127) << 23
    for (UInt32 e = 1; e <= 30; ++e)
    {
        H2F_BASE_TABLE.push_back((e - 15 + 127) << 23);
    }
    // S = 0 and E = 31: 表示+infinity or +NaN: +infinity/+NaN: 0x7F80 0000
    H2F_BASE_TABLE.push_back(0x7F800000);
    // S = 1 and E = 0: 表示-ZERO/-Un-normalized numbers: 0x8000 0000
    // 我们使用0x80000000作为它的Base，它的Mod表格中含有32位浮点数的Pattern
    H2F_BASE_TABLE.push_back(0x80000000);
    // S = 1 and E: 1, 2, 3, ..., 30: 表示-normalised values: 0x8000 0000 | (E-15+127) << 23
    for (UInt32 e = 1; e <= 30; ++e)
    {
        H2F_BASE_TABLE.push_back(0x80000000 | ((e - 15 + 127) << 23));
    }
    // S = 1 and E = 31: 表示-infinity/-NaN: 0xFF80 0000
    H2F_BASE_TABLE.push_back(0xFF800000);
}


static
void
genH2FOffsetTable ()
{
    // Offset table: 使用16位浮点数的exponent part(E)来检索: [0, 31]

    H2F_OFFSET_TABLE.clear();

    // ZERO and Un-normalized的数值使用0作为他们的Offset
    H2F_OFFSET_TABLE.push_back(0x00000000);
    // E = 1, 2, 3, ..., 31: 我们使用1024作为他的Offset
    for (UInt32 e = 1; e <= 31; ++e)
    {
        H2F_OFFSET_TABLE.push_back(1024);
    }
}


static
void
genH2FModTable ()
{
    // Mod table: 使用如下方法进行检索: Mod[Offset[E(Real16)] + mantissa(Real16)]

    H2F_MOD_TABLE.clear();

    // ZERO: 0
    H2F_MOD_TABLE.push_back(0x00000000);
    // Un-normalised的数值: [1, 1023]: 我们使用此un-normalised number的32位浮点数的Pattern
    // 生成[0x0001, 0x03FF]之间的所有un-normalized浮点数的mantissa: [1, 1023]
    for (UInt32 i = 1; i <= 0x3FF; ++i)
    {
        // we have to normalise a decimal number in binary format:
        // (0.00010001001)_2 --> 2^(-4)(1.0001001)_2
        // 0 00000 (0000000110): = 2^(-14)*(0.0000000110)_2 = 2^(-14)*2^(-8)*(1.10)_2
        // 2^(-22) *(1.10)_2  = 0 01101001 10000000000000000000000
        //                      0 01101001 10000000000000000000000
        //                      X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        // gets the mantissa/significant
        // 0 00000 1111111111
        // X XXXXX XXXXXXXXXX
        const UInt32 mantissa = i & 0x3FF;
        // finds the position of the last set bit from LSB
        const SInt32 last_set_bit = BitOperationHelper::findLSBitPos32(mantissa);
        // calculates the bits to be shifted to the left
        const SInt32 shift_count  = 10 - last_set_bit;
        // calculates new exponent:
        const SInt32 exponent_new = 127 - 14 - shift_count;
        // calculates new mantissa:
        // shift out the last set bit: 0.0000100001 ---> 1.0001000000 shift 5 left
        // --> mask out the shift 1 with 0x3FF: 1.0001000000 & 0.1111111111 --> 0001000000
        // --> shift the result left 13: 0001000000 << 13 --> 10000000000000000000
        const UInt32 mantissa_new = ((mantissa << shift_count) & 0x3FF) << 13;
        // creates F32: X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        const UInt32 f32 = (exponent_new << 23) | mantissa_new;
        H2F_MOD_TABLE.push_back(f32);
    }

    // normalised的数值: [1024, 2047]: 我们将i的数值向左移动13位：i << 13, 这里 i为[0, 3FF]中的数值
    for (UInt32 i = 0; i <= 0x3FF; ++i)
    {
        H2F_MOD_TABLE.push_back(i << 13);
    }
}



// --- 生成32位浮点数到16位浮点数转换的表格 --- //
// 1. one to one mappings:
//  i)   +ZERO: 0x0000 0000 ---> 0x0000
//  ii)  -ZERO: 0x8000 0000 ---> 0x8000
//  iii) +INFINITY:  0x7F80 0000 ---> 0x7C00
//  iv)  -INFINITY:  0XFF80 0000 ---> 0XFC00
// 2. +NAN: [0x7F80 0001, 0x7FFF FFFF]
// i)  +NAN_SPEC: [0x7F80 0001, 0x7F80 1FFF] ---> 0x7C01, special handling for them:
//                if not, we shift its mantissa, Real16 will be +infinity
// ii) +NAN     : [0x7F80 2000, 0x7FFF FFFF] ---> [0x7C01, 0x7FFF]
//     0x7C00 | (mantissa >> 13)
// 3. -NAN: [0xFF80 0001, 0xFFFF FFFF]
// i) -NAN_SPEC: [0xFF80 0001, 0xFF80 1FFF] ---> 0xFC01
// i) -NAN     : [0xFF80 2000, 0xFFFF FFFF] ---> [0xFC01, 0xFFFF]
//     0xFC00 | (mantissa >> 13)
// 4. +TINY numbers: E < -24, [0x0000 0001, 0x337F FFFF] we round them to +ZERO
// 5. -TINY numbers: E < -24, [0x8000 0001, 0xB37F FFFF] we round them to -ZERO
//    S << 15
// 6. +HUGE numbers: E > 15, [0x4780 0000, 0x7F7F FFFF] we round them to +INFINITY
// 7. -HUGE numbers: E > 15, [0xC780 0000, 0xFF7F FFFF] we round them to -INFINITY
//    (S << 15) | 0x7C00
// 8. +NORM/normalised numbers: [0x3880 0000, 0x477F FFFF]: lose some kinds of precision
// 9. -NORM/normalised numbers: [0xB880 0000, 0xC77F FFFF]
//    (S << 15) | ((E - 127) + 15) << 10 | ((mantissa & 0x7F FFFF) >> 13)
// 10. +SMALL numbers: [0x3380 0000, 0x387F FFFF]: [ 5.96046448e-08,  6.10351526e-05]
// 11. -SMALL numbers: [0xB380 0000, 0xB87F FFFF]: [-5.96046448e-08, -6.10351526e-05]
//     we have to convert them to un-normalised Real16 numbers
//     (S << 15) | (((0x400 | (mantissa >> 13)) >> (-14 -(E - 127))) & 0x3FF)
//     NOTE:
//     0x400 is used to represent that 1 in 1.xyz and make the mantissa to be 11bits
//
// --- Examples on converting Real32 to Real16 --- //
// 1. 0x3380 0000: 2^(-24) ---> -14 - (E - 127) = 10, 10000000000 >> 10 = 1
//    0 01100111 00000000000000000000000 ---> 0 00000 0000000001
// 2. 0x387F FFFF: 2^(-15)*(2^1 + 2^(-1) + 2^(-2) + ... + 2^(-10) + ... + 2^(-23))
// --->
// 0x03FF: 2^(-14)(2^(-1) + 2^(-2) + ... + 2^(-10))
// 0 01110000 11111111111111111111111 ---> 0 00000 1111111111
//
// --- Distribution of Numbers --- //
// +ZERO[0x0000 0000], +TINY[0x0000 0001, 0x337F FFFF],  +SMALL[0x3380 0000, 0x387F FFFF],
// +NORM[0x3880 0000, 0x477F FFFF], +HUGE[0x4780 0000, 0x7F7F FFFF], +INFINITY[0x7F80 0000],
// +NAN_SPEC[0x7F80 0001, 0x7F80 1FFF], +NAN[0x7F80 2000, 0x7FFF FFFF],
//
// -ZERO[0x8000 0000], -TINY[0x8000 0001, 0xB37F FFFF], -SMALL[0xB380 0000, 0xB87F FFFF],
// -NORM[0xB880 0000, 0xC77F FFFF], -HUGE[0xC780 0000, 0xFF7F FFFF], -INFINITY[0XFF80 0000],
// -NAN_SPEC[0xFF80 0001, 0xFF80 1FFF], -NAN[0xFF80 2000, 0xFFFF FFFF]
// --- Calculates Base & Shift Tablers to convert Real32 to Real16 --- //
 UInt16 F2H_BASE_TABLE[512];
 UInt16 F2H_SHIFT_TABLE[512];


static
void
genF2HTable ()
{
    // 递归所有Eexponent part E: [0, 0xFF]
    for (UInt32 e = 0; e <= 0xFF; ++e)
    {
        // 获得exponent value: E - 127
        const SInt32 exponent = e - 127;
        // +TINY numbers: exponent < (-24), [0x0000 0001, 0x337F FFFF] mapped to +ZERO
        // -TINY numbers: exponent < (-24), [0x8000 0001, 0xB37F FFFF] mapped to -ZERO
        // S << 15
        if (exponent < -24)
        {
            F2H_BASE_TABLE[e | 0x000]  = 0x0000;
            F2H_BASE_TABLE[e | 0x100]  = 0x8000;
            // shifts everything away
            F2H_SHIFT_TABLE[e | 0x000] = 24;
            F2H_SHIFT_TABLE[e | 0x100] = 24;
        }
        // +SMALL numbers: [0x3380 0000, 0x387F FFFF]: [ 5.96046448e-08,  6.10351526e-05]
        // -SMALL numbers: [0xB380 0000, 0xB87F FFFF]: [-5.96046448e-08, -6.10351526e-05]
        // exponent < -14
        // we have to convert them to un-normalised Real16 numbers
        // (S << 15) | (((0x400 | (mantissa >> 13)) >> (-14 -(E - 127))) & 0x3FF)
        // (S << 15) | (0x400 >> (-14 - (E-127))) | (mantissa >> (13 - 14 - (E-127))
        // NOTE:
        // 0x400 is used to represent that 1 in 1.xyz and make the mantissa to be 11bits
        else if (exponent < -14)
        {
            F2H_BASE_TABLE[e | 0x000]  = 0x400 >> (-exponent - 14);
            F2H_BASE_TABLE[e | 0x100]  = 0x8000 | (0x400 >> (-exponent - 14));
            F2H_SHIFT_TABLE[e | 0x000] = -exponent - 1;
            F2H_SHIFT_TABLE[e | 0x100] = -exponent - 1;
        }
        // +NORM/normalised numbers: [0x3880 0000, 0x477F FFFF]: lose some kinds of precision
        // -NORM/normalised numbers: [0xB880 0000, 0xC77F FFFF]
        // (S << 15) | ((E - 127) + 15) << 10 | ((mantissa & 0x7F FFFF) >> 13)
        else if (exponent <= 15)
        {
            F2H_BASE_TABLE[e | 0x000]  = (exponent + 15) << 10;
            F2H_BASE_TABLE[e | 0x100]  = 0x8000 | ((exponent + 15) << 10);
            F2H_SHIFT_TABLE[e | 0x000] = 13;
            F2H_SHIFT_TABLE[e | 0x100] = 13;
        }
        // +HUGE numbers: E > 15, [0x4780 0000, 0x7F7F FFFF] mapped to +INFINITY
        // -HUGE numbers: E > 15, [0xC780 0000, 0xFF7F FFFF] mapped to -INFINITY
        // (S << 15) | 0x7C00
        else if (exponent < 128)
        {
            F2H_BASE_TABLE[e | 0x000]  = 0x7C00; // +INFINITY
            F2H_BASE_TABLE[e | 0x100]  = 0xFC00; // -INFINITY
            F2H_SHIFT_TABLE[e | 0x000] = 24;
            F2H_SHIFT_TABLE[e | 0x100] = 24;
        }
        // +/-INFINITY and +/-NAN: E = 128
        // +NAN     : [0x7F80 2000, 0x7FFF FFFF] ---> [0x7C01, 0x7FFF]
        // -NAN     : [0xFF80 2000, 0xFFFF FFFF] ---> [0xFC01, 0xFFFF]
        //     0x7C00 | (mantissa >> 13)
        else
        {
            F2H_BASE_TABLE[e | 0x000]  = 0x7C00; // +INFINITY
            F2H_BASE_TABLE[e | 0x100]  = 0xFC00; // -INFINITY
            // mantissa >> 13
            F2H_SHIFT_TABLE[e | 0x000] = 13;
            F2H_SHIFT_TABLE[e | 0x100] = 13;
        }
    }
}


static
void
writeOutTables (
    TextFileWriter & io)
{
    // --- 32Bits Float ---//
    // 31 (msb)
    // |
    // | 30     23
    // | |      |
    // | |      | 22                    0 (lsb)
    // | |      | |                     |
    // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
    // |1| <-8->| |<--       23      -->|
    //  s    e               m
    // s: sign bit       : 1bit
    // e: exponential bit: 8bits: the real exponential = e - 127: [-127, +128]
    // m: mantissa bit   : 23bits

    // --- 16Bits Float --- //
    // 15 (msb)
    // |
    // | 14  10
    // | |   |
    // | |   | 9        0 (lsb)
    // | |   | |        |
    // X XXXXX XXXXXXXXXX
    // |1| 5 | | <--10->|
    //  s  e        m
    // s: sign bit       : 1bit
    // e: exponential bit: 5bits: the real exponential = e - 15: [-15, +16]
    // m: mantissa bit   : 10bits

    char _buffer[64];
    UInt32 _column_idx = 0;
    io << "//------------------------------------------------------------------------------------------------//\n";
    io << "//------------------------------------------------------------------------------------------------//\n";
    io <<
        "//                        AUTOMATICALLY GENERATED BY HALFTABLE GENERATOR                          //\n";
    io << "//------------------------------------------------------------------------------------------------//\n";
    io << "//------------------------------------------------------------------------------------------------//\n";
    io << "\n\n";
    io << "#pragma once\n";
    io << "\n\n";
    io << "// System headers\n";
    io << "// Lib headers\n";
    io << "#include \"Core/DataType/RS_DataTypeDefs.hpp\"\n";
    io << "\n\n";
    io << "namespace Core\n";
    io << "{\n";
    io << "\n";

    // --- 生成16位浮点数-->32位浮点数转换的Base Table --- //
    UInt32 _elem_idx = 0;

    io << "    // --- 生成16位浮点数-->32位浮点数转换的Base Table --- //\n";
    io << "\n";
    io << "    // 使用如下方法将16位浮点数转换到32位:\n";
    io << "    // Real32 = Base[S(Real16)|E(Real16)] | Mod[Offset[E(Real16)]+mantissa(Real16)]\n";
    io << "    // where: S(Real16) is the sign of Real16 and E(Real16) is the exponent of the Real16\n";
    io << "\n";
    io << "    // Base table: 使用16位浮点数的sign(S)以及exponent part(E)来检索\n";
    io << "    UInt32 H2F_BASE_TABLE[] = \n";
    io << "    {\n";
    io << "        // S = 0 and E = 0: 表示+ZERO/+Un-normalized的数值，我们存储0. MOD table将包括32位浮点数的Pattern\n";
    snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_BASE_TABLE[_elem_idx++]);
    io << "        " << _buffer <<",\n";
    io << "        // S = 0 and E = 1, 2, 3, ..., 30: 表示+normalised数值: (E-15+127) << 23\n";
    io << "        ";
    for (UInt32 e = 1; e <= 30; ++e)
    {
        if (_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
        {
            io << "\n";
            io << "        ";
            _column_idx = 0;
        }
        snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_BASE_TABLE[_elem_idx++]);
        io << _buffer << ", ";
        _column_idx++;
    }
    io << "\n";
    io << "        // S = 0 and E = 31: 表示+infinity or +NaN: +infinity/+NaN: 0x7F80 0000\n";
    snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_BASE_TABLE[_elem_idx++]);
    io << "        " << _buffer <<",\n";
    io << "        // S = 1 and E = 0: 表示-ZERO/-Un-normalized numbers: 0x8000 0000\n";
    snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_BASE_TABLE[_elem_idx++]);
    io << "        " << _buffer <<",\n";
    io << "        // S = 1 and E: 1, 2, 3, ..., 30: 表示-normalised values: 0x8000 0000 | (E-15+127) << 23\n";
    io << "        ";
    for (UInt32 e = 1; e <= 30; ++e)
    {
        if (_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
        {
            io << "\n";
            io << "        ";
            _column_idx = 0;
        }
        snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_BASE_TABLE[_elem_idx++]);
        io << _buffer << ", ";
        _column_idx++;
    }
    io << "\n";
    io << "        // S = 1 and E = 31: 表示-infinity/-NaN: 0xFF80 0000\n";
    snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_BASE_TABLE[_elem_idx++]);
    io << "        " << _buffer <<"\n";
    io << "    };\n";
    io << "\n\n";


    // --- 生成16位浮点数-->32位浮点数转换的Offset Table --- //
    _elem_idx   = 0;
    _column_idx = 0;
    io << "    // --- 生成16位浮点数-->32位浮点数转换的Offset Table --- //\n";
    io << "\n";
    io << "    // Offset table: 使用16位浮点数的exponent part(E)来检索: [0, 31]\n";
    io << "    UInt32 H2F_OFFSET_TABLE[] = \n";
    io << "    {\n";

    // ZERO and Un-normalized values using 0 as the offset
    io << "        // ZERO and Un-normalized的数值使用0作为他们的Offset\n";
    snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_OFFSET_TABLE[_elem_idx++]);
    io << "        " << _buffer << ",\n";
    io << "        // E = 1, 2, 3, ..., 31: 我们使用1024作为他的Offset\n";
    io << "        ";
    for (UInt32 e = 1; e <= 31; ++e)
    {
        if (_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
        {
            io << "\n";
            io << "        ";
            _column_idx = 0;
        }
        snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_OFFSET_TABLE[_elem_idx++]);
        io << _buffer << ", ";
        _column_idx++;
    }
    io << "\n";
    io << "    };\n";
    io << "\n\n";


    // --- 生成16位浮点数-->32位浮点数转换的MOD Table --- //

    _elem_idx   = 0;
    _column_idx = 0;
    io << "    // --- 生成16位浮点数-->32位浮点数转换的MOD Table --- //\n";
    io << "    // Mod table: 使用如下方法进行检索: Mod[Offset[exponent(Real16)] + mantissa(Real16)]\n";
    io << "    UInt32 H2F_MOD_TABLE[] = \n";
    io << "    {\n";
    io << "        // ZERO: 0\n";
    // ZERO: 0
    snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_OFFSET_TABLE[_elem_idx++]);
    io << "        " << _buffer << ",\n";

    // generates mantissa for all un-normalized values: [0x0001, 0x03FF]: [1, 1023]
    io << "        // Un-normalised的数值: [1, 1023]: 我们使用此un-normalised number的32位浮点数的Pattern\n";
    io << "        ";
    for (UInt32 i = 1; i <= 0x3FF; ++i)
    {
        if (_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
        {
            io << "\n";
            io << "        ";
            _column_idx = 0;
        }

        std::snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_MOD_TABLE[_elem_idx++]);
        io << _buffer << ", ";
        _column_idx++;
    }
    io << "\n";

    // normalised number: [1024, 2047]: i << 13, where i:[0, 3FF]
    _column_idx = 0;
    io << "        // normalised的数值: [1024, 2047]: 我们将i的数值向左移动13位：i << 13, 这里 i为[0, 3FF]中的数值\n";
    io << "        ";
    for (UInt32 i = 0; i <= 0x3FF; ++i)
    {
        if (_column_idx == MAX_H2F_TABLE_COLUMN_COUNT)
        {
            io << "\n";
            io << "        ";
            _column_idx = 0;
        }
        snprintf(_buffer, sizeof(_buffer), "0x%08X", H2F_MOD_TABLE[_elem_idx++]);
        io << _buffer << ", ";
        _column_idx++;
    }
    io << "\n";
    io << "    };\n";
    io << "\n\n";


    // --- Generates the Base Table to convert Real32 to Real16 --- //
    _column_idx = 0;
    io << "    // --- 生成32位浮点数到16位浮点数转换的Base Table --- //\n";
    io << "\n";
    io << "    // 使用如下方法将32位浮点数转换为16位:\n";
    io << "    // Real6 = Base[S(Real32) | E(Real32)] | mantissa(Real32) >> shift[S(Real32) | E(Real32)]\n";
    io << "    // where: S(Real32) is the sign of Real32 and E(Real32) is the exponent of the Real32\n";
    io << "\n";
    io << "    // Base table: 使用32位浮点数的sign(S)以及exponent part(E)来进行索引\n";
    io << "    UInt16 F2H_BASE_TABLE[] = \n";
    io << "    {\n";
    io << "        ";
    for (UInt32 i = 0; i < 512; ++i)
    {
        if (_column_idx == MAX_F2H_TABLE_COLUMN_COUNT)
        {
            io << "\n";
            io << "        ";
            _column_idx = 0;
        }
        snprintf(_buffer, sizeof(_buffer), "0x%04X", F2H_BASE_TABLE[i]);
        io << _buffer << ", ";
        _column_idx++;
    }
    io << "\n";
    io << "    };\n";
    io << "\n\n";


    // --- 生成Shift Table --- //
    _column_idx = 0;
    io << "    // --- 生成Shift Table --- //\n";
    io << "\n";
    io << "    // Shift table: 使用32位浮点数的sign(S)以及exponent part(E)来进行索引\n";
    io << "    UInt16 F2H_SHIFT_TABLE[] = \n";
    io << "    {\n";
    io << "        ";
    for (UInt32 i = 0; i < 512; ++i)
    {
        if (_column_idx == MAX_F2H_TABLE_COLUMN_COUNT)
        {
            io << "\n";
            io << "        ";
            _column_idx = 0;
        }
        snprintf(_buffer, sizeof(_buffer), "0x%04X", F2H_SHIFT_TABLE[i]);
        io << _buffer << ", ";
        _column_idx++;
    }
    io << "\n";
    io << "    };\n";


    // done with all dump
    io << "\n";
    io << "} // namespace Core\n";
}



//--------------------------------------------------------------------------------------------------
// TABLE CHECKING FUNCS
//--------------------------------------------------------------------------------------------------
// MARK: - 检查16位浮点数<-->32位浮点数转换表格

// 从给定的16位浮点数获得32位浮点数的Pattern
static
UInt32
getF32PatternFromH16 (
    const Real16 h16)
{
    // Real32 = Base[sign(Real16) | exponent(Real16)] |
    //          Mod[Offset[exponent(Real16)] + mantissa(Real16)]
    //
    const UInt32 sign_exponent = h16.val >> 10;
    const UInt32 base   = H2F_BASE_TABLE[sign_exponent];
    const UInt32 offset = H2F_OFFSET_TABLE[sign_exponent & 0x1F];
    const UInt32 mod    = H2F_MOD_TABLE[offset + (h16.val&0x3FF)];
    return base | mod;
}


// 从给定的32位浮点数的Pattern计算Real16
static
Real16
calcH16FromF32 (
    const UInt32 f32)
{
    // --- 32Bits Float --- //
    // 31 (msb)
    // |
    // | 30     23
    // | |      |
    // | |      | 22                    0 (lsb)
    // | |      | |                     |
    // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
    // |1| <-8->| |<--       23      -->|
    //  s    e               m
    // s: sign bit       : 1bit
    // e: exponential bit: 8bits: the real exponential = e - 127: [-127, +128]
    // m: mantissa bit   : 23bits
    //
    // --- 16Bit Float --- //
    // 15 (msb)
    // |
    // | 14  10
    // | |   |
    // | |   | 9        0 (lsb)
    // | |   | |        |
    // X XXXXX XXXXXXXXXX
    // |1| 5 | | <--10->|
    //  s  e        m
    // s: sign bit       : 1bit
    // e: exponential bit: 5bits: the real exponential = e - 15: [-15, +16]
    // m: mantissa bit   : 10bits
    //
    //
    // --- Method on convert Real32 to Real16 --- //
    // 1. one to one mappings:
    //  i)   +ZERO: 0x0000 0000 ---> 0x0000
    //  ii)  -ZERO: 0x8000 0000 ---> 0x8000
    //  iii) +INFINITY:  0x7F80 0000 ---> 0x7C00
    //  iv)  -INFINITY:  0XFF80 0000 ---> 0XFC00
    // 2. +NAN: [0x7F80 0001, 0x7FFF FFFF]
    // i)  +NAN_SPEC: [0x7F80 0001, 0x7F80 1FFF] ---> 0x7C01, special handling for them:
    //                if not, we shift its mantissa, Real16 will be +infinity
    // ii) +NAN     : [0x7F80 2000, 0x7FFF FFFF] ---> [0x7C01, 0x7FFF]
    //     0x7C00 | (mantissa >> 13)
    // 3. -NAN: [0xFF80 0001, 0xFFFF FFFF]
    // i) -NAN_SPEC: [0xFF80 0001, 0xFF80 1FFF] ---> 0xFC01
    // i) -NAN     : [0xFF80 2000, 0xFFFF FFFF] ---> [0xFC01, 0xFFFF]
    //     0xFC00 | (mantissa >> 13)
    // 4. +TINY numbers: E < -24, [0x0000 0001, 0x337F FFFF] we round them to +ZERO
    // 5. -TINY numbers: E < -24, [0x8000 0001, 0xB37F FFFF] we round them to -ZERO
    //    S << 15
    // 6. +HUGE numbers: E > 15, [0x4780 0000, 0x7F7F FFFF] we round them to +INFINITY
    // 7. -HUGE numbers: E > 15, [0xC780 0000, 0xFF7F FFFF] we round them to -INFINITY
    //    (S << 15) | 0x7C00
    // 8. +NORM/normalised numbers: [0x3880 0000, 0x477F FFFF]: lose some kinds of precision
    // 9. -NORM/normalised numbers: [0xB880 0000, 0xC77F FFFF]
    //    (S << 15) | ((E - 127) + 15) << 10 | ((mantissa & 0x7F FFFF) >> 13)
    // 10. +SMALL numbers: [0x3380 0000, 0x387F FFFF]: [ 5.96046448e-08,  6.10351526e-05]
    // 11. -SMALL numbers: [0xB380 0000, 0xB87F FFFF]: [-5.96046448e-08, -6.10351526e-05]
    //     we have to convert them to un-normalised Real16 numbers
    //     (S << 15) | (((0x400 | (mantissa >> 13)) >> (-14 -(E - 127))) & 0x3FF)
    //     NOTE:
    //     0x400 is used to represent that 1 in 1.xyz and make the mantissa to be 11bits

    // --- Examples on converting Real32 to Real16 --- //
    // 1. 0x3380 0000: 2^(-24) ---> -14 - (E - 127) = 10, 10000000000 >> 10 = 1
    //    0 01100111 00000000000000000000000 ---> 0 00000 0000000001
    // 2. 0x387F FFFF: 2^(-15)*(2^1 + 2^(-1) + 2^(-2) + ... + 2^(-10) + ... + 2^(-23))
    // --->
    // 0x03FF: 2^(-14)(2^(-1) + 2^(-2) + ... + 2^(-10))
    // 0 01110000 11111111111111111111111 ---> 0 00000 1111111111

    // --- Distribution of Numbers --- //
    // +ZERO[0x0000 0000], +TINY[0x0000 0001, 0x337F FFFF],  +SMALL[0x3380 0000, 0x387F FFFF],
    // +NORM[0x3880 0000, 0x477F FFFF], +HUGE[0x4780 0000, 0x7F7F FFFF], +INFINITY[0x7F80 0000],
    // +NAN_SPEC[0x7F80 0001, 0x7F80 1FFF], +NAN[0x7F80 2000, 0x7FFF FFFF],
    //
    // -ZERO[0x8000 0000], -TINY[0x8000 0001, 0xB37F FFFF], -SMALL[0xB380 0000, 0xB87F FFFF],
    // -NORM[0xB880 0000, 0xC77F FFFF], -HUGE[0xC780 0000, 0xFF7F FFFF], -INFINITY[0XFF80 0000],
    // -NAN_SPEC[0xFF80 0001, 0xFF80 1FFF], -NAN[0xFF80 2000, 0xFFFF FFFF]

    // 1. mapping of special patterns:
    //  i)   +ZERO: 0x0000 0000 ---> 0x0000
    //  ii)  -ZERO: 0x8000 0000 ---> 0x8000
    //  iii) +INFINITY:  0x7F80 0000 ---> 0x7C00
    //  iv)  -INFINITY:  0XFF80 0000 ---> 0XFC00
    if (f32 == 0x00000000)
    {
        return Real16{0x0000};
    }
    else if (f32 == 0x80000000)
    {
        return Real16{0x8000};
    }
    else if (f32 == 0x7F800000)
    {
        return Real16{0x7C00};
    }
    else if (f32 == 0XFF800000)
    {
        return Real16{0XFC00};
    }
    // 2. +NAN: [0x7F80 0001, 0x7FFF FFFF]
    // i)  +NAN_SPEC: [0x7F80 0001, 0x7F80 1FFF] ---> 0x7C01, otherwise if we shift its mantissa, half will be +infinity
    // ii) +NAN     : [0x7F80 2000, 0x7FFF FFFF] ---> [0x7C01, 0x7FFF]
    //     0x7C00 | (mantissa >> 13)
    else if (f32 >= 0x7F800001 && f32 <= 0x7F801FFF)
    {
        return Real16{0x7C01};
    }
    else if (f32 >= 0x7F802000 && f32 <= 0x7FFFFFFF)
    {
        const UInt32 mantissa = (f32 & 0x7FFFFF) >> 13;
        return Real16{(UInt16)(0x7C00 | mantissa)};
    }
    // 3. -NAN: [0xFF80 0001, 0xFFFF FFFF]
    // i) -NAN_SPEC: [0xFF80 0001, 0xFF80 1FFF] ---> 0xFC01
    // i) -NAN     : [0xFF80 2000, 0xFFFF FFFF] ---> [0xFC01, 0xFFFF]
    //     0xFC00 | (mantissa >> 13)
    else if (f32 >= 0xFF800001 && f32 <= 0xFF801FFF)
    {
        return Real16{0xFC01};
    }
    else if (f32 >= 0xFF802000 && f32 <= 0xFFFFFFFF)
    {
        const UInt32 mantissa = (f32 & 0x7FFFFF) >> 13;
        return Real16{(UInt16)(0xFC00 | mantissa)};
    }
    // 4. +TINY numbers: E < -24, [0x0000 0001, 0x337F FFFF] we round them to +ZERO
    // 5. -TINY numbers: E < -24, [0x8000 0001, 0xB37F FFFF] we round them to -ZERO
    //    S << 15
    else if ((f32 >= 0x00000001 && f32 <= 0x337FFFFF) ||
             (f32 >= 0x80000001 && f32 <= 0xB37FFFFF))
    {
        const UInt32 sign = f32 >> 31;
        return Real16{(UInt16)(sign << 15)};
    }
    // 6. +HUGE numbers: E > 15, [0x4780 0000, 0x7F7F FFFF] we round them to +INFINITY
    // 7. -HUGE numbers: E > 15, [0xC780 0000, 0xFF7F FFFF] we round them to -INFINITY
    //    (S << 15) | 0x7C00
    else if ((f32 >= 0x47800000 && f32 <= 0x7F7FFFFF) ||
             (f32 >= 0xC7800000 && f32 <= 0xFF7FFFFF))
    {
        const UInt32 sign = f32 >> 31;
        return Real16{(UInt16)((sign << 15) | 0x7C00)};
    }
    // 8. +NORM/normalised numbers: [0x3880 0000, 0x477F FFFF]: lose some kinds of precision
    // 9. -NORM/normalised numbers: [0xB880 0000, 0xC77F FFFF]
    //    (S << 15) | ((E - 127) + 15) << 10 | ((mantissa & 0x7F FFFF) >> 13)
    else if ((f32 >= 0x38800000 && f32 <= 0x477FFFFF) ||
             (f32 >= 0xB8800000 && f32 <= 0xC77FFFFF))
    {
        const UInt32 sign     = f32 >> 31;
        const UInt32 exponent = ((f32 & 0x7F800000) >> 23) - 127 + 15;
        const UInt32 mantissa = (f32 & 0x7FFFFF) >> 13;
        return Real16{(UInt16)((sign << 15) | (exponent << 10) | mantissa)};
        // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
    }
    // 10. +SMALL numbers: to un-normalised numbers: [0x3380 0000, 0x387F FFFF]: [ 5.96046448e-08,  6.10351526e-05]
    // 11. -SMALL numbers: to un-normalised numbers: [0xB380 0000, 0xB87F FFFF]: [-5.96046448e-08, -6.10351526e-05]
    //     (S << 15) | (((0x400 | (mantissa >> 13)) >> (-14 -(E - 127))) & 0x3FF)
    //     0x400 is used to represent that 1 in 1.xyz and make the mantissa to be 11bits
    else if ((f32 >= 0x33800000 && f32 <= 0x387FFFFF) ||
             (f32 >= 0xB3800000 && f32 <= 0xB87FFFFF))
    {
        const UInt32 sign     = f32 >> 31;
        const UInt32 mantissa = 0x400 | ((f32 & 0x7FFFFF) >> 13);
        const UInt32 shift    = 127 - ((f32 & 0x7F800000) >> 23) - 14;
        return Real16{(UInt16)((sign << 15) | ((mantissa >> shift) & 0x3FF))};
    }
    else
    {
        // ERROR
        return Real16{0};
    }
}


// 从给定的32位浮点数的2Pattern，使用生成的表格来计算Real6
static
Real16
getH16FromF32 (
    const UInt32 f32)
{
    // handle +NAN: [0x7F80 0001, 0x7F80 1FFF]  ---> 0x7C01 and
    //        -NAN: [0xFF80 0001, 0xFF80 1FFF]  ---> 0xFC01 specially,
    // otherwise, we will have wrong values
    if (f32 >= 0x7F800001 && f32 <= 0x7F801FFF)
    {
        return Real16{0x7C01};
    }
    else if (f32 >= 0xFF800001 && f32 <= 0xFF801FFF)
    {
        return Real16{0xFC01};
    }
    // normal case, we are using the table
    else
    {
        // Real16 = Base[S(Real32) | E(Real32)] | mantissa(Real32) >> shift[S(Real32) | E(Real32)]
        // S | E
        // gets the sign and exponent parts of the given Real32
        const UInt32 sign_exponent = (f32 >> 23) & 0x1FF;
        // gets the base value and shift value
        const UInt16 base  = F2H_BASE_TABLE[sign_exponent];
        const UInt16 shift = F2H_SHIFT_TABLE[sign_exponent];
        // gets the mantissa part of the given Real32
        const UInt32 mantissa = f32 & 0x7FFFFF;
        return Real16{(UInt16)(base | (mantissa >> shift))};
    }
}


// 检测是否所有Real16都可以正确的转换成Real32
static
Bool
checkH16ToF32 ()
{
    cout << "[CHECKING]: Real16 --> Real16..."  << endl;
    cout << "0%                          100%" << endl;

    char buffer[256];
    // Real32 = Base[sign(Real16) | exponent(Real16)] |
    //          Mod[Offset[exponent(Real16)] + mantissa(Real16)]
    //
    for (UInt32 v = 0; v <= 0xFFFF; ++v)
    {
        if (!(v % 0x0800))
        {
            cout << ".";
        }

        const unsigned int value_read_full = H2F_CONVERT_TABLE[v];
        const UInt32 value_read_opt = getF32PatternFromH16(Real16{(UInt16)v});
        if (value_read_full != value_read_opt)
        {
            snprintf(buffer,
                     sizeof(buffer),
                     "Real16[0x%04X] must be converted to Real32[0x%08X].\n"
                     "But it is converted to 0x%08X.",
                     v, value_read_full, value_read_opt);
            cout << buffer << endl;
            return false;
        }
    }
    cout << endl;
    cout << "[CHECKING]: Done." << endl;

    return true;
}


// Checks if all Real32 can be correctly converted to Real16
static
Bool
checkF32ToH16 ()
{
    // FIXED: NO LOG IN THE TERMINAL WINDOWS:
    // because cout will cache the output in a internal buffer, we have to call
    // - cout.flush() to manually flush the buffer or
    // - std::endl to insert a new line and flush the buffer:
    // https://en.cppreference.com/w/cpp/io/manip/endl
    //
    cout << "[CHECKING]: Real32 --> Real16..."  << endl;
    cout << "0%                          100%" << endl;

    char buffer[64];
    UInt16 current_row = 0;
    for (UInt64 i = 0x00000000u; i <= 0xFFFFFFFFu; ++i)
    {
        const Real16 half_calc  = calcH16FromF32((UInt32)i);
        const Real16 half_table = getH16FromF32 ((UInt32)i);
//        if (((i >> 28) & 0x000F) == current_row)
        if (!(i % 0x8000000))
        {
            cout << ".";
            // flushes the cache buffer, we don't need an extra new line
            cout.flush();
            ++current_row;
        }
        if (half_calc.val != half_table.val)
        {
            snprintf(buffer,
                     sizeof(buffer),
                     "Real32[0x%08X] can NOT be converted correctly\n"
                     "[calc]: 0x%04X != [table]: 0x%04X",
                     (UInt32)i, half_calc.val, half_table.val);
            cout << buffer << endl;
            return false;
        }
    }
    cout << endl;
    cout << "[CHECKING]: Done." << endl;

    return true;
}



//--------------------------------------------------------------------------------------------------
// MAIN ENTRY
//--------------------------------------------------------------------------------------------------
// MARK: - 程序主入口

SInt32
main (
    const SInt32       argc,
    const char* const argv[])
{
    using namespace std;

    cout << "[GENERATING]: Tables..." << endl;
    // 生成16位浮点数到32位浮点数转换的各种表格
    genH2FBaseTable();
    genH2FOffsetTable();
    genH2FModTable();
    // 生成32位浮点数到16位浮点数转换的各种表格
    genF2HTable();
    cout << "[GENERATING]: Done." << endl;

    // 检测生成的结果
    // 检测是否所有Real16数值都可以转换到Real32
    if (!checkH16ToF32()) {return -1;}
    // 检测是否所有Real32数值都可以转换到Real16
    if (!checkF32ToH16()) {return -1;}

    // 输出生成结果
    const ASCII *const _parent_path_rel = "../../../../Private/Core/Generated/DataType";
    const RuntimeText  _parent_path_abs = NativeFileSystem::getAbsPath (
        (const UTF8*)_parent_path_rel, NativeFileSystem::SEARCH_PATH_WORKING_FOLDER);
    const ASCII *const _file_name_rel   = "Real16_32_convert_table.hpp";
    // 确保输出的目录存在
    if (!NativeFileSystem::existsDir(_parent_path_abs.data()))
    {
        NativeFileSystem::createDir(_parent_path_abs.data());
    }

    cout << "[STORING]: Tables..." << endl
         << "[" << _parent_path_abs.data() << "/" << _file_name_rel << "]" << endl;

    // 生成输出文件的相对路径
    UTF8 _file_path [512];
    TextHelper::sprintfSafe(_file_path, 512, "%s/%s", _parent_path_rel, _file_name_rel);
    TextFileWriter _file_io(_file_path,
                            NativeFileSystem::SEARCH_PATH_WORKING_FOLDER,
                            TEXT_ENCODINGS_UTF8_BOM,
                            NativeFileSystem::WRITE_MODES_OVERWRITE);
    if (!_file_io.isValid())
    {
        cout << "Can not create the output file: "
             << _parent_path_abs.data() << "/" << _file_name_rel << endl;
        return -1;
    }

    //genOneTable(_io_stream);
    writeOutTables(_file_io);
    cout << "[STORING]: Done." << endl;

    return 0;
}
