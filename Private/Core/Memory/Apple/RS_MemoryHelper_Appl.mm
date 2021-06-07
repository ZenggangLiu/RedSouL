// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#include <mach/mach_init.h>
#include <mach/task.h>
#include <mach/task_info.h>
#include <mach/vm_page_size.h>
#if (OS_TYPE == OS_TYPE_MAC)
#include <mach/mach_vm.h>
#include <mach/vm_map.h>
#else
#include <mach/vm_map.h>
#endif //#if (OS_TYPE == OS_TYPE_MAC)
// Lib headers
// Self
#include "Core/Memory/RS_MemoryHelper.hpp"


namespace Core
{

    // 申请指定大小的内存
    //
    // @param[in] alloc_size
    //      想要申请的内存大小(字节)
    //      NOTE: 此大小必须为Page size的整数倍
    // @param[in] alloc_flag
    //      申请时候使用的Flag
    //      参考：VM allocation flags
    //      - VM_FLAGS_FIXED：尽可能在指定地址处申请内存空间
    //      - VM_FLAGS_ANYWHERE：在任意地方申请内存空间
    //      - VM_FLAGS_PURGABLE：此申请的内存空间在回收的时候不用Page out出去
    //      - VM_FLAGS_OVERWRITE|VM_FLAGS_FIXED：在指定地址申请内存空间，并且替换此处当前申请的空间
    // @param[in, out] alloc_addr
    //      如果alloc_flag为VM_FLAGS_ANYWHERE： 用来保存申请的地址
    //      如果alloc_flag为VM_FLAGS_FIXED：    目标地址
    //
    static
    Bool
    allocate_vm (
        const UInt32 alloc_size,
        const UInt32 alloc_flag,
        void **const alloc_addr)
    {
        RUNTIME_ASSERT((alloc_size & (MemoryHelper::getPageSize() - 1)) == 0,
                       "The allocated memory size must be a multiple of page size");
#if (OS_TYPE == OS_TYPE_MAC)
        // VM_FLAGS_ANYWHERE: 在VM的空间的任意位置创建一个大小合适的区域
        const  kern_return_t _result = mach_vm_allocate((vm_map_t)mach_task_self(),
                                                        (mach_vm_address_t*)alloc_addr,
                                                        (mach_vm_size_t)alloc_size,
                                                        alloc_flag);
#else
        const kern_return_t _result = vm_allocate((vm_map_t)mach_task_self(),
                                                  (vm_address_t*)alloc_addr,
                                                  (vm_size_t)alloc_size,
                                                  alloc_flag);
#endif  // #if (OS_TYPE == OS_TYPE_MAC)
        return _result == KERN_SUCCESS;
    }


    // 释放指定大小的内存
    static
    Bool
    release_vm (
        void *const  alloc_addr,
        const UInt32 alloc_size)
    {
        RUNTIME_ASSERT((alloc_size & (MemoryHelper::getPageSize() - 1)) == 0,
                       "The allocated memory size must be a multiple of page size");
#if (OS_TYPE == OS_TYPE_MAC)
        const kern_return_t _result = mach_vm_deallocate((vm_map_t)mach_task_self(),
                                                         (mach_vm_address_t)alloc_addr,
                                                         (mach_vm_size_t)alloc_size);
#else
        const kern_return_t _result = vm_deallocate((vm_map_t)mach_task_self(),
                                                    (vm_address_t)alloc_addr,
                                                    (vm_size_t)alloc_size);
#endif // #if (OS_TYPE == OS_TYPE_MAC)
        return _result == KERN_SUCCESS;
    }



    std::tuple<Bool, UInt64, UInt64>
    MemoryHelper::getMemoryUsage ()
    {

        mach_task_basic_info _task_info;
        mach_msg_type_number_t _info_size = MACH_TASK_BASIC_INFO_COUNT;
        const kern_return_t _result = task_info(
            mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&_task_info, &_info_size);
        if (_result!= KERN_SUCCESS)
        {
            return std::make_tuple(false, 0, 0);
        }
        else
        {
            return std::make_tuple(true,
                                   (UInt64)_task_info.resident_size,
                                   (UInt64)_task_info.virtual_size);
        }
    }


    UInt32
    MemoryHelper::getPageSize ()
    {
        return (UInt32)vm_page_size;
    }


    std::tuple<void*, UInt32>
    MemoryHelper::allocVMPages (
        const UInt32 page_count)
    {
        RUNTIME_ASSERT(page_count * (UInt64)getPageSize() <= 0xFFFFFFFF,
                       "We can JUST allocate 4G at most");
        const UInt32 _alloc_size = page_count * getPageSize();
        void * _memory_addr = nullptr;
        if (allocate_vm(_alloc_size, VM_FLAGS_ANYWHERE, &_memory_addr))
        {
            return std::make_tuple(_memory_addr, _alloc_size);
        }
        else
        {
            return std::make_tuple(nullptr, 0);
        }
    }


