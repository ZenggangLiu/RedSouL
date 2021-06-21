// Precompile header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
// Lib headers
#include "Core/Thread/RS_DevThreadDataBase.hpp"
// Self
#include "Core/Thread/RS_ReaderWriterLock.hpp"


namespace Core
{

    ReaderWriterLock::ReaderWriterLock (
        const ASCII *const name)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    :
        m_writer_thread(nullptr),
        m_name(name ? name : "Un-named RWLock")
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    {
        // 初始化一个RWLock
        InitializeSRWLock(&m_handle);
    }


    ReaderWriterLock::~ReaderWriterLock ()
    {

    }


    void
    ReaderWriterLock::enterRead ()
    {
        // Windows支持Recusive进入(entryRead()/entryWrite())
        // 每一个entryRead()必须有相应的exitRead()与之对应:如果不是entryWrite()将Blocking
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        m_reader_thread_lock.lock();
        for (const auto _cur_thread : m_reader_threads)
        {
            RUNTIME_ASSERT(_cur_thread != DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()),
                           "The calling thread ALREADY owns the lock in read mode");
        }
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE

        // 获得对RWLock的共享权
        AcquireSRWLockShared(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        m_reader_threads.push_back(DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()));
        m_reader_thread_lock.unlock();
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE
    }


    void
    ReaderWriterLock::enterWrite ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        RUNTIME_ASSERT(m_writer_thread != DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()),
                       "The calling thread ALREADY owns the lock in write mode");
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE

        // 获得对RWLock的独占权
        AcquireSRWLockExclusive(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 保存线程参考
        m_writer_thread = DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }


    Bool
    ReaderWriterLock::tryEnterRead ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        m_reader_thread_lock.lock();
        for (const auto _cur_thread : m_reader_threads)
        {
            RUNTIME_ASSERT(_cur_thread != DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()),
                           "The calling thread ALREADY owns the lock in read mode");
        }
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE

        // TryAcquireSRWLockShared()返回非零，如果我们获得此RWLock的共享权
        const Bool _can_use_lock = TryAcquireSRWLockShared(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        if (_can_use_lock)
        {
            m_reader_threads.push_back(DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()));
        }
        m_reader_thread_lock.unlock();
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE
        return _can_use_lock;
    }


    Bool
    ReaderWriterLock::tryEnterWrite ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        RUNTIME_ASSERT(m_writer_thread != DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()),
                       "The calling thread ALREADY owns the lock in write mode");
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE

        // TryAcquireSRWLockShared()返回非零，如果我们获得此RWLock的独占权
        const Bool _can_use_lock = TryAcquireSRWLockExclusive(&m_handle);

        if (_can_use_lock)
        {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
            // 保存线程参考
            m_writer_thread = DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
        }
        return _can_use_lock;
    }


    void
    ReaderWriterLock::exitRead ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        Bool _own_lock = false;
        m_reader_thread_lock.lock();
        for (std::vector<DevThread*>::iterator _cur_thread = m_reader_threads.begin();
            _cur_thread != m_reader_threads.end();
            ++_cur_thread)
        {
            if (*_cur_thread == DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()))
            {
                _own_lock = true;
                m_reader_threads.erase(_cur_thread);
                break;
            }
        }
        m_reader_thread_lock.unlock();
        RUNTIME_ASSERT(_own_lock, "The calling thread does NOT own the lock in read mode");
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE

        ReleaseSRWLockShared(&m_handle);
    }


    void
    ReaderWriterLock::exitWrite ()
    {
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        RUNTIME_ASSERT(m_writer_thread == DevThreadDataBase::threadFromId((DevThreadIdT)GetCurrentThreadId()),
                       "The calling thread does NOT own the lock in write mode");
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

        ReleaseSRWLockExclusive(&m_handle);

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        m_writer_thread = nullptr;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    }

}

#endif // #if (OS_TYPE == OS_TYPE_WIN)
