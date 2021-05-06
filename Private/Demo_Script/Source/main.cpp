#include <cstdio>
#include <memory>
#include "Core/Memory/RS_MemoryHelper.hpp"


int
main (
    int               argc,
    const char *const argv[])
{
    static UInt32 MEM_SIZE = 256*1024*1024;

    std::printf("------ STARTING... ------\n");
    Real32 _start_pm, _start_vm;
    Core::MemoryHelper::getMemoryUsage(_start_pm, _start_vm);
    std::printf("[VMem]: %8.2fMb, [PMem]: %8.2fMb\n", _start_vm, _start_pm);
    std::printf("------ MALLOC[malloc()] 256Mb ------\n");
    void * addr = std::malloc(MEM_SIZE);
    Real32 _cur_pm, _cur_vm;
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[VMem]: %8.2fMb, [PMem]: %8.2fMb\n", _cur_vm, _cur_pm);
    std::printf("[Diff]: %8.2fMb, [Diff]: %8.2fMb\n", _cur_vm-_start_vm, _cur_pm-_start_pm);
    std::printf("------ MEMSET[memset()] 256Mb ------\n");
    std::memset(addr, 0, MEM_SIZE);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[VMem]: %8.2fMb, [PMem]: %8.2fMb\n", _cur_vm, _cur_pm);
    std::printf("[Diff]: %8.2fMb, [Diff]: %8.2fMb\n", _cur_vm-_start_vm, _cur_pm-_start_pm);
    std::printf("------ FREE[free()] 256Mb ------\n");
    std::free(addr);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[VMem]: %8.2fMb, [PMem]: %8.2fMb\n", _cur_vm, _cur_pm);
    std::printf("[Diff]: %8.2fMb, [Diff]: %8.2fMb\n", _cur_vm-_start_vm, _cur_pm-_start_pm);

    std::printf("------ CALLOC[calloc()] 256Mb ------\n");
    addr = std::calloc(1024*1024, 256);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[VMem]: %8.2fMb, [PMem]: %8.2fMb\n", _cur_vm, _cur_pm);
    std::printf("[Diff]: %8.2fMb, [Diff]: %8.2fMb\n", _cur_vm-_start_vm, _cur_pm-_start_pm);
    std::printf("------ FREE[free()] 256Mb ------\n");
    std::free(addr);
    Core::MemoryHelper::getMemoryUsage(_cur_pm, _cur_vm);
    std::printf("[VMem]: %8.2fMb, [PMem]: %8.2fMb\n", _cur_vm, _cur_pm);
    std::printf("[Diff]: %8.2fMb, [Diff]: %8.2fMb\n", _cur_vm-_start_vm, _cur_pm-_start_pm);
    return 0;
}

