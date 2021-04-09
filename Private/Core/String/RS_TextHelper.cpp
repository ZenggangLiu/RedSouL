// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
// Self
#include "Core/String/RS_TextHelper.hpp"


namespace Core
{

    // 十六进制数值的ASCII文字表示
    static const ASCII HEX_NAMES_ASCII[]        = "0123456789abcdefABCDEF";
    static const UTF8  SPACE_UTF8               = ' ';
    static const UTF8  DOTCHAR_UTF8             = '.';
    // OSx, Unix: 0x0A
    static const UTF8  LINE_FEED_UNIX_UTF8      = '\n';
    static const UTF16 LINE_FEED_UNIX_UTF16     = u'\n';
    // Windows  : 0x0D
    static const UTF8  LINE_FEED_WINDOWS_UTF8   = '\r';
    static const UTF16 LINE_FEED_WINDOWS_UTF16  = u'\r';


    // UTF8编码方案:
    // 占用的Byte数目 编码使用的Bits 第一个编码 最后一个编码 第一Byte  第二Byte    第三Byte    第四Byte
    // +--------+----------+----------+----------+----------+----------+----------+----------+
    // | Number | Bits for |  First   |   Last   |          |          |          |          |
    // |of bytes|code point|code point|code point|  Byte 1  |  Byte 2  |  Byte 3  |  Byte 4  |
    // +--------+----------+----------+----------+----------+----------+----------+----------+
    // |   1    |    7     |  U+0000  |  U+007F  | 0xxxxxxx |          |          |          |
    // |        |          |          |          |  (0-7F)  |          |          |          |
    // +--------+----------+----------+----------+----------+----------+----------+----------+
    // |   2    |    11    |  U+0080  |  U+07FF  | 110xxxxx | 10xxxxxx |          |          |
    // |        |          |          |          |  (C0-DF) | (80-BF)  |          |          |
    // +--------+----------+----------+----------+----------+----------+----------+----------+
    // |   3    |    16    |  U+0800  |  U+FFFF  | 1110xxxx | 10xxxxxx | 10xxxxxx |          |
    // |        |          |          |          |  (E0-EF) | (80-BF)  | (80-BF)  |          |
    // +--------+----------+----------+----------+----------+----------+----------+----------+
    // |   4    |    21    |  U+10000 |  U+10FFFF| 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx |
    // |        |          |          |          |  (F0-F7) | (80-BF)  | (80-BF)  | (80-BF)  |
    // +--------+----------+----------+----------+----------+----------+----------+----------+
    //
    // UTF16编码方案：
    //  占用的整数数目   编码使用的Bits    Unicode编码    Unicode编码   第一个UTF16整数 第二个UTF16整数
    // +------------+--------------+--------------+--------------+----------+----------+
    // |   Number   |   Bits for   |    First     |     Last     |          |          |
    // |  of ints   |  code point  |  code point  |  code point  |  Int 1   |  Int  2  |
    // +------------+--------------+--------------+--------------+----------+----------+
    // |     1      |      16      |    U+0000    |    U+D7FF    |  0xxxxx  |          |
    // |            |              |    U+E000    |    U+FFFF    |          |          |
    // +------------+--------------+--------------+--------------+----------+----------+
    // |     2      |      20      |    U+10000   |    U+10FFFF  |  0xD800  |  0xDC00  |
    // |            |              |              |              |          |          |
    // +------------+--------------+--------------+--------------+----------+----------+
    // |     0      |      0       |    U+D800    |    U+DFFF    | INVALID  | INVALID  |
    // |  (INVALID) |  (INVALID)   |              |              |          |          |
    // +------------+--------------+--------------+--------------+----------+----------+
    // 0xxxxx表示使用对应的Unicode的编码

    // we use the replacement character(U+FFFD: UTF16: 0xFFFD：UTF8: EF BF BD)
    // for any given out of range value
    // http://unicode.scarfboy.com/?s=U%2bFFFD
    // the UTF8 of the replacement character
    static const UTF8 REPLACEMENT_CHAR_UTF8[] = { 0xEF, 0xBF, 0xBD };
    // the UTF16 of the replacement character
    static const UTF16 REPLACEMENT_CHAR_UTF16 = 0xFFFD;
    // the Unicode of the replacement character: U+FFFD
    static const UTF32 REPLACEMENT_CHAR_UTF32 = 0xFFFD;

    // 单字节UTF8编码的截止的编码
    static const UTF8 ONE_UTF8_CODE_END_CODE            = 0x7F;
    // 双字节编码的第一字节起始的编码
    static const UTF8 TWO_UTF8_CODES_FIRST_START_CODE   = 0xC0;
    // 双字节编码的第一字节截止的编码
    static const UTF8 TWO_UTF8_CODES_FIRST_END_CODE     = 0xDF;
    // 三字节编码的第一字节起始的编码
    static const UTF8 THREE_UTF8_CODES_FIRST_START_CODE = 0xE0;
    // 三字节编码的第一字节截止的编码
    static const UTF8 THREE_UTF8_CODES_FIRST_END_CODE   = 0xEF;
    // 四字节编码的第一字节起始的编码
    static const UTF8 FOUR_UTF8_CODES_FIRST_START_CODE  = 0xF0;
    // 四字节编码的第一字节截止的编码
    static const UTF8 FOUR_UTF8_CODES_FIRST_END_CODE    = 0xF7;

