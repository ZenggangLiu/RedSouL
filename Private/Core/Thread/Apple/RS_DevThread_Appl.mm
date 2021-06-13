// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <mach/thread_act.h>
// https://hpc-tutorials.llnl.gov/posix/
#include <pthread.h>
// Lib headers
#include "Core/Mathe/RS_MatheFuncs.hpp"
// Self
#include "Core/Thread/RS_DevThread.hpp"


namespace Core
{

    static
    SInt32
    getOSThreadPriority (
        const DevThreadPriority priority)
    {
#define OS_THREAD_PRIORITY_TIME_CRITICAL        90
#define OS_THREAD_PRIORITY_HIGHEST              80
#define OS_THREAD_PRIORITY_ABOVE_NORMAL         60
#define OS_THREAD_PRIORITY_NORMAL               50
#define OS_THREAD_PRIORITY_BELOW_NORMAL         30
#define OS_THREAD_PRIORITY_LOWEST               0

        switch (priority)
        {
            case DEV_THREAD_PRIORITY_TIME_CRITICAL:
            {
                return OS_THREAD_PRIORITY_TIME_CRITICAL;
            }

            case DEV_THREAD_PRIORITY_HIGHEST:
            {
                return OS_THREAD_PRIORITY_HIGHEST;
            }

            case DEV_THREAD_PRIORITY_ABOVE_NORMAL:
            {
                return OS_THREAD_PRIORITY_ABOVE_NORMAL;
            }

            case DEV_THREAD_PRIORITY_NORMAL:
            {
                return OS_THREAD_PRIORITY_NORMAL;
            }

            case DEV_THREAD_PRIORITY_BELOW_NORMAL:
            {
                return OS_THREAD_PRIORITY_BELOW_NORMAL;
            }

            case DEV_THREAD_PRIORITY_LOWEST:
            {
                return OS_THREAD_PRIORITY_LOWEST;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Uknown priority type");
                return OS_THREAD_PRIORITY_NORMAL;
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
        COMPILE_TIME_ASSERT_MSG(sizeof(pthread_t) == sizeof(m_handle), "Un-matching thread handle");
    }


    Bool
    DevThread::start ()
    {
        RUNTIME_ASSERT(m_state == DEV_THREAD_STATE_CREATED,
                       "The Thread is NOT in the right state: CREATED");
        // 创建线程缺省的attribute
        pthread_attr_t _thread_attr;
        pthread_attr_init(&_thread_attr);

        // 添加joinable属性，因为我们想调用join()来等待此线程的结束
        pthread_attr_setdetachstate(&_thread_attr, PTHREAD_CREATE_JOINABLE);

        // 设置堆栈大小
        // 堆栈的大小(字节)：0表示使用缺省大小
        const size_t _stack_size = m_stack_size == DEV_THREAD_STACK_SIZE_DEFAULT ? 0 : m_stack_size;
        pthread_attr_setstacksize(&_thread_attr, _stack_size);

        // 设置优先级
        if(m_priority != DEV_THREAD_PRIORITY_NORMAL)
        {
            SInt32 _scheduling_policy = -1;
            // 如果成功获得scheduling policy：pthread_attr_getschedpolicy()返回0
            if (!pthread_attr_getschedpolicy(&_thread_attr, &_scheduling_policy))
            {
                // 获取scheduling parameter
                sched_param _scheduling_param;
                pthread_attr_getschedparam(&_thread_attr, &_scheduling_param);

                // 获取支持最大/最小scheduling policy
                const int _min_priority = sched_get_priority_min(_scheduling_policy);
                const int _max_priority = sched_get_priority_max(_scheduling_policy);

                // set priority
                const SInt32 _priority = Mathe::clamp(getOSThreadPriority(m_priority),
                                                      _min_priority,
                                                      _max_priority);
                // 设置scheduling policy
                _scheduling_param.sched_priority = _priority;
                pthread_attr_setschedparam(&_thread_attr, &_scheduling_param);
            }
        }

        // 定义pthread的回调函数类型
        typedef void *(*pthread_start_routine) (void *);
        if (pthread_create(&m_handle.os_handle,
                           &_thread_attr,
                           (pthread_start_routine)&DevThread::ThreadProc,
                           this /* 传入ThreadProc()的参数 */))
        {
            RUNTIME_ASSERT(false, "Can not create OS thread");
            m_state = DEV_THREAD_STATE_INVALID;
            return false;
        }

        // 释放attribute set
        pthread_attr_destroy(&_thread_attr);

        // 设置状态
        m_state = DEV_THREAD_STATE_STARTED;

        return true;
    }


    void
    DevThread::suspend ()
    {
        RUNTIME_ASSERT(m_state == DEV_THREAD_STATE_RUNNING,
                       "The Thread is NOT in the right state: RUNNING");
        // 在调用SuspendThread()之前设置好状态
        m_state = DEV_THREAD_STATE_SUSPENDED;

        // 由于Pthread API没有提供Suspend函数，所以这里只能使用Mach API
        const mach_port_t _mac_thread = pthread_mach_thread_np(m_handle.os_handle);
        if (thread_suspend(_mac_thread) != KERN_SUCCESS)
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

        // 由于Pthread API没有提供Resume函数，所以这里只能使用Mach API
        const mach_port_t _mac_thread = pthread_mach_thread_np(m_handle.os_handle);
        if (thread_resume(_mac_thread) != KERN_SUCCESS)
        {
            m_state = DEV_THREAD_STATE_SUSPENDED;
        }
    }


    void
    DevThread::join () const
    {
        pthread_join(m_handle.os_handle, nullptr);
    }


    void
    DevThread::kill (
        const UInt32 exit_code /* = (UInt32)-1 */)
    {
        RUNTIME_ASSERT(m_state != DEV_THREAD_STATE_TERMINATED, "Can not kill a terminated thread");
        m_state = DEV_THREAD_STATE_TERMINATING;
        // 强行终止线程
        // cancel the thread: not use pthread_kill() which just sends SIG to the child thread
        pthread_cancel(m_handle.os_handle);
        //清理资源
        cleanup(exit_code);
    }


    UInt32
    DevThread::coRutine ()
    {
        RUNTIME_ASSERT(false, "We should NOT call this. Please override it in the subclass.");
        return -1;
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
        // NOTE：由于MacOS/iOS/tvOS只能设置当前运行的线程的名字，所以将名称设置放在ThreadProc中
        pthread_setname_np((const char*)_SELF->m_name.c_str());
#endif // defined(_DEBUG) || defined(DEBUG)

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
        // 清除缓存的句柄
        m_handle.os_handle = (pthread_t)-1;
        // 标记状态
        m_state = DEV_THREAD_STATE_TERMINATED;
        // 清理线程的资源如果此线程已经退出
        // NOTE：如果线程正在忙碌，此函数将Blocking
         pthread_exit((void*)&exit_code);
    }

} // namespace Core

#endif // #if defined(__APPLE__)
