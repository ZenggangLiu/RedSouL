// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Mathe/RS_MatheDefs.hpp"
#include "Core/Mathe/RS_MatheFuncs.hpp"
// Self
#include "Core/Mathe/RS_FloatPointFuncs.hpp"


namespace Core
{

    // 参见：Documents/TekDoc/Floating_Point_Number.txt
    //
    UInt16
    FloatPoint::toUInt16 (
        const Real16 data)
    {
        return data.val;
    }


    UInt32
    FloatPoint::toUInt32 (
        const Real32 data)
    {
        // NOTE: modern C++标准不允许：
        // - CASTING from one type value pointer(float) to another type point(int)
        // - READING another member from an union, if another member is assigned
        // so we will use memcpy() and let the compiler optimize the whole steps
        UInt32 _bit_pattern;
        std::memcpy(&_bit_pattern, &data, sizeof(Real32));

        return _bit_pattern;
    }


    UInt64
    FloatPoint::toUInt64 (
        const Real64 data)
    {
        UInt64 _bit_pattern;
        memcpy(&_bit_pattern, &data, sizeof(Real64));

        return _bit_pattern;
    }


    Bool
    FloatPoint::equal (
        const Real32 a,
        const Real32 b,
        const Real32 epsilon /* = (Real32)EPSILON */)
    {
        // 参考：
        // Real Time Collision Detection
        // 11.3.1：Tolerance Comparison for Floating-point Values
        //
        // 我们使用如下方式比较两个浮点数a, b:
        // 1) 绝对tolerance比较: abs(a - b) <= tolerence
        //    这种比较方式需要a与b中有一定的digit匹配，才能够认为他们相等。
        //    但是当a与b越来越大的时候，这种比较几乎相当于比较所有a与b的digits是否相同
        // 2) 相对tolerance比较:  abs(a/b - 1) <= tolerence, where |a| <= |b|
        //    这个比较关系可以写为: abs(a - b) <= tolerence * abs(b)
        //    如果忽略|a| <= |b|的条件：
        //    + 我们可以获得：abs(a - b) <= tolerence * max(abs(a), abs(b))
        //      这个比较公式只在abs(a)以及abs(b)都大于1的情况下成立
        //
        // 所以: absolute(a - b) <= tolerence * max[max(abs(a), abs(b)), 1]
        // - 当数值小于1的时候，使用绝对比较: abs(a - b) <= tolerence * 1
        // - 当数值大于1的时候，使用相对比较: abs(a - b) <= tolerence * max(abs(a), abs(b))
        // NOTE:
        // 使用如下近似无法实现1: 绝对tolerance比较
        // absolute(a - b) <= tolerence * (absolute(a) + absolute(b) + 1)
        //
        const Real32 _max_a_b = Mathe::maximum(Mathe::absolute(a), Mathe::absolute(b));
        return Mathe::absolute(a - b) <= epsilon * Mathe::maximum(_max_a_b, (Real32)1);
    }


    Bool
    FloatPoint::equal (
        const Real64 a,
        const Real64 b,
        const Real64 epsilon /* = EPSILON */)
    {
        const Real64 _max_a_b = Mathe::maximum(Mathe::absolute(a), Mathe::absolute(b));
        return Mathe::absolute(a - b) <= epsilon * Mathe::maximum(_max_a_b, (Real64)1);
    }


    Real16
    FloatPoint::fromUInt16 (
        const UInt16 bit_pattern)
    {
        return Real16{bit_pattern};
    }


    Real32
    FloatPoint::fromUInt32 (
        const UInt32 bit_pattern)
    {
        Real32 _float_data;
        std::memcpy(&_float_data, &bit_pattern, sizeof(bit_pattern));
        return _float_data;
    }


    Real64
    FloatPoint::fromUInt64 (
        const UInt64 bit_pattern)
    {
        Real64 _float_data;
        std::memcpy(&_float_data, &bit_pattern, sizeof(bit_pattern));
        return _float_data;
    }


/* 此次的函数没有进行测试 */
#if 0
    Bool
    FloatPoint::isInfinite (
        const Real16 val)
    {
        return isPositiveInfinite(val) || isNegativeInfinite(val);
    }


    Bool
    FloatPoint::isInfinite (
        const Real32 val)
    {
        return isPositiveInfinite(val) || isNegativeInfinite(val);
    }


    Bool
    FloatPoint::isInfinite (
        const Real64 val)
    {
        return isPositiveInfinite(val) || isNegativeInfinite(val);
    }