    // 单字节UTF8编码的截止UTF16编码: U+0000 <--> U+007F
    static const UTF16 ONE_UTF8_CODE_END_UTF16_CODE       = 0x007F;
    // 双字节UTF8编码的截止UTF16编码: U+0080 <--> U+07FF
    static const UTF16 TWO_UTF8_CODES_END_UTF16_CODE      = 0x07FF;
    // 三字节UTF8编码的截止UTF16编码: U+0800 <--> U+D7FF
    static const UTF16 THREE_UTF8_CODES_END_UTF16_CODE1   = 0xD7FF;
    // 三字节UTF8编码的起始UTF16编码: U+E000 <--> U+FFFF
    static const UTF16 THREE_UTF8_CODES_START_UTF16_CODE2 = 0xE000;
    // 三字节UTF8编码的截止UTF16编码: U+E000 <--> U+FFFF
    static const UTF16 THREE_UTF8_CODES_END_UTF16_CODE2   = 0xFFFF;
    // 四字节UTF8编码的起始UTF16编码: U+D800 <--> U+DBFF
    static const UTF16 FOUR_UTF8_CODES_START_UTF16_CODE   = 0xD800;
    // 四字节UTF8编码的截止双UTF16编码: U+10000 --> U+10FFFF
    static const UTF16 FOUR_UTF8_CODES_END_UTF16_CODE     = 0xDBFF;

    // 单字节UTF8编码的截止的Unicode codepoint
    static const UTF32 ONE_UTF8_CODE_END_UNICODE    = 0x00007F;
    // 双字节UTF8编码的截止的Unicode codepoint
    static const UTF32 TWO_UTF8_CODES_END_UNICODE   = 0x0007FF;
    // 三字节UTF8编码的截止的Unicode codepoint
    static const UTF32 THREE_UTF8_CODES_END_UNICODE = 0x00FFFF;
    // 四字节UTF8编码的截止的Unicode codepoint
    static const UTF32 FOUR_UTF8_CODES_END_UNICODE  = 0x10FFFF;

    // 无效UTF16起始的Unicode codepoint
    static const UTF32 INVALID_UTF16_CODE_START_UNICODE = 0x00D800;
    // 无效UTF16截止的Unicode codepoint
    static const UTF32 INVALID_UTF16_CODE_END_UNICODE   = 0x00DFFF;
    // 单整数UTF16的Unicode codepoint
    static const UTF32 ONE_UTF16_CODES1_END_UNICODE     = 0xD7FF;
    static const UTF32 ONE_UTF16_CODES2_START_UNICODE   = 0xE000;
    static const UTF32 ONE_UTF16_CODES2_END_UNICODE     = 0xFFFF;
    // 双整数UTF16起始的Unicode codepoint
    static const UTF32 TWO_UTF16_CODES_START_UNICODE    = 0x010000;
    // 双整数UTF16截止的Unicode codepoint
    static const UTF32 TWO_UTF16_CODES_END_UNICODE      = 0x10FFFF;



    //----------------------------------------------------------------------------------------------
    // HELPERS
    //----------------------------------------------------------------------------------------------
    // MARK: 辅助函数（Helpers）

    // Dump一行数据
    static
    void
    dumpRowInHexForm (
        const UInt8 *const mem,
        const UInt32       mem_offset,
        const UInt8        bytes_per_row,
        const Bool         use_upper_case,
        UTF8 *const        out_buf,
        UInt32 &           out_buf_offset)
    {
        // dump Hex部分
        for (UInt32 _byte_idx = 0; _byte_idx < bytes_per_row; ++_byte_idx)
        {
            // 获得当前Byte的Hex表示
            const UInt8 _cur_byte = mem[mem_offset + _byte_idx];
            const UInt16 _cur_rep = TextHelper::getByteHexForm(_cur_byte, use_upper_case);

            // 以如下形式Dump一个Byte：
            // 高4位 低4位 空格符
            *(out_buf + out_buf_offset++) = (UTF8)((_cur_rep & 0xFF00) >> 8);
            *(out_buf + out_buf_offset++) = (UTF8)((_cur_rep & 0x00FF)     );
            *(out_buf + out_buf_offset++) = SPACE_UTF8;
        }

        // dump Hex部分与ASCII部分之间的空格符
        *(out_buf + out_buf_offset++) = SPACE_UTF8;

        // dump ASCII部分
        for (UInt32 _byte_idx = 0; _byte_idx < bytes_per_row; ++_byte_idx)
        {
            const UInt8 _cur_byte = mem[mem_offset + _byte_idx];
            // dump控制符
            if (TextHelper::isCtlChar(_cur_byte))
            {
                *(out_buf + out_buf_offset++) = DOTCHAR_UTF8;
            }
            // 非控制符
            else
            {
                *(out_buf + out_buf_offset++) = (UTF8)_cur_byte;
            }

            *(out_buf + out_buf_offset++) = SPACE_UTF8;
        }

        // dump换行符
        *(out_buf + out_buf_offset++) = LINE_FEED_UNIX_UTF8;
    }


