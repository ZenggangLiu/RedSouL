#pragma once


// System headers
#include <vector>
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // Logging管理器
    class LoggingMgr
    {
    public:
        // 获得LoggingMgr的参考
        static
        LoggingMgr&
        getRef ();

        // Logging Info
        //
        // @param flag
        //      定义Info Log是否启动的Flag
        // @param text
        //      Log出去的文字
        void
        logInfo (
            const UInt32      flag,
            const UTF8 *const text);

        void
        logInfoAlways (
            const UTF8 *const text);

        // Logging Warnning
        //
        // @param flag
        //      定义Warning Log是否启动的Flag
        // @param text
        //      Log出去的文字
        void
        logWarning (
            const UInt32      flag,
            const UTF8 *const text);

        void
        logWarningAlways (
            const UTF8 *const text);

        // Logging Failure
        //
        // @param flag
        //      定义Failure Log是否启动的Flag
        // @param text
        //      Log出去的文字
        void
        logFailure (
            const UInt32      flag,
            const UTF8 *const text);

        void
        logFailureAlways (
            const UTF8 *const text);


    private:
        // Constructs the manager
        LoggingMgr ();

        // 检查是否给定的Flag启用
        Bool
        isFlagEnabled (
            const UInt32 flag);

    private:
        // 最大的Flag数目
        static const UInt32 MAX_FLAG_COUNT          = 16;
        // 用来存放Logger实例的空间大小
        static const UInt32 LOGGER_INSTANCE_SIZE    = 32;

        // Flag数组: 64bytes: 0 表示没有使用
        UInt32 m_flag_list[MAX_FLAG_COUNT];
        // Logger实例的存放空间
        UInt8  m_logger[LOGGER_INSTANCE_SIZE];
    };

} // namespace Core
