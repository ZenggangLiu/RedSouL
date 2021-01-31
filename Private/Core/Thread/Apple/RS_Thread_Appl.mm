// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <unistd.h> // usleep()
// Lib headers
// Self
#include "Core/Thread/RS_Thread.hpp"


namespace Core
{

    void
    sleepCallingThread (
        const SInt32 msec)
    {
        // sleep
        usleep(1000 * msec);
    }


    void
    acquireMemoryFence ()
    {
        fullMemoryFence();
    }


    void
    releaseMemoryFence ()
    {
        fullMemoryFence();
    }


    void
    fullMemoryFence ()
    {
        __asm volatile("" ::: "memory");
    }

} // namespace Core


#endif // #if defined(__APPLE__)