    // 获得一个Unicode对应的UTF8编码串
    //
    // @param[in] unicode
    //      Unicode编码
    // @param[in] utf8_code_ptr
    //      UTF8编码缓存的指针
    // @param[in,out] buffer_idx
    //      ->  当前UTF8缓存的位置
    //      <-  下一个UTF8缓存的位置
    static
    void
    getUTF8 (
        const UTF32 unicode,
        UTF8 *const utf8_code_ptr,
        UInt32 &    buffer_idx)
    {
        // U+0000 <--> U+007F:   单字节UTF8编码
        if (unicode <= ONE_UTF8_CODE_END_UNICODE)
        {
            utf8_code_ptr[buffer_idx++] = (UTF8)unicode;
        }
        // U+0080 <--> U+07FF:   双字节UTF8编码
        else if (unicode <= TWO_UTF8_CODES_END_UNICODE)
        {
            // |  U+0080  |  U+07FF  | 110xxxxx | 10xxxxxx
            // |          |          |  (C0-DF) | (80-BF)
            // 110xxxxx
            utf8_code_ptr[buffer_idx++] = TWO_UTF8_CODES_FIRST_START_CODE | (char)(unicode >> 6);
            // 10xxxxxx
            utf8_code_ptr[buffer_idx++] = 0x80 | (char)(unicode & 0x3F);
        }
        // U+0800 <--> U+D7FF:   三字节UTF8编码
        else if (unicode <= THREE_UTF8_CODES_END_UNICODE)
        {
            // |  U+0800  |  U+FFFF  | 1110xxxx | 10xxxxxx | 10xxxxxx
            // |          |          |  (E0-EF) | (80-BF)  | (80-BF)
            // 1110xxxx
            utf8_code_ptr[buffer_idx++] = THREE_UTF8_CODES_FIRST_START_CODE | (char)(unicode >> 12);
            // 10xxxxxx: FC0: 111111 000000
            utf8_code_ptr[buffer_idx++] = 0x80 | (char)((unicode & 0xFC0) >> 6);
            // 10xxxxxx
            utf8_code_ptr[buffer_idx++] = 0x80 | (char)(unicode & 0x3F);
        }
        // U+10000 --> U+10FFFF: 四字节UTF8编码
        else if (unicode <= FOUR_UTF8_CODES_END_UNICODE)
        {
            // |  U+10000 |  U+10FFFF| 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx
            // |          |          |  (F0-F7) | (80-BF)  | (80-BF)  | (80-BF)
            // 11110xxx
            utf8_code_ptr[buffer_idx++] = FOUR_UTF8_CODES_FIRST_START_CODE | (char)(unicode >> 18);
            // 10xxxxxx: 3F000: 111111 000000 000000
            utf8_code_ptr[buffer_idx++] = 0x80 | (char)((unicode & 0x3F000) >> 12);
            // 10xxxxxx: FC0: 111111 000000
            utf8_code_ptr[buffer_idx++] = 0x80 | (char)((unicode & 0xFC0) >> 6);
            // 10xxxxxx
            utf8_code_ptr[buffer_idx++] = 0x80 | (char)(unicode & 0x3F);
        }
        else
        {
            // 越界
            utf8_code_ptr[buffer_idx++] = REPLACEMENT_CHAR_UTF8[0];
            utf8_code_ptr[buffer_idx++] = REPLACEMENT_CHAR_UTF8[1];
            utf8_code_ptr[buffer_idx++] = REPLACEMENT_CHAR_UTF8[2];
        }
    }


    // 获得一个Unicode对应的UTF16编码
    //
    // @param[in] unicode
    //      Unicode编码
    // @param[in] utf16_code_ptr
    //      UTF16编码缓存的指针
    // @param[in,out] buffer_idx
    //      ->  当前UTF16缓存的位置
    //      <-  下一个UTF16缓存的位置
    static
    void
    getUTF16 (
        const UTF32  unicode,
        UTF16 *const utf16_code_ptr,
        UInt32 &     buffer_idx)
    {
        // 非法或者越界Unicode编码
        if ((unicode >= INVALID_UTF16_CODE_START_UNICODE &&
             unicode <= INVALID_UTF16_CODE_END_UNICODE)       ||
            unicode > TWO_UTF16_CODES_END_UNICODE)
        {
            utf16_code_ptr[buffer_idx++] = REPLACEMENT_CHAR_UTF16;
        }
        // 单整数UTF16编码: [U+0000, U+D7FF] or [U+E000, U+FFFF]
        else if (unicode < TWO_UTF16_CODES_START_UNICODE)
        {
            utf16_code_ptr[buffer_idx++] = (UTF16)unicode;
        }
        // 双整数UTF16编码: [U+10000, U+10FFFF]
        else
        {
            // 先减去0x10000
            const UTF32 _code_sub_10000  = unicode - TWO_UTF16_CODES_START_UNICODE;
            // 在0xD800上加入高10位
            utf16_code_ptr[buffer_idx++] = (UTF16)(0xD800 + ((_code_sub_10000 >> 10) & 0x3FF));
            // 在0xDC00上加入低十位
            utf16_code_ptr[buffer_idx++] = (UTF16)(0xDC00 + (_code_sub_10000 & 0x3FF));
        }
    }


