#pragma once


// System headers
// Lib headers
// Common
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // 伪随机数发生器：
    // - 当调用N(N无穷大)次时，将产生均匀分布的数据序列
    // - 相同的Seed，将产生相同的数值序列
    // - 不同的Seed，将产生完全不同的数值序列
    // - 比std::rand()在Windows上平均快10倍，在MacOS上平均快2.5倍
    //   + 生成1亿个数值
    //    1. macOS：
    //       [Std]:      0.623863秒
    //       [PRN]:      0.304528秒
    //       [PRN_Fast]: 0.275172秒
    //    2. Windows:
    //       [Std]:      1.784681秒
    //       [PRN]:      0.211637秒
    //       [PRN_Fast]: 0.171741秒
    //
    // 伪随机数发生器(高质量版本)
    // 数值重复周期：3.138*10^57.
    class PRN_Generator;
    // 伪随机数发生器(高速度版本)
    // 数值重复周期：1.8*10^19
    class PRN_Generator_Fast;


    class PRN_Generator
    {
    public:
        // 使用当前时间作为Seed来构建一个伪随机数发生器
        PRN_Generator ();

        // 使用给定Seed来构建一个伪随机数发生器
        PRN_Generator (
            const UInt64 seed);

        // 获得下一个16位随机数: [0, Max16]
        UInt16
        getUInt16 ();

        // 获得下一个32位随机数: [0, Max32]
        UInt32
        getUInt32 ();

        // 获得下一个64位随机数: [0, Max64]
        UInt64
        getUInt64 ();

        // 获得下一个32浮点数位随机数
        // NOTE：数值范围: [0, 1]
        Real32
        getReal32 ();

        // 获得下一个64浮点数位随机数
        // NOTE：数值范围: [0, 1]
        Real64
        getReal64 ();


    private:
        // 初始化这个伪随机数发生器
        void
        initialize (
            const UInt64 exp_seed);

    private:
        UInt64 u, v, w;
    };


    class PRN_Generator_Fast
    {
    public:
        // 使用当前时间作为Seed来构建一个伪随机数发生器
        PRN_Generator_Fast ();

        // 使用给定Seed来构建一个伪随机数发生器
        PRN_Generator_Fast (
            const UInt64 seed);


        // 获得下一个16位随机数: [0, Max16]
        UInt16
        getUInt16 ();

        // 获得下一个32位随机数: [0, Max32]
        UInt32
        getUInt32 ();

        // 获得下一个64位随机数: [0, Max64]
        UInt64
        getUInt64 ();

        // 获得下一个32浮点数位随机数
        // NOTE：数值范围: [0, 1]
        Real32
        getReal32 ();

        // 获得下一个64浮点数位随机数
        // NOTE：数值范围: [0, 1]
        Real64
        getReal64 ();


    private:
        // 初始化这个伪随机数发生器
        void
        initialize (
            const UInt64 exp_seed);

    private:
        UInt64 v;
    };

} // namespace Core
