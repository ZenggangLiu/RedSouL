#include <cstdio>
#include <memory>
#include "Core/Memory/RS_MemoryHelper.hpp"


int
main (
    int               argc,
    const char *const argv[])
{
    static UInt32 MEM_SIZE = 256*1024*1024;
    static Real32 BYTE_2_KBYTE = 1.0 / 1024;
    static Real32 BYTE_2_MBYTE = BYTE_2_KBYTE / 1024;

    std::printf("------ STARTING... ------\n");
    UInt64 _start_pm, _start_vm;
    Core::MemoryHelper::getMemoryUsage(_start_pm, _start_vm);
    std::printf("[Virtual]:   %8.2fMb, [Resident]:  %8.2fMb\n",
                _start_vm * BYTE_2_MBYTE, _start_pm * BYTE_2_MBYTE);
    std::printf("------ MALLOC[malloc()] 256Mb ------\n");
    void * addr = std::malloc(MEM_SIZE);
    UInt64 _cur_pm, _cur_vm;
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[Virtual]:   %8.2fMb, [Resident]:  %8.2fMb\n",
                _cur_vm * BYTE_2_MBYTE, _cur_pm * BYTE_2_MBYTE);
    std::printf("[Different]: %8.2fMb, [Different]: %8.2fMb\n",
                (_cur_vm-_start_vm) * BYTE_2_MBYTE, (_cur_pm-_start_pm) * BYTE_2_MBYTE);
    std::printf("------ MEMSET[memset()] 256Mb ------\n");
    std::memset(addr, 0, MEM_SIZE);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[Virtual]:   %8.2fMb, [Resident]:  %8.2fMb\n",
                _cur_vm * BYTE_2_MBYTE, _cur_pm * BYTE_2_MBYTE);
    std::printf("[Different]: %8.2fMb, [Different]: %8.2fMb\n",
                (_cur_vm-_start_vm) * BYTE_2_MBYTE, (_cur_pm-_start_pm) * BYTE_2_MBYTE);
    std::printf("------ FREE[free()] 256Mb ------\n");
    std::free(addr);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[Virtual]:   %8.2fMb, [Resident]:  %8.2fMb\n",
                _cur_vm * BYTE_2_MBYTE, _cur_pm * BYTE_2_MBYTE);
    std::printf("[Different]: %8.2fMb, [Different]: %8.2fMb\n",
                (_cur_vm-_start_vm) * BYTE_2_MBYTE, (_cur_pm-_start_pm) * BYTE_2_MBYTE);

    std::printf("------ CALLOC[calloc()] 256Mb ------\n");
    addr = std::calloc(1024*1024, 256);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[Virtual]:   %8.2fMb, [Resident]:  %8.2fMb\n",
                _cur_vm * BYTE_2_MBYTE, _cur_pm * BYTE_2_MBYTE);
    std::printf("[Different]: %8.2fMb, [Different]: %8.2fMb\n",
                (_cur_vm-_start_vm) * BYTE_2_MBYTE, (_cur_pm-_start_pm) * BYTE_2_MBYTE);
    std::printf("------ FREE[free()] 256Mb ------\n");
    std::free(addr);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[Virtual]:   %8.2fMb, [Resident]:  %8.2fMb\n",
                _cur_vm * BYTE_2_MBYTE, _cur_pm * BYTE_2_MBYTE);
    std::printf("[Different]: %8.2fMb, [Different]: %8.2fMb\n",
                (_cur_vm-_start_vm) * BYTE_2_MBYTE, (_cur_pm-_start_pm) * BYTE_2_MBYTE);
    return 0;
}