    // 获得指定位置字符的Unicode
    //
    // @param[in] utf8_text_ptr
    //      UTF8字符串的指针
    // @param[in,out] char_loc
    //      ->  当前字符的位置
    //      <-  下一个字符的位置
    static
    UTF32
    getUnicode (
        const UTF8 *const utf8_text_ptr,
        UInt32 &          char_loc)
    {
        // 需要的额外UTF8编码个数：例如对于双字节UTF8编码，此值为1
        UInt8 _extra_utf8_bytes = 0;
        UTF32 _cur_unicode      = 0;
        UTF8 _cur_utf8_code     = utf8_text_ptr[char_loc++];
        while (_cur_utf8_code)
        {
            // 如果UTF8编码越界：> F7，我们使用 U+FFFD Unicode
            if (_cur_utf8_code > FOUR_UTF8_CODES_FIRST_END_CODE)
            {
                return REPLACEMENT_CHAR_UTF32;
            }
            // 读入UTF8额外的编码数据。他们的数值范围为[80-BF]：10xxxxxx
            else if (_cur_utf8_code >= 0x80 && _cur_utf8_code <= 0xBF)
            {
                if (!_extra_utf8_bytes)
                {
                    return REPLACEMENT_CHAR_UTF32;
                }

                // 10xxxxxx
                _cur_unicode |= (_cur_utf8_code & 0x3F) << (_extra_utf8_bytes - 1) * 6;
                --_extra_utf8_bytes;

                // done
                if (!_extra_utf8_bytes)
                {
                    return _cur_unicode;
                }
            }
            // 对于U+0000 <--> U+007F的Unicode，UTF8使用单字节编码。编码范围为：0x00 <--> 0x7F
            else if (_cur_utf8_code <= ONE_UTF8_CODE_END_CODE)
            {
                return _cur_utf8_code;
            }
            // 对于U+10000 <--> U+10FFFF的Unicode，UTF8使用四个字节编码。
            else if (_cur_utf8_code >= FOUR_UTF8_CODES_FIRST_START_CODE)
            {
                // |  U+10000 |  U+10FFFF| 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx
                // |          |          |  (F0-F7) | (80-BF)  | (80-BF)  | (80-BF)
                // 保存第一个编码字节：11110xxx
                _cur_unicode = (_cur_utf8_code & 0x07) << 18;

                // 我们还需要读人3个随后字节
                _extra_utf8_bytes = 3;
            }
            // 对于U+0800 <--> U+FFFF的Unicode，UTF8使用三个字节编码。
            // 其第一个字节编码范围为：1110xxxx: E0 <--> EF
            else if (_cur_utf8_code >= THREE_UTF8_CODES_FIRST_START_CODE)
            {
                // 保存第一个编码字节: 1110xxxx
                _cur_unicode = (_cur_utf8_code & 0x0F) << 12;

                // 我们还需要读人2个随后字节
                _extra_utf8_bytes = 2;
            }
            // 对于U+0080 <--> U+07FF的Unicode，UTF8使用双字节编码。
            // 其第一个字节编码范围为：110xxxxx：C0 <--> DF
            else if (_cur_utf8_code >= TWO_UTF8_CODES_FIRST_START_CODE)
            {
                // 保存第一个编码字节：110xxxxx
                _cur_unicode = (_cur_utf8_code & 0x1F) << 6;

                // 我们还需要读人1个随后字节
                _extra_utf8_bytes = 1;
            }

            // 读人下一个UTF8编码
            _cur_utf8_code =utf8_text_ptr[char_loc++];
        }

        return REPLACEMENT_CHAR_UTF32;
    }


    // 获得指定位置字符的Unicode
    //
    // @param[in] utf16_text_ptr
    //      UTF16字符串的指针
    // @param[in,out] char_loc
    //      ->  当前字符的位置
    //      <-  下一个字符的位置
    static
    UTF32
    getUnicode (
        const UTF16 *const utf16_text_ptr,
        UInt32 &           char_loc)
    {
        const UTF16 _utf16_code = utf16_text_ptr[char_loc++];
        // U+0000 <--> U+D7FF以及U+E000 <--> U+FFFF
        if (_utf16_code  <= THREE_UTF8_CODES_END_UTF16_CODE1        ||
            (_utf16_code >= THREE_UTF8_CODES_START_UTF16_CODE2 &&
             _utf16_code <= THREE_UTF8_CODES_END_UTF16_CODE2))
        {
            return _utf16_code;
        }
        // U+10000 <--> U+10FFFF
        else
        {
            // 读入随后的UTF16编码
            const UTF16 _utf16_cons_code = utf16_text_ptr[char_loc++];
            // 提取高10位
            const UInt32 _up10bits  = ((_utf16_code - 0xD800) & 0x3FF) << 10;
            // 提取低10位
            const UInt32 _low10bits = ((_utf16_cons_code - 0xDC00) & 0x3FF);
            // 合成Unicode
            return TWO_UTF16_CODES_START_UNICODE + _up10bits + _low10bits;
        }
    }



