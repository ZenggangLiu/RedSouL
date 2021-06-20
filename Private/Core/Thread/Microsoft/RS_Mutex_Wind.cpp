// Precompile header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)
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
        InitializeCriticalSection(&m_handle);
    }


    Mutex::~Mutex ()
    {
        close();
    }


    void
    Mutex::lock ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        RUNTIME_ASSERT(m_owner_thread != DevThreadDataBase::threadFromId((UInt64)GetCurrentThreadId()),
                       "The same thread can NOT lock the same mutex more than once");
        // 等待线程计数加一
        ++m_wait_count;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

        // 获得此Mutex的使用权
        EnterCriticalSection(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 等待线程计数减一：因为我们获得到了使用权
        --m_wait_count;
        // 保存线程参考
        m_owner_thread = DevThreadDataBase::threadFromId((UInt64)GetCurrentThreadId());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    Bool
    Mutex::tryLock ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        RUNTIME_ASSERT(m_owner_thread != DevThreadDataBase::threadFromId((UInt64)GetCurrentThreadId()),
                       "The same thread can NOT lock the same mutex more than once");
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

        // TryEnterCriticalSection()返回非零，如果我们获得此Mutex的使用权
        const Bool _can_use_lock = TryEnterCriticalSection(&m_handle);
        if (_can_use_lock)
        {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            // 保存线程参考
            m_owner_thread = DevThreadDataBase::threadFromId((UInt64)GetCurrentThreadId());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
        }
        return _can_use_lock;
    }


    void
    Mutex::unlock ()
    {
        LeaveCriticalSection(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        m_owner_thread = nullptr;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    void
    Mutex::close ()
    {
        DeleteCriticalSection(&m_handle);
    }

}

#endif // #if (OS_TYPE == OS_TYPE_WIN)
