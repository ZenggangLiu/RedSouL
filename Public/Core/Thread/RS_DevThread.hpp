#pragma once


// System headers
#include <atomic>
// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"
#include "Core/Common/RS_OsDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/Thread/RS_DevThreadDefs.hpp"
#if (OS_TYPE == OS_TYPE_WIN)
#include <Windows.h>
#else
#include <pthread.h>
#endif // (OS_TYPE == OS_TYPE_WIN)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
#include "Core/String/RS_RuntimeText.hpp"
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)


namespace Core
{

    // OS线程
    class DevThread
    {
    public:
        // 构造一个DevThread
        DevThread (
            const ASCII *const       name           = "Working Thread",
            const DevThreadPriority  priority       = DEV_THREAD_PRIORITY_DEFAULT,
            const DevThreadStackSize stack_size     = DEV_THREAD_STACK_SIZE_DEFAULT);

        // 启动此线程： 让OS规划它的运行
        //
        // @return
        //      TRUE:  如果成功启动
        //      FALSE: 如果启动失败
        Bool
        start ();

        // 由于pthread无法支持这两个操作。我们必须使用其它方法来实现它
        // 挂起此线程
        void
        suspend ();

        // 恢复此线程运行
        void
        resume ();

        // 无限期等待线程退出
        void
        join () const;

        // 强行终止线程
        void
        kill (
            const UInt32 exit_code = (UInt32)-1);


    protected:
        // 此线程的真正任务所在
        // NOTE：子类必须提供具体的功能
        virtual
        UInt32
        coRutine ();

        // 等待Event之前调用
        void
        waitEvent ();

        // 获得Event之后调用
        void
        exitEvent ();

    protected:
        // 线程的句柄
#if (OS_TYPE == OS_TYPE_WIN)
        HANDLE    m_handle;
#else
        pthread_t m_handle;
#endif // #if (OS_TYPE == OS_TYPE_WIN)

        // 线程的状态
        std::atomic<DevThreadState> m_state;
        // 线程优先级
        const DevThreadPriority     m_priority;
        // 线程堆栈大小
        const DevThreadStackSize    m_stack_size;

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // thread name: just kept here in debug mode
        // and set onto thread using debugger's API
        const RuntimeText m_name;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)


    private:
        // OS线程的入口地址
        //
        // @param[in] dev_thread
        //      调用的OS线程
        static
        UInt32
        ThreadProc (
            void *const dev_thread);

        // 清理资源
        void
        cleanup (
            const UInt32 exit_code);
    };

} // namespace Core
