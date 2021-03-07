// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/DataOperation/RS_EndiannessHelper.hpp"
#include "Core/String/RS_UTF16Text.hpp"
// Self
#include "Core/FileSystem/RS_TextFileReader.hpp"


namespace Core
{

    TextFileReader::TextFileReader (
        const UTF8 *const                   rel_path,
        const NativeFileSystem::SearchPaths search_dir)
    :
        m_encoding(TEXT_ENCODINGS_UTF8)
    {
        // 创建ReadOnly文件流
        if (NativeFileSystem::openFileRead(rel_path, search_dir, m_io))
        {
            // 检查文件中字符串的编码方式
            detectEncoding();
        }
    }


    TextFileReader::~TextFileReader ()
    {
        // 关闭文件流
        m_io.close();
    }


    Bool
    TextFileReader::isValid () const
    {
        return m_io.isValid();
    }


    Bool
    TextFileReader::isEndOfStream () const
    {
        return m_io.getCursorPostion() == m_io.getLength();
    }


    Bool
    TextFileReader::peekNextChar (
        UTF8 (*utf8_code)[4])
    {
        // 保存当前流的位置
        const UInt32 _cur_pos  = m_io.getCursorPostion();
        // 读入下一个字符
        const Bool _op_success = readNextChar(utf8_code);
        // 复原流位置
        m_io.seek(_cur_pos, NativeFileSystem::SEEK_MODES_FILE_BEGIN);
        return _op_success;
    }


    Bool
    TextFileReader::peekNextChar (
        UTF16 & utf16_char)
    {
        // 保存当前流的位置
        const UInt32 _cur_pos  = m_io.getCursorPostion();
        // 读入下一个字符
        const Bool _op_success = readNextChar(utf16_char);
        // 复原流位置
        m_io.seek(_cur_pos, NativeFileSystem::SEEK_MODES_FILE_BEGIN);
        return _op_success;
    }


    Bool
    TextFileReader::readNextChar (
        UTF8 (*utf8_code)[4])
    {
        // 检查是否我么已经到达文件流的尾部
        if (isEndOfStream())
        {
            return false;
        }

        // 根据文件使用的Encoding来进行不同的处理
        switch (m_encoding)
        {
            case TEXT_ENCODINGS_UTF8:
            {
                // 判断当前UTF8编码使用字节数
                m_io >> (*utf8_code)[0];
                const UInt8 utf8_code_len = TextHelper::getUTF8CodeLength((*utf8_code)[0]);
                // 我们只支持Basic Multilingual Plane上面的Unicode
                if (utf8_code_len == 4 || utf8_code_len == 0)
                {
                    (*utf8_code)[0] = 0;
                    return false;
                }
                // 读入所有编码字节
                if (utf8_code_len > 1)
                {
                    if (m_io.read(3, 1, utf8_code_len - 1, *utf8_code) != utf8_code_len - 1)
                    {
                        (*utf8_code)[0] = 0;
                        return false;
                    }
                }

                (*utf8_code)[utf8_code_len] = 0;
                break;
            }

            case TEXT_ENCODINGS_UTF16_BE:
            {
                std::memset(*utf8_code, 0, 4);
                UInt16 _utf16_code[2] = {0};
                m_io >> _utf16_code[0];
                // 如果当前机器使用Little Endianness，交换高低8位
                if (!EndianHelper::isBigEndian())
                {
                    _utf16_code[0] = EndianHelper::swapEndian(_utf16_code[0]);
                }
                // 转换为UTF8
                TextHelper::convertUTF16ToUTF8((const UTF16*)_utf16_code, *utf8_code);
                break;
            }

            case TEXT_ENCODINGS_UTF16_LE:
            {
                std::memset(*utf8_code, 0, 4);
                UInt16 _utf16_code[2] = {0};
                m_io >> _utf16_code[0];
                // 如果当前机器使用Big Endianness，交换高低8位
                if (EndianHelper::isBigEndian())
                {
                    _utf16_code[0] = EndianHelper::swapEndian(_utf16_code[0]);
                }
                // 转换为UTF8
                TextHelper::convertUTF16ToUTF8((const UTF16*)_utf16_code, *utf8_code);
                break;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown text encoding");
                break;
            }
        }

        return true;
    }


