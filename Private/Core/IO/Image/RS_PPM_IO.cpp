// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/DataOperation/RS_EndiannessHelper.hpp"
#include "Core/FileSystem/RS_TextFileWriter.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"
#include "Core/String/RS_TextHelper.hpp"
// Self
#include "Core/IO/Image/RS_PPM_IO.hpp"


namespace Core
{

    // 输出字符串
    static
    FileWriteStream&
    operator << (
        FileWriteStream &  io,
        const ASCII *const ascii_string)
    {
        const UInt32 _data_len = (UInt32)std::strlen(ascii_string);
        io.write((const UTF8*)ascii_string, _data_len, 0, _data_len);

        return io;
    }



    void
    PPM::create (
        const UInt32                        image_width,
        const UInt32                        image_height,
        const UInt8* const                  data_ptr,
        const UInt32                        data_length,
        const Bool                          is_rgba_memory_layout,
        const Bool                          use_alpha,
        const Bool                          use_gamma_correction,
        const UTF8* const                   rel_path,
        const NativeFileSystem::SearchPaths search_dir,
        const Bool                          use_binary_ppm)
    {
        // 每个Pixel的Channel数: Red, Green, Glue, Alpha Channels
        static const UInt8  PIXEL_CHANNEL_COUNT      = 4;
        // 每个Channel所能表示的最大数值：(1 << 8) - 1: 255
        static const UInt32 MAXIMAL_CHANNEL_VALUE    = (1 << 8) - 1;
        // 工作缓存大小
        static const UInt32 WORKING_BUFFER_BYTE_SIZE = 256;

        UTF8 _wrk_buffer[WORKING_BUFFER_BYTE_SIZE];
        // 添加.ppm文件扩展符
        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%s.ppm", rel_path);

        // 创建WriteOnly文件流
        FileWriteStream _file_io;
        if (NativeFileSystem::openFileWrite(
            _wrk_buffer, search_dir, NativeFileSystem::WRITE_MODES_OVERWRITE, _file_io))
        {
            // 输出Magic number
            if (use_binary_ppm)
            {
                _file_io << "P6\n";
            }
            else
            {
                _file_io << "P3\n";
            }

            // 输出 Header
            _file_io << "# --- PPM image exporter ---\n\n"
                        "# PPM(Portable Pixel Map): 24bits RGB(RGB8)\n"
                        "# http://netpbm.sourceforge.net/doc/ppm.html\n"
                        "# Note :"
                        "# - the image data stored in the following way:\n"
                        "#  LEFT            RIGHT\n"
                        "#  TOP------------ >\n"
                        "#   |\n"
                        "#   |\n"
                        "#   |\n"
                        "#   |\n"
                        "#   v\n"
                        "#  BOTTOM\n"
                        "#"
                        "# - RGB channels stored in this order: R G B\n";

            // 输出图形的大小
            _file_io << "# --- image dimension ---\n"
                        "# width in pixels(represented in decimal)\n";
            TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%d", image_width);
            _file_io << (const ASCII*)_wrk_buffer << "\n";

            TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%d", image_height);
            _file_io << "# height in pixels(represented in decimal)\n";
            _file_io << (const ASCII*)_wrk_buffer << "\n";

            // 输出数据的最大数值
            _file_io << "# the maximal value of any channel represented in decimal\n";
            TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%d", MAXIMAL_CHANNEL_VALUE);
            _file_io << (const ASCII*)_wrk_buffer << "\n";

            // 输出图形数据
            // NOTE: 由于PPM使用如下坐标系存储数据：
            // TOP------------ > RIGHT
            //  |
            //  |
            //  |
            //  |
            //  v
            // BOTTOM
            //
            // 所以我们首先输出最后一行的数据
            for (SInt32 _row = image_height - 1; _row >= 0; --_row)
            {
                for (UInt32 _col = 0; _col < image_width; ++_col)
                {
                    const UInt32       _data_offset = (_row*image_width + _col)*PIXEL_CHANNEL_COUNT;
                    const UInt8* const _start_addr  = data_ptr + _data_offset;
                    const Real32       _alpha       = (Real32)_start_addr[3]/MAXIMAL_CHANNEL_VALUE;
                    UInt8 _red, _green, _blue;
                    if (is_rgba_memory_layout)
                    {
                        // RGBA存储格式
                        // 低位             高位
                        // +---+---+---+---+
                        // | R | G | B | A |
                        // +---+---+---+---+
                        _red   = _start_addr[0];
                        _green = _start_addr[1];
                        _blue  = _start_addr[2];
                    }
                    else
                    {
                        // BGRA存储格式
                        // 低位             高位
                        // +---+---+---+---+
                        // | B | G | R | A |
                        // +---+---+---+---+
                        _red   = _start_addr[2];
                        _green = _start_addr[1];
                        _blue  = _start_addr[0];
                    }

                    if (use_alpha)
                    {
                        _red   = (UInt8)(_red   * _alpha);
                        _green = (UInt8)(_green * _alpha);
                        _blue  = (UInt8)(_blue  * _alpha);
                    }

                    if (use_binary_ppm)
                    {
                        _file_io << _red;
                        _file_io << _green;
                        _file_io << _blue;
                    }
                    else
                    {
                        // 以字符形式输出
                        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%-3d", _red);
                        _file_io << (const ASCII*)_wrk_buffer << " ";
                        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%-3d", _green);
                        _file_io << (const ASCII*)_wrk_buffer << " ";
                        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%-3d", _blue);
                        _file_io << (const ASCII*)_wrk_buffer;
                        // 输出完一个Pixel，我们添加一些空格
                        if ((_col + 1) < image_width)
                        {
                            _file_io << "    ";
                        }
                    }
                }

                // 输出完一行
                if (!use_binary_ppm)
                {
                    _file_io << "\n";
                }
            }

            // 关闭文件流
            _file_io.close();
        }
    }


