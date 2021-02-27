// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#import <Foundation/NSData.h>
// Lib headers
// Self
#include "Core/FileSystem/Apple/RS_NativeFile_Appl.hpp"


namespace Core
{

    NativeFile::NativeFile (
        NSFileHandle *const file_io)
    :
        file_handle(file_io),
        is_opened(file_io != nil)
    {

    }


    NativeFile::NativeFile (
        const NativeFile & other)
    :
        file_handle(other.file_handle),
        is_opened(other.is_opened)
    {

    }


    Bool
    NativeFile::isOpened () const
    {
        return is_opened;
    }


    UInt32
    NativeFile::getCursorPosition () const
    {
        const UInt32 _cursor_pos = (UInt32)[file_handle offsetInFile];
        return _cursor_pos;
    }


    UInt32
    NativeFile::getLength () const
    {
        // 保存文件Cursor的位置
        const UInt32 _cursor_pos  = getCursorPosition();
        // 将Cursor移动到文件尾，来获得文件的大小
        const UInt32 _file_length = (UInt32)[file_handle seekToEndOfFile];
        // 在退出前，将Cursor设置回原来的位置
        [file_handle seekToFileOffset: _cursor_pos];

        return _file_length;
    }


    SInt32
    NativeFile::read (
        const UInt32 buffer_size,
        const UInt32 offset,
        const UInt32 count,
        UInt8 *const buffer)
    {
        // sanity check
        RUNTIME_ASSERT(buffer != nullptr, "Invalid output buffer");
        if (buffer == nullptr)
        {
            return -1;
        }

        RUNTIME_ASSERT(offset + count <= buffer_size, "No place to store all bytes");
        if (offset + count > buffer_size)
        {
            return -1;
        }

        // 同步读入数据
        NSData *const _data_array = [file_handle readDataOfLength: count];
        // 检测是否读人成功
        if (_data_array == nil)
        {
            return -1;
        }

        // 复制读入的数据
        [_data_array getBytes: buffer+offset
                       length: count];

        return (SInt32)_data_array.length;
    }


    SInt32
    NativeFile::seek (
        const SInt32                      cursor_pos,
        const NativeFileSystem::SeekModes seek_mode)
    {
        // 检查我们是否需要更新读写头的位置
        if ((cursor_pos == 0) && (seek_mode == NativeFileSystem::SEEK_MODES_CURRENT_CURSOR))
        {
            return (SInt32)getCursorPosition();
        }

        // 计算相对于文件头的偏移
        SInt32 _offset_from_file_head;
        switch (seek_mode)
        {
            case NativeFileSystem::SEEK_MODES_FILE_BEGIN:
            {
                // 无法将读写头放置在文件头之前
                if (cursor_pos < 0)
                {
                    return -1;
                }
                _offset_from_file_head = cursor_pos;
                break;
            }

            case NativeFileSystem::SEEK_MODES_CURRENT_CURSOR:
            {
                _offset_from_file_head = (SInt32)getCursorPosition() + cursor_pos;
                break;
            }

            case NativeFileSystem::SEEK_MODES_FILE_END:
            {
                // 无法将读写头放置在文件尾之后
                if (cursor_pos > 0)
                {
                    return -1;
                }
                _offset_from_file_head = (SInt32)getLength() + cursor_pos;
                break;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Uknown seek mode");
                return -1;
            }
        }

        // 相对于文件头移动读写头
        [file_handle seekToFileOffset: _offset_from_file_head];
        return _offset_from_file_head;
    }


    void
    NativeFile::write (
       const UInt8 *const buffer,
       const UInt32       buffer_size,
       const UInt32       offset,
       const UInt32       count)
    {
        // sanity check
        RUNTIME_ASSERT(buffer != nullptr, "Invalid input buffer");
        if (buffer == nullptr)
        {
            return;
        }

        RUNTIME_ASSERT(offset + count <= buffer_size, "Input buffer is not long enough");
        if (offset + count > buffer_size)
        {
            return;
        }

        // 创建输出数组
        NSData* const _data_array = [NSData dataWithBytes: buffer + offset
                                                   length: count];
        if (_data_array == nil)
        {
            return;
        }

        // 同步写出数据
        [file_handle writeData: _data_array];
    }


    void
    NativeFile::setLength (
        const UInt32 file_length)
    {
        // 保存当前读写头的位置
        const UInt32 _cursor_pos = getCursorPosition();

        // 调整文件大小，并且使用'\0'来充填
        [file_handle truncateFileAtOffset: file_length];

        // 重置文件读写头位置
        if (_cursor_pos < file_length)
        {
            seek(_cursor_pos, NativeFileSystem::SEEK_MODES_FILE_BEGIN);
        }
        else
        {
            seek(0, NativeFileSystem::SEEK_MODES_FILE_END);
        }
    }


    void
    NativeFile::close ()
    {
        // 检查是否此文件已经关闭
        if (file_handle == nil)
        {
            return;
        }

        // 关闭文件并清除Handle
        [file_handle closeFile];
        file_handle = nil;
        is_opened   = false;
    }

} // namespace Core

#endif // #if defined(__APPLE__)
