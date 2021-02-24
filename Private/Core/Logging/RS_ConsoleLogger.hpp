#pragma once


// System headers
// Lib headers
#include "Core/Logging/RS_Logger.hpp"


namespace Core
{

    // Logging信息使用Console
    class ConsoleLogger : public Logger
    {
    public:
        // --- OS specific methods START --- //
        // Implementations are in RS_ConsoleLogger_Appl/Wind/Andr.???
        // Constructor
        ConsoleLogger ();

        // Destructor
        virtual
        ~ConsoleLogger ();

        // Log指定的Info文字
        virtual
        void
        logInfoText (
            const UTF8 *const text) override;

        // Log指定的Warning文字
        virtual
        void
        logWarningText (
            const UTF8 *const text) override;

        // Log指定的Failure文字
        virtual
        void
        logFailureText (
            const UTF8 *const text) override;
        // --- OS specific methods END --- //

    private:
        typedef Logger SUPER;

        // 各种类型信息的Tag
        static const UTF8 FAIL_TYPE_TAG[];
        static const UTF8 WARN_TYPE_TAG[];
        static const UTF8 INFO_TYPE_TAG[];
    };

} // namespace Core
