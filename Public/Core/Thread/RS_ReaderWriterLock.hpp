#pragma once


#include "Core/Common/RS_CompilerDefs.hpp"
// System headers
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

    // ReaderWriterLock(RWLock)：对于共享资源的多重读入操作和单一写入操作
    // ?? RWLock是不是recursive ??
    class ReaderWriterLock
    {
    public:
        // 创建一个RWLock
        ReaderWriterLock (
            const ASCII *const name = nullptr);

        ~ReaderWriterLock ();

        // 获得此RWLock的读入权限
        void
        enterRead ();

        // 获得此RWLock的写入权限
        void
        enterWrite ();

        // 尝试获得此RWLock的使用权
        //
        // @return
        //      TRUE: 如果成功获得使用权
        Bool
        tryEnterRead ();

        Bool
        tryEnterWrite ();

        // 释放对此RWLock的使用权
        void
        exitRead ();

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
#if defined(__APPLE__)
        pthread_mutex_t     m_handle;
#elif (OS_TYPE == OS_TYPE_WIN)
        UInt64   m_handle;
#else
#error NO Implementation
#endif // #if defined(__APPLE__)

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // Writer线程
        DevThread *         m_writer_thread;
        // RWLock的名称
        RuntimeText         m_name;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    };

} // namespace Core
