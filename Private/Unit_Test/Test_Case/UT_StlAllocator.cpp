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
        UInt32 value;
    };

    const UInt32 ALLOCATE_MAGIC_NUMBER  = FOUR_CC('A', 'L', 'O', 'C');
    const UInt32 CONSTRUCT_MAGIC_NUMBER = FOUR_CC('C', 'O', 'S', 'T');
    const UInt32 DESTRUCT_MAGIC_NUMBER  = FOUR_CC('D', 'E', 'S', 'T');

    SECTION("Checking StlAllocator:")
    {
        std::printf("--- Checking StlAllocator...\n");

        std::vector<CheckDataType, Core::StlAllocator<CheckDataType>> _array;
        _array.resize(1);
        REQUIRE((_array[0].value == CONSTRUCT_MAGIC_NUMBER));

        Core::StlAllocator<CheckDataType> _allocator;
        CheckDataType * _instance = _allocator.allocate(1);
        REQUIRE((_instance->value == ALLOCATE_MAGIC_NUMBER));

        _allocator.construct(_instance);
        REQUIRE((_instance->value == CONSTRUCT_MAGIC_NUMBER));

        _allocator.destroy(_instance);
        REQUIRE((_instance->value == DESTRUCT_MAGIC_NUMBER));

        std::printf("--- Checking StlAllocator: OK!\n");
    }
} // TEST_CASE("Checking StlAllocator", "[StlAllocator]")

#endif // #if (STL_ALLOCATOR_CHECKING == 1)
