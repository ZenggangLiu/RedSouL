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
        UInt64 & physical_mem,
        UInt64 & virtual_mem)
    {
        physical_mem = virtual_mem = 0;

        mach_task_basic_info _task_info;
        mach_msg_type_number_t _info_size = MACH_TASK_BASIC_INFO_COUNT;
        const kern_return_t _result = task_info(
            mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&_task_info, &_info_size);
        if (_result!= KERN_SUCCESS)
        {
            return false;
        }
        else
        {
            physical_mem = (UInt64)_task_info.resident_size;
            virtual_mem  = (UInt64)_task_info.virtual_size;
            return true;
        }
    }


} // namespace Core

#endif // #if defined(__APPLE__)
