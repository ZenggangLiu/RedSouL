#pragma once


// System headers
// Lib headers
#include "Core/Assert/RS_CompiletimeAssert.hpp"
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Profiling/RS_SampleMgr.hpp"
#include "Core/Time/RS_Time.hpp"


namespace Core
{

    // One tree node used to store one frame data of a sample
    struct SampleNode
    {
        // --- Unique Data --- //
        /// the name of this sample
        const UTF8 *const name;

        // --- Tree Hierarchy --- //
        // +-----+-------+     +-------+     +-------+            +-------+
        // |  N  | CHILD | --> | N | S | --> | N | S | ...... --> | N | S | --+
        // |  O  | IDX   |     +-------+     +-------+            +-------+   |
        // |  D  |-------+                                                    =
        // |  E  | SIB   |     +-------+     +-------+     +-------+             +-------+
        // |     | IDX   | --> | N | S | --> | N | S | --> | N | S | ....... --> | N | S | --+
        // +-----+-------+     +-------+     +-------+     +-------+             +-------+   |
        //                                                                                   =
        //
        // calculate the address of a node from index:
        // Addr = this + SInt32(IDX - SELF)*sizeof(SampleNode)
        // the index of the parent node: -1 means NO parent node
        SInt16            parent_idx;
        // the index of this node: -1 means Root node
        SInt16            self_idx;
        // the index of the first child node: -1 means NO child node
        SInt16            child_idx;
        // the index of the first sibling node: -1 means NO sibling node
        SInt16            sib_idx;

        // --- Frame Data --- //
        // the number of the frame in which the data was collected
        UInt32            frame_num;
        // #used: the number of usages of this sample
        UInt32            used_num;

        // #opened: the number of opened(recursive calls)
        UInt32            opened_num;
        // the start time related to the application's begin
        Real32            start_time;

        // the totally spent time(including time in this sample and all children samples)
        Real32            total_time;

        // Constructs an invalid node
        INLINE_FUNCTION
        SampleNode ()
        :
            name(nullptr),
            parent_idx(-1),
            self_idx(-1),
            child_idx(-1),
            sib_idx(-1),
            frame_num(0),
            used_num(0),
            opened_num(0),
            total_time(0)
        {

        }


        // Constructs a valid/unused node
        //
        // @param[in] sample_name
        //      the name of the sample
        // @param[in] parent_sample_idx
        //      the index of the parent sample in the sample tree
        // @param[in] self_sample_idx
        //      the index of this sample in the sample tree
        INLINE_FUNCTION
        SampleNode(
            const UTF8 *const sample_name,
            const SInt16      parent_sample_idx,
            const SInt16      self_sample_idx)
        :
            name(sample_name),
            parent_idx(parent_sample_idx),
            self_idx(self_sample_idx),
            child_idx(-1),
            sib_idx(-1),
            frame_num(0),
            used_num(0),
            opened_num(0),
            total_time(0)
        {
            // NOTE: no need to update the children list of the parent sample,
            //       because it will be updated when we are creating a new sample
        }


        // Checks if the current node is ROOT node
        INLINE_FUNCTION
        Bool
        isRoot () const
        {
            return name == nullptr && parent_idx == -1;
        }

        // Checks if the current node has parent node
        INLINE_FUNCTION
        Bool
        hasParent () const
        {
            return parent_idx >= 0;
        }


        // Checks if the current node has child node
        INLINE_FUNCTION
        Bool
        hasChild () const
        {
            return child_idx >= 0;
        }


        // Checks if the current node has sibling node
        INLINE_FUNCTION
        Bool
        hasSibling () const
        {
            return sib_idx >= 0;
        }


        // Gets the time spent in all children samples
        INLINE_FUNCTION
        Real32
        childrenTime () const
        {
            Real32 _children_time = 0;
            // if the current sample has sub-samples
            if (child_idx >= 0)
            {
                // gets the pointer to the first child
                const SampleNode* _child = nodeAddr(child_idx);
                while (_child)
                {
                    // accumulates the child time
                    _children_time += _child->total_time;

                    // goes to the next child
                    // +---+---+     +---+---+     +---+---+            +---+---+
                    // | N | C | --> | N | S | --> | N | S | ...... --> | N | S | --+
                    // +---+---+     +---+---+     +---+---+            +---+---+   |
                    //                                                              =
                    //
                    _child = _child->sibling();
                }
            }

            return _children_time;
        }


        // Gets the pointer to the parent node
        INLINE_FUNCTION
        const SampleNode*
        parent () const
        {
            if (parent_idx < 0)
            {
                return nullptr;
            }
            else
            {
                return nodeAddr(parent_idx);
            }
        }


        INLINE_FUNCTION
        SampleNode*
        parent ()
        {
            if (parent_idx < 0)
            {
                return nullptr;
            }
            else
            {
                return nodeAddr(parent_idx);
            }
        }


        // Gets the pointer to the first child node
        INLINE_FUNCTION
        const SampleNode*
        child () const
        {
            if (child_idx < 0)
            {
                return nullptr;
            }
            else
            {
                return nodeAddr(child_idx);
            }
        }


