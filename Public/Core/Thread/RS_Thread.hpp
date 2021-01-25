#pragma once


// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // 将当前调用Thread休眠指定milli-seconds
    // NOTE：负的milli-seconds表示永远休眠
    //
    // @param[in] msec
    //      The sleep time in milli-seconds.
    //      Negative milli-second means sleep forever
    void
    sleepCallingThread (
        const SInt32 msec);

    // Yields the calling thread
    INLINE_FUNCTION
    void
    yieldCallingThread () { sleepCallingThread(0); }

    // Puts an acquire memory fence in the calling location
    // - finish loading before going on
    void
    acquireMemoryFence();

    // Puts a read memory fence in the calling location before going on
    INLINE_FUNCTION
    void
    readMemoryFence() { acquireMemoryFence(); }

    // Puts a release memory fence in the calling location
    // - finish storing before going on
    void
    releaseMemoryFence();

    // Puts a write memory fence in the calling location before going on
    INLINE_FUNCTION
    void
    writeMemoryFence() { releaseMemoryFence(); }

    // Puts a full memory fence in the calling location
    // - finish loading and storing before going on
    void
    fullMemoryFence();

    // Puts a read and write memory fence in the calling location before going on
    INLINE_FUNCTION
    void
    readWriteMemoryFence() { fullMemoryFence(); }

} // namespace Core
