1. OS特定的Thread句柄保存在如下的struct的os_handle中
    struct DevThreadHandle
    {
        // OS的线程句柄
        UInt64 os_handle = (UInt64)-1;
    };
    a) 在macOS中它是pthread_t， 在Windows中它是HANDLE
    b) 它的具体数值在DevThread::start()中存储：RS_DevThread_Appl.mm/RS_DevThread_Wind.cpp
