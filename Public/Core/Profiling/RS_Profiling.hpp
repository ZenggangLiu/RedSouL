#pragma once


// System headers
// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    class StatsIterator;


    // CPU代码Profiling
    class Profiling
    {
    public:
        // 设置Sample的总数
        // NOTE: 请在程序启动的时候调用此函数。否则Sample的数目将设定为DEFAULT_SAMPLE_COUNT
        static
        void
        setTotalSampleCount (
            const UInt16 count);

        // Dump所有Sample的统计数据
        static
        void
        dumpStatsData ();

        // 开始Profiling：启动一个名字为name的Sample
        Profiling (
            const UTF8 *const name);

        // 停止Profiling：暂停当前的Sample
        ~Profiling ();


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
#define DEFAULT_SAMPLE_COUNT                500
// 最大可支持的采样数目
#define MAX_SAMPLE_COUNT                    (1 << 15)

// 定义Profiling使用的宏
#if !defined(PROFILING_MODE)
#error Please define PROFILING_MODE in the build setting("Preprocessor Macros")
#endif // #if !defined(PROFILING_MODE)

#if (PROFILING_MODE == 1)
    // 期待一个name的Profiling Sample
    // NOTE: name必须为静态字符串指针：PROFILING("Literal")
    #define PROFILING(name)                Core::Profiling TOKEN_COMBINE(_sample_, __LINE__)(name)
    // Dump所有的Sample
    #define PROFILING_DUMP()               Core::Profiling::dumpStatsData()
    // 设定最大的Sample数目
    #define PROFILING_SET_MAX_SAMPLES(num) Core::Profiling::setTotalSampleCount(num)

#else
    #define PROFILING(name)
    #define PROFILING_DUMP()
    #define PROFILING_SET_MAX_SAMPLES(num)
#endif // #if (PROFILING_MODE == 1)
