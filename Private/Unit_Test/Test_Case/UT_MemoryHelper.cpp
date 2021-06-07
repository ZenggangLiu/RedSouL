// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (MEMORY_HELPER_CHECKING == 1)
#include <cstring>              // std::memcmp
#include <stdlib.h>             // nrand48
#include "Core/Memory/RS_MemoryHelper.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking MemoryHelper", "[MemoryHelper]")
{

    struct MirroredMemeryChecker
    {
        static
        void
        check (
           const UInt32 alloc_size,
           const UInt32 mirror_count)
        {
            void * _alloc_addr;
            UInt32 _alloc_size;
            std::tie(_alloc_addr, _alloc_size) = MemoryHelper::allocMirroredVMem(alloc_size,
                                                                                 mirror_count);

            unsigned short seed[3] = { 0 };
            char *const _data_addr = (char*)_alloc_addr;
            // 检查对主内存的写入操作是否所有镜像都可见
            for(size_t i = 0; i < alloc_size; i++)
            {
                _data_addr[i] = nrand48(seed);
            }
            for(unsigned _mirror_idx = 0; _mirror_idx < mirror_count; ++_mirror_idx)
            {
                REQUIRE(std::memcmp(_data_addr, _data_addr + alloc_size*(_mirror_idx+1), alloc_size) == 0);
            }

            // 检查是否在镜像内存的写入操作主内存可见
            for(unsigned _mirror_idx = 0; _mirror_idx < mirror_count; ++_mirror_idx)
            {
                for(size_t i = 0; i < alloc_size; i++)
                {
                    _data_addr[alloc_size * (_mirror_idx+1) + i] = nrand48(seed);
                }
                REQUIRE(std::memcmp(_data_addr, _data_addr + alloc_size*(_mirror_idx+1), alloc_size) == 0);
            }

            MemoryHelper::releaseMirroredVMem(_alloc_addr, _alloc_size, mirror_count);
        }
    };


    SECTION("Checking MemoryHelper:")
    {
        std::printf("--- Checking MemoryHelper...\n");

        Bool result;
        UInt64 rmem_s = 0, vmem_s = 0;
        UInt64 rmem_c = 0, vmem_c = 0;
        std::tie(result, rmem_s, vmem_s) = MemoryHelper::getMemoryUsage();
        printf("[Start]: [RMem]: %6lld, [VMem]: %6lld\n", rmem_s, vmem_s);

        for(UInt32 _mirror_count = 1; _mirror_count < 10; ++_mirror_count)
        {
            MirroredMemeryChecker::check(MemoryHelper::getPageSize(),       _mirror_count);
            MirroredMemeryChecker::check(MemoryHelper::getPageSize() *   2, _mirror_count);
            MirroredMemeryChecker::check(MemoryHelper::getPageSize() *  10, _mirror_count);
            MirroredMemeryChecker::check(MemoryHelper::getPageSize() * 100, _mirror_count);
        }

        std::tie(result, rmem_c, vmem_c) = MemoryHelper::getMemoryUsage();
        printf("[End]:   [RMem]: %6lld, [VMem]: %6lld\n", rmem_c, vmem_c);
        printf("[Used]:  [RMem]: %6lld, [VMem]: %6lld\n",
               (SInt64)rmem_c - (SInt64)rmem_s, (SInt64)vmem_c - (SInt64)vmem_s);

        std::printf("--- Checking MemoryHelper: OK!\n");
    }
} // TEST_CASE("Checking MemoryHelper", "[MemoryHelper]")

#endif // #if (MEMORY_HELPER_CHECKING == 1)
