// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/DataOperation/RS_EndiannessHelper.hpp"
#include "Core/String/RS_UTF16Text.hpp"
// Self
#include "Core/FileSystem/RS_TextFileWriter.hpp"


namespace Core
{

    TextFileWriter::TextFileWriter (
        const UTF8 *const                   rel_path,
        const NativeFileSystem::SearchPaths search_dir,
        const TextEncodings                 encoding)
    :
        m_encoding(encoding)
    {
        // 创建WriteOnly文件流
        if (NativeFileSystem::openFileWrite(rel_path, search_dir, m_io))
        {
            // 输出BOM
            if (encoding != TEXT_ENCODINGS_UTF8) writeBOM();
        }
    }


    TextFileWriter::~TextFileWriter ()
    {
        // 关闭文件流
        m_io.close();
    }


    Bool
    TextFileWriter::isValid () const
    {
        return m_io.isValid();
    }


    Bool
    TextFileWriter::writeText (
        const ASCII *const ascii_text)
    {
        RUNTIME_ASSERT(isValid(),
                       "File stream is invalid."
                       "Please check its validity with isValid() before calling this function");
        // 根据文件使用的Encoding来进行不同的处理
        switch (m_encoding)
        {
            case TEXT_ENCODINGS_UTF8:
            case TEXT_ENCODINGS_UTF8_BOM:
            {
                // 直接输出给定字符串: 不输出结尾\0
                const UInt8 *const _data_ptr = (const UInt8*)ascii_text;
                const UInt32       _data_len = (UInt32)std::strlen(ascii_text);
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            case TEXT_ENCODINGS_UTF16_BE:
            {
                // 将ASCII编码扩展到UTF16编码：8位数 -> 16位数
                UTF16Text    _utf16_text;
                const UInt32 _text_len = (UInt32)std::strlen(ascii_text);
                _utf16_text.resize(_text_len);
                for (UInt32 _idx = 0; _idx < _text_len; ++_idx)
                {
                    // 如果当前机器使用Little Endianness，交换高低8位
                    if (!EndianHelper::isBigEndian())
                    {
                        _utf16_text[_idx] = EndianHelper::swapEndian16(ascii_text[_idx]);
                    }
                    else
                    {
                        _utf16_text[_idx] = (UTF16)ascii_text[_idx];
                    }
                }
                // 输出UTF16编码的字符串: 不输出结尾\00
                const UInt8 *const _data_ptr = (const UInt8*)_utf16_text.data();
                const UInt32       _data_len = _text_len * sizeof(UTF16);
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            case TEXT_ENCODINGS_UTF16_LE:
            {
                // 将ASCII编码扩展到UTF16编码：8位数 -> 16位数
                UTF16Text    _utf16_text;
                const UInt32 _text_len = (UInt32)std::strlen(ascii_text);
                _utf16_text.resize(_text_len);
                for (UInt32 _idx = 0; _idx < _text_len; ++_idx)
                {
                    // 如果当前机器使用Big Endianness，交换高低8位
                    if (EndianHelper::isBigEndian())
                    {
                        _utf16_text[_idx] = EndianHelper::swapEndian16(ascii_text[_idx]);
                    }
                    else
                    {
                        _utf16_text[_idx] = (UTF16)ascii_text[_idx];
                    }
                }
                // 输出UTF16编码的字符串: 不输出结尾\00
                const UInt8 *const _data_ptr = (const UInt8*)_utf16_text.data();
                const UInt32       _data_len = _text_len * sizeof(UTF16);
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown text encoding");
                return false;
            }
        }
    }


    Bool
    TextFileWriter::writeText (
        const RuntimeText & utf8_text)
    {
        RUNTIME_ASSERT(isValid(),
                       "File stream is invalid."
                       "Please check its validity with isValid() before calling this function");
        // 根据文件使用的Encoding来进行不同的处理
        switch (m_encoding)
        {
            case TEXT_ENCODINGS_UTF8:
            case TEXT_ENCODINGS_UTF8_BOM:
            {
                // 直接输出给定字符串: 不输出结尾\0
                const UInt8 *const _data_ptr = (const UInt8*)utf8_text.data();
                const UInt32       _data_len = (UInt32)utf8_text.size();
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            case TEXT_ENCODINGS_UTF16_BE:
            {
                // 转换UTF8编码字符串到UTF16
                UTF16Text _utf16_text(utf8_text.data());
                // 如果当前机器使用Little Endianness，交换高低8位
                if (!EndianHelper::isBigEndian())
                {
                    for (UInt32 _idx = 0; _idx < (UInt32)_utf16_text.size(); ++_idx)
                    {
                        _utf16_text[_idx] = EndianHelper::swapEndian16(_utf16_text[_idx]);
                    }
                }
                // 输出UTF16编码的字符串: 不输出结尾\00
                const UInt8 *const _data_ptr = (const UInt8*)_utf16_text.data();
                const UInt32       _data_len = (UInt32)_utf16_text.size() * sizeof(UTF16);
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            case TEXT_ENCODINGS_UTF16_LE:
            {
                // 转换UTF8编码字符串到UTF16
                UTF16Text _utf16_text(utf8_text.data());
                // 如果当前机器使用Big Endianness，交换高低8位
                if (EndianHelper::isBigEndian())
                {
                    for (UInt32 _idx = 0; _idx < (UInt32)_utf16_text.size(); ++_idx)
                    {
                        _utf16_text[_idx] = EndianHelper::swapEndian16(_utf16_text[_idx]);
                    }
                }
                // 输出UTF16编码的字符串: 不输出结尾\00
                const UInt8 *const _data_ptr = (const UInt8*)_utf16_text.data();
                const UInt32       _data_len = (UInt32)_utf16_text.size() * sizeof(UTF16);
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown text encoding");
                return false;
            }
        }
    }


    Bool
    TextFileWriter::writeText (
        const UTF16Text & utf16_text)
    {
        RUNTIME_ASSERT(isValid(),
                       "File stream is invalid."
                       "Please check its validity with isValid() before calling this function");
        // 根据文件使用的Encoding来进行不同的处理
        switch (m_encoding)
        {
            case TEXT_ENCODINGS_UTF8:
            case TEXT_ENCODINGS_UTF8_BOM:
            {
                // 将UTF16编码字符串转换为UTF8
                RuntimeText  _utf8_text;
                const UInt32 _text_len = TextHelper::getUTF8TextLength(utf16_text.data());
                _utf8_text.resize(_text_len);
                TextHelper::convertUTF16ToUTF8(utf16_text.data(), (UTF8*)_utf8_text.data());
                // 输出UTF8编码的字符串: 不输出结尾\0
                const UInt8 *const _data_ptr = (const UInt8*)_utf8_text.data();
                const UInt32       _data_len = (UInt32)_utf8_text.size();
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            case TEXT_ENCODINGS_UTF16_BE:
            {
                UTF16Text _utf16_text(utf16_text);
                // 如果当前机器使用Little Endianness，交换高低8位
                if (!EndianHelper::isBigEndian())
                {
                    for (UInt32 _idx = 0; _idx < (UInt32)_utf16_text.size(); ++_idx)
                    {
                        _utf16_text[_idx] = EndianHelper::swapEndian16(_utf16_text[_idx]);
                    }
                }
                // 直接输出给定字符串: 不输出结尾\00
                const UInt8 *const _data_ptr = (const UInt8*)_utf16_text.data();
                const UInt32       _data_len = (UInt32)_utf16_text.size() * sizeof(UTF16);
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            case TEXT_ENCODINGS_UTF16_LE:
            {
                // 直接输出给定字符串: 不输出结尾\00
                const UInt8 *const _data_ptr = (const UInt8*)utf16_text.data();
                const UInt32       _data_len = (UInt32)utf16_text.size() * sizeof(UTF16);
                return m_io.write(_data_ptr, _data_len, 0, _data_len) == _data_len;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown text encoding");
                return false;
            }
        }
    }

     TextFileWriter&
     TextFileWriter::operator << (
         const SInt8 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%d", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const UInt8 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%u", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const SInt16 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%d", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const UInt16 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%u", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const SInt32 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%d", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const UInt32 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%u", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const SInt64 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%lld", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const UInt64 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%llu", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const Real16 data)
    {
        // TODO: 需要实现！！
        return *this;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const Real32 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%.4f", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const Real64 data)
    {
        TextHelper::sprintfSafe((UTF8*)m_buffer, sizeof(m_buffer), "%.8lf", data);
        return (*this) << m_buffer;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const ASCII *const ascii_text)
    {
        writeText(ascii_text);
        return *this;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const RuntimeText & utf8_text)
    {
        writeText(utf8_text);
        return *this;
    }


     TextFileWriter&
     TextFileWriter::operator << (
         const UTF16Text & utf16_text)
    {
        writeText(utf16_text);
        return *this;
    }


    void
    TextFileWriter::close ()
    {
        m_io.close();
    }


    void
    TextFileWriter::writeBOM ()
    {
        // 文件开头三个字节为BOM， 如果当前编码需要的话
        // BOM：U+FEFF, UTF16: 0xFF 0xFE(LE), 0xFE 0xFF(BE), UTF8: 0xEF 0xBB 0xBF
        //
        // UTF8编码字符串带有BOM(U+FEFF): 编码为UTF8为 EF BB BF
        // +----+----+----+-----+-------...--------+
        // | EF | BB | BF | ... | UTF8 Encoding... |
        // +----+----+----+-----+-------...--------+
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
        switch (m_encoding)
        {
            case TEXT_ENCODINGS_UTF8_BOM:
            {
                m_io.write((const UInt8*)TextHelper::BOM_UTF8,
                           sizeof(TextHelper::BOM_UTF8),
                           0,
                           sizeof(TextHelper::BOM_UTF8));
                break;
            }

            case TEXT_ENCODINGS_UTF16_LE:
            {
                m_io << TextHelper::BOM_UTF16;
                break;
            }

            case TEXT_ENCODINGS_UTF16_BE:
            {
                UTF16 _swap_data;
                // 输出BOM
                _swap_data = (UTF16)EndianHelper::swapEndian16((UInt16)TextHelper::BOM_UTF16);
                m_io << _swap_data;
                break;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown text encoding");
                break;
            }
        }
    }

} // namespace Core
