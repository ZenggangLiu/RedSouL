// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <unistd.h>         // usleep()
// Lib headers
// Self
#include "Core/Thread/RS_ThreadHelper.hpp"


namespace Core
{

    static const pthread_t g_Main_Thread_Id = pthread_self();



    Bool
    ThreadHelper::isMainThread ()
    {
        // pthread_equal()返回NON-Zero如果相同
        return pthread_equal(pthread_self(), g_Main_Thread_Id) != 0;
    }


    void
    ThreadHelper::sleep ()
    {
        usleep(-1);
    }


    void
    ThreadHelper::sleep (
        UInt32 milli_sec)
    {
        usleep(1000 * milli_sec);
    }

} // namespace Core

#endif // #if defined(__APPLE__)
