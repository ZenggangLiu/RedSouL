// Precompiled header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
// 禁止Windows.h包含winsock.h, 因为我们使用winsock2.h
#define _WINSOCKAPI_
#include <Windows.h>
#undef _WINSOCKAPI_
#include <psapi.h> 
#include <sysinfoapi.h>
// Lib headers
// Self
#include "Core/Memory/RS_MemoryHelper.hpp"


namespace Core
{
    std::tuple<Bool, UInt64, UInt64>
    MemoryHelper::getMemoryUsage ()
    {
        PROCESS_MEMORY_COUNTERS _mem_info;
        // WorkingSetSize: Working set/Resident set: the amount of physical memory mapped to this process
        // PagefileUsage : VM size: the amount of virtual memory committed to this process
        // GetProcessMemoryInfo(): 返回非零，如果操作成功
        const Bool _success = GetProcessMemoryInfo(GetCurrentProcess(), &_mem_info, sizeof(_mem_info));
        if (_success)
        {
            return std::make_tuple(true,
                                   (UInt64)_mem_info.WorkingSetSize,
                                   (UInt64)_mem_info.PagefileUsage);
        }
        else
        {
            return std::make_tuple(false, 0, 0);
        }
    }


    UInt32
    MemoryHelper::getPageSize ()
    {
        SYSTEM_INFO _sys_info;
        GetSystemInfo(&_sys_info);
        return (UInt32)_sys_info.dwPageSize;
    }

} // namespace Core

#endif // #if (OS_TYPE == OS_TYPE_WIN)
