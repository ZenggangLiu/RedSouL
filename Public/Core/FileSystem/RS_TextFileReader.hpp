#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/FileSystem/RS_FileReadStream.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"
#include "Core/String/RS_TextHelper.hpp"


namespace Core
{

    class UTF16Text;


    // 字符文件读入器：提供UTF8/16编码的字符串文件的输入
    // NOTE: 我们只支持Basic Multilingual Plane上的Unicode: [U+0000, U+FFFF]，
    //       因为他们对应单整数UTF16编码
    class TextFileReader
    {
    public:
        // 创建一个字符文件读入器
        //
        // @param[in] rel_path
        //      相对路径
        // @param[in] search_dir
        //      定义相对哪个目录进行操作
        TextFileReader (
            const UTF8 *const                   rel_path,
            const NativeFileSystem::SearchPaths search_dir);

        // 释放一个字符文件读入器
        ~TextFileReader ();

        // 检查此读入器是否有效
        Bool
        isValid () const;

        // 检查是否当前读入器已经到达文件流的结尾
        Bool
        isEndOfStream () const;

        // Peek文件流中的下一个字符, 并且返回它的UTF8编码
        // NOTE: 此函数不会移动流的当前位置
        //
        // @param[out] utf8_code
        //      UTF8数组的指针
        // @return
        //      TRUE    如果读入成功
        //      FALSE   如果读入失败或者已经到达文件流的尾部
        Bool
        peekNextChar (
            UTF8 (*utf8_code)[4]);

        // Peek文件流中的下一个字符, 并且返回它的UTF16编码
        // NOTE: 此函数不会移动流的当前位置
        //
        // @param[out] utf16_char
        //      读入的UTF16编码的字符
        // @return
        //      TRUE    如果读入成功
        //      FALSE   如果读入失败或者已经到达文件流的尾部
        Bool
        peekNextChar (
            UTF16 & utf16_char);

        // 读入文件流中的下一个字符, 并且返回它的UTF8编码
        //
        // @param[out] utf8_code
        //      UTF8数组的指针
        // @return
        //      TRUE    如果读入成功
        //      FALSE   如果读入失败或者已经到达文件流的尾部
        Bool
        readNextChar (
            UTF8 (*utf8_code)[4]);

        // 读入文件流中的下一个字符, 并且返回它的UTF16编码
        //
        // @param[out] utf16_char
        //      读入的UTF16编码的字符
        // @return
        //      TRUE    如果读入成功
        //      FALSE   如果读入失败或者已经到达文件流的尾部
        Bool
        readNextChar (
            UTF16 & utf16_char);

        // 读入文件流中的一行，并且返回它的UTF8编码的字符串(不包括回车字符)
        //
        // @param[out] utf8_text
        //      读入的一行UTF8编码的字符串
        // @return
        //      TRUE    如果读入成功
        //      FALSE   如果读入失败或者已经到达文件流的尾部
        Bool
        readLine (
            RuntimeText & utf8_text);

        // 读入文件流中的一行，并且返回它的UTF16编码的字符串(不包括回车字符)
        //
        // @param[out] utf16_text
        //      读入的一行UTF16编码的字符串
        // @return
        //      TRUE    如果读入成功
        //      FALSE   如果读入失败或者已经到达文件流的尾部
        Bool
        readLine (
            UTF16Text & utf16_text);

        // 将当前文件流的读入位置设置回头部
        void
        rewind ();

        // 关闭此字符文件读入器
        void
        close ();


    private:
        // 检查文件中字符串的编码方式
        void
        detectEncoding ();

    private:
        // 文件流
        FileReadStream m_io;
        // 文件中字符串的编码
        TextEncodings  m_encoding;
    };

} // namespace Core

