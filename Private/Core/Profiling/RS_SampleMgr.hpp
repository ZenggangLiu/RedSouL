#pragma once


// System headers
#include <vector>
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    struct SampleNode;
    struct StatsNode;


    class SampleMgr
    {
    public:
        // Gets the single reference to the sample manager
        static
        SampleMgr&
        getRef ();

        // Gets the starting time of the application
        static
        Real64
        appStartTime ();

        // Gets the current frame number
        UInt32
        frameNum () const;

        // Checks if all samples are closed
        Bool
        allClosed () const;

        // Checks if the given sample index is valid
        Bool
        isValid (
            const UInt16 sample_idx) const;

        // Gets the size of the sample tree
        SInt16
        treeSize () const;

        // Gets the constant reference to the given sample node
        const SampleNode&
        node (
            const UInt16 sample_idx) const;

        // Gets the constant reference to the given stats node
        const StatsNode&
        statsNode (
            const UInt16 sample_idx) const;

        // Sets total number of samples
        void
        setTotalSampleCount (
            const UInt16 count);

        // Opens the named sample and
        // makes it the current sample, if it was not
        void
        openNamedSample (
            const UTF8 *const name);

        // Closes the current sample and
        // returns to the parent sample if we can
        void
        closeCurSample ();

        // Closes all pending samples
        void
        closeAll ();

        // Updates stats of all collected samples
        void
        updateStats ();

        // Resets the frame data of all samples,
        // which is supposed to be called in the end of each frame
        void
        reset ();

        // Ends the sample collection for the current frame
        // NOTE: this function is supposed to be called at the end of each frame
        void
        endFrame();


    private:
        SampleMgr ();

        // Resets the frame data of the given sample
        void
        reset (
            SampleNode *const sample);

        // Updates the stats for the given sample
        void
        updateStats (
            const SampleNode *const sample);

    private:
        typedef std::vector<SampleNode> SampleTree;
        typedef std::vector<StatsNode>  StatsTree;

        // the application start time
        static const Real64 APPLICATION_START_TIME_SECS;

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
        SampleTree  m_sample_tree;
        // the stats tree
        StatsTree   m_stats_tree;
        // the current sample node
        SampleNode* m_cur_node;
        // the current frame number: [1, N]
        UInt32      m_frame_num;
        // the index of the first free slot
        SInt16      m_free_slot;
    };

} // namespace Core

