// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (XXTEA_CHECKING == 1)
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Encoding/RS_Codec.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking XXTEA encode/decode", "[XXTEA]")
{
    const UInt8 _key[] = { 0, 1, 2, 3, 4, 5 };
    const UInt8 _clear_txt[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    const UInt8 _encoded_txt[] =
    {
        0x50, 0x23, 0x8A, 0xAF,
        0x5C, 0xF7, 0x59, 0x71,
        0xCD, 0xA8, 0x1A, 0x12,
        0x56, 0xAD, 0x01, 0x1B
    };
    // 操作用的缓存
    UInt32 _buffer[sizeof(_clear_txt) >> 2];


    SECTION("Checking Encoding:")
    {
        std::printf("--- Checking XXTEA Encoding...\n");

        std::memcpy(_buffer, _clear_txt, sizeof(_buffer));
        XXTEA_ENCODE(_key, sizeof(_key), _buffer);
        for (UInt32 c = 0; c < sizeof(_buffer); ++c)
        {
            REQUIRE(((const UInt8*)_buffer)[c] == _encoded_txt[c]);
        }

        std::printf("--- Checking XXTEA Encoding: OK!\n");
    }

    SECTION("Checking Decoding:")
    {
        std::printf("--- Checking XXTEA Decoding...\n");

        std::memcpy(_buffer, _encoded_txt, sizeof(_buffer));
        XXTEA_DECODE(_key, sizeof(_key), _buffer);
        for (UInt32 c = 0; c < sizeof(_buffer); ++c)
        {
            REQUIRE(((const UInt8*)_buffer)[c] == _clear_txt[c]);
        }

        std::printf("--- Checking XXTEA Decoding: OK!\n");
    }
} // TEST_CASE("Checking XXTEA encode/decode", "[XXTEA]")

#endif // #if (XXTEA_CHECKING == 1)
