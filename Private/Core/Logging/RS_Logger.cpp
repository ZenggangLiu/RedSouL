// Precompiled header
#include "PrecompiledH.hpp"
// System headers
#include <ctime>        // time()
// Lib headers
// Self
#include "Core/Logging/RS_Logger.hpp"


namespace Core
{

    Logger::Logger (
        const LoggerTypes type)
    :
        m_type(type)
    {

    }


    Logger::~Logger ()
    {

    }


    void
    Logger::getTimeTag (
        UTF8 *const  buffer,
        const UInt32 buffer_size)
    {
        // 获得当前时间戳: time since epoch
        const std::time_t _cur_time_stamp = std::time(nullptr);
        // 使用Local Time Zone来分解当前时间
        // convert the current time stemp into calandar time using local time zone
#if (OS_TYPE == OS_TYPE_WIN)
        std::tm _cur_time_local;
        const std::tm* const _cur_time_local_ptr = &_cur_time_local;
        localtime_s(&_cur_time_local, &_cur_time_stamp);
#else
        const std::tm *const _cur_time_local_ptr = std::localtime(&_cur_time_stamp);
#endif // #if (OS_TYPE == OS_TYPE_WIN)
        // 创建时间头: 格式字符请参考： https://en.cppreference.com/w/cpp/chrono/c/strftime
        std::strftime((char*)buffer, buffer_size, "[%d/%B %H:%M:%S] ", _cur_time_local_ptr);
    }

} // namespace Core
