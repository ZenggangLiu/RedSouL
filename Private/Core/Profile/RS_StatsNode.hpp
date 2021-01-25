#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // One node used to store statistic data of a sample over frames
    struct StatsNode
    {
        // --- Unique Data --- //
        // the count of collected frames
        UInt32  frame_count = 1;

        // --- Data in the last Frame --- //
        // the number of usage in the last frame
        UInt32  used_num;
        // the self time in the last frame
        Real32 self_time;
        // the total time in the last frame
        Real32 total_time;

        // --- Data over Frames --- //
        // --- Self Time --- //
        // the averaged self time
        Real32 avg_self_time;
        // the minimal self time
        Real32 min_self_time;
        // the maximal self time
        Real32 max_self_time;
        // the variance of the self time
        Real32 var_self_time;

        // --- Total Time --- //
        // the averaged total time
        Real32 avg_total_time;
        // the minimal total time
        Real32 min_total_time;
        // the maximal total time
        Real32 max_total_time;
        // the variance of the total time
        Real32 var_total_time;

        // --- Quick Calc on the total Time --- //
        // the averaged total time
        Real32 quick_avg_total_time;
        // the minimal total time
        Real32 quick_min_total_time;
        // the maximal total time
        Real32 quick_max_total_time;
    };

} // namespace Core
