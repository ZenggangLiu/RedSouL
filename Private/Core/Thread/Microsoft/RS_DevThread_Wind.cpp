// Precompiled header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
#define _WINSOCKAPI_ // 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#include <Windows.h>
#undef _WINSOCKAPI_
// Lib headers
// Self
#include "Core/Thread/RS_DevThread.hpp"


namespace Core
{

    static
    SInt32
    getOSThreadPriority (
        const DevThreadPriority priority)
    {
        switch (priority)
        {
            case DEV_THREAD_PRIORITY_TIME_CRITICAL:
            {
                return THREAD_PRIORITY_TIME_CRITICAL;
            }

            case DEV_THREAD_PRIORITY_HIGHEST:
            {
                return THREAD_PRIORITY_HIGHEST;
            }

            case DEV_THREAD_PRIORITY_ABOVE_NORMAL:
            {
                return THREAD_PRIORITY_ABOVE_NORMAL;
            }

            case DEV_THREAD_PRIORITY_NORMAL:
            {
                return THREAD_PRIORITY_NORMAL;
            }

            case DEV_THREAD_PRIORITY_BELOW_NORMAL:
            {
                return THREAD_PRIORITY_BELOW_NORMAL;
            }

            case DEV_THREAD_PRIORITY_LOWEST:
            {
                return THREAD_PRIORITY_LOWEST;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Uknown priority type");
                return THREAD_PRIORITY_NORMAL;
            }
        }
    }



    DevThread::DevThread (
        const ASCII *const       name       /* = "Working Thread" */,
        const DevThreadPriority  priority   /* = DEV_THREAD_PRIORITY_DEFAULT */,
        const DevThreadStackSize stack_size /* = DEV_THREAD_STACK_SIZE_DEFAULT */)
    :
        m_state(DEV_THREAD_STATE_CREATED),
        m_priority(priority),
        m_stack_size(stack_size)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        ,m_name(name)
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    {
        COMPILE_TIME_ASSERT_MSG(sizeof(HANDLE) == sizeof(m_handle), "Un-matching thread  handle");
    }


    Bool
    DevThread::start ()
    {
        RUNTIME_ASSERT(m_state == DEV_THREAD_STATE_CREATED,
                       "The Thread is NOT in the right state: CREATED");
        // 堆栈的大小(字节)：0表示使用缺省大小
        const size_t _stack_size = m_stack_size == DEV_THREAD_STACK_SIZE_DEFAULT ? 0 : m_stack_size;

        // 创建一个挂起的OS线程
        m_handle.os_handle=(UInt64)CreateThread(nullptr,     /* 此句柄不可继承 */
                                                _stack_size, /* 保留的堆栈大小：0为缺省大小 */
                                                (LPTHREAD_START_ROUTINE)&DevThread::ThreadProc,
                                                this,        /* 传入ThreadProc()的参数 */
                                                /* 挂起此线程，并且设定保留的堆栈大小 */
                                                CREATE_SUSPENDED|STACK_SIZE_PARAM_IS_A_RESERVATION,
                                                nullptr       /* 不保留ThreadID */);
        if (m_handle.os_handle == 0)
        {
            RUNTIME_ASSERT(false, "Can not create OS thread");
            return false;
        }

        // 设定优先级
        const SInt32 _exp_priority = getOSThreadPriority(m_priority);
        if (!SetThreadPriority((HANDLE)m_handle.os_handle, _exp_priority))
        {
            // TODO: show INFO
        }

        // 设置状态
        m_state = DEV_THREAD_STATE_STARTED;

        // 启动线程
        if (ResumeThread((HANDLE)m_handle.os_handle) == (DWORD)-1)
        {
            // 启动失败
            m_state = DEV_THREAD_STATE_INVALID;
            return false;
        }

        return true;
    }


    void
    DevThread::suspend ()
    {
        RUNTIME_ASSERT(m_state == DEV_THREAD_STATE_RUNNING,
                       "The Thread is NOT in the right state: RUNNING");
        // 在调用SuspendThread()之前设置好状态
        m_state = DEV_THREAD_STATE_SUSPENDED;
        // Suspend线程。NOTE：SuspendThread()在返回前将暂停线程的运行
        if (SuspendThread((HANDLE)m_handle.os_handle) == (DWORD)(-1))
        {
            m_state = DEV_THREAD_STATE_RUNNING;
        }
    }


