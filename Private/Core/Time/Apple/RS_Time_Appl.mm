// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <QuartzCore/CABase.h>      // CFTimeInterval, CACurrentMediaTime()
// Self
#include "Core/Time/RS_Time.hpp"


namespace Core
{
    Real64
    getCurTimeSec ()
    {
        // CACurrentMediaTime is accurate enough
        const CFTimeInterval _time_in_second = CACurrentMediaTime();

        // --- Alternatively, we can also use the following Method --- //
        // we need this header file: #include <mach/mach_time.h>
        // gets tick in nano-second and tick scale: based on the CPU
        //mach_timebase_info_data_t _time_scale;
        //mach_timebase_info(&_time_scale);

        // gets the current ticks
        //const UInt64 _time_in_nano_second = (UInt64)mach_absolute_time();

        // calculates the convert scale from nano-second to second
        //const float64 NANOSECOND_TO_SECOND = 1.0 / 1000000000;
        //const float64 _time_in_second      = _time_in_nano_second*_time_scale.numer*NANOSECOND_TO_SECOND / _time_scale.denom;

        return _time_in_second;
    }

} // namespace Core

#endif // #if defined(__APPLE__)
