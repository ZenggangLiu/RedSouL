#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"


namespace Core
{

    class RuntimeText;


    // 提供对PPM(Portable Pixel Format)文件的支持
    struct PPM
    {
        // Dumps给定color buffer到一个PPM文件
        // Pre-multiplied with alpha
        // https://en.wikipedia.org/wiki/Netpbm_format
        // 
        // 图形数据使用如下坐标系:
        //
        //    ^ UP
        //    |
        //    +-------------+--------------+
        //    | First Pixel | Second Pixel |  <----- 第二行
        //    |      3      |      4       |
        //    +-------------+--------------+
        //    | First Pixel | Second Pixel |  <----- 第一行
        //    |      1      |      2       |
        //  O +-------------+--------------+----> RIGHT
        //
        // @param[in] image_width
        //      图形的宽度(Pixel)
        // @param[in] image_height
        //      图形的高度(Pixel)
        // @param[in] data_ptr
        //      图形数据缓存
        // @param[in] data_length
        //      图形缓存的长度(字节)
        // @param[in] is_rgba_memory_layout
        //      图形的数据是否以RGBA的顺序存储
        //      YES：图形数据存储顺序如下：
        //           低位             高位
        //           +---+---+---+---+
        //           | R | G | B | A |
        //           +---+---+---+---+
        //      NO： 图形数据存储顺序如下：
        //           低位             高位
        //           +---+---+---+---+
        //           | B | G | R | A |
        //           +---+---+---+---+
        // @param[in] use_alpha
        //      表示是否使用Alpha Channel
        // @param[in] use_gamma_correction
        //      NO SUPPORT
        // @param[in] rel_path
        //      相对路径
        //      NOTE: .ppm文件扩展符将添加到给定的路径名上
        // @param[in] search_dir
        //      定义相对哪个目录进行操作
        // @param[in] use_binary_ppm
        //      是否使用二进制PPM格式
        //
        // 24位RGB格式：RGB8
        static
        void
        create (
            const UInt32                        image_width,
            const UInt32                        image_height,
            const UInt8 *const                  data_ptr,
            const UInt32                        data_length,
            const Bool                          is_rgba_memory_layout,
            const Bool                          use_alpha,
            const Bool                          use_gamma_correction,
            const UTF8* const                   rel_path,
            const NativeFileSystem::SearchPaths search_dir,
            const Bool                          use_binary_ppm);

        // 48位RGB格式：RGB16
        static
        void
        create (
            const UInt32                        image_width,
            const UInt32                        image_height,
            const UInt16 *const                 data_ptr,
            const UInt32                        data_length,
            const Bool                          is_rgba_memory_layout,
            const Bool                          use_alpha,
            const UTF8* const                   rel_path,
            const NativeFileSystem::SearchPaths search_dir,
            const Bool                          use_binary_ppm);
    };

} // namespace Core
