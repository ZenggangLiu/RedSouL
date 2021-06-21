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
#include "Core/Thread/RS_ThreadHelper.hpp"


namespace Core
{

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
