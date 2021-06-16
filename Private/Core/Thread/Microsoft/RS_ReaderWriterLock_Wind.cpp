// Precompile header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)
// System headers
#define _WINSOCKAPI_ // 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#include <Windows.h>
#undef _WINSOCKAPI_
// Lib headers
#include "Core/Thread/RS_DevThreadDataBase.hpp"
// Self
#include "Core/Thread/RS_ReaderWriterLock.hpp"


namespace Core
{

    ReaderWriterLock::ReaderWriterLock (
        const ASCII *const name /* = nullptr */)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    :
        m_writer_thread(nullptr),
        m_name(name ? name : "Un-named RWLock")
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    {
        // sizeof(SRWLOCK) == 8
        COMPILE_TIME_ASSERT_MSG(sizeof(SRWLOCK) == sizeof(m_handle), "Un-matching ReaderWriterLock handle");
        // 初始化一个RWLock
        InitializeSRWLock((SRWLOCK*)&m_handle);
    }


    ReaderWriterLock::~ReaderWriterLock ()
    {

    }


    void
    ReaderWriterLock::enterRead ()
    {
        // 获得对RWLock的共享权
        AcquireSRWLockShared((SRWLOCK*)&m_handle);
    }


    void
    ReaderWriterLock::enterWrite ()
    {
        // 获得对RWLock的独占权
        AcquireSRWLockExclusive((SRWLOCK*)&m_handle);
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 保存线程参考
        m_writer_thread = DevThreadDataBase::threadFromId(GetCurrentThreadId());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    Bool
    ReaderWriterLock::tryEnterRead ()
    {
        // TryAcquireSRWLockShared()返回非零，如果我们获得此RWLock的共享权
        const Bool _can_use_lock = TryAcquireSRWLockShared((SRWLOCK*)&m_handle);
        return _can_use_lock;
    }


    Bool
    ReaderWriterLock::tryEnterWrite ()
    {
        // TryAcquireSRWLockShared()返回非零，如果我们获得此RWLock的独占权
        const Bool _can_use_lock = TryAcquireSRWLockExclusive((SRWLOCK*)&m_handle);
        if (_can_use_lock)
        {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            // 保存线程参考
            m_writer_thread = DevThreadDataBase::threadFromId(GetCurrentThreadId());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
        }
        return _can_use_lock;
    }


    void
    ReaderWriterLock::exitRead ()
    {
        ReleaseSRWLockShared((SRWLOCK*)&m_handle);
    }


    void
    ReaderWriterLock::exitWrite ()
    {
        !! make sure the  calling thread is the same as m_writer_thread !!
        ReleaseSRWLockExclusive((SRWLOCK*)&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        m_writer_thread = nullptr;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }

}

#endif // #if (OS_TYPE == OS_TYPE_WIN)
