// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (BIT_OPERATION_CHECKING == 1)
#include "Core/DataOperation/RS_BitOperationHelper.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking Bit Operation", "[Bit Operation]")
{
    SECTION("Checking Bit Operation:")
    {
        std::printf("--- Checking Bit Operation...\n");

        // makeBitsSetRange()
        // 单个位测试：0到63位其中一位为1
        for (UInt8 _idx = 0; _idx < 63; ++_idx)
        {
            const UInt64 _pattern = BitOperationHelper::makeBitsSetRange(_idx, _idx);
            REQUIRE(_pattern == 1ul << _idx);
        }
        // 区域测试
        // 全部都是1
        UInt64 _pattern = BitOperationHelper::makeBitsSetRange(0, 63);
        REQUIRE(_pattern == 0xFFFFFFFFFFFFFFFFUL);
        _pattern = BitOperationHelper::makeBitsSetRange(0, 15);
        REQUIRE(_pattern == 0xFFFFUL);
        _pattern = BitOperationHelper::makeBitsSetRange(4, 63);
        REQUIRE(_pattern == 0xFFFFFFFFFFFFFFF0UL);
        _pattern = BitOperationHelper::makeBitsSetRange(8, 63);
        REQUIRE(_pattern == 0xFFFFFFFFFFFFFF00UL);
        _pattern = BitOperationHelper::makeBitsSetRange(24, 47);
        REQUIRE(_pattern == 0b0000000000000000111111111111111111111111000000000000000000000000UL);

        // isBitSet32/64()
        REQUIRE(BitOperationHelper::isBitSet32(0b00000000000000010000000000000000, 16));
        REQUIRE(BitOperationHelper::isBitSet32(0b00000000100000010000000000000000, 23));
        REQUIRE(BitOperationHelper::isBitSet64(
            0b0000000000000000111111111111111111111111000000000000000000000000UL, 24));

        // findFSBitPos32/64()
        REQUIRE(BitOperationHelper::findFSBitPos32(0b00000000100000010000000000000000) == 16);
        REQUIRE(BitOperationHelper::findFSBitPos64(
            0b0000000000000000111111111111111111111111000000000000000000000000UL) == 24);

        // findLSBitPos32/64()
        REQUIRE(BitOperationHelper::findLSBitPos32(0b00000000100000010000000000000000) == 23);
        REQUIRE(BitOperationHelper::findLSBitPos64(
            0b0000000000000000111111111111111111111111000000000000000000000000UL) == 47);


        std::printf("--- Checking Bit Operation: OK!\n");
    }
} // TEST_CASE("Checking Bit Operation", "[Bit Operation]")

#endif // #if (BIT_OPERATION_CHECKING == 1)
