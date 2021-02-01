#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // Logger的类型定义
    enum LoggerTypes
    {

        // Dumb logger: 此Logger将不输入任何信息
        LOGGER_TYPES_DUMB       = 0,
        // Console logger：将Logging的信息输出到Console窗口
        LOGGER_TYPES_CONSOLE    = 1,
        // File logger：将Logging的信息输出到指定文件中
        LOGGER_TYPES_FILE       = 2,
        // Network logger：将Logging的信息保存到Logging服务器
        LOGGER_TYPES_NETWORK    = 3,

        LOGGER_TYPES_COUNT
    };


    // Logger的基类
    class Logger
    {
    public:
        // Constructor
        //
        //@param[in] type
        //   Logger的类型
        Logger (
            const LoggerTypes type);

        // Destructor
        virtual
        ~Logger ();

        // Log指定的Info文字
        virtual
        void
        logInfoText (
            const Char *const text) = 0;

        // Log指定的Warning文字
        virtual
        void
        logWarningText (
            const Char *const text) = 0;

        // Log指定的Failure文字
        virtual
        void
        logFailureText (
            const Char *const text) = 0;

    protected:
        // 创建当前的时间Tag：[Jan-29 15:35:20]
        void
        getTimeTag (
            Char *const  buffer,
            const UInt32 buffer_size);

    protected:
        // Logger的类型
        LoggerTypes m_type;
    };

} // namespace Core
