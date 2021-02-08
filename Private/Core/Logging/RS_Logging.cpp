// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Logging/RS_LoggingMgr.hpp"
// Self
#include "Core/Logging/RS_Logging.hpp"


namespace Core
{

    // 由变化长度的输入参数生成Log的文字
    #define PREPARE_LOG_TEXT(buffer, buffer_size)                                                  \
    {                                                                                              \
        va_list _arg_list;                                                                         \
        va_start(_arg_list, format);                                                               \
        std::vsnprintf((char*)buffer, buffer_size, (const char*)format, _arg_list);                \
        va_end(_arg_list);                                                                         \
    }


    // 临时Buffer的大小
    static const UInt32 BUFFER_SIZE = 512;


    void
    LoggingHelper::logInfo (
        const UInt32      flag,
        const UTF8 *const format,
        ...)
    {
        // 创建Logging字符串
        UTF8 _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_TEXT(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logInfo(flag, _tmp_buffer);
    }


    void
    LoggingHelper::logInfoAlways (
        const UTF8 *const format,
        ...)
    {
        // 创建Logging字符串
        UTF8 _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_TEXT(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logInfoAlways(_tmp_buffer);
    }


    void
    LoggingHelper::logWarning (
        const UInt32      flag,
        const UTF8 *const format,
        ...)
    {
        // 创建Logging字符串
        UTF8 _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_TEXT(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logWarning(flag, _tmp_buffer);
    }


    void
    LoggingHelper::logWarningAlways (
        const UTF8 *const format,
        ...)
    {
        // 创建Logging字符串
        UTF8 _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_TEXT(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logWarningAlways(_tmp_buffer);
    }


    void
    LoggingHelper::logFailure (
        const UInt32      flag,
        const UTF8 *const format,
        ...)
    {
        // 创建Logging字符串
        UTF8 _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_TEXT(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logFailure(flag, _tmp_buffer);
    }


    void
    LoggingHelper::logFailureAlways (
        const UTF8 *const format,
        ...)
    {
        // 创建Logging字符串
        UTF8 _tmp_buffer[BUFFER_SIZE];
        PREPARE_LOG_TEXT(_tmp_buffer, BUFFER_SIZE);

        // 调用Logging管理器输入信息
        LoggingMgr::getRef().logFailureAlways(_tmp_buffer);
    }

} // namespace Core
