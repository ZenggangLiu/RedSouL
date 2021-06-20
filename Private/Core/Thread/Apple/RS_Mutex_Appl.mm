// Precompile header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
// Lib headers
#include "Core/Thread/RS_DevThreadDataBase.hpp"
// Self
#include "Core/Thread/RS_Mutex.hpp"


namespace Core
{

    Mutex::Mutex (
        const ASCII *const name)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    :
        m_owner_thread(nullptr),
        m_name(name ? name : "Un-named Mutex"),
        m_wait_count(0)
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    {
        pthread_mutex_init(&m_handle, nullptr);
    }


    void
    Mutex::lock ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        RUNTIME_ASSERT(m_owner_thread != DevThreadDataBase::threadFromId((DevThreadIdT)pthread_self()),
                       "The same thread can NOT lock the same mutex more than once");
        // 将等待线程计数加一
        ++m_wait_count;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

        // 获得此Mutex的使用权
        pthread_mutex_lock(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 将等待线程计数减一：因为我们获得到了使用权
        --m_wait_count;
        // 保存线程参考
        m_owner_thread = DevThreadDataBase::threadFromId((DevThreadIdT)pthread_self());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    Bool
    Mutex::tryLock ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        RUNTIME_ASSERT(m_owner_thread != DevThreadDataBase::threadFromId((DevThreadIdT)pthread_self()),
                       "The same thread can NOT lock the same mutex more than once");
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

        // pthread_mutex_trylock()返回零，如果我们获得此Mutex的使用权
        const Bool _can_use_mutex = pthread_mutex_trylock(&m_handle) == 0;
        if (_can_use_mutex)
        {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            // 保存线程参考
            m_owner_thread = DevThreadDataBase::threadFromId((DevThreadIdT)pthread_self());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
        }
        return _can_use_mutex;
    }


    void
    Mutex::unlock ()
    {
        pthread_mutex_unlock(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        m_owner_thread = nullptr;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    void
    Mutex::close ()
    {
        pthread_mutex_destroy(&m_handle);
    }

}

#endif // #if defined(__APPLE__)
