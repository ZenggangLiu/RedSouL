// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Logging/RS_LoggingMgr.hpp"
// Self
#include "Core/Logging/RS_Logging.hpp"


namespace Core
{

    // 由变化长度的输入参数生成Log字符串
    #define PREPARE_LOG_STRING(buffer, buffer_size)                                                \
    {                                                                                              \
        va_list _arg_list;                                                                         \
        va_start(_arg_list, format);                                                               \
        std::vsnprintf(buffer, buffer_size, format, _arg_list);                                    \
        va_end(_arg_list);                                                                         \
    }


    // 临时Buffer的大小
    static const UInt32 BUFFER_SIZE = 512;


    void
    LoggingHelper::logInfo (
        const UInt32      flag,
        const Char *const format,
        ...)
    {
        // 创建Logging字符串
        Char _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_STRING(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logInfo(flag, _tmp_buffer);
    }


    void
    LoggingHelper::logInfoAlways (
        const Char *const format,
        ...)
    {
        // 创建Logging字符串
        Char _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_STRING(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logInfoAlways(_tmp_buffer);
    }


    void
    LoggingHelper::logWarning (
        const UInt32      flag,
        const Char *const format,
        ...)
    {
        // 创建Logging字符串
        Char _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_STRING(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logWarning(flag, _tmp_buffer);
    }


    void
    LoggingHelper::logWarningAlways (
        const Char *const format,
        ...)
    {
        // 创建Logging字符串
        Char _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_STRING(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logWarningAlways(_tmp_buffer);
    }


    void
    LoggingHelper::logFailure (
        const UInt32      flag,
        const Char *const format,
        ...)
    {
        // 创建Logging字符串
        Char _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_STRING(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logFailure(flag, _tmp_buffer);
    }


    void
    LoggingHelper::logFailureAlways (
        const Char *const format,
        ...)
    {
        // 创建Logging字符串
        Char _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_STRING(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logFailureAlways(_tmp_buffer);
    }

} // namespace Core
