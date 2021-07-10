// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Time/RS_Time.hpp"
// Self
#include "Core/Mathe/RS_PRN_Generator.hpp"



namespace Core
{

    // PRN Generator的索引
    static UInt64 g_prn_generator_index = 0;



    // 参考：Numerical recipes 3rd at page 341 - 357 for more details: http://www.nr.com/
    //
    PRN_Generator::PRN_Generator ()
    :
        v(4101842887655102017ULL),
        w(1)
    {
        // 创建基于时间的Seed
        const UInt64 _seed_value = (UInt64)getCurTimeSec() + (g_prn_generator_index++);
        // 使用Seed内存的位置来得到一个变化的数值
        const UInt64 _vary_value = (UInt64)&_seed_value;

        // 初始化
        initialize(_seed_value ^ _vary_value);
    }


    PRN_Generator::PRN_Generator (
        const UInt64 seed)
    :
        v(4101842887655102017ULL),
        w(1)
    {
        initialize(seed);
    }


    UInt16
    PRN_Generator::getUInt16 ()
    {
        const UInt32 _rand_num = getUInt32();

        return (UInt16)(((_rand_num & 0xFFFF0000) >> 16) ^ (_rand_num & 0x0000FFFF));
    }


    UInt32
    PRN_Generator::getUInt32 ()
    {
        const UInt64 _rand_num = getUInt64();

        return (UInt32)(((_rand_num & 0xFFFFFFFF00000000ULL) >> 32) ^ (_rand_num & 0xFFFFFFFF));
    }


    UInt64
    PRN_Generator::getUInt64 ()
    {
        u  = u * 2862933555777941757ULL + 7046029254386353087ULL;
        v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
        w  = 4294957665U * (w & 0xffffffff) + (w >> 32);

        UInt64 x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;

        return (x + v) ^ w;
    }


    Real32
    PRN_Generator::getReal32()
    {
        //  1
        //  --   * UInt32
        // 2^32
        return (Real32)2.328306436538696289E-10 * getUInt32();
    }


    Real64
    PRN_Generator::getReal64 ()
    {
        //  1
        //  --   * int64
        // 2^64
        return 5.42101086242752217E-20 * getUInt64();
    }


    void
    PRN_Generator::initialize (
        const UInt64 exp_seed)
    {
        UInt64 _seed = exp_seed;
        if (4101842887655102017ULL == _seed)
        {
            _seed++;
        }

        u = _seed ^ v; getUInt64();
        v = u;         getUInt64();
        w = v;         getUInt64();
    }



    PRN_Generator_Fast::PRN_Generator_Fast ()
    :
        v(4101842887655102017ULL)
    {
        // 创建基于时间的Seed
        const UInt64 _seed_value = (UInt64)getCurTimeSec() + (g_prn_generator_index++);
        // 使用Seed内存的位置来得到一个变化的数值
        const UInt64 _vary_value = (UInt64)&_seed_value;

        // 初始化
        initialize(_seed_value ^ _vary_value);
    }


    PRN_Generator_Fast::PRN_Generator_Fast (
        const UInt64 seed)
    :
        v(4101842887655102017ULL)
    {
        initialize(seed);
    }


    UInt16
    PRN_Generator_Fast::getUInt16 ()
    {
        const UInt32 _rand_num = getUInt32();

        return (UInt16)(((_rand_num & 0xFFFF0000) >> 16) ^ (_rand_num & 0x0000FFFF));
    }


    UInt32
    PRN_Generator_Fast::getUInt32 ()
    {
        const UInt64 _rand_num = getUInt64();

        return (UInt32)(((_rand_num & 0xFFFFFFFF00000000ULL) >> 32) ^ (_rand_num & 0xFFFFFFFF));
    }


    UInt64
    PRN_Generator_Fast::getUInt64 ()
    {
        v ^= v >> 21; v ^= v << 35; v ^= v >> 4;
        return v * 2685821657736338717ULL;
    }


    Real32
    PRN_Generator_Fast::getReal32 ()
    {
        //  1
        //  --   * UInt32
        // 2^32
        return (Real32)2.328306436538696289E-10 * getUInt32();
    }


    Real64
    PRN_Generator_Fast::getReal64 ()
    {
        //  1
        //  --   * UInt64
        // 2^64
        return 5.42101086242752217E-20 * getUInt64();
    }


    void
    PRN_Generator_Fast::initialize (
        const UInt64 exp_seed)
    {
        v ^= exp_seed; v = getUInt64();
    }

} // namespace Core
