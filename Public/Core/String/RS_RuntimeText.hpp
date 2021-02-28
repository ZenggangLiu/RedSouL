#pragma once


// System headers
#include <string>
// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    class RuntimeText;


    struct TextHelper
    {
        // 检测给定ASCII编码是否控制符编码：不可见字符
        static
        Bool
        isCtlChar (
            const UInt8 ascii_code);

        // 检测给定ASCII编码是否为0到9数字的编码
        static
        Bool
        isNumChar (
            const UInt8 ascii_code);

        // 检测给定ASCII编码是否为换行符
        static
        Bool
        isLineFeedChar (
            const UInt8 ascii_code);

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
        // Core::TextHelper::dumpMemInHexForm((UInt8*)array, sizeof(array), 4, true, _dump);
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
        // 获得UTF8编码串的长度：不包括结尾的‘\0'
        //
        // @param[in] utf16_text
        //      UTF16编码串
        static
        UInt32
        getUTF8TextLen (
            const UTF16 *const utf16_text);

        // 获得UTF8编码串的长度：不包括结尾的‘\0'
        //
        // @param[in] unicode_text
        //      UTF32编码串
        static
        UInt32
        getUTF8TextLen (
            const UTF32 *const unicode_text);

        // 获得UTF16编码串的长度：不包括结尾的‘\0'
        //
        // @param[in] utf8_text
        //      UTF8编码串
        static
        UInt32
        getUTF16TextLen (
            const UTF8 *const utf8_text);

        // 获得Unicode字符串的长度：不包括结尾的‘\0'
        //
        // @param[in] utf8_text
        //      UTF8编码串
        static
        UInt32
        getUnicodeTextLen (
            const UTF8 *const utf8_text);

        // 将给定UTF16编码串转换为UTF8编码串
        // NOTE:
        // 1. 此函数不转换结尾的‘\0'
        // 2. conv_utf8_text必须足够大。请使用如下方法获得conv_utf8_text的长度：
        //    conv_utf8_text的长度 = getUTF8TextLen()
        //
        // @param[in]  utf16_text
        //      UTF16编码串
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
        //    conv_utf8_text的长度 = getUTF8TextLen()
        //
        // @param[in]  unicode_text
        //      Unicode字符串
        // @param[out] conv_utf8_text
        //      转换的UTF8编码串
        static
        void
        convertUnicodeToUTF8 (
            const UTF32 *const unicode_text,
            UTF8 *const        conv_utf8_text);

        // 将给定UTF8编码串转换为UTF16编码串
        // NOTE:
        // 1. 此函数不转换结尾的‘\0'
        // 2. conv_utf16_text必须足够大。请使用如下方法获得conv_utf16_text的长度：
        //    conv_utf16_text的长度 = getUTF16TextLen()
        //
        // @param[in]  utf8_text
        //      UTF8编码串
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
        //    conv_unicode_text的长度 = getUnicodeTextLen()
        //
        // @param[in]  utf8_text
        //      UTF8编码串
        // @param[out] conv_unicode_text
        //      转换的Uncide字符串
        static
        void
        convertUTF8ToUnicode (
            const UTF8 *const utf8_text,
            UTF32 *const      conv_unicode_text);

    };


    // UTF8编码字符串
    class RuntimeText : public std::basic_string< UTF8, std::char_traits<UTF8>>
    {
    public:
        // 构建一个空字符串
        RuntimeText ();

        // 构建一个指定长度的字符串
        // @param[in] text_length
        //      字符串的长度：不包括结尾'\0'
        // @param[in] filled_with
        //      使用何字符来充填此字符串
        RuntimeText (
            const UInt32 text_length,
            const UTF8   filled_with = 'c');

        // 使用给定的ASCII字符串来构建一个字符串
        RuntimeText (
            const char *const ascii_text);

        // 使用给定的UTF8编码的字符串构建一个字符串
        RuntimeText (
            const UTF8 *const utf8_text);

        // 使用给定的UTF16编码的字符串构建一个字符串
        // NOTE:
        // 1. 使用u"xyz..."来创建一个UTF16编码的字符串(小写U): u"編碼测试"
        // 2. 如果要保存含有UTF16字符串的文件，我们必须使用BOM。其UTF8编码为: 0xEF,0xBB,0xBF
        //    否则微软编译器将视此文件为ASCII文件。
        //    https://en.wikipedia.org/wiki/Byte_order_mark#UTF-8
        RuntimeText (
            const UTF16 *const utf16_text);

        // 使用给定的Unicode编码的字符串构建一个字符串
        // NOTE:
        // - 1. 使用U"xyz..."来创建一个Unicode编码的字符串(大写U)：U"編碼测试"
        // 2. 如果要保存含有UTF16字符串的文件，我们必须使用带有BOM。其UTF8编码为: 0xEF,0xBB,0xBF
        //    否则微软编译器将视此文件为ASCII文件。
        //    https://en.wikipedia.org/wiki/Byte_order_mark#UTF-8
        RuntimeText (
            const UTF32 *const unicode_text);

        // 获得此字符串的UTF8编码串
        const UTF8*
        c_str () const;


    private:
        using SUPER = std::basic_string< UTF8, std::char_traits<UTF8> >;
    };

} // namespace
