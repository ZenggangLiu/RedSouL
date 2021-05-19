// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Profiling/RS_Profiling.hpp"              // DEFAULT_SAMPLE_COUNT
#include "Core/Profiling/RS_SampleNode.hpp"
#include "Core/Profiling/RS_StatsNode.hpp"
// Self
#include "Core/Profiling/RS_SampleMgr.hpp"


namespace Core
{

    // the application start time
    const Real64 SampleMgr::APPLICATION_START_TIME_SECS = getCurTimeSec();



    SampleMgr&
    SampleMgr::getRef ()
    {
        static SampleMgr s_instance;
        return s_instance;
    }


    Real64
    SampleMgr::appStartTime ()
    {
        return APPLICATION_START_TIME_SECS;
    }


    UInt32
    SampleMgr::frameNum() const
    {
        return m_frame_num;
    }


    Bool
    SampleMgr::allClosed () const
    {
        return m_cur_node->isRoot();
    }


    Bool
    SampleMgr::isValid (
        const UInt16 sample_idx) const
    {
        return sample_idx && sample_idx < m_free_slot;
    }


    SInt16
    SampleMgr::treeSize () const
    {
        return (SInt16)m_sample_tree.size();
    }


    const SampleNode&
    SampleMgr::node (
        const UInt16 sample_idx) const
    {
        RUNTIME_ASSERT(sample_idx < (UInt16)m_sample_tree.size(), "Branch index is out of range");
        return m_sample_tree[sample_idx];
    }


    const StatsNode&
    SampleMgr::statsNode (
        const UInt16 sample_idx) const
    {
        RUNTIME_ASSERT(sample_idx < (UInt16)m_sample_tree.size(), "Branch index is out of range");
        return m_stats_tree[sample_idx];
    }


    void
    SampleMgr::setTotalSampleCount (
        const UInt16 count)
    {
        // resizes sample tree and stats array
        RUNTIME_ASSERT(count <= MAX_SAMPLE_COUNT,
                       "The given sample count is TOO large"
                       "Currently we are using SInt16 to represent node index");
        m_sample_tree.resize(count);
        m_stats_tree.resize(count);
    }


    void
    SampleMgr::openNamedSample (
        const UTF8 *const name)
    {
        // 检查是否我们正在开启一个新的Sample: 是否与当前Sample的名字匹配
        if (m_cur_node->name != name)
        {
            // --- NOTE：此处开启的Sample始终被视为当前Sample的子节点 ---

            // 使用给定名称在当前Sample中查找子节点
            SampleNode *const _parent_node = m_cur_node;
            m_cur_node = m_cur_node->child(name);
            // 全新的子节点
            if (m_cur_node == nullptr)
            {
                RUNTIME_ASSERT(m_free_slot <= MAX_SAMPLE_COUNT - 1, "No place to store more samples");
                // 使用空余Slot的Index为此子节点的索引
                const SInt16 _node_idx = m_free_slot++;
                // 初始化此子节点
                SampleNode *const _child_node = m_sample_tree.data() + _node_idx;
                m_cur_node = new(_child_node) SampleNode(name, _parent_node->self_idx, _node_idx);

                // 将新的子节点视为当前父节点的第一个子节点
                // +-------------+     +---------+     +-----------+            +---------+
                // | 新创建的节点 | --> | child n | --> | child n-1 | ...... --> | child 0 |--+
                // +-------------+     +---------+     +-----------+            +---------+  |
                //                                                                           =
                m_cur_node->sib_idx     = _parent_node->child_idx;
                _parent_node->child_idx = _node_idx;
            }
        }

        m_cur_node->use();
    }


    void
    SampleMgr::closeCurSample ()
    {
        // stops the current sample and checks if we need to go back to the parent sample
        if (m_cur_node->exit())
        {
            m_cur_node = m_cur_node->parent();
        }
    }


    void
    SampleMgr::closeAll ()
    {
        // closes all pending parent nodes
        while (m_cur_node->hasParent())
        {
            closeCurSample();
        }
    }


