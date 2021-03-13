// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/FileSystem/RS_TextFileReader.hpp"
#include "Core/Logging/RS_ConsoleLogger.hpp"
#include "Core/Logging/RS_DumbLogger.hpp"
// Self
#include "RS_LoggingMgr.hpp"


namespace Core
{

    #define CONSOLE_LOGGER_NAME         "ConsoleLogger"
    #define FILE_LOGGER_NAME            "FileLogger"
    #define NETWORK_LOGGER_NAME         "NetworkLogger"


    // 表示LoggingMgr是否正在进行初始化
    //                      +--------------------------------+
    //                      |                                |
    //                      v                                |
    // 为了防止Call Loop: LoggingMgr --> NativeFile Open --> INFO
    static Bool g_is_initializing = false;


    Bool
    LoggingMgr::isInitializing ()
    {
        return g_is_initializing;
    }


    LoggingMgr&
    LoggingMgr::getRef ()
    {
        static LoggingMgr s_instance;
        return s_instance;
    }


    void
    LoggingMgr::logInfo (
        const UInt32      flag,
        const UTF8 *const text)
    {
        if (isFlagEnabled(flag))
        {
            logInfoAlways(text);
        }
    }


    void
    LoggingMgr::logInfoAlways (
        const UTF8 *const text)
    {
        // 使用当前的Logger输出Info信息
        Logger *const _logger = (Logger*)m_logger;
        _logger->logInfoText(text);
    }


    void
    LoggingMgr::logWarning (
        const UInt32      flag,
        const UTF8 *const text)
    {
        if (isFlagEnabled(flag))
        {
            logWarningAlways(text);
        }
    }


    void
    LoggingMgr::logWarningAlways (
        const UTF8 *const text)
    {
        // 使用当前的Logger输出Warning信息
        Logger *const _logger = (Logger*)m_logger;
        _logger->logWarningText(text);
    }


    void
    LoggingMgr::logFailure (
    const UInt32      flag,
    const UTF8 *const text)
    {
        if (isFlagEnabled(flag))
        {
            logFailureAlways(text);
        }
    }


    void
    LoggingMgr::logFailureAlways (
        const UTF8 *const text)
    {
        // 使用当前的Logger输出Failure信息
        Logger *const _logger = (Logger*)m_logger;
        _logger->logFailureText(text);
    }


