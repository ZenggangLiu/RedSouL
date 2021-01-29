#pragma once


// System headers
// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Logging/RS_Logger.hpp"


namespace Core
{

    class DumbLogger : public Logger
    {
    public:
        // Constructor
        INLINE_FUNCTION
        DumbLogger () : Logger(LoggerTypes::LOGGER_TYPES_DUMB) {}

        // Destructor
        INLINE_FUNCTION
        virtual
        ~DumbLogger () {}

        // Log指定的Info文字
        INLINE_FUNCTION
        virtual
        void
        logInfoText (
            const Char *const text) override {}

        // Log指定的Warning文字
        INLINE_FUNCTION
        virtual
        void
        logWarningText (
            const Char *const text) override {}

        // Log指定的Failure文字
        INLINE_FUNCTION
        virtual
        void
        logFailureText (
            const Char *const text) override {}
    };

} // namespace Core
