// Precompiled header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
#define _WINSOCKAPI_ // 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
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
        return ms_main_thread.m_handle.os_handle == (UInt64)GetCurrentThreadId();
    }


    MainDevThread::MainDevThread ()
    :
        SUPER("Main")
    {
        m_handle.os_handle = (UInt64)GetCurrentThreadId();
    }

} // namespace Core

#endif // #if (OS_TYPE == OS_TYPE_WIN)
