// Precompiled header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
#include <assert.h>
#include <stdio.h>
// 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#define _WINSOCKAPI_
#include <Windows.h>
#undef _WINSOCKAPI_
// Lib headers
// Self
#include "Core/Assert/RS_RuntimeAssert.hpp"


#if (BUILD_MODE == DEBUG_BUILD_MODE)
namespace Core
{

    void
    RuntimeAssert (
        const Bool        condition,
        const UTF8* const condition_text,
        const UTF8* const expression_text,
        const UTF8* const file_name,
        const UInt32      line_num)
    {
        if (condition == true)
        {
            return;
        }

        UTF8 _buffer[512] = { 0 };

        if (expression_text == nullptr)
        {
            TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                    "Assert raised: in file: %s, at line: %d\n",
                                    file_name, line_num);
        }
        else
        {
            TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                    "Assert raised: condition: %s, in file: %s, at line: %d\n",
                                    condition_text, file_name, line_num);
        }

        OutputDebugStringA(LPCSTR(_buffer));

        DebugBreak();
    }

} // namespace Core
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)

#endif // #if (OS_TYPE == OS_TYPE_WIN)
