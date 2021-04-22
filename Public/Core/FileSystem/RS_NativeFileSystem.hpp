#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/String/RS_RuntimeText.hpp"


namespace Core
{

    class FileStream;
    class FileReadStream;
    class FileWriteStream;


    struct NativeFileSystem
    {
        // --- 路径名称转换接口 --- //
        // 将所有Slash(/)转换成Back Slash(\)
        //
        // @param[in] path
        //      包含Slash(/)的路径
        static
        RuntimeText
        convFrontSlashToBack (
            const UTF8 *const path);

        // 将Back Slash(\)转换成Slash(/)
        //
        // @param[in] path
        //      包含Back Slash(\)的路径
        static
        RuntimeText
        convBackSlashToFront (
            const UTF8 *const path);


        // --- 目录操作接口 --- //
        // 获得Bundle目录（可执行文件所在的目录）
        static
        const UTF8*
        getBundleDir ();

        // 获得Cache目录
        static
        const UTF8*
        getCacheDir ();

        // 获得Document目录（用户的可读写目录）
        //
        // NOTE: 在PC上此函数将返回当前工作目录
        static
        const UTF8*
        getDocumentDir ();

        // 获得临时目录
        static
        const UTF8*
        getTemporaryDir ();

        // 获得当前工作目录
        static
        const UTF8*
        getWorkingDir ();


        // --- OS specific methods START --- //
        // Implementations are in RS_NativeFileSystem_Appl/Wind/Andr.???
        // 检测给定的目录是否存在
        //
        // @param[in] abs_path
        //      绝对路径
        // @return
        //      TRUE    如果目录存在
        //      FALSE   如果目录不存在
        static
        Bool
        existsDir (
            const UTF8 *const abs_path);

        // 设定当前工作目录
        //
        // @param[in] abs_path
        //      当前工作目录的绝对路径
        // @return
        //      TRUE    如果目录设定成功
        //      FALSE   如果目录设定失败
        static
        Bool
        setWorkingDir (
            const UTF8 *const abs_path);

        // 创建一个目录使用给定的路径名
        //
        // @param[in] abs_path
        //      目录的绝对路径
        // @return
        //      TRUE    如果目录创建成功
        //      FALSE   如果目录创建失败
        // NOTE：此函数会创建所有路径参考的中间目录
        static
        Bool
        createDir (
            const UTF8 *const abs_path);

        // 删除指定的目录
        //
        // @param[in] abs_path
        //      目录的绝对路径
        // @return
        //      TRUE    如果目录删除成功
        //      FALSE   如果目录删除失败
        // NOTE: 此函数将删除所有中间目录
        static
        Bool
        deleteDir (
            const UTF8 *const abs_path);
        // --- OS specific methods END --- //


        // --- 文件操作接口 --- //
        // NOTE: 所以文件操作都使用相对路径名

        // 定义文件从何处开始查找
        enum SearchPaths
        {
            // 在Bundle目录下(只读)
            SEARCH_PATH_BUNDLE_FOLDER,
            // 在Cache目录下
            SEARCH_PATH_CACHE_FOLDER,
            // 在Document目录下(读写)
            SEARCH_PATH_DOCUMENT_FOLDER,
            // 在临时目录下
            SEARCH_PATH_TEMPORARY_FOLDER,
            // 在当前工作目录下
            SEARCH_PATH_WORKING_FOLDER,
        };

        // 定义访问模式
        enum AccessModes
        {
            // 只读
            ACCESS_MODES_READ_ONLY,
            // 读写
            ACCESS_MODES_READ_WRITE,
            // 只写
            ACCESS_MODES_WRITE_ONLY,
        };

        // 定义如何更新读写头的位置
        enum SeekModes
        {
            // 相对于文件头位置
            SEEK_MODES_FILE_BEGIN,
            // 相对于当前读写头位置
            SEEK_MODES_CURRENT_CURSOR,
            // 相对于文件尾位置
            SEEK_MODES_FILE_END,
        };

        // 定义如何对文件进行写入操作
        enum WriteModes
        {
            // 写入的内容附加在文件现有内容的后面
            WRITE_MODES_APPEND,
            // 覆盖文件中的现有内容(设置文件的长度为0，然后写入内容)
            WRITE_MODES_OVERWRITE,
        };

        // 获得文件的Base name
        // 文件名格式为：BaseName.Extension
        //
        // @param rel_name
        //      文件的相对路径名
        // @return
        //      返回BaseName部分
        static
        RuntimeText
        baseName (
            const RuntimeText & rel_name);