    void
    PPM::create (
        const UInt32                        image_width,
        const UInt32                        image_height,
        const UInt16* const                 data_ptr,
        const UInt32                        data_length,
        const Bool                          is_rgba_memory_layout,
        const Bool                          use_alpha,
        const UTF8* const                   rel_path,
        const NativeFileSystem::SearchPaths search_dir,
        const Bool                          use_binary_ppm)
    {
        // 每个Pixel的Channel数: Red, Green, Glue, Alpha Channels
        static const UInt8  PIXEL_CHANNEL_COUNT      = 4;
        // 每个Channel所能表示的最大整数值: (1 << 16) - 1: 65535
        static const UInt32 MAXIMAL_CHANNEL_VALUE    = (1 << 16) -1;
        // 工作缓存大小
        static const UInt32 WORKING_BUFFER_BYTE_SIZE = 256;

        UTF8 _wrk_buffer[WORKING_BUFFER_BYTE_SIZE];
        // 添加.ppm文件扩展符
        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%s.ppm", rel_path);

        // 创建WriteOnly文件流
        FileWriteStream _file_io;
        if (NativeFileSystem::openFileWrite(
            _wrk_buffer, search_dir, NativeFileSystem::WRITE_MODES_OVERWRITE, _file_io))
        {
            // 输出Magic number
            if (use_binary_ppm)
            {
                _file_io << "P6\n";
            }
            else
            {
                _file_io << "P3\n";
            }

            // 输出 Header
            _file_io << "# --- PPM image exporter ---\n\n"
                        "# PPM(Portable Pixel Map): 48bits RGB(RGB16)\n"
                        "# http://netpbm.sourceforge.net/doc/ppm.html\n"
                        "# Note :"
                        "# - the image data stored in the following way:\n"
                        "#  LEFT            RIGHT\n"
                        "#  TOP------------ >\n"
                        "#   |\n"
                        "#   |\n"
                        "#   |\n"
                        "#   |\n"
                        "#   v\n"
                        "#  BOTTOM\n"
                        "#"
                        "# - RGB channels stored in this order: R G B\n";

            // 输出图形的大小
            _file_io << "# --- image dimension ---\n"
                        "# width in pixels(represented in decimal)\n";
            TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%d", image_width);
            _file_io << (const ASCII*)_wrk_buffer << "\n";

            TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%d", image_height);
            _file_io << "# height in pixels(represented in decimal)\n";
            _file_io << (const ASCII*)_wrk_buffer << "\n";

            // 输出数据的最大数值
            _file_io << "# the maximal value of any channel represented in decimal\n";
            TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%d", MAXIMAL_CHANNEL_VALUE);
            _file_io << (const ASCII*)_wrk_buffer << "\n";

            // 输出图形数据
            // NOTE: 由于PPM使用如下坐标系存储数据：
            // TOP------------ > RIGHT
            //  |
            //  |
            //  |
            //  |
            //  v
            // BOTTOM
            //
            // 所以我们首先输出最后一行的数据
            for (SInt32 _row = image_height - 1; _row >= 0; --_row)
            {
                for (UInt32 _col = 0; _col < image_width; ++_col)
                {
                    const UInt32        _data_offset = (_row*image_width + _col)*PIXEL_CHANNEL_COUNT;
                    const UInt16* const _start_addr  = data_ptr + _data_offset;
                    const Real32        _alpha       = (Real32)_start_addr[3]/MAXIMAL_CHANNEL_VALUE;
                    UInt16 _red, _green, _blue;
                    if (is_rgba_memory_layout)
                    {
                        // RGBA存储格式
                        // 低位             高位
                        // +---+---+---+---+
                        // | R | G | B | A |
                        // +---+---+---+---+
                        _red   = _start_addr[0];
                        _green = _start_addr[1];
                        _blue  = _start_addr[2];
                    }
                    else
                    {
                        // BGRA存储格式
                        // 低位             高位
                        // +---+---+---+---+
                        // | B | G | R | A |
                        // +---+---+---+---+
                        _red   = _start_addr[2];
                        _green = _start_addr[1];
                        _blue  = _start_addr[0];
                    }

                    if (use_alpha)
                    {
                        _red   = (UInt16)(_red   * _alpha);
                        _green = (UInt16)(_green * _alpha);
                        _blue  = (UInt16)(_blue  * _alpha);
                    }

                    if (use_binary_ppm)
                    {
                        // NOTE: 二进制RGB16模式，使用Big Endianess方式存储数据
                        // GIMP也是期待Big Endianess
                        _file_io << EndianHelper::swapEndian16(_red);
                        _file_io << EndianHelper::swapEndian16(_green);
                        _file_io << EndianHelper::swapEndian16(_blue);
                    }
                    else
                    {
                        // 以字符形式输出
                        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%-5d", _red);
                        _file_io << (const ASCII*)_wrk_buffer << " ";
                        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%-5d", _green);
                        _file_io << (const ASCII*)_wrk_buffer << " ";
                        TextHelper::sprintfSafe(_wrk_buffer, sizeof(_wrk_buffer), "%-5d", _blue);
                        _file_io << (const ASCII*)_wrk_buffer;
                        // 输出完一个Pixel，我们添加一些空格
                        if ((_col + 1) < image_width)
                        {
                            _file_io << "    ";
                        }
                    }
                }

                // 输出完一行
                if (!use_binary_ppm)
                {
                    _file_io << "\n";
                }
            }

            // 关闭文件流
            _file_io.close();
        }
    }

} // namespace Util
