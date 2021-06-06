#pragma once


// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    struct ThreadHelper
    {
        // 检查当前调用线程是否为主线程
        static
        Bool
        isMainThread ();

        // 让调用线程永久休眠
        static
        void
        sleep ();

        // 让调用线程休眠
        //
        // @param[in] milli_sec
        //      等待的时间(毫秒)
        static
        void
        sleep (
           const UInt32 milli_sec);

    };

} // namespace Core