    Bool
    FloatPoint::isNan (
        const Real16 data)
    {
        // 15 (msb)
        // |
        // | 14  10
        // | |   |
        // | |   | 9        0 (lsb)
        // | |   | |        |
        // v v   v v        v
        // X XXXXX XXXXXXXXXX
        // |1| 5 | |<--10-->|
        //  s  e        m
        //
        // E = 31 并且 M != 0
        // - infinity: m == 0: 0x7C00
        // - NaN     : m != 0: 0x7C01
        //
        // 提取指数位E以及Mantissa M
        const UInt8 _exponential = (data.val & 0b0111110000000000) >> 10;
        const UInt16 _mantissa   = (data.val & 0b0000001111111111);

        return (_exponential == 31) && (_mantissa != 0);
    }


    Bool
    FloatPoint::isNan (
        const Real32 data)
    {
        // 31 (msb)
        // |
        // | 30     23
        // | |      |
        // | |      | 22                    0 (lsb)
        // | |      | |                     |
        // v v      v v                     v
        // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        // |1| <-8->| |<--       23      -->|
        //  s    e               m
        //
        // e = 255 并且 M ！= 0
        // - infinity: m == 0: 0x7F800000: 0 111 1111 1 000 0000 0000 0000 0000 0000
        // - NaN     : m != 0: 0x7F800100
        //
        // NOTE:
        // but if we trying to compare the NaN, compiler will generate
        // fucompp, which will fire an exception and cause the equality checking failed
        // commented out, because some compiler could optimize this to ALWAYS FALSE:
        // - visual studio: if we are using Fast (/fp:fast) as the floating point model
        // return val != val;

        // 提取指数位E以及Mantissa M
        const UInt32 _bit_pattern = toUInt32(data);
        const UInt8 _exponential  = (_bit_pattern & 0x7F800000) >> 23;
        const UInt32 _mantissa    = (_bit_pattern & 0x007FFFFF);

        return (_exponential == 255) && (_mantissa != 0);
    }


    Bool
    FloatPoint::isNan (
        const Real64 data)
    {
        // 63 (msb)
        // |
        // | 62        52
        // | |         |
        // | |         | 51                                                 0 (lsb)
        // | |         | |                                                  |
        // v v         v v                                                  v
        // X XXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // |1| <--11-->| |             <--       52      -->                |
        //  s    e               m
        //
        // E = 2047 并且 M != 0
        // - infinity: m == 0: 0x7FF0000000000000
        // - NaN     : m != 0: 0x7FF0010000000000
        //
        // 提取指数位E以及Mantissa M
        const UInt64 _bit_pattern = toUInt64(data);
        const UInt16 _exponential = (_bit_pattern & 0x7FF0000000000000UL) >> 52;
        const UInt64 _mantissa    = (_bit_pattern & 0x000FFFFFFFFFFFFFUL);

        return (_exponential == 2047) && (_mantissa != 0);
    }


    Bool
    FloatPoint::isNegativeInfinite (
        const Real16 data)
    {
        // 15 (msb)
        // |
        // | 14  10
        // | |   |
        // | |   | 9        0 (lsb)
        // | |   | |        |
        // v v   v v        v
        // X XXXXX XXXXXXXXXX
        // |1| 5 | |<--10-->|
        //  s  e        m
        //
        // E = 31 并且 M != 0
        // - negative infinity: m == 0: 1 111 11 00 0000 0000: 0xFC00
        //
        return data.val == 0xFC00;
    }


    Bool
    FloatPoint::isNegativeInfinite (
        const Real32 data)
    {
        // 31 (msb)
        // |
        // | 30     23
        // | |      |
        // | |      | 22                    0 (lsb)
        // | |      | |                     |
        // v v      v v                     v
        // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        // |1| <-8->| |<--       23      -->|
        //  s    e               m
        //
        // e = 255 并且 M ！= 0
        // - infinity: m == 0: 0xFF800000: 1 111 1111 1 000 0000 0000 0000 0000 0000
        //
        const UInt32 _bit_pattern = toUInt32(data);
        return _bit_pattern == 0xFF800000;
    }


    Bool
    FloatPoint::isNegativeInfinite (
        const Real64 data)
    {
        // 63 (msb)
        // |
        // | 62        52
        // | |         |
        // | |         | 51                                                 0 (lsb)
        // | |         | |                                                  |
        // v v         v v                                                  v
        // X XXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // |1| <--11-->| |             <--       52      -->                |
        //  s    e               m
        //
        // E = 2047 并且 M != 0
        // - infinity: m == 0: 0xFFF0000000000000
        //
        const UInt64 _bit_pattern = toUInt64(data);
        return _bit_pattern == 0xFFF0000000000000UL;
    }


