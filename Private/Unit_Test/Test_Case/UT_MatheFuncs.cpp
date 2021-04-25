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


    SECTION("Checking MatheFuncw:")
    {
        std::printf("--- Checking Murmur3 Hash...\n");



        std::printf("--- Checking Murmur3 Hash: OK!\n");
    }
} // TEST_CASE("Checking MatheFuncs", "[MatheFunc]")

#endif // #if (MATHE_FUNCS_CHECKING == 1)
