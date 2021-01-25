#pragma once


// System headers
// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    class StatsIterator;


    // CPU代码Profiling
    class Profile
    {
    public:
        // 设置Sample的总数
        // NOTE: 请在程序启动的时候调用此函数。否则Sample的数目将设定为DEFAULT_SAMPLE_COUNT
        static
        void
        setTotalSampleCount (
            const UInt16 count);

        // Dump出所有Sample的统计数据
        static
        void
        dumpStatsData ();

        // 创建一个名字为name的Profile
        Profile (
            const Char *const name);

        ~Profile ();


    private:
        // Dump当前Branch的Stats
        //
        // @param[in,out] stats_it
        //      the reference to the stats iterator
        // @param[in] indent
        //      the level of indent starting from 0
        static
        void
        dumpStatsData (
            StatsIterator& stats_it,
            const UInt16   indent);
    };

} // namespace Core


// 缺省采样数目
#define DEFAULT_SAMPLE_COUNT                1500
// 最大可支持的采样数目
#define MAX_SAMPLE_COUNT                    (1 << 15)

// 定义Profiling使用的宏
#if !defined(PROFILING_MODE)
#error Please define PROFILING_MODE in the build setting("Preprocessor Macros")
#endif // #if !defined(PROFILING_MODE)

#if (PROFILING_MODE == 1)
    // NOTE: name必须为静态字符串指针：PROFILE("Literal")
    #define PROFILE(name)                   Core::Profile TOKEN_COMBINE(_profile_, __LINE__)(name)
    #define PROFILE_DUMP()                  Core::Profile::dumpStatsData()
    #define PROFILE_SET_MAX_SAMPLES(count)  Core::Profile::setTotalSampleCount(count)

#else
    #define PROFILE(name)
    #define PROFILE_DUMP()
    #define PROFILE_SET_MAX_SAMPLES(count)
#endif // #if (PROFILING_MODE == 1)