    Bool
    FloatPoint::isNegativeZero (
        const Real16 data)
    {
        // 15 (msb)
        // |
        // | 14  10
        // | |   |
        // | |   | 9        0 (lsb)
        // | |   | |        |
        // v v   v v        v
        // X XXXXX abcdefghij
        // |1| 5 | |<--10-->|
        //  s  e        m
        // - ZERO: 符号位为：0或者1，指数位：0: 2^(-15)， 尾数位：0
        //   2^(-15) * (0*2^0 + 0)
        //   0x8000: -0
        //
        return data.val == 0x8000;
    }


    Bool
    FloatPoint::isNegativeZero (
        const Real32 data)
    {
        // 31 (msb)
        // |
        // | 30     23
        // | |      |
        // | |      | 22                    0 (lsb)
        // | |      | |                     |
        // v v      v v                     v
        // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        // |1| <-8->| |<--       23      -->|
        //  s    e               m
        // - ZERO: 2^(-127) * (0*2^0 + 0)
        //   0x80000000 = -0
        //
        const UInt32 _bit_pattern = toUInt32(data);
        return _bit_pattern == 0x80000000;
    }


    Bool
    FloatPoint::isNegativeZero (
        const Real64 data)
    {
        // 63 (msb)
        // |
        // | 62        52
        // | |         |
        // | |         | 51                                                 0 (lsb)
        // | |         | |                                                  |
        // X XXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // |1| <--11-->| |             <--       52      -->                |
        //  s    e               m
        // - ZERO: 2^(-1023) * (0*2^0 + 0)
        //   0x8000000000000000: -0
        //
        const UInt64 _bit_pattern = toUInt64(data);
        return _bit_pattern == 0x8000000000000000UL;
    }


    Bool
    FloatPoint::isPositiveInfinite (
        const Real16 data)
    {
        // 15 (msb)
        // |
        // | 14  10
        // | |   |
        // | |   | 9        0 (lsb)
        // | |   | |        |
        // v v   v v        v
        // X XXXXX XXXXXXXXXX
        // |1| 5 | |<--10-->|
        //  s  e        m
        //
        // E = 31 并且 M != 0
        // - positive infinity: m == 0: 0 111 11 00 0000 0000: 0x7C00
        //
        return data.val == 0x7C00;
    }


    Bool
    FloatPoint::isPositiveInfinite (
        const Real32 data)
    {
        // 31 (msb)
        // |
        // | 30     23
        // | |      |
        // | |      | 22                    0 (lsb)
        // | |      | |                     |
        // v v      v v                     v
        // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        // |1| <-8->| |<--       23      -->|
        //  s    e               m
        //
        // e = 255 并且 M ！= 0
        // - infinity: m == 0: 0x7F800000: 0 111 1111 1 000 0000 0000 0000 0000 0000
        //
        const UInt32 _bit_pattern = toUInt32(data);
        return _bit_pattern == 0x7F800000;
    }


    Bool
    FloatPoint::isPositiveInfinite (
        const Real64 data)
    {
        // 63 (msb)
        // |
        // | 62        52
        // | |         |
        // | |         | 51                                                 0 (lsb)
        // | |         | |                                                  |
        // v v         v v                                                  v
        // X XXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // |1| <--11-->| |             <--       52      -->                |
        //  s    e               m
        //
        // E = 2047 并且 M != 0
        // - infinity: m == 0: 0x7FF0000000000000
        //
        const UInt64 _bit_pattern = toUInt64(data);
        return _bit_pattern == 0x7FF0000000000000UL;
    }


    Bool
    FloatPoint::isPositiveZero (
        const Real16 data)
    {
        // 15 (msb)
        // |
        // | 14  10
        // | |   |
        // | |   | 9        0 (lsb)
        // | |   | |        |
        // v v   v v        v
        // X XXXXX abcdefghij
        // |1| 5 | |<--10-->|
        //  s  e        m
        // - ZERO: 符号位为：0或者1，指数位：0: 2^(-15)， 尾数位：0
        //   2^(-15) * (0*2^0 + 0)
        //   0x0000: +0
        //
        return data.val == 0;
    }


    Bool
    FloatPoint::isPositiveZero (
        const Real32 data)
    {
        // 31 (msb)
        // |
        // | 30     23
        // | |      |
        // | |      | 22                    0 (lsb)
        // | |      | |                     |
        // v v      v v                     v
        // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        // |1| <-8->| |<--       23      -->|
        //  s    e               m
        // - ZERO: 2^(-127) * (0*2^0 + 0)
        //   0x00000000: 2^(-127): 0
        //
        const UInt32 _bit_pattern = toUInt32(data);
        return _bit_pattern == 0;
    }


