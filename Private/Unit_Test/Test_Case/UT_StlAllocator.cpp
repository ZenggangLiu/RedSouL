// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (STL_ALLOCATOR_CHECKING == 1)
#include <vector>
#include "Core/Memory/RS_StlAllocator.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking StlAllocator", "[StlAllocator]")
{

    struct CheckDataType
    {
        UInt16 value[2];
    };

    const UInt8 ALLOCATE_MAGIC_NUMBER[]  = { 0xA0, 0x0B };
    const UInt8 CONSTRUCT_MAGIC_NUMBER[] = { 0xC0, 0x0B };


    SECTION("Checking StlAllocator:")
    {
        std::printf("--- Checking StlAllocator...\n");

        std::vector<CheckDataType, Core::StlAllocator<CheckDataType>> _array;
        _array.resize(1);
        REQUIRE((((const UInt8*)_array[0].value)[0] == CONSTRUCT_MAGIC_NUMBER[0] &&
                 ((const UInt8*)_array[0].value)[1] == CONSTRUCT_MAGIC_NUMBER[1]));

        Core::StlAllocator<CheckDataType> _allocator;
        CheckDataType * _instance = _allocator.allocate(1);
        REQUIRE((((const UInt8*)_instance->value)[0] == ALLOCATE_MAGIC_NUMBER[0] &&
                 ((const UInt8*)_instance->value)[1] == ALLOCATE_MAGIC_NUMBER[1]));

        _allocator.construct(_instance);
        REQUIRE((((const UInt8*)_instance->value)[0] == CONSTRUCT_MAGIC_NUMBER[0] &&
                 ((const UInt8*)_instance->value)[1] == CONSTRUCT_MAGIC_NUMBER[1]));
        _allocator.destroy(_instance);

        std::printf("--- Checking StlAllocator: OK!\n");
    }
} // TEST_CASE("Checking StlAllocator", "[StlAllocator]")

#endif // #if (STL_ALLOCATOR_CHECKING == 1)
