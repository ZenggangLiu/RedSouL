# if (RED_SOUL_CHECKING == 1)


#include "Core/Assert/RS_RuntimeAssert.hpp"
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Hash/RS_RuntimeHash.hpp"
#include "Core/Logging/RS_Logging.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"
#include "Core/String/RS_RuntimeText.hpp"


#define HASH_CHECKING       0
#define UTF8_CHECKING       1
#define LOGGING_CHECKING    0


using namespace Core;

void Check()
{
    // Check Hash
#if (HASH_CHECKING == 1)
    {
        const UInt32 seed = FOUR_CC('S', 'e', 'e', 'D');
        const UInt32 compiletime_hash = COMPILE_TIME_HASH(seed, "INFO(TestFlag, \"WDR: %s\", Core::NativeFileSystem::getCurrentWorkingDir());");
        const UInt32 runtime_kash     = Core::hash32(seed, "INFO(TestFlag, \"WDR: %s\", Core::NativeFileSystem::getCurrentWorkingDir());");
        RUNTIME_ASSERT(compiletime_hash == runtime_kash, "No matching");
    }
#endif

    // UTF8编码测试
#if (UTF8_CHECKING == 1)
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
        const UTF8 _all_utf8_exp[]   =
        {
            0xE7, 0xB7, 0xA8,
            0xE7, 0xA2, 0xBC,
            0xE6, 0xB5, 0x8B,
            0xE8, 0xAF, 0x95,
            0xF0, 0x9F, 0x80, 0x80,
            0xF0, 0x9F, 0x80, 0x85
        };

        const Core::RuntimeText _bian_str(_bian);
        const Core::RuntimeText _ma_str(_ma);
        const Core::RuntimeText _ce_str(_ce);
        const Core::RuntimeText _shi_str(_shi);
        const Core::RuntimeText _eastwind_str(_eastwind);
        const Core::RuntimeText _fa_str(_fa);
        const Core::RuntimeText _all_str(_all);

        const UTF8 *const _bian_utf8     = _bian_str.c_str();
        const UTF8 *const _ma_utf8       = _ma_str.c_str();
        const UTF8 *const _ce_utf8       = _ce_str.c_str();
        const UTF8 *const _shi_utf8      = _shi_str.c_str();
        const UTF8 *const _eastwind_utf8 = _eastwind_str.c_str();
        const UTF8 *const _fa_utf8       = _fa_str.c_str();
        const UTF8 *const _all_utf8      = _all_str.c_str();

        RUNTIME_ASSERT(_bian_utf8[0] == 0xE7 && _bian_utf8[1] == 0xB7 && _bian_utf8[2] == 0xA8,
                       "Wrong UTF8 encoding for Bian");
        RUNTIME_ASSERT(_ma_utf8[0] == 0xE7 && _ma_utf8[1] == 0xA2 && _ma_utf8[2] == 0xBC,
                       "Wrong UTF8 encoding for Ma");
        RUNTIME_ASSERT(_ce_utf8[0] == 0xE6 && _ce_utf8[1] == 0xB5 && _ce_utf8[2] == 0x8B,
                       "Wrong UTF8 encoding for Ce");
        RUNTIME_ASSERT(_shi_utf8[0] == 0xE8 && _shi_utf8[1] == 0xAF && _shi_utf8[2] == 0x95,
                       "Wrong UTF8 encoding for Shi");
        RUNTIME_ASSERT(_eastwind_utf8[0] == 0xF0 && _eastwind_utf8[1] == 0x9F &&
                       _eastwind_utf8[2] == 0x80 && _eastwind_utf8[3] == 0x80,
                       "Wrong UTF8 encoding for EastWind");
        RUNTIME_ASSERT(_fa_utf8[0] == 0xF0 && _fa_utf8[1] == 0x9F &&
                       _fa_utf8[2] == 0x80 && _fa_utf8[3] == 0x85,
                       "Wrong UTF8 encoding for Fa");

        RUNTIME_ASSERT(Core::TextHelper::getUTF16TextLen(_bian_utf8) == 1,
                       "Wrong UTF16 code length for Bian");
        UTF16 _bian_utf16 = 0;
        Core::TextHelper::convertUTF8ToUTF16(_bian_utf8, &_bian_utf16);
        RUNTIME_ASSERT(_bian_utf16 == 0x7DE8,
                       "Wrong UTF16 encoding for Bian");
        RUNTIME_ASSERT(Core::TextHelper::getUTF16TextLen(_ma_utf8) == 1,
                       "Wrong UTF16 code length for Ma");
        UTF16 _ma_utf16 = 0;
        Core::TextHelper::convertUTF8ToUTF16(_ma_utf8, &_ma_utf16);
        RUNTIME_ASSERT(_ma_utf16 == 0x78BC,
                       "Wrong UTF16 encoding for Ma");
        RUNTIME_ASSERT(Core::TextHelper::getUTF16TextLen(_ce_utf8) == 1,
                       "Wrong UTF16 code length for Ce");
        UTF16 _ce_utf16 = 0;
        Core::TextHelper::convertUTF8ToUTF16(_ce_utf8, &_ce_utf16);
        RUNTIME_ASSERT(_ce_utf16 == 0x6D4B,
                       "Wrong UTF16 encoding for Ce");
        RUNTIME_ASSERT(Core::TextHelper::getUTF16TextLen(_shi_utf8) == 1,
                       "Wrong UTF16 code length for Shi");
        UTF16 _shi_utf16 = 0;
        Core::TextHelper::convertUTF8ToUTF16(_shi_utf8, &_shi_utf16);
        RUNTIME_ASSERT(_shi_utf16 == 0x8BD5,
                       "Wrong UTF16 encoding for Shi");
        RUNTIME_ASSERT(Core::TextHelper::getUTF16TextLen(_eastwind_utf8) == 2,
                       "Wrong UTF16 code length for EastWind");
        UTF16 _eastwind_utf16[2] = {0};
        Core::TextHelper::convertUTF8ToUTF16(_eastwind_utf8, _eastwind_utf16);
        RUNTIME_ASSERT(_eastwind_utf16[0] == 0xD83C && _eastwind_utf16[1] == 0xDC00,
                       "Wrong UTF16 encoding for EastWind");
        RUNTIME_ASSERT(Core::TextHelper::getUTF16TextLen(_fa_utf8) == 2,
                       "Wrong UTF16 code length for Fa");
        UTF16 _fa_utf16[2] = {0};
        Core::TextHelper::convertUTF8ToUTF16(_fa_utf8, _fa_utf16);
        RUNTIME_ASSERT(_fa_utf16[0] == 0xD83C && _fa_utf16[1] == 0xDC05,
                       "Wrong UTF16 encoding for Fa");

        RUNTIME_ASSERT(_all_str.size() == ARRAY_ITEM_COUNT(_all_utf8_exp),
                       "Wring RuntimeText's size");
        for (UInt8 idx = 0; idx < (UInt8)_all_str.size(); ++idx)
        {
            if (_all_utf8[idx] != _all_utf8_exp[idx])
            {
                RUNTIME_ASSERT(false, "Wrong UTF8 encoding for All");
            }
        }
    }
#endif


    //    const UInt16 hex = Core::TextHelper::getByteHexForm(0xAB, false);
    //    UInt8 array[] = {0xAB, 0xCD, 0xEF, 0x11, 0x22, 0x41, 0x42};
    //    Core::RuntimeText _dump;
    //    Core::TextHelper::dumpMemInHexForm((UInt8*)array, sizeof(array), 4, true, _dump);
    //    const UTF8* text = _dump.c_str();

    // Check Logging
#if (LOGGING_CHECKING == 1)
    {
        INFO(TestFlag, "WDR: %s", Core::NativeFileSystem::getCurrentWorkingDir());
        WARNING(TestFlag, "Checking logging of Warning");
        FAILURE(TestFlag, "Checking logging of Failure: %s", "this is a failure");
        TODO("We have to do this");
    }
#endif
}


#endif // # if (RED_SOUL_CHECKING == 1)
