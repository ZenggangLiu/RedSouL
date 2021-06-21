// Precompiled header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
// 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#define _WINSOCKAPI_
#include <Windows.h>
#undef _WINSOCKAPI_
// Self
#include "Core/Time/RS_Time.hpp"


namespace Core
{

    Real64
    getCurTimeSec ()
    {
        // gets the frequency of the high quality counter
        LARGE_INTEGER _counter_freq;
        QueryPerformanceFrequency(&_counter_freq);

        // gets the current value of the high quality counter
        LARGE_INTEGER _counter_val;
        QueryPerformanceCounter(&_counter_val);

        // calculates the current time in second
        const Real64 _time_in_second = (Real64)_counter_val.QuadPart / (Real64)_counter_freq.QuadPart;

        return _time_in_second;
    }

  } // namespace Core

#endif // #if (OS_TYPE == OS_TYPE_WIN)
