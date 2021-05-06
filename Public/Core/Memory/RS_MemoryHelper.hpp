#pragma once


#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    struct MemoryHelper
    {
        // --- OS specific methods START --- //
        // 获取此进程/程序的内存使用情况
        //
        // @param[in] physical_mem_Mb
        //      使用的物理内存：以Mega Byte计算
        // @param[in] virtual_mem_Mb
        //      使用的虚拟内存：以Mega Byte计算
        // @return
        //      TRUE：如果操作成功
        static
        Bool
        getMemoryUsage (
            Real32 & physical_mem_Mb,
            Real32 & virtual_mem_Mb);
        // --- OS specific methods END --- //
    };

} // namespace Core
