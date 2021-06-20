#pragma once


// System headers
// Lib headers
#include "Core/Common/RS_OsDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"
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

#if (BUILD_MODE == DEBUG_BUILD_MODE)
    class DevThread;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

    // AutoResetEvent：一个带有自动复位的事件
    // NOTE：OS将在呼醒一个等待线程后复位此事件：将其状态设置为NON-SIGNALED
    //  如果多个线程等待，set后，是不是只是等待线程数减一 : 对头
    class AutoResetEvent
    {
    public:
        // 创建一个AutoResetEvent
        //
        // @param[in] set_on_create
        //      TRUE:  将此Event设置为Signaled
        //      FALSE: 将此Event设置为Non-Signaled
        AutoResetEvent (
            const Bool         set_on_create,
            const ASCII *const name);

        ~AutoResetEvent ();

        // Signal此AutoResetEvent，来呼醒一个等待线程
        // NOTE: 累计调用没有累计效果
        //
        //      TRUE:  如果此AutoResetEvent在Signal状态
        //      FALSE: 如果发生错误
        Bool
        set ();

        // 等待此AutoResetEvent的Signal
        //
        // @return
        //      TRUE:  如果此AutoResetEvent在Signal状态
        //      FALSE: 如果发生错误
        Bool
        wait ();

        // 在指定的时间内，等待此AutoResetEvent的Signal
        //
        // @param[in] wait_time
        //      等待的时间(毫秒)
        // @return
        //      TRUE:  如果此AutoResetEvent在Signal状态
        //      FALSE: 如果Timeout或者发生错误
        Bool
        wait (
            const UInt32 wait_time);


    private:
        // copy constructor: deleted
        AutoResetEvent (
            const AutoResetEvent&) = delete;

        // assignment operator: deleted
        AutoResetEvent& operator = (
            const AutoResetEvent&) = delete;

        // 销毁此Mutex
        void
        close ();

    private:
        // AutoResetEvent句柄
#if (OS_TYPE == OS_TYPE_WIN)
        HANDLE          m_handle;
#else
        pthread_mutex_t m_lock;
        pthread_cond_t  m_handle;
#endif // #if (OS_TYPE == OS_TYPE_WIN)

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // AutoResetEvent的名称
        RuntimeText     m_name;
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    };

} // namespace Core
