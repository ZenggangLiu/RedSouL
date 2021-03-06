// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Profiling/RS_SampleNode.hpp"
#include "Core/Profiling/RS_StatsNode.hpp"
// Self
#include "Core/Profiling/RS_StatsIterator.hpp"


namespace Core
{

    const UInt8 StatsIterator::BOOKKEEPING_DATA_NUM = 1;



    // Sample tree:
    //         +-----------------+
    //         |    ROOT NODE    |
    //         +-----------------+
    //          /       |       \
    //       +---+    +---+    +---+
    //       | N |    | N |    | N | ---------+
    //       +---+    +---+    +---+          |
    //         |        |                     |
    //       +---+      =                  +------+
    //       | N |                         | NODE |
    //       +---+                         +------+
    //         |                            /    \
    //         =                         +---+  +---+
    //                                   | N |  | N |
    //                                   +---+  +---+
    //                                     |      |
    //                                     =    +---+
    //                                          | N |
    //                                          +---+
    //                                            |
    //                                            =
    //
    StatsIterator::StatsIterator (
        const UInt16 max_subbranch_count)
    {
        // reserves enough space for data
        m_data.reserve(max_subbranch_count + BOOKKEEPING_DATA_NUM);

        // setups the iterator of the ROOT node
        setup(0);
    }


    UInt16
    StatsIterator::count () const
    {
        return (UInt16)(m_data.size() - BOOKKEEPING_DATA_NUM);
    }


    SampleStats
    StatsIterator::stats () const
    {
        SampleStats _stats;
        const SInt16 _self_sample_idx = m_data[0];
        // gets the constant reference to the sample node
        const SampleNode& _self_node = SampleMgr::getRef().node(_self_sample_idx);
        // gets the constant reference to the stats node
        const StatsNode& _self_stats = SampleMgr::getRef().statsNode(_self_sample_idx);
        // creates the stats
        _stats.name           = _self_node.name;
        _stats.used_num       = _self_stats.used_num;
        _stats.self_time      = _self_stats.self_time;
        _stats.total_time     = _self_stats.total_time;
        _stats.avg_self_time  = _self_stats.avg_self_time;
        _stats.min_self_time  = _self_stats.min_self_time;
        _stats.max_self_time  = _self_stats.max_self_time;
        _stats.var_self_time  = _self_stats.var_self_time;
        _stats.avg_total_time = _self_stats.avg_total_time;
        _stats.min_total_time = _self_stats.min_total_time;
        _stats.max_total_time = _self_stats.max_total_time;
        _stats.var_total_time = _self_stats.var_total_time;

        return _stats;
    }


    void
    StatsIterator::stepInto (
        const UInt16 subbranch_idx)
    {
        // checks if the given index is valid
        RUNTIME_ASSERT(subbranch_idx < count(), "Sub-branch index is out of range");

        // 由于内部的子Sample是按如下方式存储的：
        // +-------+-------+-------+     +-------+
        // | nth C | n-1 C | n-2 C | ... | 1st C |
        // +-------+-------+-------+     +-------+
        //
        // 但是用户希望得到如下的排序方式：
        // +-------+-------+-------+     +-------+
        // | 1st C | 2nd C | 3rd C | ... | nth C |
        // +-------+-------+-------+     +-------+
        //
        // calculate the index used to access the child list
        const UInt16 _access_idx = count() + BOOKKEEPING_DATA_NUM - 1 - subbranch_idx;

        // gets the index of child sample(in the sample tree)
        const UInt16 _child_sample_idx = m_data[_access_idx];

        // setups the data table
        setup(_child_sample_idx);
    }


    void
    StatsIterator::stepOut ()
    {
        // gets the parent sample index of the current branch
        const SampleNode& _branch = SampleMgr::getRef().node(m_data[0]);
        if (_branch.hasParent())
        {
            // setups the data table
            setup(_branch.parent_idx);
        }
    }


    void
    StatsIterator::setup (
        const UInt16 branch_idx)
    {
        // 清除现有节点列表
        m_data.clear();

        // 存储Branch索引
        m_data.push_back(branch_idx);

        // 创建子树列表
        const SampleNode & _branch = SampleMgr::getRef().node(branch_idx);
        if (_branch.hasChild())
        {
            const UInt32 _cur_frame_num = SampleMgr::getRef().frameNum();
            const SampleNode* _child = _branch.child();
            // 收集所有子树索引
            while (_child)
            {
                // 判断此子树是否属于为当前帧
                if (_child->frame_num == _cur_frame_num)
                {
                    m_data.push_back(_child->self_idx);
                }

                // 获得下一个子树的索引：
                // +---+---+     +---+---+     +---+---+            +---+---+
                // | N | C | --> | N | S | --> | N | S | ...... --> | N | S | --+
                // +---+---+     +---+---+     +---+---+            +---+---+   |
                //                                                              =
                //
                _child = _child->sibling();
            }
        }
    }

} // namespace Core
