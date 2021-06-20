// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <pthread.h>
// Lib headers
// Self
#include "Core/Thread/RS_MainDevThread.hpp"


namespace Core
{

    Bool
    MainDevThread::isMainThread ()
    {
        // 返回非0，如果两个线程相同
        return pthread_equal(ms_main_thread.m_handle, pthread_self()) != 0;
    }


    MainDevThread::MainDevThread ()
    :
        SUPER("Main Thread")
    {
        m_handle = pthread_self();
    }

} // namespace Core

#endif // #if defined(__APPLE__)
