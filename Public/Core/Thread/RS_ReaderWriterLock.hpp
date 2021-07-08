#pragma once


// System headers
// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"
#include "Core/Common/RS_OsDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"
#if (OS_TYPE == OS_TYPE_WIN)
// 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#define _WINSOCKAPI_
#include <Windows.h>
#undef _WINSOCKAPI_
#else
#include <pthread.h>
#endif // (OS_TYPE == OS_TYPE_WIN)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
#include <atomic>
#include <mutex>
#include "Core/String/RS_RuntimeText.hpp"
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)


namespace Core
{

#if (BUILD_MODE == DEBUG_BUILD_MODE)
    class DevThread;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

    // ReaderWriterLock(RWLock)：对于共享资源的提供多重读入操作和单一写入操作。
    // 写入操作具有优先级：如果一个写入操作请求此Lock，它会压制所有随后的读入操作
    // NOTE：我们禁止同一线程多次申请读入/写入权限：NOT RECURSIVE/REENTRANT ReaderWriterLock
    //
    class ReaderWriterLock
    {
    public:
        // 创建一个RWLock
        ReaderWriterLock (
            const ASCII *const name);

        ~ReaderWriterLock ();

        // 获得此RWLock的读入权限
        void
        enterRead ();

        // 获得此RWLock的写入权限
        void
        enterWrite ();

        // 尝试获得此RWLock的读入权限
        //
        // @return
        //      TRUE:  如果成功获得读入权限
        //      FALSE：如果无法获得读入权限
        Bool
        tryEnterRead ();

        // 尝试获得此RWLock的写入权限
        //
        // @return
        //      TRUE:  如果成功获得写入权限
        //      FALSE：如果无法获得写入权限
        Bool
        tryEnterWrite ();

        // 释放对此RWLock的读入权限
        void
        exitRead ();

        // 释放对此RWLock的写入权限
        void
        exitWrite ();

    private:
        // copy constructor: deleted
        ReaderWriterLock (
            const ReaderWriterLock&) = delete;

        // assignment operator: deleted
        ReaderWriterLock& operator = (
            const ReaderWriterLock&) = delete;

    private:
        // RWLock句柄
#if (OS_TYPE == OS_TYPE_WIN)
        SRWLOCK          m_handle;
#else
        pthread_rwlock_t m_handle;
#endif // #if (OS_TYPE == OS_TYPE_WIN)

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // Reader线程列表锁
        std::mutex              m_reader_thread_lock;
        // Reader线程列表
        std::vector<DevThread*> m_reader_threads;
        // Writer线程
        std::atomic<DevThread*> m_writer_thread;
        // RWLock的名称
        RuntimeText             m_name;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    };

} // namespace Core
