#pragma once


// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"          // BUILD_MODE
#include "Core/DataType/RS_DataTypeDefs.hpp"        // Bool, Char, UInt32


#if !defined(RUNTIME_ASSERT)
    #if (BUILD_MODE == DEBUG_BUILD_MODE)
        namespace Core
        {

            // --- OS specific methods START --- //
            // Implementations are in RS_RuntimeAssert_Appl/Wind/Andr.???

            void
            RuntimeAssert (
                const Bool        condition,
                const Char *const condition_text,
                const Char *const expression_text,
                const Char *const file_name,
                const UInt32      line_num);

            // --- OS specific methods END --- //

        } // namespace Core

        #define RUNTIME_ASSERT(condition, expression)                                              \
        Core::RuntimeAssert(condition, #condition, expression, __FILE__, __LINE__)

    #else
        #define RUNTIME_ASSERT(condition, expression)
    #endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
#endif // #if !defined(RUNTIME_ASSERT)
