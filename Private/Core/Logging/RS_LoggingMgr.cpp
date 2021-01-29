// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Logging/RS_ConsoleLogger.hpp"
#include "Core/Logging/RS_DumbLogger.hpp"
// Self
#include "RS_LoggingMgr.hpp"


namespace Core
{

    LoggingMgr&
    LoggingMgr::getRef ()
    {
        static LoggingMgr s_instance;
        return s_instance;
    }


    void
    LoggingMgr::logInfo (
        const UInt32      flag,
        const Char *const text)
    {
        if (isFlagEnabled(flag))
        {
            logInfoAlways(text);
        }
    }


    void
    LoggingMgr::logInfoAlways (
        const Char *const text)
    {
        // 使用当前的Logger输出Info信息
        Logger *const _logger = (Logger*)m_logger;
        _logger->logInfoText(text);
    }


    void
    LoggingMgr::logWarning (
        const UInt32      flag,
        const Char *const text)
    {
        if (isFlagEnabled(flag))
        {
            logWarningAlways(text);
        }
    }


    void
    LoggingMgr::logWarningAlways (
        const Char *const text)
    {
        // 使用当前的Logger输出Warning信息
        Logger *const _logger = (Logger*)m_logger;
        _logger->logWarningText(text);
    }


    void
    LoggingMgr::logFailure (
    const UInt32      flag,
    const Char *const text)
    {
        if (isFlagEnabled(flag))
        {
            logFailureAlways(text);
        }
    }


    void
    LoggingMgr::logFailureAlways (
        const Char *const text)
    {
        // 使用当前的Logger输出Failure信息
        Logger *const _logger = (Logger*)m_logger;
        _logger->logFailureText(text);
    }


    LoggingMgr::LoggingMgr ()
    {
        // 清理Flag数组
        memset(m_flag_list, 0, MAX_FLAG_COUNT*sizeof(UInt32));

        // 从Document目录下读入配置文件：Logging.cfg
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
        std::printf("[TODO]- !!!Need Implementation!!!\n");

        // Test code
        // 创建Logger
        // 检测Logging.cfg中Logger开关部分。如果不存在则创建DumbLogger
        COMPILE_TIME_ASSERT_MSG(sizeof(DumbLogger) <= LOGGER_INSTANCE_SIZE,
                                "The DumbLogger's size is too large");
        new(m_logger)DumbLogger();

        // create ConsoleLogger
        COMPILE_TIME_ASSERT_MSG(sizeof(ConsoleLogger) <= LOGGER_INSTANCE_SIZE,
                                "The ConsoleLogger's size is too large");
        new(m_logger)ConsoleLogger();

        // enable TestFlag
        UInt8 _cur_idx = 0;
        m_flag_list[_cur_idx++] = hash32(HASH_SEED_LOG, "TestFlag");
        m_flag_list[_cur_idx++] = hash32(HASH_SEED_LOG, "TODO");
    }


    Bool
    LoggingMgr::isFlagEnabled (
        const UInt32 flag)
    {
        // 检查开关
        for (UInt8 _idx = 0; _idx < MAX_FLAG_COUNT; ++_idx)
        {
            // 检查Flag数组的当前位置是否存储一个合法的Flag：0表示非法
            if (m_flag_list[_idx] == 0)
            {
                return false;
            }

            // 找到相应的Flag
            if (m_flag_list[_idx] == flag)
            {
                return true;
            }
        }

        return false;
    }

} // namespace Core