    Bool
    TextFileReader::readNextChar (
        UTF16 & utf16_char)
    {
        // 检查是否我么已经到达文件流的尾部
        if (isEndOfStream())
        {
            return false;
        }

        // 根据文件使用的Encoding来进行不同的处理
        switch (m_encoding)
        {
            case TEXT_ENCODINGS_UTF8:
            {
                // 判断当前UTF8编码使用字节数
                UInt8 _utf8_code[4] = {0};
                m_io >> _utf8_code[0];
                const UInt8 utf8_code_len = TextHelper::getUTF8CodeLength(_utf8_code[0]);
                // 我们只支持Basic Multilingual Plane上面的Unicode
                if (utf8_code_len == 4 || utf8_code_len == 0)
                {
                    return false;
                }
                // 读入所有编码字节
                if (utf8_code_len > 1)
                {
                    if (m_io.read(2, 0, utf8_code_len-1, _utf8_code+1) != utf8_code_len - 1)
                    {
                        return false;
                    }
                }
                // 转换为UTF16
                TextHelper::convertUTF8ToUTF16(_utf8_code, &utf16_char);
                break;
            }

            case TEXT_ENCODINGS_UTF16_BE:
            {
                // 读入两个字节
                UInt16 _utf16_be;
                m_io >> _utf16_be;
                // 如果当前机器使用Little Endianness，交换高低8位
                if (!EndianHelper::isBigEndian())
                {
                    utf16_char = EndianHelper::swapEndian(_utf16_be);
                }
                else
                {
                    utf16_char = _utf16_be;
                }
                break;
            }

            case TEXT_ENCODINGS_UTF16_LE:
            {
                // 直接读入返回
                UInt16 _utf16_le;
                m_io >> _utf16_le;
                // 如果当前机器使用Big Endianness，交换高低8位
                if (EndianHelper::isBigEndian())
                {
                    utf16_char = EndianHelper::swapEndian(_utf16_le);
                }
                else
                {
                    utf16_char = _utf16_le;
                }
                break;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown text encoding");
                break;
            }

        }

        return true;
    }


    Bool
    TextFileReader::readLine (
        RuntimeText & utf8_text)
    {
        const UInt32 DEFAULT_LINE_LENGHT = 256;

        // 申请使用空间
        utf8_text.reserve(DEFAULT_LINE_LENGHT);
        utf8_text.clear();

        UTF8 _next_char[4];
        // 没有到达文件流的尾部或则没有遇见回车
        while (readNextChar(&_next_char) &&
               _next_char[3] == 0        &&
               !TextHelper::isLineFeedChar((ASCII)_next_char[0]))
        {
            for (UInt8 _idx = 0;_next_char[_idx] != 0; ++_idx)
            {
                utf8_text.push_back(_next_char[_idx]);
            }
        }

        return !utf8_text.empty();
    }


    Bool
    TextFileReader::readLine (
        UTF16Text & utf16_text)
    {
        const UInt32 DEFAULT_LINE_LENGHT = 256;

        // 申请使用空间
        utf16_text.reserve(DEFAULT_LINE_LENGHT);
        utf16_text.clear();

        UTF16 _next_char;
        // 没有到达文件流的尾部或则没有遇见回车
        while (readNextChar(_next_char) && !TextHelper::isLineFeedChar(_next_char))
        {
            utf16_text.push_back(_next_char);
        }

        return !utf16_text.empty();
    }


    void
    TextFileReader::rewind ()
    {
        m_io.seek(0, NativeFileSystem::SEEK_MODES_FILE_BEGIN);
    }


    void
    TextFileReader::close ()
    {
        m_io.close();
    }


    void
    TextFileReader::detectEncoding ()
    {
        // 假设缺省编码方式为UTF8

        // 读入文件开头三个字节，比较它是否为BOM的UTF16或者UTF8的编码
        // BOM：U+FEFF, UTF16: 0xFF 0xFE(LE), 0xFE 0xFF(BE), UTF8: 0xEF 0xBB 0xBF
        //
        // UTF16 Little Endianess(LE):
        // +----+----+-----+---------...---------+
        // | FF | FE | ... |  UTF16 Encoding...  |
        // +----+----+-----+---------...---------+
        //
        // UTF16 Big Endianess(BE):
        // +----+----+-----+---------...---------+
        // | FE | FF | ... |  UTF16 Encoding...  |
        // +----+----+-----+---------...---------+
        //
        // UTF8编码字符串带有BOM(U+FEFF): 编码为UTF8为 EF BB BF
        // +----+----+----+-----+-------...--------+
        // | EF | BB | BF | ... | UTF8 Encoding... |
        // +----+----+----+-----+-------...--------+

        UInt8 _first_byte, _second_byte;
        // 检查是否文件存储BOM：U+FEFF, UTF16: 0xFF 0xFE(LE), 0xFE 0xFF(BE), UTF8: 0xEF 0xBB 0xBF
        if (m_io.getLength() > 3)
        {
            // 读入开头的两个字节
            m_io >> _first_byte >> _second_byte;

            // UTF16 BE
            if (_first_byte == 0xFE && _second_byte == 0xFF)
            {
                m_encoding = TEXT_ENCODINGS_UTF16_BE;
            }
            // UTF16 LE
            else if (_first_byte == 0xFF && _second_byte == 0xFE)
            {
                m_encoding = TEXT_ENCODINGS_UTF16_LE;
            }
            // 可能是UTF8编码的BOM
            else if (_first_byte == 0xEF && _second_byte == 0xBB)
            {
                // 读入第三个字节
                UInt8 _third_byte;
                m_io >> _third_byte;
                // 不是UTF8编码的BOM
                if (_third_byte != 0xBF)
                {
                    // 复位读写头
                    m_io.seek(0, NativeFileSystem::SEEK_MODES_FILE_BEGIN);
                }
            }
            // 没有BOM
            else
            {
                // 复位读写头
                m_io.seek(0, NativeFileSystem::SEEK_MODES_FILE_BEGIN);
            }
        }
    }

} // namespace Core
