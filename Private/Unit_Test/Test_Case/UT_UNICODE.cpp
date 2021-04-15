// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (UNICODE_CHECKING == 1)
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/String/RS_RuntimeText.hpp"
#include "Core/String/RS_TextHelper.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking Unicode", "[Unicode]")
{
    //      編: U+7DE8 : UTF8: E7 B7 A8     UTF16: 0x7DE8
    //      碼: U+78BC : UTF8: E7 A2 BC     UTF16: 0x78BC
    //      测: U+6D4B : UTF8: E6 B5 8B     UTF16: 0x6D4B
    //      试: U+8BD5 : UTF8: E8 AF 95     UTF16: 0x8BD5
    // 東風：🀀: u+1F000: UTF8: F0 9F 80 80  UTF16: 0xD83C 0xDC00
    // 發：  🀅: U+1F005: UTF8: F0 9F 80 85  UTF16: 0xD83C 0xDC05
    const UTF32 *const _bian     = U"編";
    const UTF32 *const _ma       = U"碼";
    const UTF32 *const _ce       = U"测";
    const UTF32 *const _shi      = U"试";
    const UTF32 *const _eastwind = U"🀀";
    const UTF32 *const _fa       = U"🀅";
    const UTF32 *const _all      = U"編碼测试🀀🀅";
    const UTF8         _all_utf8_exp[] =
    {
        0xE7, 0xB7, 0xA8,
        0xE7, 0xA2, 0xBC,
        0xE6, 0xB5, 0x8B,
        0xE8, 0xAF, 0x95,
        0xF0, 0x9F, 0x80, 0x80,
        0xF0, 0x9F, 0x80, 0x85
    };

    const RuntimeText _bian_str(_bian);
    const RuntimeText _ma_str(_ma);
    const RuntimeText _ce_str(_ce);
    const RuntimeText _shi_str(_shi);
    const RuntimeText _eastwind_str(_eastwind);
    const RuntimeText _fa_str(_fa);
    const RuntimeText _all_str(_all);

    const UTF8 *const _bian_utf8     = _bian_str.c_str();
    const UTF8 *const _ma_utf8       = _ma_str.c_str();
    const UTF8 *const _ce_utf8       = _ce_str.c_str();
    const UTF8 *const _shi_utf8      = _shi_str.c_str();
    const UTF8 *const _eastwind_utf8 = _eastwind_str.c_str();
    const UTF8 *const _fa_utf8       = _fa_str.c_str();
    const UTF8 *const _all_utf8      = _all_str.c_str();


    SECTION("Checking RuntimeText:")
    {
        std::printf("--- Checking RuntimeText...\n");

        REQUIRE((_bian_utf8[0] == 0xE7 && _bian_utf8[1] == 0xB7 && _bian_utf8[2] == 0xA8));
        REQUIRE((_ma_utf8[0]   == 0xE7 && _ma_utf8[1]   == 0xA2 && _ma_utf8[2]   == 0xBC));
        REQUIRE((_ce_utf8[0]   == 0xE6 && _ce_utf8[1]   == 0xB5 && _ce_utf8[2]   == 0x8B));
        REQUIRE((_shi_utf8[0]  == 0xE8 && _shi_utf8[1]  == 0xAF && _shi_utf8[2]  == 0x95));
        REQUIRE((_eastwind_utf8[0] == 0xF0 && _eastwind_utf8[1] == 0x9F &&
                 _eastwind_utf8[2] == 0x80 && _eastwind_utf8[3] == 0x80));
        REQUIRE((_fa_utf8[0] == 0xF0 && _fa_utf8[1] == 0x9F &&
                 _fa_utf8[2] == 0x80 && _fa_utf8[3] == 0x85));
        REQUIRE(_all_str.size() == ARRAY_ITEM_COUNT(_all_utf8_exp));
        for (UInt8 idx = 0; idx < (UInt8)_all_str.size(); ++idx)
        {
            REQUIRE(_all_utf8[idx] == _all_utf8_exp[idx]);
        }

        std::printf("--- Checking RuntimeText: OK!\n");
    }


    SECTION("Checking UTF8 --> UTF16 Encoding:")
    {
        std::printf("--- Checking UTF8 --> UTF16 Encoding...\n");

        UTF16 _bian_utf16 = 0;
        REQUIRE(TextHelper::getUTF16TextLength(_bian_utf8) == 1);
        TextHelper::convertUTF8ToUTF16(_bian_utf8, &_bian_utf16);
        REQUIRE(_bian_utf16 == 0x7DE8);

        UTF16 _ma_utf16 = 0;
        REQUIRE(TextHelper::getUTF16TextLength(_ma_utf8) == 1);
        TextHelper::convertUTF8ToUTF16(_ma_utf8, &_ma_utf16);
        REQUIRE(_ma_utf16 == 0x78BC);

        UTF16 _ce_utf16 = 0;
        REQUIRE(TextHelper::getUTF16TextLength(_ce_utf8) == 1);
        TextHelper::convertUTF8ToUTF16(_ce_utf8, &_ce_utf16);
        REQUIRE(_ce_utf16 == 0x6D4B);

        UTF16 _shi_utf16 = 0;
        REQUIRE(TextHelper::getUTF16TextLength(_shi_utf8) == 1);
        TextHelper::convertUTF8ToUTF16(_shi_utf8, &_shi_utf16);
        REQUIRE(_shi_utf16 == 0x8BD5);

        UTF16 _eastwind_utf16[2] = {0};
        REQUIRE(TextHelper::getUTF16TextLength(_eastwind_utf8) == 2);
        TextHelper::convertUTF8ToUTF16(_eastwind_utf8, _eastwind_utf16);
        REQUIRE((_eastwind_utf16[0] == 0xD83C && _eastwind_utf16[1] == 0xDC00));

        UTF16 _fa_utf16[2] = {0};
        REQUIRE(TextHelper::getUTF16TextLength(_fa_utf8) == 2);
        TextHelper::convertUTF8ToUTF16(_fa_utf8, _fa_utf16);
        REQUIRE((_fa_utf16[0] == 0xD83C && _fa_utf16[1] == 0xDC05));

        std::printf("--- Checking UTF8 --> UTF16 Encoding: OK!\n");
    }

} // TEST_CASE("Checking Unicode", "[Unicode]")

#endif // #if (UNICODE_CHECKING == 1)
