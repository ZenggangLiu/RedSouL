1. 由于我们无法使用Kern中的mutex，所以我们将pthread api暴露出来，来使用它提供的mutex。
    struct DevThreadHandle
    {
        // OS的线程句柄
#if defined(__APPLE__)
        pthread_t os_handle = (pthread_t)-1;
#elif (OS_TYPE == OS_TYPE_WIN)
        HANDLE    os_handle = (HANDLE)-1;
#else
#error No implementation for this os type
#endif // #if defined(__APPLE__)
    };

