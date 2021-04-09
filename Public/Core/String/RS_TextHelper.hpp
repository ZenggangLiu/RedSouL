#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    class RuntimeText;


    // 字符串编码
    enum TextEncodings
    {
        // UTF8编码
        TEXT_ENCODINGS_UTF8,
        // 带有BOM的UTF8编码
        TEXT_ENCODINGS_UTF8_BOM,
        // UTF16 Big Endianess编码
        TEXT_ENCODINGS_UTF16_BE,
        // UTF16 Little Endianess编码
        TEXT_ENCODINGS_UTF16_LE,

        TEXT_ENCODINGS_MAX
    };


    // 字符串处理辅助函数
    struct TextHelper
    {
        // BOM(U+FEFF)的UTF8编码：EF BB BF
        static const UTF8 BOM_UTF8[3];

        // BOM(U+FEFF)的UTF16编码
        static const UTF16 BOM_UTF16;

        // 检测给定ASCII编码是否控制符编码：不可见字符
        static
        Bool
        isCtlChar (
            const ASCII ascii_code);

        // 检测给定ASCII编码是否为0到9数字的编码
        static
        Bool
        isNumChar (
            const ASCII ascii_code);

        // 检测给定ASCII编码是否为换行符
        static
        Bool
        isLineFeedChar (
            const ASCII ascii_code);

        // 检测给定UTF16编码是否为换行符
        static
        Bool
        isLineFeedChar (
            const UTF16 utf16_code);

        // 使用给定Format来生成文字
        //
        // @param[out] buffer
        //      输出的Buffer
        // @param[in]  size
        //      Buffer的大小
        // @param[in]  format
        //      ASCII字符串定义输出格式：参考printf()
        // @return
        //      返回输出的字符个数
        static
        UInt32
        sprintfSafe (
            UTF8 *const        buffer,
            const UInt32       size,
            const ASCII *const format,
            ...);

        // 获得一个字节十六进制(Hexadecimal)ASCII表示
        // 例如: 对于给定的一个字节为0xAB, 我们将得到'A''B' := 0x4142
        //
        // @param[in] byte
        //      8Bits数值
        // @param[in] use_upper_case
        //      TRUE , 返回大写ASCII代码
        //      FALSE, 返回小写ASCII代码
        static
        UInt16
        getByteHexForm (
            const UInt8 byte,
            const Bool  use_upper_case);

        // 使用十六进制Dump一块内存的内容
        //
        // @param[in] mem
        //      内存的指针
        // @param[in] data_size
        //      数据的大小
        // @param[in] bytes_per_row
        //      每行要显示多少字节
        // @param[in] use_upper_case
        //     TRUE , 如果使用大写Hex
        //     FALSE, 如果使用小写Hex
        // @param[out] hex_text
        //      内存数据的Hex形式
        // 范例:
        // UInt8 array[] = {0xAB, 0xCD, 0xEF, 0x11, 0x22, 0x41, 0x42};
        // Core::RuntimeText _dump;
        // Core::TextHelper::dumpMemInHexForm(array, sizeof(array), 4, true, _dump);
        static
        void
        dumpMemInHexForm (
            const UInt8 *const mem,
            const UInt32       data_size,
            const UInt8        bytes_per_row,
            const Bool         use_upper_case,
            RuntimeText &      hex_text);


        // --- UNICODE 函数 --- //
        // Unicode使用32位整数表示一个编码(Code Point)，所以Unicode也叫做UTF32
        // 其Codepoint表示为：U+AAAABBBB
        // UTF16编码使用1到2个16位整数来表示一个Unicode的Codepoint
        // UTF8编码使用1，2，3或者4个字节来一个Unicode的Codepoint
        //
        // 获得UTF8编码的字节长度
        //
        // @param[in] first_byte
        //      UTF8编码的第一字节
        static
        UInt8
        getUTF8CodeLength (
            const UTF8 first_byte);

        // 获得UTF8编码串的长度：不包括结尾的‘\0'
        //
        // @param[in] utf16_text
        //      以‘\0'结尾的UTF16编码串
        static
        UInt32
        getUTF8TextLength (
            const UTF16 *const utf16_text);

        // 获得UTF8编码串的长度：不包括结尾的‘\0'
        //
        // @param[in] unicode_text
        //      以‘\0'结尾的UTF32编码串
        static
        UInt32
        getUTF8TextLength (
            const UTF32 *const unicode_text);

        // 获得UTF16编码串的长度：不包括结尾的‘\0'
        //
        // @param[in] utf8_text
        //      以‘\0'结尾的UTF8编码串
        static
        UInt32
        getUTF16TextLength (
            const UTF8 *const utf8_text);

        // 获得UTF16编码串的长度：不包括结尾的‘\0'
        //
        // @param[in] unicode_text
        //      以‘\0'结尾的Unicode编码串
        static
        UInt32
        getUTF16TextLength (
            const UTF32 *const unicode_text);

        // 获得Unicode字符串的长度：不包括结尾的‘\0'
        //
        // @param[in] utf8_text
        //      以‘\0'结尾的UTF8编码串
        static
        UInt32
        getUnicodeTextLength (
            const UTF8 *const utf8_text);

        // 将给定UTF16编码串转换为UTF8编码串
        // NOTE:
        // 1. 此函数不转换结尾的‘\0'
        // 2. conv_utf8_text必须足够大。请使用如下方法获得conv_utf8_text的长度：
        //    conv_utf8_text的长度 = getUTF8TextLength()
        //
        // @param[in]  utf16_text
        //      以‘\0'结尾的UTF16编码串
        // @param[out] conv_utf8_text
        //      转换的UTF8编码串
        static
        void
        convertUTF16ToUTF8 (
            const UTF16 *const utf16_text,
            UTF8 *const        conv_utf8_text);

        // 将给定Unicode字符串转换为UTF8编码串
        // NOTE:
        // 1. 此函数不转换结尾的‘\0'
        // 2. conv_utf8_text必须足够大。请使用如下方法获得conv_utf8_text的长度：
        //    conv_utf8_text的长度 = getUTF8TextLength()
        //
        // @param[in]  unicode_text
        //      以‘\0'结尾的Unicode字符串
        // @param[out] conv_utf8_text
        //      转换的UTF8编码串
        static
        void
        convertUnicodeToUTF8 (
            const UTF32 *const unicode_text,
            UTF8 *const        conv_utf8_text);

        // 将给定Unicode编码串转换为UTF16编码串
        // NOTE:
        // 1. 此函数不转换结尾的‘\0'
        // 2. conv_utf16_text必须足够大。请使用如下方法获得conv_utf16_text的长度：
        //    conv_utf16_text的长度 = getUTF16TextLength()
        //
        // @param[in]  unicode_text
        //      以‘\0'结尾的Unicode字符串6
        // @param[out] conv_utf16_text
        //      转换的UTF16编码串
        static
        void
        convertUnicodetoUTF16 (
            const UTF32 *const unicode_text,
            UTF16 *const       conv_utf16_text);

        // 将给定UTF8编码串转换为UTF16编码串
        // NOTE:
        // 1. 此函数不转换结尾的‘\0'
        // 2. conv_utf16_text必须足够大。请使用如下方法获得conv_utf16_text的长度：
        //    conv_utf16_text的长度 = getUTF16TextLength()
        //
        // @param[in]  utf8_text
        //      以‘\0'结尾的UTF8编码串
        // @param[out] conv_utf16_text
        //      转换的UTF16编码串
        static
        void
        convertUTF8ToUTF16 (
            const UTF8 *const utf8_text,
            UTF16 *const      conv_utf16_text);

        // 将给定UTF8编码串转换为Unicode字符串
        // NOTE:
        // 1. 此函数不转换结尾的‘\0'
        // 2. conv_unicode_text必须足够大。请使用如下方法获得conv_unicode_text的长度：
        //    conv_unicode_text的长度 = getUnicodeTextLength()
        //
        // @param[in]  utf8_text
        //      以‘\0'结尾的UTF8编码串
        // @param[out] conv_unicode_text
        //      转换的Uncide字符串
        static
        void
        convertUTF8ToUnicode (
            const UTF8 *const utf8_text,
            UTF32 *const      conv_unicode_text);
    };

} // namespace Core