    Bool
    FloatPoint::isPositiveZero (
        const Real64 data)
    {
        // 63 (msb)
        // |
        // | 62        52
        // | |         |
        // | |         | 51                                                 0 (lsb)
        // | |         | |                                                  |
        // X XXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // |1| <--11-->| |             <--       52      -->                |
        //  s    e               m
        // - ZERO: 2^(-1023) * (0*2^0 + 0)
        //   0x0000000000000000: 2^(-1023): 0
        //
        const UInt64 _bit_pattern = toUInt64(data);
        return _bit_pattern == 0UL;
    }


    Bool
    FloatPoint::isZero (
        const Real16 data)
    {
        return isPositiveZero(data) || isNegativeZero(data);
    }


    Bool
    FloatPoint::isZero (
        const Real32 data)
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        return isPositiveZero(data) || isNegativeZero(data);
#else
        // if we just simply use the compare, compiler will generate the CPU instruction:
        // fucompp, which will ignore the sign bit for zero so +0 == -0
        return data == 0;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    Bool
    FloatPoint::isZero (
        const Real64 data)
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        return isPositiveZero(data) || isNegativeZero(data);
#else
        // if we just simply use the compare, compiler will generate the CPU instruction:
        // fucompp, which will ignore the sign bit for zero so +0 == -0
        return data == 0;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    Real32
    FloatPoint::nextFloat (
        const Real32 data)
    {
        // 32位单精度浮点数表示为：
        // 31 (msb)
        // |
        // | 30     23
        // | |      |
        // | |      | 22                    0 (lsb)
        // | |      | |                     |
        // v v      v v                     v
        // X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
        // |1| <-8->| |<--       23      -->|
        //  s    e               m
        // s: 符号位(sign bit)       : 1bit
        // e: 指数位(exponential bit): 8bits: the real exponential = e - 127: [-127, +128]
        // m: 尾数位(significand bit): 23bits
        // 十进制计算方法：S * 2^(e-127) * (1 + a*2^-1 + b*2^-2 + ... + j*2^-23)
        //
        // 两个浮点数之间的间隔为：2^E * 2^(-23)，只要是因为最小的变化为2^(-23)，如果只改变LSB位，这里E = e-127
        // 所以如果我们考虑[1, 2]之间的数值，此时E = 0， 所以他们的间隔为2^(-23)
        //
        // 检测是否为正无穷
        if (isPositiveInfinite(data))
        {
            return data;
        }

        // flip负零
        const Real32 _data_used = isNegativeZero(data) ? 0 : data;
        const UInt32 _next_bit_pattern = _data_used < 0 ?
                             toUInt32(_data_used) - 1 : toUInt32(_data_used) + 1;

        return fromUInt32(_next_bit_pattern);
    }


    Real64
    FloatPoint::nextFloat (
        const Real64 data)
    {
        // 检测是否为正无穷
        if (isPositiveInfinite(data))
        {
            return data;
        }

        // flip负零
        const Real64 _data_used = isNegativeZero(data) ? 0 : data;
        const UInt64 _next_bit_pattern = _data_used < 0 ?
                             toUInt64(_data_used) - 1 : toUInt64(_data_used) + 1;

        return fromUInt64(_next_bit_pattern);
    }


    Real32
    FloatPoint::prevFloat (
        const Real32 data)
    {
        // 检测是否为负无穷
        if (isNegativeInfinite(data))
        {
            return data;
        }

        // flip负零
        const Real32 _data_used = isNegativeZero(data) ? 0 : data;
        const UInt32 _prev_bit_pattern = _data_used < 0 ?
                             toUInt32(_data_used) + 1 : toUInt32(_data_used) - 1;

        return fromUInt32(_prev_bit_pattern);
    }


    Real64
    FloatPoint::prevFloat (
        const Real64 data)
    {
        // 检测是否为负无穷
        if (isNegativeInfinite(data))
        {
            return data;
        }

        // flip负零
        const Real64 _data_used = isNegativeZero(data) ? 0 : data;
        const UInt64 _prev_bit_pattern = _data_used < 0 ?
                             toUInt64(_data_used) + 1 : toUInt64(_data_used) - 1;

        return fromUInt64(_prev_bit_pattern);
    }
#endif // /* 此次的函数没有进行测试 */ #if 0

} // namespace Core
