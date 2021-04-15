#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/FileSystem/RS_FileWriteStream.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"
#include "Core/String/RS_TextHelper.hpp"


namespace Core
{

    class UTF16Text;


    // 字符文件输出器：提供UTF8/16编码的字符串文件输出
    // NOTE: 我们只支持Basic Multilingual Plane上的Unicode: [U+0000, U+FFFF]，
    //       因为他们对应单整数UTF16编码
    class TextFileWriter
    {
    public:
        // 创建一个字符文件输出器
        //
        // @param[in] rel_path
        //      相对路径
        // @param[in] search_dir
        //      定义相对哪个目录进行操作
        // @param[in] encoding
        //      定义文件使用的编码方法
        TextFileWriter (
            const UTF8 *const                   rel_path,
            const NativeFileSystem::SearchPaths search_dir,
            const TextEncodings                 encoding);

        // 释放一个字符文件输出器
        ~TextFileWriter ();

        // 检查此输出器是否有效
        Bool
        isValid () const;

        // 输出ASCII编码的字符串
        //
        // @param[in] ascii_text
        //      ASCII编码的字符串
        // @return
        //      TRUE    如果成功
        //      FALSE   如果失败
        Bool
        writeText (
            const ASCII *const ascii_text);

        // 输出UTF8编码的字符串
        //
        // @param[in] utf8_text
        //      UTF8编码的字符串
        // @return
        //      TRUE    如果成功
        //      FALSE   如果失败
        Bool
        writeText (
            const RuntimeText & utf8_text);

        // 输出UTF16编码的字符串
        //
        // @param[in] utfin16_text
        //      UTF16编码的字符串
        // @return
        //      TRUE    如果成功
        //      FALSE   如果失败
        Bool
        writeText (
            const UTF16Text & utf16_text);

        // 输出操作符
        TextFileWriter&
        operator << (
            const SInt8 data);

        TextFileWriter&
        operator << (
            const UInt8 data);

        TextFileWriter&
        operator << (
            const SInt16 data);

        TextFileWriter&
        operator << (
            const UInt16 data);

        TextFileWriter&
        operator << (
            const SInt32 data);

        TextFileWriter&
        operator << (
            const UInt32 data);

        TextFileWriter&
        operator << (
            const SInt64 data);

        TextFileWriter&
        operator << (
            const UInt64 data);

        TextFileWriter&
        operator << (
            const Real16 data);

        TextFileWriter&
        operator << (
            const Real32 data);

        TextFileWriter&
        operator << (
            const Real64 data);

        TextFileWriter&
        operator << (
            const ASCII *const ascii_text);

        TextFileWriter&
        operator << (
            const RuntimeText & utf8_text);

        TextFileWriter&
        operator << (
            const UTF16Text & utf16_text);

        // 关闭此字符文件读入器
        void
        close ();


    private:
        void
        writeBOM ();

    private:
        // 文件流
        FileWriteStream m_io;
        // 文件中字符串的编码
        TextEncodings   m_encoding;
        // 转换数据时使用的缓存
        ASCII           m_buffer[32];
    };

} // namespace Core