    //----------------------------------------------------------------------------------------------
    // TEXTHELPER
    //----------------------------------------------------------------------------------------------
    // MARK: - Texthelper结构中定义的函数

    const UTF8 TextHelper::BOM_UTF8[3]  =
    {
        0xEF, 0xBB, 0xBF
    };
    const UTF16 TextHelper::BOM_UTF16   = 0xFEFF;


    Bool
    TextHelper::isCtlChar (
        const ASCII ascii_code)
    {
        switch (ascii_code)
        {
            case 127: // DEL
            case 249:
            case 250:
            case 255:
            {
                return true;
            }

            default:
            {
                // [0, 31]: [NULL, UNIT SEPARATION]
                return ascii_code <= 31;
            }
        }
    }


    Bool
    TextHelper::isNumChar (
        const ASCII ascii_code)
    {
        return ascii_code >= '0' && ascii_code <= '9';
    }


    Bool
    TextHelper::isLineFeedChar (
        const ASCII ascii_code)
    {
        switch (ascii_code)
        {
            case LINE_FEED_UNIX_UTF8:
            case LINE_FEED_WINDOWS_UTF8:
            {
                return true;
            }

            default:
            {
                return false;
            }
        }
    }


    Bool
    TextHelper::isLineFeedChar (
        const UTF16 utf16_code)
    {
        switch (utf16_code)
        {
            case LINE_FEED_UNIX_UTF16:
            case LINE_FEED_WINDOWS_UTF16:
            {
                return true;
            }

            default:
            {
                return false;
            }
        }
    }


    UInt32
    TextHelper::sprintfSafe (
        UTF8 *const        buffer,
        const UInt32       size,
        const ASCII *const format,
        ...)
    {
        va_list _arg_list;
        va_start(_arg_list, format);
        // 输出字符串并且获得输出的字符个数。负值表示失败
        const SInt32 _out_count = (SInt32)std::vsnprintf((char*)buffer, size,
                                                         format,
                                                         _arg_list);
        va_end(_arg_list);
        return _out_count > 0 ? (UInt32)_out_count : 0;
    }


    UInt16
    TextHelper::getByteHexForm (
        const UInt8 byte,
        const Bool  use_upper_case)
    {
        // 获得低4bits：lower quartet: lower 4bits
        const UInt8 _lo_val =  byte & 0x0F;
        // 获得高4bit：higher quartet: upper 4bits
        const UInt8 _hi_val = (byte & 0xF0) >> 4;

        // HEX_NAMES_ASCII[]: "0123456789abcdefABCDEF";
        UInt16 _hex_rep;
        // 从表格中读取低4bit的ASCII文字表示代码
        // 使用小写输出或者输入0-9的数字
        if (!use_upper_case || _lo_val < 10)
        {
            _hex_rep = (UInt8)HEX_NAMES_ASCII[_lo_val];
        }
        // 使用大写输出并且输出10-15的数字
        else
        {
            _hex_rep = (UInt8)HEX_NAMES_ASCII[_lo_val + 6];
        }

        // 从表格中读取高4bit的ASCII文字表示代码
        // 使用小写输出或者输入0-9的数字
        if (!use_upper_case || _hi_val < 10)
        {
            _hex_rep |= (UInt8)HEX_NAMES_ASCII[_hi_val] << 8;
        }
        // 使用大写输出并且输出10-15的数字
        else
        {
            _hex_rep |= (UInt8)HEX_NAMES_ASCII[_hi_val + 6] << 8;
        }

        return _hex_rep;
    }