    Bool
    MemoryHelper::releaseVMPages (
        void* const  alloc_addr,
        const UInt32 page_count)
    {
        RUNTIME_ASSERT(page_count * (UInt64)getPageSize() <= 0xFFFFFFFF,
                       "We can JUST release 4G at most");
        const UInt32 _alloc_size = page_count * getPageSize();
        return release_vm(alloc_addr, _alloc_size);
    }


    std::tuple<void*, UInt32>
    MemoryHelper::allocMirroredVMem (
        const UInt32 alloc_size,
        const UInt32 mirror_count)
    {
        RUNTIME_ASSERT((alloc_size & (MemoryHelper::getPageSize() - 1)) == 0,
                       "The allocated memory size must be a multiple of page size");
        RUNTIME_ASSERT(alloc_size * (mirror_count+1) <= 0xFFFFFFFF,
                       "We can not allocate too much memory");
        // 计算申请的大小：主内存 + #镜像内存
        const UInt32 _memory_size = alloc_size * (mirror_count+1);
        void * _memory_addr = nullptr;

        // 在VM的空间的任意位置创建一个大小合适的区域
        if (allocate_vm(_memory_size, VM_FLAGS_ANYWHERE, &_memory_addr))
        {
            // 重新绑定主内存快
            if (allocate_vm(alloc_size, VM_FLAGS_FIXED|VM_FLAGS_OVERWRITE, &_memory_addr))
            {
                // 创建主内存块的句柄
                mem_entry_name_port_t _memory_handle;
                kern_return_t _result = mach_make_memory_entry((vm_map_t)mach_task_self(),
                                                               (vm_size_t *)&alloc_size,
                                                               (vm_offset_t)_memory_addr,
                                                               VM_PROT_READ | VM_PROT_WRITE,
                                                               &_memory_handle,
                                                               (mem_entry_name_port_t)NULL);
                if (_result != KERN_SUCCESS)
                {
                    releaseMirroredVMem(_memory_addr, alloc_size, mirror_count);
                    return std::make_tuple(nullptr, 0);
                }

                // 创建镜像内存
                for (UInt32 _mirror_idx = 0; _mirror_idx < mirror_count; ++_mirror_idx)
                {
                    void *const _mirror_addr = (UInt8*)_memory_addr + alloc_size*(_mirror_idx+1);
#if (OS_TYPE == OS_TYPE_MAC)
                    const kern_return_t _result = mach_vm_map((vm_map_t)mach_task_self(),
                                                              (mach_vm_address_t*)&_mirror_addr,
                                                              (mach_vm_size_t)alloc_size,
                                                              (mach_vm_offset_t)0, // mask
                                                              VM_FLAGS_FIXED|VM_FLAGS_OVERWRITE,
                                                              (mem_entry_name_port_t)_memory_handle,
                                                              (memory_object_offset_t)0,
                                                              false,
                                                              (vm_prot_t)VM_PROT_READ|VM_PROT_WRITE,
                                                              (vm_prot_t)VM_PROT_READ|VM_PROT_WRITE,
                                                              (vm_inherit_t)VM_INHERIT_NONE);
#else
                    const kern_return_t _result = vm_map((vm_map_t)mach_task_self(),
                                                         (vm_address_t*)&_mirror_addr,
                                                         (vm_size_t)alloc_size,
                                                         (vm_address_t)0, // mask
                                                         VM_FLAGS_FIXED|VM_FLAGS_OVERWRITE,
                                                         (mem_entry_name_port_t)_memory_handle,
                                                         (vm_offset_t)0,
                                                         false,
                                                         (vm_prot_t)VM_PROT_READ|VM_PROT_WRITE,
                                                         (vm_prot_t)VM_PROT_READ|VM_PROT_WRITE,
                                                         (vm_inherit_t)VM_INHERIT_NONE);
#endif // #if (OS_TYPE == OS_TYPE_MAC)
                    if (_result != KERN_SUCCESS)
                    {
                        // 镜像失败
                        releaseMirroredVMem(_memory_addr, alloc_size, mirror_count);
                        return std::make_tuple(nullptr, 0);
                    }
                }
                return std::make_tuple(_memory_addr, alloc_size);
            }
            else
            {
                releaseMirroredVMem(_memory_addr, alloc_size, mirror_count);
            }
        }
        return std::make_tuple(nullptr, 0);
    }


    Bool
    MemoryHelper::releaseMirroredVMem (
        void* const  alloc_addr,
        const UInt32 alloc_size,
        const UInt32 mirror_count)
    {
        const UInt32 _alloc_size = alloc_size * (mirror_count+1);
        return release_vm(alloc_addr, _alloc_size);
    }

} // namespace Core

#endif // #if defined(__APPLE__)