        INLINE_FUNCTION
        SampleNode*
        child ()
        {
            if (child_idx < 0)
            {
                return nullptr;
            }
            else
            {
                return nodeAddr(child_idx);
            }
        }


        // Gets the pointer to the matching child
        INLINE_FUNCTION
        SampleNode*
        child (
            const UTF8 *const name)
        {
            // if the current sample has sub-samples
            if (child_idx >= 0)
            {
                // gets the pointer to the first child
                SampleNode* _child = nodeAddr(child_idx);
                while (_child)
                {
                    // matching child
                    if (_child->name == name)
                    {
                        return _child;
                    }
                    // goes to the next child
                    else
                    {
                        // +---+---+     +---+---+     +---+---+            +---+---+
                        // | N | C | --> | N | S | --> | N | S | ...... --> | N | S | --+
                        // +---+---+     +---+---+     +---+---+            +---+---+   |
                        //                                                              =
                        //
                        _child = _child->sibling();
                    }
                }
            }

            // no matching sub-sample
            return nullptr;
        }


        // Gets the pointer to the first sibling node
        INLINE_FUNCTION
        const SampleNode*
        sibling () const
        {
            if (sib_idx < 0)
            {
                return nullptr;
            }
            else
            {
                return nodeAddr(sib_idx);
            }
        }


        INLINE_FUNCTION
        SampleNode*
        sibling ()
        {
            if (sib_idx < 0)
            {
                return nullptr;
            }
            else
            {
                return nodeAddr(sib_idx);
            }
        }


        // Gets the memory location of a sample in the sample tree using its index
        //
        // @param[in] sample_idx
        //      the index of the sample in the sample tree
        INLINE_FUNCTION
        const SampleNode*
        nodeAddr (
            const SInt16 sample_idx) const
        {
            RUNTIME_ASSERT(sample_idx >= 0, "Invalid sample index");
            RUNTIME_ASSERT(sample_idx < SampleMgr::getRef().treeSize(),
                           "Sample index is out of range");

            // Addr = (UInt8*)this + (SInt32)(IDX - SELF)*(SInt32)sizeof(SampleNode)
            const SInt16 _node_bytesize = sizeof(SampleNode);
            const SInt32 _byte_offsets  = (SInt32)(sample_idx - self_idx)*_node_bytesize;
            return (const SampleNode*)((const UInt8*)this + _byte_offsets);
        }


        INLINE_FUNCTION
        SampleNode*
        nodeAddr (
            const SInt16 sample_idx)
        {
            const SampleNode *const _node_addr = ((const SampleNode*)this)->nodeAddr(sample_idx);
            return const_cast<SampleNode*>(_node_addr);
        }


        // Uses this sample to collect spent time
        INLINE_FUNCTION
        void
        use ()
        {
            // increments the #used
            ++used_num;

            // on FIRST use
            if (opened_num++ == 0)
            {
                // stores the frame number
                frame_num = SampleMgr::getRef().frameNum();

                // stores the start time
                const Real64 _app_start_time = SampleMgr::getRef().appStartTime();
                start_time = Real32(Core::getCurTimeSec() - _app_start_time);
            }
        }


        // Exits this sample to stop the time collection
        // and back to its parent or previous occurrence
        //
        // @return
        //      TRUE:  backs to the parent sample
        //      FALSE: backs to the previous occurrence: some instances of this sample still opened
        INLINE_FUNCTION
        Bool
        exit ()
        {
            // on LAST occurrence
            if (--opened_num == 0)
            {
                // calculates the delta/elapsed time
                const Real64 _cur_time       = Core::getCurTimeSec();
                const Real64 _app_start_time = SampleMgr::getRef().appStartTime();
                const Real32 _end_time       = Real32(_cur_time - _app_start_time);
                const Real32 _delta_time     = _end_time - start_time;
                // updates the total time
                total_time += _delta_time;
                return true;
            }
            // exits to the previous occurrence
            else
            {
                return false;
            }
        }


        // Resets all frame data
        INLINE_FUNCTION
        void
        reset ()
        {
            // resets the frame data
            used_num   = 0;
            opened_num = 0;
            total_time = 0;
        }


        // Dumps the data
        INLINE_FUNCTION
        void
        dump (
            const UInt16 indention) const
        {
            // dumps indention
            for (UInt16 c = 0; c < indention; ++c)
            {
                printf("  ");
            }

            // dumps itself
            printf("- %s: [#used]: %d, [total]: %.6fs\n", name, used_num, total_time);
//            printf("- %s: "
//                   "[#used]: %d, [total]: %.6fs, "
//                   "[self idx]: %d, [child idx]: %d, [sibling idx]: %d\n",
//                   name,
//                   used_num, total_time,
//                   self_idx, child_idx, sib_idx);
        }
    };
    COMPILE_TIME_ASSERT_MSG(sizeof(SampleNode) == 40, "Wrong sample node size");

} // namespace Core
