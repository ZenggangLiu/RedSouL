#pragma once


// System headers
#include <tuple>
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    struct MemoryHelper
    {
        // --- OS specific methods START --- //
        // 获取此进程/程序的内存使用情况
        //
        // @return
        //      如果申请成功：<- (true, 使用的物理内存(字节), 使用的虚拟内存(字节))
        //      如果申请失败：<- (false, 0, 0)
        static
        std::tuple<Bool, UInt64, UInt64>
        getMemoryUsage ();

        // 获得OS提供的内存页大小(字节)
        static
        UInt32
        getPageSize ();

        // 申请指定数目的虚拟Page
        //
        // @param[in] page_count
        //      Page的数目
        // @return
        //      如果申请成功：<- (申请的内存地址, 申请的内存大小)
        //      如果申请失败：<- (nullptr, 0)
        // @example:
        //     申请16页虚拟内存：
        //     const void * alloc_addr; UInt32 alloc_size;
        //     std::tie(alloc_addr, alloc_size) = Core::MemoryHelper::allocVMPages(16);
        static
        std::tuple<void*, UInt32>
        allocVMPages (
            const UInt32 page_count);

        // 释放指定的虚拟Page
        //
        // @param[in] alloc_addr
        //      申请到的起始内存地址
        // @param[in] page_count
        //      Page的数目
        // @return
        //      TRUE:  如果内存释放成功
        //      FALSE: 如果失败
        static
        Bool
        releaseVMPages (
            void* const  alloc_addr,
            const UInt32 page_count);

        // 申请带有镜像功能的虚拟内存
        // 带有镜像功能的内存排布：
        //  主内存：        镜像的内存：
        // +---------------+---------------+
        // | Contents:     | Mirrored:     |
        // | xxx yyyy zz   | xxx yyyy zz   |
        // +---------------+---------------+
        // NOTE：此函数将使用alloc_size + alloc_size*mirror_count倍虚拟/物理内存
        //
        // @param[in] alloc_size
        //      申请的主内存大小(字节)
        //      NOTE：申请的大小必须为Page size的整数倍
        // @param[in] mirror_count
        //      镜像内存的个数
        // @return
        //      如果申请成功：<- (申请的内存地址, 申请的内存大小)
        //      如果申请失败：<- (nullptr, 0)
        static
        std::tuple<void*, UInt32>
        allocMirroredVMem (
            const UInt32 alloc_size,
            const UInt32 mirror_count);

        // 释放带有镜像的内存
        //
        // @param[in] alloc_addr
        //      准备释放的内存
        // @param[in] alloc_size
        //      主内存大小(字节)
        //      NOTE：申请的大小必须为Page size的整数倍
        // @param[in] mirror_count
        //      镜像内存的个数
        // @return
        //      TRUE：操作成功
        static
        Bool
        releaseMirroredVMem (
            void* const  alloc_addr,
            const UInt32 alloc_size,
            const UInt32 mirror_count);
        // --- OS specific methods END --- //
    };

} // namespace Core
