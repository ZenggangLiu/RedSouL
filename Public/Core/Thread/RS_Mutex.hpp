#pragma once


#include "Core/Common/RS_CompilerDefs.hpp"
// System headers
#if (BUILD_MODE == DEBUG_BUILD_MODE)
#include <atomic>
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
// Lib headers
#include "Core/Common/RS_OsDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"
#if (BUILD_MODE == DEBUG_BUILD_MODE)
#include "Core/String/RS_RuntimeText.hpp"
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)


namespace Core
{

#if (BUILD_MODE == DEBUG_BUILD_MODE)
    class DevThread;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

    // Mutex：对于共享资源的单一访问：只能有一个线程Read/Write使用这个共享资源
    // NOTE：我们禁止同一线程多次锁定同一个Mutex：NOT RECURSIVE/REENTRANT Mutex
    class Mutex
    {
    public:
        // 创建一个Mutex
        Mutex (
            const ASCII *const name = nullptr);

        ~Mutex ();

        // 获得此Mutex的使用权
        void
        lock ();

        // 尝试获得此Mutex的使用权
        //
        // @return
        //      TRUE: 如果成功获得使用权
        Bool
        tryLock ();

        // 释放对此Mutex的使用权
        void
        unlock ();


    private:
        // copy constructor: deleted
        Mutex (
            const Mutex&) = delete;

        // assignment operator: deleted
        Mutex& operator = (
            const Mutex&) = delete;

        // 销毁此Mutex
        void
        close ();

    private:
        // Mutex句柄
#if defined(__APPLE__)
        pthread_mutex_t     m_handle;
#elif (OS_TYPE == OS_TYPE_WIN)
        UInt8   m_handle[40];
#else
#error NO Implementation
#endif // #if defined(__APPLE__)

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 正在使用此Mutex的线程
        DevThread *         m_owner_thread;
        // Mutex的名称
        RuntimeText         m_name;
        // 多少线程在等待此Mutex
        std::atomic<UInt32> m_wait_count;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    };

} // namespace Core
