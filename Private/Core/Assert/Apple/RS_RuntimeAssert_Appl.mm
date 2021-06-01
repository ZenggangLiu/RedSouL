// Precompiled header
#include "PrecompiledH.hpp"

#if defined(__APPLE__)


// System headers
#if OS_TYPE != OS_TYPE_IOS
#include <assert.h>
#endif // #if OS_TYPE != OS_TYPE_IOS
// Lib headers
// Self
#include "Core/Assert/RS_RuntimeAssert.hpp"


#if (BUILD_MODE == DEBUG_BUILD_MODE)
namespace Core
{

    void
    RuntimeAssert (
        const Bool         condition,
        const UTF8  *const condition_text,
        const UTF8  *const expression_text,
        const UTF8  *const file_name,
        const UInt32       line_num)
    {
        if (condition == true)
        {
            return;
        }

        std::printf("Assert raised: condition: %s, ", condition_text);

        if (expression_text != nullptr)
        {
            std::printf("%s, ", expression_text);
        }

        std::printf("in file: %s, at line: %d\n", file_name, line_num);

        //assert(false);
        // __builtin_trap:
        // - in the debugger, debugger will stop at the line immediately before the __builtin_trap call
        // - normal mode: the program will crash and generate a crash report.
        __builtin_trap();
    }

} // namespace Core
#endif // #if（BUILD_MODE == DEBUG_BUILD_MODE)

#endif // #if defined(__APPLE__)
