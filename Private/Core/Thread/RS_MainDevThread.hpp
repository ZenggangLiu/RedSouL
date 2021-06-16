#pragma once


// System headers
// Lib headers
#include "RS_DevThread.hpp"


namespace Core
{

    // Dummy main thread
    class MainDevThread : public DevThread
    {
    public:
        // 检查当前调用线程是否为主线程
        static
        Bool
        isMainThread ();

        // 获得对于主线程的参考
        static
        DevThread*
        mainThread ();

        MainDevThread ();

        Bool
        start();

        void
        suspend ();

        void
        resume ();

        void
        join () const;

        void
        kill (
            UInt32 exit_code = (UInt32)-1);

    private:
        using SUPER = DevThread;

        // 唯一的主线程实例
        static MainDevThread ms_main_thread;
    };

} // namespace Core