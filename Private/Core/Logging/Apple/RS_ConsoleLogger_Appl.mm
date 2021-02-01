// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#import <Foundation/NSString.h>
// Lib headers
// Self
#include "Core/Logging/RS_ConsoleLogger.hpp"


namespace Core
{

    // 错误信息的格式为: 时间Tag               信息颜色Tag    信息类型Tag    信息内容
    //                [Jan-29 15:35:20]    💔           [FAIL]:       Some failures happend
    // 信息的颜色Tag
    static NSString *const FAIL_COLOR_TAG = @"💔 ";
    static NSString *const WARN_COLOR_TAG = @"💛 ";
    static NSString *const INFO_COLOR_TAG = @"💚 ";


    ConsoleLogger::ConsoleLogger ()
    :
        SUPER(LOGGER_TYPES_CONSOLE)
    {

    }

    ConsoleLogger::~ConsoleLogger ()
    {

    }


    void
    ConsoleLogger::logInfoText (
        const Char *const text)
    {
        // 创建时间Tag
        Char _time_tag[32];
        getTimeTag(_time_tag, sizeof(_time_tag));
        // 输出信息
        std::cout << _time_tag << [INFO_COLOR_TAG UTF8String] << INFO_TYPE_TAG << text << std::endl;
//        std::printf("%s%s%s%s\n", _time_tag, [INFO_COLOR_TAG UTF8String], INFO_TYPE_TAG, text);
    }


    void
    ConsoleLogger::logWarningText (
        const Char *const text)
    {
        // 创建时间Tag
        Char _time_tag[32];
        getTimeTag(_time_tag, sizeof(_time_tag));
        // 输出信息
        std::cout << _time_tag << [WARN_COLOR_TAG UTF8String] << WARN_TYPE_TAG << text << std::endl;
    }


    void
    ConsoleLogger::logFailureText (
        const Char *const text)
    {
        // 创建时间Tag
        Char _time_tag[32];
        getTimeTag(_time_tag, sizeof(_time_tag));
        // 输出信息
        std::cout << _time_tag << [FAIL_COLOR_TAG UTF8String] << FAIL_TYPE_TAG << text << std::endl;
    }

} // namespace Core


#endif // #if defined(__APPLE__)
