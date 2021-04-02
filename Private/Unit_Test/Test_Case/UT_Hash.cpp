// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (HASH_CHECKING == 1)
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Hash/RS_CompiletimeHash.hpp"
#include "Core/Hash/RS_RuntimeHash.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking Compiletime/Runtime Murmur3 Hash", "[Murmur3 Hash]")
{
    // Hash的Seed
    const UInt32 seed = FOUR_CC('S', 'e', 'e', 'D');


    SECTION("Checking Hash:")
    {
        const UInt32 compiletime_hash = COMPILE_TIME_HASH(
            seed, "INFO(TestFlag, \"WDR: %s\", NativeFileSystem::getWorkingDir());");
        const UInt32 runtime_hash     = hash32(
            seed, "INFO(TestFlag, \"WDR: %s\", NativeFileSystem::getWorkingDir());");
        REQUIRE(compiletime_hash == runtime_hash);
    }
} // TEST_CASE("Checking Compile/Runtime Murmur3 Hash", "[Murmur3 Hash]")

#endif // #if (HASH_CHECKING == 1)
