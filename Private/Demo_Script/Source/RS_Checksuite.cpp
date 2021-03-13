# if (RED_SOUL_CHECKING == 1)

#define FILE_READ_WRITE_STREAM_CHECKING     0
#define HASH_CHECKING                       0
#define LOGGING_CHECKING                    1
#define TEXT_FILE_READ_WRITE_CHECKING       0
#define UTF8_CHECKING                       0



#include "Core/Assert/RS_RuntimeAssert.hpp"
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Logging/RS_Logging.hpp"
#include "Core/String/RS_TextHelper.hpp"



# if (FILE_READ_WRITE_STREAM_CHECKING == 1)
#include "Core/FileSystem/RS_FileStream.hpp"
#include "Core/FileSystem/RS_FileReadStream.hpp"
#include "Core/FileSystem/RS_FileWriteStream.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"
#endif

#if (HASH_CHECKING == 1)
#include "Core/Hash/RS_RuntimeHash.hpp"
#endif

# if (LOGGING_CHECKING == 1)
#include "Core/Logging/RS_Logging.hpp"
#endif

#if (TEXT_FILE_READ_WRITE_CHECKING == 1)
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/DataOperation/RS_EndiannessHelper.hpp"
#include "Core/FileSystem/RS_FileWriteStream.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"
#include "Core/FileSystem/RS_TextFileReader.hpp"
#include "Core/FileSystem/RS_TextFileWriter.hpp"
#include "Core/String/RS_UTF16Text.hpp"
#endif

# if (UTF8_CHECKING == 1)
#include "Core/String/RS_RuntimeText.hpp"
#endif



