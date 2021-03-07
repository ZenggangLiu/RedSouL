// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/String/RS_TextHelper.hpp"
// Self
#include "Core/String/RS_UTF16Text.hpp"


namespace Core
{

    //----------------------------------------------------------------------------------------------
    // UTF16Text
    //----------------------------------------------------------------------------------------------
    // MARK: - UTF16Text

    UTF16Text::UTF16Text ()
    :
        SUPER()
    {

    }


    UTF16Text::UTF16Text (
        const UInt32 text_length,
        const UTF16  filled_with /* = u'空' */)
    :
        SUPER(filled_with, text_length)
    {
        // TODO：Allocate text_length+1 bytes to accomodate the UTF8 encoded string
    }


    UTF16Text::UTF16Text (
        const char *const ascii_text)
    {
        UTF16 _cur_char;
        for (UInt32 _idx = 0; _idx < (UInt32)std::strlen(ascii_text); ++_idx)
        {
            _cur_char = ascii_text[_idx];
            push_back(_cur_char);
        }
    }


    UTF16Text::UTF16Text (
        const UTF8 *const utf8_text)
    {
        const UInt32 _utf16_text_len = TextHelper::getUTF16TextLength(utf8_text);
        TODO("We need to alloc _utf16_text_len+1 bytes, and insert the terminating ZERO");
        resize(_utf16_text_len);
        TextHelper::convertUTF8ToUTF16(utf8_text, const_cast<UTF16*>(data()));
    }


    UTF16Text::UTF16Text (
        const UTF16 *const utf16_text)
    :
        SUPER(utf16_text)
    {

    }


    UTF16Text::UTF16Text (
        const UTF32 *const unicode_text)
    {
        const UInt32 _utf16_text_len = TextHelper::getUTF16TextLength(unicode_text);
        TODO("We need to alloc _utf16_text_len+1 bytes, and insert the terminating ZERO");
        resize(_utf16_text_len);

        // 将Unicode编码的文字转换到UTF16
        TextHelper::convertUnicodetoUTF16(unicode_text, const_cast<UTF16*>(data()));
    }

} // namespace Core
