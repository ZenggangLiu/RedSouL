#pragma once


// System headers
// Lib headers
#include "Core/Thread/RS_DevThread.hpp"


namespace Core
{

    // Dummy main thread
    class MainDevThread : public DevThread
    {
    public:
        MainDevThread ();

        // 启动此线程： 让OS规划它的运行
        //
        // @return
        //      TRUE:  如果成功启动
        //      FALSE: 如果启动失败
        Bool
        start();

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
            const UInt32);

    private:
        using SUPER = DevThread;

    };


    // 声明MainThread的全局实例
    extern const MainDevThread g_Main_Thread;

} // namespace Core
