// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <mach/mach_init.h>
#include <mach/task.h>
#include <mach/task_info.h>
// Lib headers
// Self
#include "Core/Memory/RS_MemoryHelper.hpp"


namespace Core
{

    Bool
    MemoryHelper::getMemoryUsage (
        Real32 & physical_mem_Mb,
        Real32 & virtual_mem_Mb)
    {
        static const Real32 SCALE_FACTOR = 1.0 / 1024 / 1024;

        physical_mem_Mb = virtual_mem_Mb = 0;

        mach_task_basic_info _task_info;
        mach_msg_type_number_t _info_num = MACH_TASK_BASIC_INFO_COUNT;
        const kern_return_t _result = task_info(
            mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&_task_info, &_info_num);
        if (_result!= KERN_SUCCESS)
        {
            return false;
        }
        else
        {
            physical_mem_Mb = _task_info.resident_size * SCALE_FACTOR;
            virtual_mem_Mb  = _task_info.virtual_size  * SCALE_FACTOR;
            return true;
        }
    }


} // namespace Core

#endif // #if defined(__APPLE__)
