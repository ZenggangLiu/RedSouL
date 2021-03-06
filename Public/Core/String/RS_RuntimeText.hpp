#pragma once


// System headers
#include <string>
// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // UTF8编码字符串
    class RuntimeText : public std::basic_string< UTF8, std::char_traits<UTF8> >
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


    private:
        using SUPER = std::basic_string< UTF8, std::char_traits<UTF8> >;
    };

} // namespace
