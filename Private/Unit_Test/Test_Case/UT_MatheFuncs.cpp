// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (MATHE_FUNCS_CHECKING == 1)
#include "Core/Mathe/RS_MatheDefs.hpp"
#include "Core/Mathe/RS_MatheFuncs.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking MatheFuncs", "[MatheFunc]")
{


    SECTION("Checking MatheFunc:")
    {
        std::printf("--- Checking MatheFunc...\n");

        const UInt32 _mini = 123;
        const UInt32 _maxi = 567;
        const Real32 _minr = 2.777f;
        const Real32 _maxr = 3.1425f;
        REQUIRE(Mathe::minimum(_mini, _maxi) == _mini);
        REQUIRE(Mathe::minimum(_minr, _maxr) == _minr);

        std::printf("--- Checking MatheFunc: OK!\n");
    }
} // TEST_CASE("Checking MatheFuncs", "[MatheFunc]")

#endif // #if (MATHE_FUNCS_CHECKING == 1)