    void
    TextHelper::dumpMemInHexForm (
        const UInt8 *const mem,
        const UInt32       data_size,
        const UInt8        bytes_per_row,
        const Bool         use_upper_case,
        RuntimeText &      hex_text)
    {
        RUNTIME_ASSERT(mem != nullptr, "Memory pointer is null pointer");
        RUNTIME_ASSERT(data_size != 0, "Zero data size"                );

        if (mem == nullptr || data_size == 0)
        {
            return;
        }

        // 我们使用如下的格式Dump：
        // FF_41_BB_43__._A_._C\n
        // 注意其中_符号表示空格符
        // 各部分占用如下的字符数：
        // Hex部分：例如
        // FF_ <--- FF以及空格: 2 + 1 = 3个字符
        // ASCII部分： 例如
        // ._  <--- 点以及空格: 1 + 1 = 2个字符

        // Hex部分占用的字符数
        static const UInt8 HEX_PART_CHAR_COUNT   = 2 + 1;
        // ASCII部分占用的字符数
        static const UInt8 ASCII_PART_CHAR_COUNT = 1 + 1;
        // 每行中其它字符的数目: Hex部分与ASCII部分之间的一个空格符，换行符
        static const UInt8 ROW_EXTRA_CHAR_COUNT  = 1 + 1;
        // 每一字节所需要的字符数
        static const UInt8 CHAR_COUNT_PER_BYTE   = HEX_PART_CHAR_COUNT + ASCII_PART_CHAR_COUNT;

        // 计算输出的行数：完整行
        const UInt16 _row_count = data_size / bytes_per_row;
        // 计算剩余的字节数
        const UInt8 _rest_bytes = data_size % bytes_per_row;
        // 每一行所需要的字符数目：每行可显示的Byte数*每Byte需要的字符数 + 每行额外的字符数
        const UInt16 _char_count_per_row = bytes_per_row*CHAR_COUNT_PER_BYTE + ROW_EXTRA_CHAR_COUNT;
        // 计算所有文字需要的字符数
        // NOTE: 如果有Rest，我们还需要Hex部分与ASCII部分之间的空格符以及换行符
        const UInt32 _total_char_count = _row_count         * _char_count_per_row +
                                         _rest_bytes        * CHAR_COUNT_PER_BYTE +
                                         (_rest_bytes != 0) * ROW_EXTRA_CHAR_COUNT;
        INFO(RuntimeText, "Total characters needed for the buffer: %d", _total_char_count);

        // 开始输出
        UInt32 _mem_offset;
        UInt32 _cur_char_idx = 0;
        hex_text.resize(_total_char_count);

        // dumps完整行
        for (UInt16 _row_idx = 0; _row_idx < _row_count; ++_row_idx)
        {
            _mem_offset = _row_idx * bytes_per_row;
            dumpRowInHexForm(mem, _mem_offset,
                             bytes_per_row,
                             use_upper_case,
                             const_cast<UTF8*>(hex_text.data()), _cur_char_idx);
        }

        // dumps余下的Bytes
        _mem_offset = _row_count * bytes_per_row;
        dumpRowInHexForm(mem, _mem_offset,
                         _rest_bytes,
                         use_upper_case,
                         const_cast<UTF8*>(hex_text.data()), _cur_char_idx);
    }


    UInt8
    TextHelper::getUTF8CodeLength (
        const UTF8 first_byte)
    {
        // [0， 0x7F]
        if (first_byte <= ONE_UTF8_CODE_END_CODE)
        {
            return 1;
        }
        // [0xC0, 0xDF]
        else if (first_byte >= TWO_UTF8_CODES_FIRST_START_CODE &&
                 first_byte <= TWO_UTF8_CODES_FIRST_END_CODE)
        {
            return 2;
        }
        // [0xE0, 0xEF]
        else if (first_byte >= THREE_UTF8_CODES_FIRST_START_CODE &&
                 first_byte <= THREE_UTF8_CODES_FIRST_END_CODE)
        {
            return 3;
        }
        // [0xF0, 0xF7]
        else if (first_byte >= FOUR_UTF8_CODES_FIRST_START_CODE &&
                 first_byte <= FOUR_UTF8_CODES_FIRST_END_CODE)
        {
            return 4;
        }
        else
        {
            RUNTIME_ASSERT(false, "Wrong first byte of a UTF8 encoding");
            return 0;
        }
    }


    UInt32
    TextHelper::getUTF8TextLength (
        const UTF16 *const utf16_text)
    {
        UInt32 _cur_utf16_idx = 0;
        UInt32 _utf8_text_len = 0;
        UTF16 _cur_utf16_code = utf16_text[_cur_utf16_idx++];
        // 处理当前UTF16编码
        while (_cur_utf16_code)
        {
            // --- 单整数UTF16编码 --- //

            // UTF8单字节编码
            if (_cur_utf16_code <= ONE_UTF8_CODE_END_UTF16_CODE)
            {
                ++_utf8_text_len;
            }
            // UTF8两字节编码
            else if (_cur_utf16_code <= TWO_UTF8_CODES_END_UTF16_CODE)
            {
                _utf8_text_len += 2;
            }
            // UTF8三字节编码
            else if (_cur_utf16_code  <= THREE_UTF8_CODES_END_UTF16_CODE1      ||
                     (_cur_utf16_code >= THREE_UTF8_CODES_START_UTF16_CODE2 &&
                      _cur_utf16_code <= THREE_UTF8_CODES_END_UTF16_CODE2))
            {
                _utf8_text_len += 3;
            }

            // --- 双整数UTF16编码 --- //

            // 四字节UTF8编码
            else if (_cur_utf16_code >= FOUR_UTF8_CODES_START_UTF16_CODE &&
                     _cur_utf16_code <= FOUR_UTF8_CODES_END_UTF16_CODE)
            {
                _utf8_text_len += 4;
                ++_cur_utf16_idx;
            }
            else
            {
                RUNTIME_ASSERT(false,
                               "Reading consecutive bytes, but its byte count is not set");
            }

            // 读入下一个UTF16编码
            _cur_utf16_code = utf16_text[_cur_utf16_idx++];
        }

        return _utf8_text_len;
    }


