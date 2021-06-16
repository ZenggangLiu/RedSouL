1. OS特定的Thread句柄保存在如下的struct的os_handle中
    struct DevThreadHandle
    {
        // OS的线程句柄
        UInt64 os_handle = (UInt64)-1;
    };
    a) 在macOS中它是pthread_t， 在Windows中它是HANDLE
    b) 它的具体数值在DevThread::start()中存储：RS_DevThread_Appl.mm/RS_DevThread_Wind.cpp
   NOTE：由于我们无法获得主线程的Handle，所有对于DevMainThread，我们只能保存它的Thread ID。
2. Job可以之间定义为std::function的functor：typedef std::function<void(void)> JobT
   - 还是使用其它的保存方式？