    void
    DevThread::resume ()
    {
        RUNTIME_ASSERT(m_state == DEV_THREAD_STATE_SUSPENDED,
                       "The Thread is NOT in the right state: SUSPENDED");
        // 在调用ResumeThread()之前设置好状态
        m_state = DEV_THREAD_STATE_RUNNING;
        // Resume线程。NOTE：ResumeThread()在返回之前将继续运行挂起的线程
        if (ResumeThread((HANDLE)m_handle.os_handle) == (DWORD)(-1))
        {
            m_state = DEV_THREAD_STATE_SUSPENDED;
        }
    }


    void
    DevThread::join () const
    {
        WaitForSingleObject((HANDLE)m_handle.os_handle, INFINITE);
    }


    void
    DevThread::kill (
        UInt32 exit_code /* = (UInt32)-1 */)
    {
        RUNTIME_ASSERT(m_state != DEV_THREAD_STATE_TERMINATED, "Can not kill a terminated thread");
        m_state = DEV_THREAD_STATE_TERMINATING;
        // 强行终止线程
        TerminateThread((HANDLE)m_handle.os_handle, (DWORD)exit_code);
        //清理资源
        cleanup(exit_code);
    }


    UInt32
    DevThread::coRutine ()
    {
        RUNTIME_ASSERT(false, "We should NOT call this. Please override it in the subclass.");
        return (UInt32)-1;
    }


    UInt32
    DevThread::ThreadProc (
        void* const dev_thread)
    {
        // 不同的线程入口：
        // Pthread: void* ThreadProc(void* )
        // Windows: DWORD ThreadProc(LPVOID)
        //
        DevThread* const _SELF = (DevThread*)dev_thread;
        RUNTIME_ASSERT(_SELF->m_state == DEV_THREAD_STATE_STARTED,
                       "The Thread is NOT in the right state: STARTED");
        _SELF->m_state = DEV_THREAD_STATE_RUNNING;

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 设置线程名称
        // NOTE：由于Windows只能设置当前运行的线程的名字，所以将名称设置放在ThreadProc中
#pragma pack(push,8)
        typedef struct tagTHREADNAME_INFO
        {
            DWORD dwType;     // Must be 0x1000.
            LPCSTR szName;    // Pointer to name (in user addr space).
            DWORD dwThreadID; // Thread ID：-1表示当前线程
            DWORD dwFlags;    // Reserved for future use, must be zero.
        } THREADNAME_INFO;
#pragma pack(pop)

        THREADNAME_INFO _thread_name_info;
        _thread_name_info.dwType = 0x1000;
        _thread_name_info.szName = (const LPCSTR)_SELF->m_name.c_str();
        _thread_name_info.dwThreadID = -1; // -1表示当前线程
        _thread_name_info.dwFlags    = 0;
        __try
        {
            RaiseException(0x406D1388,
                           0,
                           sizeof(_thread_name_info) / sizeof(ULONG_PTR),
                           (ULONG_PTR*)&_thread_name_info);
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {}

        // 注册此线程
        DevThreadDataBase::add(_SELF->m_handle.os_handle, _SELF);
#endif // if (BUILD_MODE == DEBUG_BUILD_MODE)

        // 调用coRutine()
        const UInt32 _exit_code = _SELF->coRutine();

        // coRutine()调用结束，清理资源
        _SELF->m_state = DEV_THREAD_STATE_TERMINATING;
        _SELF->cleanup(_exit_code);

        return _exit_code;
    }


    void
    DevThread::cleanup (
        const UInt32 exit_code)
    {
        RUNTIME_ASSERT(m_state == DEV_THREAD_STATE_TERMINATING,
                       "The Thread is NOT in the right state: TERMINATING");
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 注销此线程
        DevThreadDataBase::remove(m_handle.os_handle);
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

        // 关闭句柄
        CloseHandle((HANDLE)m_handle.os_handle);
        // 清除缓存的句柄
        m_handle.os_handle = (UInt64)-1;
        // 标记状态
        m_state = DEV_THREAD_STATE_TERMINATED;
        // 清理线程的资源如果此线程已经退出
        // NOTE：如果线程正在忙碌，此函数将Blocking
        ExitThread(exit_code);
    }

} // namespace Core

#endif // #if (OS_TYPE == OS_TYPE_WIN)