void Check()
{
    using namespace Core;

    //    const UInt16 hex = TextHelper::getByteHexForm(0xAB, false);
    //    UInt8 array[] = {0xAB, 0xCD, 0xEF, 0x11, 0x22, 0x41, 0x42};
    //    RuntimeText _dump;
    //    TextHelper::dumpMemInHexForm((UInt8*)array, sizeof(array), 4, true, _dump);
    //    const UTF8* text = _dump.c_str();


    // Checking FileReadStream & FileWriteStream
#if (FILE_READ_WRITE_STREAM_CHECKING == 1)
    {
        std::printf("[FILE_READ_WRITE_STREAM_CHECKING] START...\n");

        // 输出各种类型数据
        const SInt8  si8data = -127;
        const UInt8  ui8data = 127;
        const SInt16 si16data = -640;
        const UInt16 ui16data = 640;
        const SInt32 si32data = -1640;
        const UInt32 ui32data = 1640;
        const SInt64 si64data = -2640;
        const UInt64 ui64data = 2640;
        const Real32 r32data = 2.34;
        const Real64 r64data = 4.56;
        const ASCII *const text = "Test Text";
        FileWriteStream _write_io;
        NativeFileSystem::openFileWrite(
            (const UTF8*)"test.dat", NativeFileSystem::SEARCH_PATH_TEMPORARY_FOLDER, _write_io);
        _write_io << si8data << ui8data;
        _write_io << si16data << ui16data;
        _write_io << si32data << ui32data;
        _write_io << si64data << ui64data;
        _write_io << r32data << r64data;
        _write_io << text;
        _write_io.close();

        // 输入各种类型数据
        SInt8  _si8data;
        UInt8  _ui8data;
        SInt16 _si16data;
        UInt16 _ui16data;
        SInt32 _si32data;
        UInt32 _ui32data;
        SInt64 _si64data;
        UInt64 _ui64data;
        Real32 _r32data;
        Real64 _r64data;
        FileReadStream  _read_io;
        NativeFileSystem::openFileRead(
            (const UTF8*)"test.dat", NativeFileSystem::SEARCH_PATH_TEMPORARY_FOLDER, _read_io);
        _read_io >> _si8data;
        _read_io >> _ui8data;
        _read_io >> _si16data;
        _read_io >> _ui16data;
        _read_io >> _si32data;
        _read_io >> _ui32data;
        _read_io >> _si64data;
        _read_io >> _ui64data;
        _read_io >> _r32data;
        _read_io >> _r64data;

        // 检查数据
        RUNTIME_ASSERT(si8data  == _si8data,  "Wrong Data");
        RUNTIME_ASSERT(ui8data  == _ui8data,  "Wrong Data");
        RUNTIME_ASSERT(si16data == _si16data, "Wrong Data");
        RUNTIME_ASSERT(ui16data == _ui16data, "Wrong Data");
        RUNTIME_ASSERT(si32data == _si32data, "Wrong Data");
        RUNTIME_ASSERT(ui32data == _ui32data, "Wrong Data");
        RUNTIME_ASSERT(si64data == _si64data, "Wrong Data");
        RUNTIME_ASSERT(ui64data == _ui64data, "Wrong Data");
        RUNTIME_ASSERT(r32data  == _r32data,  "Wrong Data");
        RUNTIME_ASSERT(r64data  == _r64data,  "Wrong Data");

        std::printf("[FILE_READ_WRITE_STREAM_CHECKING] OK\n\n");
    }
#endif


    // Check Hash
#if (HASH_CHECKING == 1)
    {
        std::printf("[HASH_CHECKING] START...\n");

        const UInt32 seed = FOUR_CC('S', 'e', 'e', 'D');
        const UInt32 compiletime_hash = COMPILE_TIME_HASH(
            seed, "INFO(TestFlag, \"WDR: %s\", NativeFileSystem::getWorkingDir());");
        const UInt32 runtime_kash     = hash32(
            seed, "INFO(TestFlag, \"WDR: %s\", NativeFileSystem::getWorkingDir());");
        RUNTIME_ASSERT(compiletime_hash == runtime_kash, "No matching");

        std::printf("[HASH_CHECKING] OK\n\n");
    }
#endif


    // Check Logging
#if (LOGGING_CHECKING == 1)
    {
        std::printf("[LOGGING_CHECKING] START...\n");

        INFO(TestFlag,
             "[Checking Info]: Checking logging of Info");
        WARNING(TestFlag,
                "[Checking Warning]: Checking logging of Warning");
        FAILURE(TestFlag,
                "[Checking Failure]: Checking logging of Failure: %s",
                "this is a failure");
        TODO("[Checking]: We have to do this");

        std::printf("[LOGGING_CHECKING] OK\n\n");
    }
#endif


    // Checking text file reader/writer
#if (TEXT_FILE_READ_WRITE_CHECKING == 1)
    std::printf("[TEXT_FILE_READ_WRITE_CHECKING] START...\n");

    // a, B, 1
    // 編: U+7DE8 : UTF8: E7 B7 A8     UTF16: 7D E8
    // 碼: U+78BC : UTF8: E7 A2 BC     UTF16: 78 BC
    // 测: U+6D4B : UTF8: E6 B5 8B     UTF16: 6D 4B
    // 试: U+8BD5 : UTF8: E8 AF 95     UTF16: 8B D5
    const UTF8 _all_utf8_codes[] =
    {
        'a',  'B',  '1',
        0xE7, 0xB7, 0xA8,
        0xE7, 0xA2, 0xBC,
        0xE6, 0xB5, 0x8B,
        0xE8, 0xAF, 0x95
    };
    const UTF16 _all_utf16_codes[] =
    {
        u'a', u'B', u'1', u'編', u'碼', u'测', u'试'
    };

    // BOM(U+FEFF): 编码为UTF8为 EF BB BF
    const UTF8 _utf8_BOM[] =
    {
        0xEF, 0xBB, 0xBF
    };
    const UTF16 _utf16_BOM = 0xFEFF;

    // 检查测试文件是否存在: Document/Text_file_UTF8.txt
    if (!NativeFileSystem::existsFile(
           (const UTF8*)"Text_file_UTF8.txt", NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER))
    {
        FileWriteStream _file_io;
        const Bool _file_op = NativeFileSystem::openFileWrite(
            (const UTF8*)"Text_file_UTF8.txt",
            NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
            _file_io);
        RUNTIME_ASSERT(_file_op, "Can NOT write to Text_file_UTF8.txt");
        // 输出文字
        _file_io.write(
            (const UInt8*)_all_utf8_codes, sizeof(_all_utf8_codes), 0, sizeof(_all_utf8_codes));
        _file_io.close();
    }
    // 检查测试文件是否存在: Document/Text_file_UTF8_BOM.txt
    if (!NativeFileSystem::existsFile(
           (const UTF8*)"Text_file_UTF8_BOM.txt", NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER))
    {
        FileWriteStream _file_io;
        const Bool _file_op = NativeFileSystem::openFileWrite(
            (const UTF8*)"Text_file_UTF8_BOM.txt",
            NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
            _file_io);
        RUNTIME_ASSERT(_file_op, "Can NOT write to Text_file_UTF8_BOM.txt");
        // 输出BOM
        _file_io.write((const UInt8*)_utf8_BOM, sizeof(_utf8_BOM), 0, sizeof(_utf8_BOM));
        // 输出文字
        _file_io.write((
            const UInt8*)_all_utf8_codes, sizeof(_all_utf8_codes), 0, sizeof(_all_utf8_codes));
        _file_io.close();
    }
    // 检查测试文件是否存在: Document/Text_file_UTF16_LE.txt
    if (!NativeFileSystem::existsFile(
           (const UTF8*)"Text_file_UTF16_LE.txt", NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER))
    {
        FileWriteStream _file_io;
        const Bool _file_op = NativeFileSystem::openFileWrite(
            (const UTF8*)"Text_file_UTF16_LE.txt",
            NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
            _file_io);
        RUNTIME_ASSERT(_file_op, "Can NOT write to Text_file_UTF16_LE.txt");
        // 输出BOM
        _file_io.write((const UInt8*)&_utf16_BOM, sizeof(_utf16_BOM), 0, sizeof(_utf16_BOM));
        // 输出文字
        _file_io.write((
            const UInt8*)_all_utf16_codes, sizeof(_all_utf16_codes), 0, sizeof(_all_utf16_codes));
        _file_io.close();
    }
    // 检查测试文件是否存在: Document/Text_file_UTF16_BE.txt
    if (!NativeFileSystem::existsFile(
           (const UTF8*)"Text_file_UTF16_BE.txt", NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER))
    {
        FileWriteStream _file_io;
        const Bool _file_op = NativeFileSystem::openFileWrite(
            (const UTF8*)"Text_file_UTF16_BE.txt",
            NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
            _file_io);
        RUNTIME_ASSERT(_file_op, "Can NOT write to Text_file_UTF16_BE.txt");
        UInt16 _swap_data;
        // 输出BOM
        _swap_data = EndianHelper::swapEndian((UInt16)_utf16_BOM);
        _file_io.write((const UInt8*)&_swap_data, sizeof(_swap_data), 0, sizeof(_swap_data));
        // 输出文字
        for (UInt8 _idx = 0; _idx < ARRAY_ITEM_COUNT(_all_utf16_codes); ++_idx)
        {
            _swap_data = EndianHelper::swapEndian((UInt16)_all_utf16_codes[_idx]);
            _file_io.write((const UInt8*)&_swap_data, sizeof(_swap_data), 0, sizeof(_swap_data));
        }
        _file_io.close();
    }


    // 检查Document/Text_file_UTF8.txt
    {
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8.txt");
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF8 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf8_codes[_idx], _utf8_content[_idx], _idx);
                RUNTIME_ASSERT(_utf8_content[_idx] == _all_utf8_codes[_idx], _buffer);
            }
        }
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF16 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf16_codes[_idx], _utf16_content[_idx], _idx);
                RUNTIME_ASSERT(_utf16_content[_idx] == _all_utf16_codes[_idx], _buffer);
            }
        }
    }
    // 检查Document/Text_file_UTF8_BOM.txt
    {
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_BOM.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_BOM.txt");
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF8 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf8_codes[_idx], _utf8_content[_idx], _idx);
                RUNTIME_ASSERT(_utf8_content[_idx] == _all_utf8_codes[_idx], _buffer);
            }
        }
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_BOM.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_BOM.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF16 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf16_codes[_idx], _utf16_content[_idx], _idx);
                RUNTIME_ASSERT(_utf16_content[_idx] == _all_utf16_codes[_idx], _buffer);
            }
        }
    }
    // 检查Document/Text_file_UTF16_LE.txt
    {
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF16_LE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF16_LE.txt");
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF8 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf8_codes[_idx], _utf8_content[_idx], _idx);
                RUNTIME_ASSERT(_utf8_content[_idx] == _all_utf8_codes[_idx], _buffer);
            }
        }
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF16_LE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);

            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF16_LE.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF16 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf16_codes[_idx], _utf16_content[_idx], _idx);
                RUNTIME_ASSERT(_utf16_content[_idx] == _all_utf16_codes[_idx], _buffer);
            }
        }
    }
    // 检查Document/Text_file_UTF16_BE.txt
    {
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF16_BE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF16_BE.txt");
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF8 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf8_codes[_idx], _utf8_content[_idx], _idx);
                RUNTIME_ASSERT(_utf8_content[_idx] == _all_utf8_codes[_idx], _buffer);
            }
        }
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF16_BE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);

            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF16_BE.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                UTF8 _buffer[128];
                TextHelper::sprintfSafe(_buffer, sizeof(_buffer),
                                        "NO matching UTF16 content: 0x%X(Exp) != 0x%X(Read)[%d]",
                                        _all_utf16_codes[_idx], _utf16_content[_idx], _idx);
                RUNTIME_ASSERT(_utf16_content[_idx] == _all_utf16_codes[_idx], _buffer);
            }
        }
    }

    std::printf("[TEXT_FILE_READ_WRITE_CHECKING] OK\n\n");
