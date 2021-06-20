// Precompile header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
// Lib headers
#include "Core/Thread/RS_DevThreadDataBase.hpp"
// Self
#include "Core/Thread/RS_AutoResetEvent.hpp"


namespace Core
{

    AutoResetEvent::AutoResetEvent (
        const Bool         set_on_create,
        const ASCII *const name)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    :
        m_name(name ? name : "Un-named AutoResetEvent")
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    {
        pthread_mutex_init(&m_lock,   nullptr);
        pthread_cond_init (&m_handle, nullptr);
    }


    AutoResetEvent::~AutoResetEvent ()
    {
        close();
    }


    Bool
    AutoResetEvent::set ()
    {
        // pthread_cond_signal(): 返回0如果成功
        const Bool _success_set = pthread_cond_signal(&m_handle) == 0;
        return _success_set;
    }


    Bool
    AutoResetEvent::wait ()
    {
        // pthread_cond_wait(): 返回零如果成功
        const Bool _success_wait = pthread_cond_wait(&m_handle, &m_lock) == 0;
        return _success_wait;
    }


    Bool
    AutoResetEvent::wait (
        const UInt32 wait_time)
    {
        timespec _wait_time_def = { 0, wait_time * 1000000 };
        // pthread_cond_timedwait_relative_np(): 返回0，如果AutoResetEvent在Signal状态
        const Bool _success_wait = pthread_cond_timedwait_relative_np(
            &m_handle, &m_lock, &_wait_time_def) == 0;
        return _success_wait;
    }


    void
    AutoResetEvent::close ()
    {
        pthread_mutex_destroy(&m_lock  );
        pthread_cond_destroy (&m_handle);
    }

}

#endif // #if defined(__APPLE__)
