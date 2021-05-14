// System headers
#include <vector>
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // The statistic of a profiling sample
    struct SampleStats
    {
        // Sample的名称
        const UTF8* name;

        // --- 最后一个帧的Sample数据 --- //
        // 此Sample使用次数
        UInt32      used_num;
        // --- 时间 --- //
        // 此Sample所包含代码占用的时间(秒): 不包括所有子Samples占用的时间
        Real32      self_time;
        // 当Sample总共占用的时间(秒): 所包含代码+所有子节点包含的代码总共占用的时间
        Real32      total_time;

        // --- Data over Frames --- //
        // --- Self Time --- //
        // the averaged self time
        Real32      avg_self_time;
        // the minimal self time
        Real32      min_self_time;
        // the maximal self time
        Real32      max_self_time;
        // the variance of the self time
        Real32      var_self_time;

        // --- Total Time --- //
        // the averaged total time
        Real32      avg_total_time;
        // the minimal total time
        Real32      min_total_time;
        // the maximal total time
        Real32      max_total_time;
        // the variance of the time
        Real32      var_total_time;
    };


    class StatsIterator
    {
    public:
        // Constructs an iterator
        //
        // @param[in] max_subbranch_count
        //      the maximally allowed number of sub-branches
        StatsIterator (
            const UInt16 max_subbranch_count);

        // Gets the total number of sub-branches
        UInt16
        count () const;

        // Gets the stats of this branch
        SampleStats
        stats () const;

        // Steps into the given sub-branch
        //
        // @param[in] subbranch_idx
        //      the index of the subbranch, which must be: [0, count()-1]
        void
        stepInto (
            const UInt16 subbranch_idx);


        // Steps out of the current branch and goes back to the parent branch
        void
        stepOut ();


    private:
        // Setups the iterator for the given branch
        void
        setup (
            const UInt16 branch_idx);

    private:
        // the number of data members used for bookkeeping
        static const UInt8 BOOKKEEPING_DATA_NUM;

        // the data stored in this iterator
        //     branch index
        //     |
        //     v    int16   int16
        // +--------+-------+-------+-------+     +-------+
        // | Branch | nth C | n-1 C | n-2 C | ... | 1st C |
        // +--------+-------+-------+-------+     +-------+
        // int16        ^
        //              |
        //              the last sub-branch/child
        //
        std::vector<SInt16> m_data;
    };

}
