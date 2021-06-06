// Precompiled header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
#define _WINSOCKAPI_ // 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#include <Windows.h>
#undef _WINSOCKAPI_
// Lib headers
// Self
#include "Core/Thread/RS_ThreadHelper.hpp"


namespace Core
{

    static const DWORD g_Main_Thread_Id = GetCurrentThreadId();



    Bool
    ThreadHelper::isMainThread ()
    {
        return GetCurrentThreadId() == g_Main_Thread_Id;
    }


    Bool
    ThreadHelper::isMainThread ()
    {

    }


    void
    ThreadHelper::sleep ()
    {
        Sleep(-1);
    }


    void
    ThreadHelper::sleep (
        UInt32 milli_sec)
    {
        Sleep(milli_sec);
    }

} // namespace Core

#endif // #if (OS_TYPE == OS_TYPE_WIN)
