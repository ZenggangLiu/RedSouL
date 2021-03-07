// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/String/RS_TextHelper.hpp"
// Self
#include "Core/String/RS_RuntimeText.hpp"


namespace Core
{

    //----------------------------------------------------------------------------------------------
    // RUNTIMETEXT
    //----------------------------------------------------------------------------------------------
    // MARK: - RuntimeText

    RuntimeText::RuntimeText ()
    :
        SUPER()
    {

    }


    RuntimeText::RuntimeText (
        const UInt32 text_length,
        const UTF8   filled_with /* = 'c' */)
    :
        SUPER(filled_with, text_length)
    {
        // TODO：Allocate text_length+1 bytes to accomodate the UTF8 encoded string
    }


    RuntimeText::RuntimeText (
        const char *const ascii_text)
    :
        SUPER((const UTF8*)ascii_text)
    {

    }


    RuntimeText::RuntimeText (
        const UTF8 *const utf8_text)
    :
        SUPER(utf8_text)
    {

    }


    RuntimeText::RuntimeText (
        const UTF16 *const utf16_text)
    {
        // https://codepoints.net
        // 所有的汉字都在这个的范围：U+2E80 --> U+FF65
        // 对于的字符为：⺀--> ･
        //      編: U+7DE8 : UTF8: E7 B7 A8     UTF16: 7D E8
        //      碼: U+78BC : UTF8: E7 A2 BC     UTF16: 78 BC
        //      测: U+6D4B : UTF8: E6 B5 8B     UTF16: 6D 4B
        //      试: U+8BD5 : UTF8: E8 AF 95     UTF16: 8B D5
        // 東風：🀀: u+1F000: UTF8: F0 9F 80 80  UTF16: D8 3C DC 00
        // 發：  🀅: U+1F005: UTF8: F0 9F 80 85  UTF16: D8 3C DC 05

        // 获得UTF8编码的长度
        const UInt32 _utf8_text_len = TextHelper::getUTF8TextLength(utf16_text);

        // Resize字符串
        TODO("We need to alloc _utf8_text_len+1 bytes, and insert the terminating ZERO");
        resize(_utf8_text_len);

        // 将UTF16编码的文字转换到UTF8
        TextHelper::convertUTF16ToUTF8(utf16_text, const_cast<UTF8*>(data()));
    }


    RuntimeText::RuntimeText (
        const UTF32 *const unicode_text)
    {
        // 所有的汉字都在这个的范围：U+2E80 --> U+FF65
        // 对于的字符为：⺀--> ･
        //      編: U+7DE8 : UTF8: E7 B7 A8     UTF16: 7D E8
        //      碼: U+78BC : UTF8: E7 A2 BC     UTF16: 78 BC
        //      测: U+6D4B : UTF8: E6 B5 8B     UTF16: 6D 4B
        //      试: U+8BD5 : UTF8: E8 AF 95     UTF16: 8B D5
        // 東風：🀀: u+1F000: UTF8: F0 9F 80 80  UTF16: D8 3C DC 00
        // 發：  🀅: U+1F005: UTF8: F0 9F 80 85  UTF16: D8 3C DC 05

        // 获得UTF8编码的长度
        const UInt32 _utf8_text_len = TextHelper::getUTF8TextLength(unicode_text);

        // Resize字符串
        TODO("We need to alloc _utf8_text_len+1 bytes, and insert the terminating ZERO");
        resize(_utf8_text_len);

        // 将Unicode编码的文字转换到UTF8
        TextHelper::convertUnicodeToUTF8(unicode_text, const_cast<UTF8*>(data()));
    }

} // namespace Core