#endif


    // UTF8编码测试
#if (UTF8_CHECKING == 1)
    std::printf("[UTF8_CHECKING] START...\n");

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

        RUNTIME_ASSERT(TextHelper::getUTF16TextLength(_bian_utf8) == 1,
                       "Wrong UTF16 code length for Bian");
        UTF16 _bian_utf16 = 0;
        TextHelper::convertUTF8ToUTF16(_bian_utf8, &_bian_utf16);
        RUNTIME_ASSERT(_bian_utf16 == 0x7DE8,
                       "Wrong UTF16 encoding for Bian");
        RUNTIME_ASSERT(TextHelper::getUTF16TextLength(_ma_utf8) == 1,
                       "Wrong UTF16 code length for Ma");
        UTF16 _ma_utf16 = 0;
        TextHelper::convertUTF8ToUTF16(_ma_utf8, &_ma_utf16);
        RUNTIME_ASSERT(_ma_utf16 == 0x78BC,
                       "Wrong UTF16 encoding for Ma");
        RUNTIME_ASSERT(TextHelper::getUTF16TextLength(_ce_utf8) == 1,
                       "Wrong UTF16 code length for Ce");
        UTF16 _ce_utf16 = 0;
        TextHelper::convertUTF8ToUTF16(_ce_utf8, &_ce_utf16);
        RUNTIME_ASSERT(_ce_utf16 == 0x6D4B,
                       "Wrong UTF16 encoding for Ce");
        RUNTIME_ASSERT(TextHelper::getUTF16TextLength(_shi_utf8) == 1,
                       "Wrong UTF16 code length for Shi");
        UTF16 _shi_utf16 = 0;
        TextHelper::convertUTF8ToUTF16(_shi_utf8, &_shi_utf16);
        RUNTIME_ASSERT(_shi_utf16 == 0x8BD5,
                       "Wrong UTF16 encoding for Shi");
        RUNTIME_ASSERT(TextHelper::getUTF16TextLength(_eastwind_utf8) == 2,
                       "Wrong UTF16 code length for EastWind");
        UTF16 _eastwind_utf16[2] = {0};
        TextHelper::convertUTF8ToUTF16(_eastwind_utf8, _eastwind_utf16);
        RUNTIME_ASSERT(_eastwind_utf16[0] == 0xD83C && _eastwind_utf16[1] == 0xDC00,
                       "Wrong UTF16 encoding for EastWind");
        RUNTIME_ASSERT(TextHelper::getUTF16TextLength(_fa_utf8) == 2,
                       "Wrong UTF16 code length for Fa");
        UTF16 _fa_utf16[2] = {0};
        TextHelper::convertUTF8ToUTF16(_fa_utf8, _fa_utf16);
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

        std::printf("[UTF8_CHECKING] OK\n\n");
    }
#endif



}


#endif // # if (RED_SOUL_CHECKING == 1)
