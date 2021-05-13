#pragma once


#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    struct MemoryHelper
    {
        // --- OS specific methods START --- //
        // 获取此进程/程序的内存使用情况
        //
        // @param[out] physical_mem
        //      使用的物理内存(字节)
        // @param[out] virtual_mem
        //      使用的虚拟内存(字节)
        // @return
        //      TRUE：如果操作成功
        static
        Bool
        getMemoryUsage (
            UInt64 & physical_mem,
            UInt64 & virtual_mem);
        // --- OS specific methods END --- //
    };

} // namespace Core
