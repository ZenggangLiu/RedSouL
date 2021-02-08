#pragma once


// System headers
// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Hash/RS_CompiletimeHash.hpp"


namespace Core
{

    // 定义宏使用的接口
    struct LoggingHelper
    {
        // 显示Info log
        static
        void
        logInfo (
            const UInt32      flag,
            const UTF8 *const format,
            ...);

        static
        void
        logInfoAlways (
            const UTF8 *const format,
            ...);

        // 显示Warnning log
        static
        void
        logWarning (
            const UInt32      flag,
            const UTF8 *const format,
            ...);

        static
        void
        logWarningAlways (
            const UTF8 *const format,
            ...);

        // 显示Failure log
        static
        void
        logFailure (
            const UInt32      flag,
            const UTF8 *const format,
            ...);

        static
        void
        logFailureAlways (
            const UTF8 *const format,
            ...);
    };

}


// 检测LOGGING_MODE是否定义
#if !defined(LOGGING_MODE)
#error Please define LOGGING_MODE in the build setting("Preprocessor Macros")
#endif // #if !defined(LOGGING_MODE)

// Logging使用的Hash seeds
#define HASH_SEED_LOG               FOUR_CC('L', 'O', 'G', 'G')

// --- 使用Logging功能 ---
// 1. 定义LOGGING_MODE=1
// 2. 定义相应的宏来开启INFO(), WARNING()以及FAILURE()的宏：
//    ENABLE_INFO_LOGGING, ENABLE_WARNING_LOGGING, ENABLE_FAILURE_LOGGING
// 3. 在Document目录下产生一个配置文件：Logging.cfg
// 它的格式为
//  a) Logger开关:
//     - ConsoleLogger                               <-- 表示使用ConsoleLogger
//     - FileLogger = Folder_Name/Logging_output.txt <-- 表示使用FileLogger
//       输出文件Folder_Name/Logging_output.txt相对于Document目录
//     - NetworkLogger = 8866                        <-- 表示使用LoggingServer
//       8866位LoggingServer的Port
//   b) Flag Enable开关:
//     - SomeFlag
//     - OtherFlag
//     NOTE: Flag为当前Flag的名称。他们不带任何引号(此规则适用于配置文件和C++代码)
// Examples：
// INFO(TestFlag, "Checking logging of Info: int(100): %d", 100);
// WARNING(TestFlag, "Checking logging of Warning");
// FAILURE(TestFlag, "Checking logging of Failure: %s", "this is a failure");
// TODO("We have to do this");
//
// --- Logging信息使用如下格式 ---
// Logger种类:      ConsoleLogger             FileLogger              LogServer
// Failure         Red[FAIL]:                [---FAIL---]:           Msg_FailLog
// Warnning        Yellow[WARN]:             [---WARN---]:           Msg_WarnLog
// Info            Green[INFO]:              [---INFO---]:           Msg_InfoLog
//
//
#if (LOGGING_MODE == 1)
    // 使用INFO()
    #if defined(ENABLE_INFO_LOGGING)
        #define INFO(flag, format, ...)                                                            \
        Core::LoggingHelper::logInfo(COMPILE_TIME_HASH(HASH_SEED_LOG, TOKEN_QUOTE(flag)),          \
                                     (const UTF8*)format,                                          \
                                     ##__VA_ARGS__)
        #define INFO_ALWAYS(format, ...)                                                           \
        Core::LoggingHelper::logInfoAlways((const UTF8*)format, ##__VA_ARGS__)
        #define TODO(text)                                                                         \
        INFO(TODO, "[TODO]- %s", text)
    // 禁止INFO()
    #else
        #define INFO(flag, format, ...)
        #define INFO_ALWAYS(format, ...)
        #define TODO(text)

    #endif // fined(ENABLE_INFO_LOGGING)

    // 使用WARNING()
    #if defined(ENABLE_WARNING_LOGGING)
        #define WARNING(flag, format, ...)                                                         \
        Core::LoggingHelper::logWarning(COMPILE_TIME_HASH(HASH_SEED_LOG, TOKEN_QUOTE(flag)),       \
                                        (const UTF8*)format,                                       \
                                        ##__VA_ARGS__)
        #define WARNING_ALWAYS(format, ...)                                                        \
        Core::LoggingHelper::logWarningAlways((const UTF8*)format, ##__VA_ARGS__)
    // 禁止WARNING()
    #else
        #define WARNING(flag, format, ...)
        #define WARNING_ALWAYS(format, ...)
    #endif // #if defined(ENABLE_WARNING_LOGGING)

    // 使用FAILURE()
    #if defined(ENABLE_FAILURE_LOGGING)
        #define FAILURE(flag, format, ...)                                                         \
        Core::LoggingHelper::logFailure(COMPILE_TIME_HASH(HASH_SEED_LOG, TOKEN_QUOTE(flag)),       \
                                        (const UTF8*)format,                                       \
                                        ##__VA_ARGS__)
        #define FAILURE_ALWAYS(format, ...)                                                        \
        Core::LoggingHelper::logFailureAlways((const UTF8*)format, ##__VA_ARGS__)
    // 禁止FAILURE()
    #else
        #define FAILURE(flag, format, ...)
        #define FAILURE_ALWAYS(format, ...)
    #endif // #if defined(ENABLE_FAILURE_LOGGING)

// 禁止所有Logging功能
#else
    #define INFO(flag, format, ...)
    #define INFO_ALWAYS(format, ...)
    #define TODO(text)
    #define WARNING(flag, format, ...)
    #define WARNING_ALWAYS(format, ...)
    #define FAILURE(flag, format, ...)
    #define FAILURE_ALWAYS(format, ...)
#endif // #if (LOGGING_MODE == 1)