        // 获得文件的Extension
        // 文件名格式为：BaseName.Extension
        //
        // @param rel_name
        //      文件的相对路径名
        // @return
        //      返回Extension部分
        static
        RuntimeText
        extension (
            const RuntimeText & rel_file);

        // 获得文件的绝对路径
        //
        // @param[in] rel_path
        //      相对路径
        // @param[in] search_dir
        //      定义相对哪个目录进行操作
        static
        RuntimeText
        getAbsPath (
            const UTF8 *const rel_path,
            const SearchPaths search_dir);

        // 检测给定的文件是否存在
        //
        // @param[in] rel_path
        //      相对路径
        // @param[in] search_dir
        //      定义相对哪个目录进行操作
        // @return
        //      TRUE    如果文件存在
        //      FALSE   如果文件不存在
        static
        Bool
        existsFile (
            const UTF8 *const rel_path,
            const SearchPaths search_dir);

        // 删除指定文件
        //
        // @param[in] rel_path
        //      相对路径
        // @param[in] search_dir
        //      定义相对哪个目录进行操作
        // @return
        //      TRUE    如果文件删除成功
        //      FALSE   如果文件删除失败
        static
        Bool
        deleteFile (
            const UTF8 *const rel_path,
            const SearchPaths search_dir);


        // --- OS specific methods START --- //
        // Implementations are in RS_NativeFileSystem_Appl/Wind/Andr.???
        // 创建一个新文件/覆盖一个现有文件
        //
        // @param[in]  rel_path
        //      相对路径
        // @param[in]  search_dir
        //      定义相对哪个目录进行操作
        // @param[out] file_stream
        //      Read/Write文件流
        // @return
        //      TRUE    如果文件创建/覆盖成功
        //      FALSE   如果文件创建/覆盖失败
        static
        Bool
        createFile (
            const UTF8 *const rel_path,
            const SearchPaths search_dir,
            FileStream &      file_stream);

        // 打开一个存在的文件进行读入
        //
        // @param[in]  rel_path
        //      相对路径
        // @param[in]  search_dir
        //      定义相对哪个目录进行操作
        // @param[out] file_stream
        //      ReadOnly文件流
        // @return
        //      TRUE    如果文件打开成功
        //      FALSE   如果文件打开失败
        static
        Bool
        openFileRead (
            const UTF8 *const rel_path,
            const SearchPaths search_dir,
            FileReadStream &  file_stream);

        // 打开一个现有文件(长度设置为0)/创建一个新文件来进行写入
        //
        // @param[in]  rel_path
        //      相对路径
        // @param[in]  search_dir
        //      定义相对哪个目录进行操作
        // @param[in]  write_mode
        //      定义如何写入新的内容
        // @param[out] file_stream
        //      WriteOnly文件流
        // @return
        //      TRUE    如果文件打开/创建成功
        //      FALSE   如果文件打开/创建失败
        static
        Bool
        openFileWrite (
            const UTF8 *const rel_path,
            const SearchPaths search_dir,
            const WriteModes  write_mode,
            FileWriteStream & file_stream);

        // 打开一个现有文件/创建一个新文件来进行读写
        //
        // @param[in]  rel_path
        //      相对路径
        // @param[in]  search_dir
        //      定义相对哪个目录进行操作
        // @param[out] file_stream
        //      Read/Write文件流
        // @return
        //      TRUE    如果文件打开/创建成功
        //      FALSE   如果文件打开/创建失败
        static
        Bool
        openFile (
            const UTF8 *const rel_path,
            const SearchPaths search_dir,
            FileStream &      file_stream);
        // --- OS specific methods END --- //


    private:
        // --- OS specific methods START --- //
        // Implementations are in RS_NativeFileSystem_Appl/Wind/Andr.???
        // 检测绝对路径所指定的文件是否存在
        static
        Bool
        existsFileAbs (
            const RuntimeText & abs_path);

        // 删除绝对路径所指定的文件
        static
        Bool
        deleteFileAbs (
            const RuntimeText & abs_path);
        // --- OS specific methods END --- //

    private:
        // Bundle目录：其实工作目录
        static RuntimeText  ms_bundle_dir;
        // Caching目录
        static RuntimeText  ms_cache_dir;
        // 当前工作目录：可以通过调用setWorkingDir()来改变
        static RuntimeText  ms_working_dir;
        // Document目录
        static RuntimeText  ms_document_dir;
        // 临时目录
        static RuntimeText  ms_temporary_dir;
    };

} // namespace Core
