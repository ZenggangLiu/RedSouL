#pragma once


// System headers
#import  <Foundation/NSFileHandle.h>
// Lib headers
#include "Core/FileSystem/RS_NativeFileSystem.hpp"


namespace Core
{

    // File: 文件长度为N
    //  文件头          最后字节   文件尾
    // +----+----+-----+----+-----------+
    // | AA | BB | ... | EE | EndOfFile |
    // +----+----+-----+----+-----------+
    //   0    1    ...   N-1
    //        ^
    //        |
    //        +-- 读写头位置
    struct NativeFile
    {
        // 文件句柄
        NSFileHandle * file_handle;
        // 表示此文件是否成功打开
        Bool           is_opened;


        // 使用文件句柄来创建实例
        NativeFile (
            NSFileHandle *const file_io);

        // 克隆另外一个实例
        NativeFile (
           const NativeFile & other);

        // 检查当前文件是否打开
        Bool
        isOpened () const;

        // 获得文件当前Cursor(读写头)的位置: [0， 文件长度]
        UInt32
        getCursorPosition () const;

        // 获得文件的长度
        UInt32
        getLength () const;

        // 读入指定字节数，并返回真正读入的字节数目
        //
        // @param[in] buffer_size
        //      缓存的大小
        // @param[in] offset
        //      缓存中的起始位置
        // @param[in] count
        //      希望读入的字节数
        // @param[out] buffer
        //      数据存入的缓存
        // @return
        //      > 0     如果读入成功，读入的字节数目
        //      < 0     如果读入失败
        SInt32
        read (
            const UInt32 buffer_size,
            const UInt32 offset,
            const UInt32 count,
            UInt8 *const buffer);

        // 移动文件读写头的位置, 并且返回当前位置: [0， 文件长度]
        //
        // @param[in] cursor_pos
        //      相对于SeekMode的读写头的偏移量
        // @param[in] seek_mode
        //      定义如何偏移读写头
        // @return
        //      > 0     如果移动成功，当前的读写头的位置
        //      < 0     如果移动失败
        SInt32
        seek (
            const SInt32                      cursor_pos,
            const NativeFileSystem::SeekModes seek_mode);

        // 输出指定字节数
        //
        // @param[in] buffer
        //      数据的缓存
        // @param[in] buffer_size
        //      缓存的大小
        // @param[in] offset
        //      缓存中的起始位置
        // @param[in] count
        //      希望输出的字节数
        void
        write (
           const UInt8 *const buffer,
           const UInt32       buffer_size,
           const UInt32       offset,
           const UInt32       count);

        // 设置文件的长度，并且使用‘\0'来充填扩展的部分
        //
        // @param[in] file_length
        //      新的文件长度
        void
        setLength (
            const UInt32 file_lenght);

        // 关闭此文件
        void
        close ();
    };

} // namespace Core
