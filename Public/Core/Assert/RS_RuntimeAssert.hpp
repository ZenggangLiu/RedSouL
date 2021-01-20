#pragma once


// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"          // BUILD_MODE
#include "Core/DataType/RS_DataTypeDefs.hpp"        // Bool, Char, UInt32


#if !defined(RUNTIME_ASSERT)
    #if (BUILD_MODE == DEBUG_BUILD_MODE)
        namespace Core
        {

            // --- The implementation is in RS_RuntimeAssert_appl/wind/andr.??? --- //
            void
            RuntimeAssert (
                const Bool        condition,
                const Char *const condition_str,
                const Char *const expression_str,
                const Char *const file_name,
                const UInt32      line_num);

        } // namespace Core

        #define RUNTIME_ASSERT(condition, expression)                                              \
        Core::RuntimeAssert(condition, #condition, expression, __FILE__, __LINE__)

    #else
        #define RUNTIME_ASSERT(condition, expression)
    #endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
#endif // #if !defined(RUNTIME_ASSERT)
