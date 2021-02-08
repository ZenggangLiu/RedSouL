#pragma once


// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"          // BUILD_MODE
#include "Core/DataType/RS_DataTypeDefs.hpp"        // Bool, UTF8, UInt32


#if !defined(RUNTIME_ASSERT)
    #if (BUILD_MODE == DEBUG_BUILD_MODE)
        namespace Core
        {

            // --- OS specific methods START --- //
            // Implementations are in RS_RuntimeAssert_Appl/Wind/Andr.???

            void
            RuntimeAssert (
                const Bool        condition,
                const UTF8 *const condition_text,
                const UTF8 *const expression_text,
                const UTF8 *const file_name,
                const UInt32      line_num);

            // --- OS specific methods END --- //

        } // namespace Core

        //RUNTIME_ASSERT(const Bool, const char *const)
        #define RUNTIME_ASSERT(condition, expression)                                              \
        Core::RuntimeAssert(condition,                                                             \
                            (const UTF8*)#condition,                                               \
                            (const UTF8*)expression,                                               \
                            (const UTF8*)__FILE__,                                                 \
                            (UInt32)     __LINE__)

    #else
        #define RUNTIME_ASSERT(condition, expression)
    #endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
#endif // #if !defined(RUNTIME_ASSERT)