    LoggingMgr::LoggingMgr ()
    {
        // 设置初始化Flag
        g_is_initializing = true;

        // 清理Flag数组
        std::memset(m_flag_list, 0, MAX_FLAG_COUNT*sizeof(UInt32));

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
        const UTF8 *const _file_name = (const UTF8*)"Logging.cfg";
        const NativeFileSystem::SearchPaths _find_dir=NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER;
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        const RuntimeText _file_abs_path = NativeFileSystem::getAbsPath(_file_name,_find_dir);
        std::printf("[LoggingMgr][Init]: Reading config file: %s\n", _file_abs_path.c_str());
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
        TextFileReader _cfg_reader(_file_name, _find_dir);

        LoggerTypes _logger_type = LOGGER_TYPES_DUMB;
        if (_cfg_reader.isValid())
        {
            // CFG文件语法: 注意CFG文件中所有配置信息为UTF8/ASCII编码
            // <文件>   := <注释行> | <Logger类型> | <Logging开关>
            // <注释行> := # [^\n]^* \n
            // <Logger类型>  := ConsoleLogger | <文件Logger> | <网络Logger>
            // <文件Logger>  := FileLogger [SPACE]^* = [SPACE]^* <文件路径>
            // <网络Logger>  := NetworkLogger [SPACE]^* = [SPACE]^* <端口号>
            // <Logging开关> := [SPACE]^* [^\n]^1 [SPACE]^* \n
            static const UTF8   SPACE               = ' ';
            static const UTF8   COMMENT             = '#';
            static const UInt32 CONSOLE_LOGGER_HASH = COMPILE_TIME_HASH(HASH_SEED_LOG,
                                                                        CONSOLE_LOGGER_NAME);
            static const UInt32 CONSOLE_LOGGER_LEN  = static_text_length(CONSOLE_LOGGER_NAME);
            static const UInt32 FILE_LOGGER_HASH    = COMPILE_TIME_HASH(HASH_SEED_LOG,
                                                                        FILE_LOGGER_NAME);
            static const UInt32 FILE_LOGGER_LEN     = static_text_length(FILE_LOGGER_NAME);
            static const UInt32 NETWORK_LOGGER_HASH = COMPILE_TIME_HASH(HASH_SEED_LOG,
                                                                        NETWORK_LOGGER_NAME);
            static const UInt32 NETWORK_LOGGER_LEN  = static_text_length(NETWORK_LOGGER_NAME);

            RuntimeText _cfg_line;
            UTF8        _buffer[16];
            UInt8       _flag_idx = 0;
            // 逐行读入文件内容
            while (_cfg_reader.readLine(_cfg_line))
            {
                // 逐个字符处理
                for (UInt32 _char_idx = 0; _char_idx < (UInt32)_cfg_line.size(); ++_char_idx)
                {
                    // 忽略空格字符
                    if (_cfg_line[_char_idx] == SPACE)
                    {
                        continue;
                    }

                    // 忽略注释行
                    if (_cfg_line[_char_idx] == COMMENT)
                    {
                        // 处理下一行
                        break;
                    }

                    // 判断是否为Logger类型定义
                    const UInt32 _rest_char_count = (UInt32)_cfg_line.size() - _char_idx;
                    // 是否使用ConsoleLogger
                    if (_rest_char_count >= CONSOLE_LOGGER_LEN)
                    {
                        std::memcpy(_buffer, _cfg_line.data() + _char_idx, CONSOLE_LOGGER_LEN);
                        if (hash32(HASH_SEED_LOG,_buffer,CONSOLE_LOGGER_LEN) == CONSOLE_LOGGER_HASH)
                        {
                            _logger_type = LOGGER_TYPES_CONSOLE;
                            // 处理下一行
                            break;
                        }
                    }
                    // 是否使用FileLogger
                    if (_rest_char_count >= FILE_LOGGER_LEN)
                    {
                        std::memcpy(_buffer, _cfg_line.data() + _char_idx, FILE_LOGGER_LEN);
                        if (hash32(HASH_SEED_LOG , _buffer , FILE_LOGGER_LEN) == FILE_LOGGER_HASH)
                        {
                            _logger_type = LOGGER_TYPES_FILE;
                            // 处理下一行
                            break;
                        }
                    }
                    // 是否使用NetworkLogger
                    if (_rest_char_count >= NETWORK_LOGGER_LEN)
                    {
                        std::memcpy(_buffer, _cfg_line.data() + _char_idx, NETWORK_LOGGER_LEN);
                        if (hash32(HASH_SEED_LOG,_buffer,NETWORK_LOGGER_LEN) == NETWORK_LOGGER_HASH)
                        {
                            _logger_type = LOGGER_TYPES_NETWORK;
                            // 处理下一行
                            break;
                        }
                    }

                    // 保存各种Flag
                    // Flag的起始字符
                    const UInt8 *const _start_char_ptr = _cfg_line.data() + _char_idx;
                    m_flag_list[_flag_idx++] = hash32(HASH_SEED_LOG, _start_char_ptr);
                    break;
                }
            }
            _cfg_reader.close();
        }

        // 创建Logger
        switch (_logger_type)
        {
            case LOGGER_TYPES_DUMB:
            {
                COMPILE_TIME_ASSERT_MSG(sizeof(DumbLogger) <= LOGGER_INSTANCE_SIZE,
                                        "The DumbLogger's size is too large");
                new(m_logger)DumbLogger();
                break;
            }

            case LOGGER_TYPES_CONSOLE:
            {
                COMPILE_TIME_ASSERT_MSG(sizeof(ConsoleLogger) <= LOGGER_INSTANCE_SIZE,
                                        "The ConsoleLogger's size is too large");
                new(m_logger)ConsoleLogger();
                break;
            }

            default:
            {
                RUNTIME_ASSERT(false, "No implementation for this logger type");
                COMPILE_TIME_ASSERT_MSG(sizeof(DumbLogger) <= LOGGER_INSTANCE_SIZE,
                                        "The DumbLogger's size is too large");
                new(m_logger)DumbLogger();
                break;
            }
        }

        // 复位初始化Flag
        g_is_initializing = false;
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
