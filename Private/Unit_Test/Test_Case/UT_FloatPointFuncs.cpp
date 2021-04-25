// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (FLOATPOINT_FUNCS_CHECKING == 1)
#include "Core/Mathe/RS_FloatPointFuncs.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking FloatPointFuncs", "[FloatPointFuncs]")
{

    SECTION("Checking FloatPointFuncs:")
    {
        std::printf("--- Checking FloatPointFuncs...\n");

        // toUInt32/64
        const Real32 _one_32 = FloatPoint::fromUInt32(0x3F800000);
        REQUIRE(_one_32 == 1);
        const UInt32 _one_pattern = FloatPoint::toUInt32(_one_32);
        REQUIRE(_one_pattern == 0x3F800000);
        const Real64 _neg_zero = FloatPoint::fromUInt64(0x8000000000000000UL);
        REQUIRE(_neg_zero == -0);
        const UInt64 _neg_zero_pattern = FloatPoint::toUInt64(_neg_zero);
        REQUIRE(_neg_zero_pattern == 0x8000000000000000UL);

        // equals(32/64)
//        REQUIRE(FloatPoint::equals((Real32)ONE_PI, (Real32)(ONE_PI + EPSILON)));
//        Real32 a = (Real32)ONE_PI;
//        Real32 b = (Real32)ONE_PI + 2*(Real32)EPSILON;
//        REQUIRE(!FloatPoint::equals((Real32)ONE_PI, (Real32)ONE_PI + 2*(Real32)EPSILON));
//        REQUIRE(FloatPoint::equals((Real32)ONE_PI, (Real32)(ONE_PI - EPS
//                                                            ILON)));
//        REQUIRE(FloatPoint::equals((Real32)ONE_PI, (Real32)(ONE_PI - 2*EPSILON)));
//
//        REQUIRE(FloatPoint::equals(ONE_PI, ONE_PI + EPSILON));
//        REQUIRE(FloatPoint::equals(ONE_PI, ONE_PI - EPSILON));

        std::printf("--- Checking FloatPointFuncs: OK!\n");
    }
} // TEST_CASE("Checking FloatPointFuncs", "[FloatPointFuncs]")

#endif // #if (FLOATPOINT_FUNCS_CHECKING == 1)
