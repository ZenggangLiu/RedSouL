// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Profiling/RS_SampleMgr.hpp"
#include "Core/Profiling/RS_StatsIterator.hpp"
#include "Core/String/RS_TextHelper.hpp"
// Self
#include "Core/Profiling/RS_Profiling.hpp"


namespace Core
{

    void
    Profiling::setTotalSampleCount (
        const UInt16 count)
    {
        SampleMgr::getRef().setTotalSampleCount(count);
    }


    void
    Profiling::dumpStatsData ()
    {
        // 任意一个Sample最大支持的子Sample的个数
        static const SInt16 DEFAULT_MAX_CHILD_SAMPLES_NUMBER = 32;

        // Profiling workflow:
        // 一帧的开始                                               一帧的结尾
        // |                                                                |
        // v                                                                v
        // +-----------------+       +--------------+       +---------------+
        // | Collect Samples | ----> | Update Stats | ----> | Reset Samples | ---+
        // +-----------------+       +--------------+       +---------------+    |
        //          ^                                                            |
        //          |                                                            |
        //          +------------------------------------------------------------+
        //
        // 检查是否所有Sample都已经闭合
        if (!SampleMgr::getRef().allClosed())
        {
            printf("[dumpStatsData] called with some UNCLOSED nodes\n");

            // closes all pending samples
            SampleMgr::getRef().closeAll();
        }

        // updates the stats
        SampleMgr::getRef().updateStats();

        // resets the frame data
        SampleMgr::getRef().reset();

        // --- Dumps Stats --- //
        // 为根节点创建iterator
        StatsIterator _stats_it(DEFAULT_MAX_CHILD_SAMPLES_NUMBER);

        // dump根节点下的所有子节点
        const UInt16 _child_count = _stats_it.count();
        for (UInt16 c = 0; c < _child_count; ++c)
        {
            _stats_it.stepInto(c);
            dumpStatsData(_stats_it, 0);
            _stats_it.stepOut();
        }

        // 标记当前帧Sample采集结束
        SampleMgr::getRef().endFrame();
    }


    Profiling::Profiling (
        const UTF8 *const name)
    {
        // opens the named sample
        SampleMgr::getRef().openNamedSample(name);
    }


    Profiling::~Profiling ()
    {
        // closes the current sample
        SampleMgr::getRef().closeCurSample();
    }


    void
    Profiling::dumpStatsData (
        StatsIterator & stats_it,
        const UInt16    indent)
    {
        // dumps itself
        for (UInt16 c = 0; c < indent; ++c)
        {
            printf("  ");
        }

        UTF8 _buffer[256];
        TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                "- %%-%ds: "
                                "[#used]: %%4d, "
                                "[T]: %%6.3fs, [S]: %%6.3fs, [AT]: %%6.3fs, [AS]: %%6.3fs\n",
                                32 - (SInt16)indent * 2);
        // 获得当前Iterator所保存的统计数据
        const SampleStats _stats = stats_it.stats();
        printf((const char*)_buffer,
               _stats.name,
               _stats.used_num,
               _stats.total_time, _stats.avg_total_time, _stats.self_time, _stats.avg_self_time);

        // dump all children
        const UInt16 _child_count = stats_it.count();
        for (UInt16 c = 0; c < _child_count; ++c)
        {
            stats_it.stepInto(c);
            dumpStatsData(stats_it, indent + 1);
            stats_it.stepOut();
        }
    }

} // namespace Core

