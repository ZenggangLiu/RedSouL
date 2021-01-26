// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Profiling/RS_SampleMgr.hpp"
#include "Core/Profiling/RS_StatsIterator.hpp"
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
        // the default maximally supported number of children of a given sample
        static const SInt16 DEFAULT_MAX_CHILD_SAMPLE_NUM = 32;

        // Profiling workflow:
        // Frame Start                                                 Frame End
        // |                                                                |
        // v                                                                v
        // +-----------------+       +--------------+       +---------------+
        // | Collect Samples | ----> | Update Stats | ----> | Reset Samples | ---+
        // +-----------------+       +--------------+       +---------------+    |
        //          ^                                                            |
        //          |                                                            |
        //          +------------------------------------------------------------+
        //
        // checks if the all samples are closed
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
        // creates the iterator for the ROOT node
        StatsIterator _stats_it(DEFAULT_MAX_CHILD_SAMPLE_NUM);

        // dump all children under to ROOT node
        const UInt16 _child_count = _stats_it.count();
        for (UInt16 c = 0; c < _child_count; ++c)
        {
            _stats_it.stepInto(c);
            dumpStatsData(_stats_it, 0);
            _stats_it.stepOut();
        }

        // ends the sample collection of this frame
        SampleMgr::getRef().endFrame();
    }


    Profiling::Profiling (
        const Char *const name)
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
        StatsIterator& stats_it,
        const UInt16   indent)
    {
        // dumps itself
        for (UInt16 c = 0; c < indent; ++c)
        {
            printf("  ");
        }

        const SampleStats _stats = stats_it.stats();
        printf("- %s: "
               "[#used]: %d, "
               "[Total]: %.4f, [Avg Total]: %.4fs, [Self]: %.4f, [Avg Self]: %.4f\n",
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

