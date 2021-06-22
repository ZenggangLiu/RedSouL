// Precompiled header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
// 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#define _WINSOCKAPI_
#include <Windows.h>
#undef _WINSOCKAPI_
// Lib headers
// Self
#include "Core/Thread/RS_MainDevThread.hpp"


namespace Core
{

    Bool
    MainDevThread::isMainThread ()
    {
        // 主线程使用ID进行比较：由于我们无法获得它的句柄
        return ms_main_thread.m_id == GetCurrentThreadId();
    }


    MainDevThread::MainDevThread ()
    :
        SUPER("Main Thread")
    {
        // 保存主线程的ID
        m_id = GetCurrentThreadId();
    }

} // namespace Core

#endif // #if (OS_TYPE == OS_TYPE_WIN)