    UInt32
    TextHelper::getUTF8TextLength (
        const UTF32 *const unicode_text)
    {
        UInt32 _cur_unicode_idx = 0;
        UInt32 _utf8_text_len   = 0;
        UTF32 _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        // 处理当前Unicode
        while (_cur_unicode_code)
        {
            // 越界
            if (_cur_unicode_code > FOUR_UTF8_CODES_END_UNICODE)
            {
                _utf8_text_len += sizeof(REPLACEMENT_CHAR_UTF8);
            }
            // 可用单字节UTF8编码表示
            else if (_cur_unicode_code <= ONE_UTF8_CODE_END_UNICODE)
            {
                ++_utf8_text_len;
            }
            // 可用双字节UTF8编码表示
            else if (_cur_unicode_code <= TWO_UTF8_CODES_END_UNICODE)
            {
                _utf8_text_len += 2;
            }
            // 可用三字节UTF8编码表示
            else if (_cur_unicode_code <= THREE_UTF8_CODES_END_UNICODE)
            {
                _utf8_text_len += 3;
            }
            // 可用四字节UTF8编码表示
            else
            {
                _utf8_text_len += 4;
            }

            // 读入下一个Unicode
            _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        }

        return _utf8_text_len;
    }


    UInt32
    TextHelper::getUTF16TextLength (
        const UTF8 *const utf8_text)
    {
        RUNTIME_ASSERT((utf8_text != nullptr) && (utf8_text[0] != 0),
                       "Empty UTF8 text");

        UInt32 _cur_utf8_idx   = 0;
        UInt32 _utf16_text_len = 0;
        // 需要的额外UTF8编码个数：例如对于双字节UTF8编码，此值为1
        UInt8 _extra_utf8_bytes = 0;
        UTF8 _cur_utf8_code = utf8_text[_cur_utf8_idx++];
        // 处理当前UTF8编码
        while (_cur_utf8_code)
        {
            // 如果UTF8编码越界或者为单字节编码：> F7 || <= 7F
            // NOTE: 对于越界UTF8编码我们使用0xFFFD UTF16编码
            if (_cur_utf8_code >  FOUR_UTF8_CODES_FIRST_END_CODE ||
                _cur_utf8_code <= ONE_UTF8_CODE_END_CODE)
            {
                _extra_utf8_bytes = 0;
                ++_utf16_text_len;
            }
            // --- UTF8编码在(80, F7]之间 --- //

            // UTF8四个字节编码：U+10000 <--> U+10FFFF
            else if (_cur_utf8_code >= FOUR_UTF8_CODES_FIRST_START_CODE)
            {
                _extra_utf8_bytes = 3;
                // 使用双整数UTF16编码
                _utf16_text_len  += 2;
            }
            // UTF8三个字节编码：U+0800 <--> U+FFFF
            else if (_cur_utf8_code >= THREE_UTF8_CODES_FIRST_START_CODE)
            {
                _extra_utf8_bytes = 2;
                // 对于位于[U+D800, U+DFFF]范围内的Unicode，
                // 虽然他们具有UTF8编码， 但是却没有相应的UTF16编码。此时我们使用0xFFFD UTF16编码
                ++_utf16_text_len;
            }
            // UTF8双字节编码：U+0080 <--> U+07FF
            else if (_cur_utf8_code >= TWO_UTF8_CODES_FIRST_START_CODE)
            {
                _extra_utf8_bytes = 1;
                ++_utf16_text_len;
            }
            // 读入UTF8额外的编码数据。他们的数值范围为[80-BF]：10xxxxxx
            else
            {
                if (_extra_utf8_bytes)
                {
                    --_extra_utf8_bytes;
                }
                else
                {
                    RUNTIME_ASSERT(false,
                                   "Reading consecutive bytes, but its byte count is not set");
                }
            }

            // 读入下一个UTF8编码
            _cur_utf8_code = (unsigned char)utf8_text[_cur_utf8_idx++];
        }

        return _utf16_text_len;
    }


    UInt32
    TextHelper::getUTF16TextLength (
        const UTF32 *const unicode_text)
    {
        UInt32 _cur_unicode_idx = 0;
        UInt32 _utf16_text_len  = 0;
        UTF32 _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        // 处理当前Unicode
        while (_cur_unicode_code)
        {
            // [U+0000, U+D7FF] [U+E000, U+FFFF]
            if (_cur_unicode_code  <= ONE_UTF16_CODES1_END_UNICODE      ||
                (_cur_unicode_code >= ONE_UTF16_CODES2_START_UNICODE &&
                 _cur_unicode_code <= ONE_UTF16_CODES2_END_UNICODE))
            {
                ++_utf16_text_len;
            }
            // [U+10000, U+10FFFF]
            else if (_cur_unicode_code >= TWO_UTF16_CODES_START_UNICODE &&
                     _cur_unicode_code <= TWO_UTF16_CODES_END_UNICODE)
            {
                _utf16_text_len += 2;
            }

            // 读入下一个Unicode
            _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        }

        return _utf16_text_len;
    }