    void
    SampleMgr::updateStats ()
    {
        // +-----+-------+     +-------+     +-------+            +-------+
        // |  N  | CHILD | --> | N | S | --> | N | S | ...... --> | N | S | --+
        // |  O  | IDX   |     +-------+     +-------+            +-------+   |
        // |  D  |-------+                                                    =
        // |  E  | SIB   |     +-------+     +-------+     +-------+             +-------+
        // |     | IDX   | --> | N | S | --> | N | S | --> | N | S | ....... --> | N | S | --+
        // +-----+-------+     +-------+     +-------+     +-------+             +-------+   |
        //                                                                                   =

        // checks if the ROOT node has any child
        const SampleNode* _child = m_sample_tree[0].child();
        while (_child)
        {
            // updates the stats for the current child
            updateStats(_child);

            // goes to the next child
            _child = _child->sibling();
        }
    }


    void
    SampleMgr::reset ()
    {
        // resets the frame data of all samples
        // checks if the ROOT node has any child
        SampleNode* _child = m_sample_tree[0].child();
        while (_child)
        {
            // resets the frame data of the current child
            reset(_child);

            // goes to the next child
            _child = _child->sibling();
        }
    }


    void
    SampleMgr::endFrame()
    {
        // increases the frame number
        ++m_frame_num;
    }


    SampleMgr::SampleMgr ()
    :
        m_sample_tree(DEFAULT_SAMPLE_COUNT),
        m_stats_tree(DEFAULT_SAMPLE_COUNT),
        m_cur_node(m_sample_tree.data()),
        m_frame_num(1),
        m_free_slot(1)
    {
        // creates the root sample node: no name, no parent
        m_sample_tree[0].self_idx = 0;
    }


    void
    SampleMgr::reset (
        SampleNode *const sample)
    {
        // resets the current sample
        sample->reset();

        // resets all children samples
        SampleNode* _child = sample->child();
        while (_child)
        {
            // reset the current child
            reset(_child);

            // goes to the next child
            _child = _child->sibling();
        }
    }


    void
    SampleMgr::updateStats (
        const SampleNode *const sample)
    {
        StatsNode& _stats = m_stats_tree[sample->self_idx];

        // calculates the self time of the given sample
        const Real32 _total_time = sample->total_time;
        const Real32 _self_time  = _total_time - sample->childrenTime();

        // updates the data in the last frame
        _stats.used_num   = sample->used_num;
        _stats.self_time  = _self_time;
        _stats.total_time = _total_time;

        // the current frame count: N
        const UInt32 _frame_count = _stats.frame_count;
        // updates the data over frame
        if (_frame_count == 1)
        {
            // --- Self Time --- //
            _stats.avg_self_time = _self_time;
            _stats.min_self_time = _self_time;
            _stats.max_self_time = _self_time;
            //_stats.var_self_time;

            // --- Total Time --- //
            _stats.avg_total_time = _total_time;
            _stats.min_total_time = _total_time;
            _stats.max_total_time = _total_time;
            //_stats.var_total_time;

            // --- Quick Calc on the total time --- //
            _stats.quick_avg_total_time = _total_time;
            _stats.quick_min_total_time = _total_time;
            _stats.quick_max_total_time = _total_time;
        }
        else
        {
            // N-1
            const UInt32 _n_1 = _frame_count - 1;
            // Avg_n = Avg_n1 + (Xn - Avg_n1)/n
            //       = [(n-1)Avg_n1 + Xn]/n
            // --- Self Time --- //
            _stats.avg_self_time = (_n_1*_stats.avg_self_time + _self_time)/_frame_count;
            if (_stats.min_self_time > _self_time)
            {
                _stats.min_self_time = _self_time;
            }
            if (_stats.max_self_time < _self_time)
            {
                _stats.max_self_time = _self_time;
            }
            //_stats.var_self_time;

            // --- Total Time --- //
            _stats.avg_total_time = (_n_1*_stats.avg_total_time + _total_time)/_frame_count;
            if (_stats.min_total_time > _total_time)
            {
                _stats.min_total_time = _total_time;
            }
            if (_stats.max_total_time < _total_time)
            {
                _stats.max_total_time = _total_time;
            }
            //_stats.var_total_time;

            // --- Quick Calc on the total Time --- //
            _stats.quick_avg_total_time = _total_time;
            _stats.quick_min_total_time = _total_time;
            _stats.quick_max_total_time = _total_time;
        }

        // increase the frame count
        ++_stats.frame_count;

        // calculates the stats of all children samples
        const SampleNode* _child = sample->child();
        while (_child)
        {
            // updates the stats for the current child
            updateStats(_child);

            // goes to the next child
            _child = _child->sibling();
        }
    }

} // namespace Core
