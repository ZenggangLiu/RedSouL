// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (TEXT_FILE_READ_WRITE_CHECKING == 1)
#include <vector>
#include "Core/DataOperation/RS_EndiannessHelper.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"
#include "Core/FileSystem/RS_TextFileReader.hpp"
#include "Core/FileSystem/RS_TextFileWriter.hpp"
#include "Core/String/RS_UTF16Text.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking TextFileRead/WriteStream", "[TextFileRead/WriteStream]")
{
    const SInt8  _pos8   =  123;
    const SInt8  _neg8   = -123;
    const UInt8  _uint8  =  253;
    const SInt16 _pos16  =  4096;
    const SInt16 _neg16  = -9217;
    const UInt16 _uint16 = 65535;
    // TODO: 需要实现！！
    // const Real16 _half = 11;
    const Real32 _real32 = 1.2345478f;
    const Real64 _real64 = 88.77665544332211;

    const ASCII* const _space        = " ";
    const ASCII* const _feed         = "\n";
    const ASCII* const _pos8_str     = "123";
    const ASCII* const _neg8_str     = "-123";
    const ASCII* const _uint8_str    = "253";
    const ASCII* const _pos16_str    = "4096";
    const ASCII* const _neg16_str    = "-9217";
    const ASCII* const _uint16_str   = "65535";
    // TODO: 需要实现！！
    // const ASCII* const _half_str = 11;
    const ASCII* const _real32_str = "1.2345";
    const ASCII* const _real64_str = "88.77665544";

    // ASCII编码的字符串
    const ASCII *const _ascii_text = "AB12\t\n e";
    // UTF8编码的字符串：aB1編碼测试
    //
    // 編: U+7DE8 : UTF8: E7 B7 A8     UTF16: 7D E8
    // 碼: U+78BC : UTF8: E7 A2 BC     UTF16: 78 BC
    // 测: U+6D4B : UTF8: E6 B5 8B     UTF16: 6D 4B
    // 试: U+8BD5 : UTF8: E8 AF 95     UTF16: 8B D5
    const RuntimeText _utf8_text("aB1\xE7\xB7\xA8\xE7\xA2\xBC\xE6\xB5\x8B\xE8\xAF\x95");
    // UTF16编码的字符串：aB1編碼测试
    const UTF16Text  _utf16_text(u"aB1編碼测试");
    // UTF16编码的字符串的字节长度
    const UInt32     _utf16_text_byte_size = (UInt32)_utf16_text.size() * sizeof(UTF16);


    SECTION("Checking Writing Text_file_ASCII.txt:")
    {
        std::printf("--- Checking Writing ASCII string using UTF8 encoding...\n");

        {
            // 输出ASCII字符串: Text_file_ASCII
            TextFileWriter _writer((const UTF8*)"Text_file_ASCII.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                   TextEncodings::TEXT_ENCODINGS_UTF8);
            const Bool _write_op = _writer.writeText(_ascii_text);
            RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_ASCII.txt");
            _writer.close();

            // 读入文件内容
            FileReadStream _file_io;
            const Bool _file_op = NativeFileSystem::openFileRead(
                (const UTF8*)"Text_file_ASCII.txt",
                NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                _file_io);
            RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_ASCII.txt");
            ASCII _buffer[sizeof(_ascii_text)] = {0};
            _file_io.read(sizeof(_buffer), 0, _file_io.getLength(), (UInt8*)_buffer);
            _file_io.close();
            for (UInt32 _idx = 0; _idx < sizeof(_ascii_text); ++_idx)
            {
                REQUIRE(_buffer[_idx] == _ascii_text[_idx]);
            }
        }

        std::printf("--- Checking Writing ASCII string using UTF8 encoding: OK!\n");
    }

    SECTION("Checking Writing UTF8 text:")
    {
        std::printf("--- Checking Writing UTF8 text...\n");

        {
            // --- 输出UTF8字符串 --- //
            // UTF8字符串使用UTF8编码输出: Text_file_UTF8_UTF8
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF8_UTF8.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF8);
                const Bool _write_op = _writer.writeText(_utf8_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF8_UTF8.txt");
                _writer.close();
            }
            // UTF8字符串使用BOM UTF8编码输出: Text_file_UTF8_UTF8_BOM
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF8_UTF8_BOM.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF8_BOM);
                const Bool _write_op = _writer.writeText(_utf8_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF8_UTF8_BOM.txt");
                _writer.close();
            }
            // UTF8字符串使用UTF16LE编码输出: Text_file_UTF8_UTF16_LE
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF8_UTF16_LE.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF16_LE);
                const Bool _write_op = _writer.writeText(_utf8_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF8_UTF16_LE.txt");
                _writer.close();
            }
            // UTF8字符串使用UTF16BE编码输出: Text_file_UTF8_UTF16_BE
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF8_UTF16_BE.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF16_BE);
                const Bool _write_op = _writer.writeText(_utf8_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF8_UTF16_BE.txt");
                _writer.close();
            }

            // --- 读入文件检查内容 --- //
            // UTF8编码文件: Text_file_UTF8_UTF8
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF8_UTF8.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF8_UTF8.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == (UInt32)_utf8_text.size());
                // 比较文件内容
                RuntimeText _buffer;
                _buffer.resize(_file_len);
                _file_io.read((UInt32)_buffer.size(), 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                for (UInt32 _idx = 0; _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == _utf8_text[_idx]);
                }
            }
            // BOM UTF8编码文件: Text_file_UTF8_UTF8_BOM
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF8_UTF8_BOM.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF8_UTF8_BOM.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == (UInt32)_utf8_text.size() + sizeof(TextHelper::BOM_UTF8));
                // 比较文件内容
                RuntimeText _buffer;
                _buffer.resize(_file_len);
                _file_io.read((UInt32)_buffer.size(), 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                // 检查BOM
                for (UInt32 _idx = 0; _idx < sizeof(TextHelper::BOM_UTF8); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == TextHelper::BOM_UTF8[_idx]);
                }
                for (UInt32 _idx = sizeof(TextHelper::BOM_UTF8); _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == _utf8_text[_idx - sizeof(TextHelper::BOM_UTF8)]);
                }
            }
            // UTF16LE编码文件: Text_file_UTF8_UTF16_LE
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF8_UTF16_LE.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF8_UTF16_LE.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == _utf16_text_byte_size + sizeof(TextHelper::BOM_UTF16));
                // 比较文件内容
                // 用来存储BOM已经文件内容的缓存
                UTF16Text _buffer;
                _buffer.resize(_file_len / sizeof(UTF16));
                _file_io.read(_file_len, 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                // 检查BOM
                REQUIRE(_buffer[0] == TextHelper::BOM_UTF16);
                for (UInt32 _idx = 1; _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == _utf16_text[_idx - 1]);
                }
            }
            // UTF16BE编码文件: Text_file_UTF8_UTF16_BE
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF8_UTF16_BE.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF8_UTF16_BE.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == _utf16_text_byte_size + sizeof(TextHelper::BOM_UTF16));
                // 比较文件内容
                UTF16Text _buffer;
                _buffer.resize(_file_len / sizeof(UTF16));
                _file_io.read(_file_len, 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                // 检查BOM
                REQUIRE(EndianHelper::swapEndian16(_buffer[0]) == TextHelper::BOM_UTF16);
                for (UInt32 _idx = 1; _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(EndianHelper::swapEndian16(_buffer[_idx]) == _utf16_text[_idx - 1]);
                }
            }
        }

        std::printf("--- Checking Writing UTF8 text: OK!\n");
    }

    SECTION("Checking Writing UTF16 text:")
    {
        std::printf("--- Checking Writing UTF16 text...\n");

        {
            // --- 输出UTF16字符串 --- //
            // UTF16字符串使用UTF8编码输出: Text_file_UTF16_UTF8
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF16_UTF8.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF8);
                const Bool _write_op = _writer.writeText(_utf16_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF16_UTF8.txt");
                _writer.close();
            }
            // UTF16字符串使用BOM UTF8编码输出: Text_file_UTF16_UTF8_BOM
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF16_UTF8_BOM.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF8_BOM);
                const Bool _write_op = _writer.writeText(_utf16_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF16_UTF8_BOM.txt");
                _writer.close();
            }
            // UTF16字符串使用UTF16LE编码输出: Text_file_UTF16_UTF16_LE
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF16_UTF16_LE.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF16_LE);
                const Bool _write_op = _writer.writeText(_utf16_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF16_UTF16_LE.txt");
                _writer.close();
            }
            // UTF16字符串使用UTF16BE编码输出: Text_file_UTF16_UTF16_BE
            {
                TextFileWriter _writer((const UTF8*)"Text_file_UTF16_UTF16_BE.txt",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       TextEncodings::TEXT_ENCODINGS_UTF16_BE);
                const Bool _write_op = _writer.writeText(_utf16_text);
                RUNTIME_ASSERT(_write_op, "Can NOT write in Text_file_UTF16_UTF16_BE.txt");
                _writer.close();
            }

            // --- 读入文件检查内容 --- //
            // UTF8编码文件: Text_file_UTF16_UTF8
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF16_UTF8.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF16_UTF8.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == (UInt32)_utf8_text.size());
                // 比较文件内容
                RuntimeText _buffer;
                _buffer.resize(_file_len);
                _file_io.read((UInt32)_buffer.size(), 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                for (UInt32 _idx = 0; _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == _utf8_text[_idx]);
                }
            }
            // BOM UTF8编码文件: Text_file_UTF16_UTF8_BOM
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF16_UTF8_BOM.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF16_UTF8_BOM.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == (UInt32)_utf8_text.size() + sizeof(TextHelper::BOM_UTF8));
                // 比较文件内容
                RuntimeText _buffer;
                _buffer.resize(_file_len);
                _file_io.read((UInt32)_buffer.size(), 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                // 检查BOM
                for (UInt32 _idx = 0; _idx < sizeof(TextHelper::BOM_UTF8); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == TextHelper::BOM_UTF8[_idx]);
                }
                for (UInt32 _idx = sizeof(TextHelper::BOM_UTF8); _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == _utf8_text[_idx - sizeof(TextHelper::BOM_UTF8)]);
                }
            }
            // UTF16LE编码文件: Text_file_UTF16_UTF16_LE
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF16_UTF16_LE.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF16_UTF16_LE.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == _utf16_text_byte_size + sizeof(TextHelper::BOM_UTF16));
                // 比较文件内容
                // 用来存储BOM已经文件内容的缓存
                UTF16Text _buffer;
                _buffer.resize(_file_len / sizeof(UTF16));
                _file_io.read(_file_len, 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                // 检查BOM
                REQUIRE(_buffer[0] == TextHelper::BOM_UTF16);
                for (UInt32 _idx = 1; _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(_buffer[_idx] == _utf16_text[_idx - 1]);
                }
            }
            // UTF16BE编码文件: Text_file_UTF16_UTF16_BE
            {
                FileReadStream _file_io;
                const Bool _file_op = NativeFileSystem::openFileRead(
                    (const UTF8*)"Text_file_UTF16_UTF16_BE.txt",
                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                    _file_io);
                RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_UTF16_UTF16_BE.txt");
                // 比较文件长度
                const UInt32 _file_len = _file_io.getLength();
                REQUIRE(_file_len == _utf16_text_byte_size + sizeof(TextHelper::BOM_UTF16));
                // 比较文件内容
                UTF16Text _buffer;
                _buffer.resize(_file_len / sizeof(UTF16));
                _file_io.read(_file_len, 0, _file_len, (UInt8*)_buffer.data());
                _file_io.close();
                // 检查BOM
                REQUIRE(EndianHelper::swapEndian16(_buffer[0]) == TextHelper::BOM_UTF16);
                for (UInt32 _idx = 1; _idx < (UInt32)_buffer.size(); ++_idx)
                {
                    REQUIRE(EndianHelper::swapEndian16(_buffer[_idx]) == _utf16_text[_idx - 1]);
                }
            }
        }

        std::printf("--- Checking Writing UTF16 text: OK!\n");
    }

    SECTION("Checking Reading UTF8 text:")
    {
        std::printf("--- Checking Reading UTF8 text...\n");

        // UTF8字符串使用UTF8编码输出: Text_file_UTF8_UTF8
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF8.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF8.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                REQUIRE(_utf8_content[_idx] == _utf8_text[_idx]);
            }
        }
        // BOM UTF8编码文件: Text_file_UTF8_UTF8_BOM
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF8_BOM.txt",
                                    NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF8_BOM.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                REQUIRE(_utf8_content[_idx] == _utf8_text[_idx]);
            }
        }
        // UTF16LE编码文件: Text_file_UTF8_UTF16_LE
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF16_LE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF16_LE.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                REQUIRE(_utf8_content[_idx] == _utf8_text[_idx]);
            }
        }
        // UTF16BE编码文件: Text_file_UTF8_UTF16_BE
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF16_BE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            RuntimeText _utf8_content;
            const Bool _read_op = _reader.readLine(_utf8_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF16_BE.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf8_content.size(); ++_idx)
            {
                REQUIRE(_utf8_content[_idx] == _utf8_text[_idx]);
            }
        }

        std::printf("--- Checking Reading UTF8 text: OK!\n");
    }

    SECTION("Checking Reading UTF16 text:")
    {
        std::printf("--- Checking Reading UTF16 text...\n");

        // UTF8字符串使用UTF8编码输出: Text_file_UTF8_UTF8
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF8.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF8.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                REQUIRE(_utf16_content[_idx] == _utf16_text[_idx]);
            }
        }
        // BOM UTF8编码文件: Text_file_UTF8_UTF8_BOM
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF8_BOM.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);
            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF8_BOM.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                REQUIRE(_utf16_content[_idx] == _utf16_text[_idx]);
            }
        }
        // UTF16LE编码文件: Text_file_UTF8_UTF16_LE
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF16_LE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);

            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF16_LE.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                REQUIRE(_utf16_content[_idx] == _utf16_text[_idx]);
            }
        }
        // UTF16BE编码文件: Text_file_UTF8_UTF16_BE
        {
            TextFileReader _reader((const UTF8*)"Text_file_UTF8_UTF16_BE.txt",
                                   NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER);

            UTF16Text _utf16_content;
            const Bool _read_op = _reader.readLine(_utf16_content);
            RUNTIME_ASSERT(_read_op, "Can NOT read in Text_file_UTF8_UTF16_BE.txt");
            _reader.close();
            for (UInt32 _idx = 0; _idx < (UInt32)_utf16_content.size(); ++_idx)
            {
                REQUIRE(_utf16_content[_idx] == _utf16_text[_idx]);
            }
        }

        std::printf("--- Checking Reading UTF16 text: OK!\n");
    }


    SECTION("Checking Writing operators:")
    {
        std::printf("--- Checking Writing operator:...\n");

        TextFileWriter _writer((const UTF8*)"Text_file_operators.txt",
                               NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                               TextEncodings::TEXT_ENCODINGS_UTF8);
        // 输出Primitive数据
        _writer << _pos8   << _space << _neg8   << _space << _uint8  << _feed;
        _writer << _pos16  << _space << _neg16  << _space << _uint16 << _feed;
        _writer << /* _real16 << _space */ _real32 << _space << _real64 << _feed;
        // 输出字符串
        _writer << _ascii_text << _feed;
        _writer << _utf8_text  << _feed;
        _writer << _utf16_text << _feed;
        _writer.close();

        // 读入文件内容
        FileReadStream _file_io;
        const Bool _file_op = NativeFileSystem::openFileRead(
            (const UTF8*)"Text_file_operators.txt",
            NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
            _file_io);
        RUNTIME_ASSERT(_file_op, "Can NOT read in Text_file_operators.txt");
        UInt32 _file_len = _file_io.getLength();
        std::vector<UInt8> _buffer;
        _buffer.resize(_file_len);
        _file_io.read(_file_len, 0, _file_len, _buffer.data());
        _file_io.close();

        UInt32 _cur_idx = 0;
        // SUInt8
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _pos8_str, std::strlen(_pos8_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_pos8_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _space, std::strlen(_space)) == 0);
        _cur_idx += (UInt32)std::strlen(_space);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _neg8_str, std::strlen(_neg8_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_neg8_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _space, std::strlen(_space)) == 0);
        _cur_idx += (UInt32)std::strlen(_space);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _uint8_str, std::strlen(_uint8_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_uint8_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _feed, std::strlen(_feed)) == 0);
        _cur_idx += (UInt32)std::strlen(_feed);
        // SUInt16
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _pos16_str, std::strlen(_pos16_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_pos16_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _space, std::strlen(_space)) == 0);
        _cur_idx += (UInt32)std::strlen(_space);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _neg16_str, std::strlen(_neg16_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_neg16_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _space, std::strlen(_space)) == 0);
        _cur_idx += (UInt32)std::strlen(_space);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _uint16_str, std::strlen(_uint16_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_uint16_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _feed, std::strlen(_feed)) == 0);
        _cur_idx += (UInt32)std::strlen(_feed);
        // Real32/64
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _real32_str, std::strlen(_real32_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_real32_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _space, std::strlen(_space)) == 0);
        _cur_idx += (UInt32)std::strlen(_space);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _real64_str, std::strlen(_real64_str)) == 0);
        _cur_idx += (UInt32)std::strlen(_real64_str);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _feed, std::strlen(_feed)) == 0);
        _cur_idx += (UInt32)std::strlen(_feed);
        // Ascii text
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _ascii_text, std::strlen(_ascii_text)) == 0);
        _cur_idx += (UInt32)std::strlen(_ascii_text);
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _feed, std::strlen(_feed)) == 0);
        _cur_idx += (UInt32)std::strlen(_feed);
        // UTF8 text
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _utf8_text.data(), _utf8_text.size()) == 0);
        _cur_idx += (UInt32)_utf8_text.size();
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _feed, std::strlen(_feed)) == 0);
        _cur_idx += (UInt32)std::strlen(_feed);
        // UTF16 text
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _utf8_text.data(), _utf8_text.size()) == 0);
        _cur_idx += (UInt32)_utf8_text.size();
        REQUIRE(std::memcmp(_buffer.data()+_cur_idx, _feed, std::strlen(_feed)) == 0);
        _cur_idx += (UInt32)std::strlen(_feed);

        std::printf("--- Checking Writing operators: OK!\n");
    }
} // TEST_CASE("Checking TextFileRead/WriteStream", "[TextFileRead/WriteStream]")

#endif // #if (FILE_READ_WRITE_STREAM_CHECKING == 1)