    UInt32
    TextHelper::getUnicodeTextLength (
        const UTF8 *const utf8_text)
    {
        RUNTIME_ASSERT(utf8_text != nullptr && utf8_text[0] != 0,
                       "Empty UTF8 text");

        UInt32 _cur_utf8_idx     = 0;
        UInt32 _unicode_text_len = 0;
        // 需要的额外UTF8编码个数：例如对于双字节UTF8编码，此值为1
        UInt8 _extra_utf8_bytes  = 0;
        UTF8 _cur_utf8_code = utf8_text[_cur_utf8_idx++];
        while (_cur_utf8_code)
        {
            // 如果UTF8编码越界或者为单字节编码：> F7 || <= 7F
            // NOTE: 对于越界UTF8编码我们使用 U+FFFD Unicode
            if (_cur_utf8_code >  FOUR_UTF8_CODES_FIRST_END_CODE ||
                _cur_utf8_code <= ONE_UTF8_CODE_END_CODE)
            {
                _extra_utf8_bytes = 0;
                ++_unicode_text_len;
            }
            // UTF8四个字节编码：U+10000 <--> U+10FFFF
            else if (_cur_utf8_code >= FOUR_UTF8_CODES_FIRST_START_CODE)
            {
                _extra_utf8_bytes = 3;
                ++_unicode_text_len;
            }
            // UTF8三个字节编码：U+0800 <--> U+FFFF
            else if (_cur_utf8_code >= THREE_UTF8_CODES_FIRST_START_CODE)
            {
                _extra_utf8_bytes = 2;
                ++_unicode_text_len;
            }
            // UTF8双字节编码：U+0080 <--> U+07FF
            else if (_cur_utf8_code >= TWO_UTF8_CODES_FIRST_START_CODE)
            {
                _extra_utf8_bytes = 1;
                ++_unicode_text_len;
            }
            // 读入UTF8额外的编码数据。他们的数值范围为[80-BF]：10xxxxxx
            else
            {
                if (_extra_utf8_bytes)
                {
                    --_extra_utf8_bytes;
                }
                else
                {
                    RUNTIME_ASSERT(false,
                                   "Reading consecutive bytes, but its byte count is not set");
                }
            }

            // 读入下一个UTF8编码
            _cur_utf8_code = utf8_text[_cur_utf8_idx++];
        }

        return _unicode_text_len;
    }


    void
    TextHelper::convertUTF16ToUTF8 (
        const UTF16 *const utf16_text,
        UTF8 *const        conv_utf8_text)
    {
        UInt32 _cur_utf8_idx  = 0;
        UInt32 _cur_utf16_idx = 0;
        while (utf16_text[_cur_utf16_idx])
        {
            // 先将当前的UTF16编码转换为Unicode
            const UTF32 _cur_unicode_code = getUnicode(utf16_text, _cur_utf16_idx);

            // 然后将Unicode转换为UTF8
            getUTF8(_cur_unicode_code, conv_utf8_text, _cur_utf8_idx);
        }
    }


    void
    TextHelper::convertUnicodeToUTF8 (
        const UTF32 *const unicode_text,
        UTF8 *const        conv_utf8_text)
    {
        UInt32 _cur_utf8_idx    = 0;
        UInt32 _cur_unicode_idx = 0;
        UTF32 _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        while (_cur_unicode_code)
        {
            getUTF8(_cur_unicode_code, conv_utf8_text, _cur_utf8_idx);

            // 读入下一个Unicode
            _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        }
    }


    void
    TextHelper::convertUnicodetoUTF16 (
        const UTF32 *const unicode_text,
        UTF16 *const       conv_utf16_text)
    {
        UInt32 _cur_utf16_idx   = 0;
        UInt32 _cur_unicode_idx = 0;
        UTF32 _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        while (_cur_unicode_code)
        {
            getUTF16(_cur_unicode_code, conv_utf16_text, _cur_utf16_idx);

            // 读入下一个Unicode
            _cur_unicode_code = unicode_text[_cur_unicode_idx++];
        }
    }


    void
    TextHelper::convertUTF8ToUTF16 (
        const UTF8 *const utf8_text,
        UTF16 *const      conv_utf16_text)
    {
        UInt32 _cur_utf16_idx = 0;
        UInt32 _cur_utf8_idx  = 0;
        while (utf8_text[_cur_utf8_idx])
        {
            // 先将当前的UTF8编码转换为Unicode
            const UTF32 _unicode = getUnicode(utf8_text, _cur_utf8_idx);

            // 然后将Unicode转换为UTF16
            getUTF16(_unicode, conv_utf16_text, _cur_utf16_idx);
        }
    }


    void
    TextHelper::convertUTF8ToUnicode (
        const UTF8 *const utf8_text,
        UTF32 *const      conv_unicode_text)
    {
        UInt32 _cur_utf8_idx    = 0;
        UInt32 _cur_unicode_idx = 0;
        while (utf8_text[_cur_utf8_idx])
        {
            conv_unicode_text[_cur_unicode_idx++] = getUnicode(utf8_text, _cur_utf8_idx);
        }
    }

} // namespace Core
