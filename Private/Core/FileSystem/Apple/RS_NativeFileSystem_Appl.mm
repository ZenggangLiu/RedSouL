// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
#import  <Foundation/NSBundle.h>
#import  <Foundation/NSFileHandle.h>
#import  <Foundation/NSFileManager.h>
#include <sys/stat.h>                   // stat() and struct stat
// Lib headers
#include "Core/FileSystem/Apple/RS_NativeFile_Appl.hpp"
#include "Core/FileSystem/RS_FileReadStream.hpp"
#include "Core/FileSystem/RS_FileStream.hpp"
#include "Core/FileSystem/RS_FileWriteStream.hpp"

// Self
#include "Core/FileSystem/RS_NativeFileSystem.hpp"


namespace Core
{

    //----------------------------------------------------------------------------------------------
    // HELPERS
    //----------------------------------------------------------------------------------------------
    // MARK: - Helpers

    // NOTE:
    // 不要使用getBundleDir, getCacheDir, getDocumentDir，getTemporary, getWorkingDir
    // 由于这些函数名与类中定义的函数冲突

    // 获得Bundle目录
    INLINE_FUNCTION
    static
    RuntimeText
    initBundleDir ()
    {
        // gets the full pathname of the receiver’s bundle directory
        NSString *const _bundle_dir = [[NSBundle mainBundle] bundlePath];
#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 无法使用Logging，由于LoggingMgr的初始化要使用文件系统
        std::printf("[Bundle folder]: %s\n",
                    [_bundle_dir cStringUsingEncoding: NSUTF8StringEncoding]);
#endif
        return [_bundle_dir cStringUsingEncoding: NSUTF8StringEncoding];
    }


    // 获得Cache目录
    INLINE_FUNCTION
    static
    RuntimeText
    initCacheDir ()
    {
        // 获得Cache目录列表
        // NSSearchPathForDirectoriesInDomains(): creates a list of directory search paths
        NSArray *const _cache_dir_list = NSSearchPathForDirectoriesInDomains(
            NSCachesDirectory,  NSUserDomainMask, YES);

        // 从列表中，获得第一个Cache目录
        id _cache_dir = [_cache_dir_list objectAtIndex: 0];
        RUNTIME_ASSERT([_cache_dir isKindOfClass: [NSString class]] == YES, "Wrong return type");

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 无法使用Logging，由于LoggingMgr的初始化要使用文件系统
        std::printf("[Cache folder]: %s\n",
                    [(NSString*)_cache_dir cStringUsingEncoding: NSUTF8StringEncoding]);
#endif
        return [(NSString*)_cache_dir cStringUsingEncoding: NSUTF8StringEncoding];
    }


    // 获得Document目录
    INLINE_FUNCTION
    static
    RuntimeText
    initDocumentDir ()
    {
        // 获得Document目录列表
        // NSSearchPathForDirectoriesInDomains(): creates a list of directory search paths
        NSArray *const _doc_dir_list = NSSearchPathForDirectoriesInDomains(
            NSDocumentDirectory, NSUserDomainMask, YES);

        // 从列表中，获得第一个Docuemnt目录
        id _doc_dir = [_doc_dir_list objectAtIndex: 0];
        RUNTIME_ASSERT([_doc_dir isKindOfClass: [NSString class]] == YES, "Wrong return type");

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 无法使用Logging，由于LoggingMgr的初始化要使用文件系统
        std::printf("[Document folder]: %s\n",
                    [(NSString*)_doc_dir cStringUsingEncoding: NSUTF8StringEncoding]);
#endif
        return [(NSString*)_doc_dir cStringUsingEncoding: NSUTF8StringEncoding];
    }


    // 获得Temporary目录
    INLINE_FUNCTION
    static
    RuntimeText
    initTemporaryDir ()
    {

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 无法使用Logging，由于LoggingMgr的初始化要使用文件系统
        std::printf("[Temparary folder]: %s\n",
                    [NSTemporaryDirectory() cStringUsingEncoding: NSUTF8StringEncoding]);
#endif
        return [NSTemporaryDirectory() cStringUsingEncoding: NSUTF8StringEncoding];
    }


    // 获得当前工作目录
    INLINE_FUNCTION
    static
    RuntimeText
    initWorkingDir ()
    {
        // 获得缺省NSFileManager
        NSFileManager *const _file_mgr = [NSFileManager defaultManager];
        RUNTIME_ASSERT(_file_mgr != nil, "No default NSFIleManager instance");

        // 获得当前工作目录
        NSString * _cwd = [_file_mgr currentDirectoryPath];

        // 检查当前工作目录是否为Root："/"
        // 如果是Root，我们将其指向Bundle目录
        if ([_cwd isEqualToString: @"/"] == YES)
        {
            _cwd = [[NSBundle mainBundle] bundlePath];
        }

#if (BUILD_MODE == DEBUG_BUILD_MODE)
        // 无法使用Logging，由于LoggingMgr的初始化要使用文件系统
        std::printf("[Working folder]: %s\n",
                    [_cwd cStringUsingEncoding: NSUTF8StringEncoding]);
#endif
        return [_cwd cStringUsingEncoding: NSUTF8StringEncoding];
    }


    // 使用绝对路径打开文件并为其生成文件流
    static
    NSFileHandle*
    openFileAbsForHandle (
        NSString *const                     abs_path,
        const NativeFileSystem::AccessModes access_mode)
    {
        switch (access_mode)
        {
            // 只读模式
            case NativeFileSystem::ACCESS_MODES_READ_ONLY:
            {
                // 如果文件不存在返回nil
                return [NSFileHandle fileHandleForReadingAtPath: abs_path];
            }

            // 读写模式
            case NativeFileSystem::ACCESS_MODES_READ_WRITE:
            {
                // 如果文件不存在返回nil
                return [NSFileHandle fileHandleForUpdatingAtPath: abs_path];
            }

            // 只写模式
            case NativeFileSystem::ACCESS_MODES_WRITE_ONLY:
            {
                // 如果文件不存在返回nil
                return [NSFileHandle fileHandleForWritingAtPath: abs_path];
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unkown file access mode");
                return nil;
            }
        }
    }


    // 创建/覆盖指定文件
    static
    Bool
    createFileAbs (
        NSString *const abs_path)
    {
        // 获得缺省NSFileManager
        NSFileManager *const _file_mgr = [NSFileManager defaultManager];
        RUNTIME_ASSERT(_file_mgr != nil, "No default NSFIleManager instance");

        // 检查此文件所在的目录是否存在
        NSString *const _folder_path = [abs_path stringByDeletingLastPathComponent];
        if (!NativeFileSystem::existsDir(
            (const UTF8*)[_folder_path cStringUsingEncoding:NSUTF8StringEncoding]))
        {
            // 创建此包含目录
            const BOOL _result = [_file_mgr createDirectoryAtPath:_folder_path
                                      withIntermediateDirectories:YES
                                                       attributes:nil
                                                            error:nil];
            if (!_result)
            {
                return false;
            }
        }

        // NOTE：createFileAtPath:contents:attributes函数将创建/覆盖指定文件
        if ([_file_mgr createFileAtPath: abs_path contents: nil attributes: nil] == YES)
        {
            return true;
        }
        else
        {
            return false;
        }
    }



    //----------------------------------------------------------------------------------------------
    // NATIVEFILESYSTEM
    //----------------------------------------------------------------------------------------------
    // MARK: - NativeFileSystem

    RuntimeText NativeFileSystem::ms_bundle_dir         = initBundleDir();
    RuntimeText NativeFileSystem::ms_cache_dir          = initCacheDir();
    RuntimeText NativeFileSystem::ms_document_dir       = initDocumentDir();
    RuntimeText NativeFileSystem::ms_temporary_dir      = initTemporaryDir();
    RuntimeText NativeFileSystem::ms_working_dir        = initWorkingDir();



    Bool
    NativeFileSystem::existsDir (
        const UTF8 *const abs_path)
    {
        RUNTIME_ASSERT(abs_path != nullptr, "Empty path name");

        // NOTE: 我们必须使用 struct keyword:
        // 由于 strut stat 以及 stat()在此处同时定义

        // 获得指定目录的信息
        struct stat _dir_info;
        if (stat((const char*)abs_path, &_dir_info) != 0 ||  // 无法获得目录信息
           (_dir_info.st_mode & S_IFDIR) == 0)               // 给定的路径不是目录
        {
            return false;
        }

        return true;
    }


    Bool
    NativeFileSystem::setWorkingDir (
        const UTF8 *const abs_path)
    {
#if (OS_TYPE == OS_TYPE_MAC)
        // macOS
        RUNTIME_ASSERT(abs_path != nullptr, "Empty path of the working directory");

        // 获得缺省NSFileManager
        NSFileManager *const _file_mgr = [NSFileManager defaultManager];
        RUNTIME_ASSERT(_file_mgr != nil, "No default NSFIleManager instance");

        return [_file_mgr changeCurrentDirectoryPath:
                       [NSString stringWithCString: (const char*)abs_path
                                          encoding: NSUTF8StringEncoding]] == YES;

#else
        // iOS/tvOS
        // 无法改变当前工作目录在iOS/tvOS系统
        TODO("We need to confirm this feature");
        return false;
#endif // #if (OS_TYPE == OS_TYPE_MAC)
    }


    Bool
    NativeFileSystem::createDir (
        const UTF8 *const abs_path)
    {
        // 检查是否指定的目录以及创建
        if (existsDir(abs_path))
        {
            return true;
        }

        // 获得缺省NSFileManager
        NSFileManager *const _file_mgr = [NSFileManager defaultManager];
        RUNTIME_ASSERT(_file_mgr != nil, "No default NSFIleManager instance");

        // 创建路径名称
        NSString *const _path_name = [NSString stringWithCString: (const char*)abs_path
                                                        encoding: NSUTF8StringEncoding];

        // 递归创建目录hierarchy
        return [_file_mgr createDirectoryAtPath: _path_name
                    withIntermediateDirectories: YES
                                     attributes: nil
                                          error: nil] == YES;
    }


    Bool
    NativeFileSystem::deleteDir (
        const UTF8 *const abs_path)
    {
        // 检查是否指定的目录已经删除
        if (existsDir(abs_path))
        {
            return true;
        }

        // 获得缺省NSFileManager
        NSFileManager *const _file_mgr = [NSFileManager defaultManager];
        RUNTIME_ASSERT(_file_mgr != nil, "No default NSFIleManager instance");

        // 创建路径名称
        NSString *const _path_name = [NSString stringWithCString: (const char*)abs_path
                                                        encoding: NSUTF8StringEncoding];

        // 递归删除这个目录hierarchy
        return [_file_mgr removeItemAtPath: _path_name
                                     error: nil] == YES;
    }


    Bool
    NativeFileSystem::createFile (
        const UTF8 *const rel_path,
        const SearchPaths search_dir,
        FileStream &      file_stream)
    {
        // --- 创建/覆盖一个文件 --- //

        // 我们无法在Bundle目录下创建
        RUNTIME_ASSERT(search_dir != SEARCH_PATH_BUNDLE_FOLDER,
                       "No way to create in bundle folder");

        // 使用绝对路径来创建文件
        const RuntimeText _abs_path_name = getAbsPath(rel_path, search_dir);
        INFO(NativeFileSystem, "[Create file]: %s", _abs_path_name.c_str());
        NSString *const _abs_path = [NSString stringWithCString:(const char*)_abs_path_name.c_str()
                                                       encoding:NSUTF8StringEncoding];
        if (createFileAbs(_abs_path))
        {
            // 打开此文件进行读写
            NativeFile _file_opened(openFileAbsForHandle(_abs_path, ACCESS_MODES_READ_WRITE));
            if (_file_opened.isOpened())
            {
                file_stream.initWithFile(_file_opened);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }


    Bool
    NativeFileSystem::openFileRead (
        const UTF8 *const rel_path,
        const SearchPaths search_dir,
        FileReadStream &  file_stream)
    {
        // --- 打开一个存在的文件进行读入 --- //

        // 创建文件的绝对路径
        const RuntimeText _abs_path_name = getAbsPath(rel_path, search_dir);
        INFO(NativeFileSystem, "[Read file]: %s", _abs_path_name.c_str());
        NSString *const _abs_path = [NSString stringWithCString:(const char*)_abs_path_name.c_str()
                                                       encoding:NSUTF8StringEncoding];

        // 使用绝对路径打开此文件进行读出操作
        NativeFile _file_opened(openFileAbsForHandle(_abs_path, ACCESS_MODES_READ_ONLY));
        if (_file_opened.isOpened())
        {
            file_stream.initWithFile(_file_opened);
            return true;
        }
        else
        {
            return false;
        }
    }


    Bool
    NativeFileSystem::openFileWrite (
        const UTF8 *const rel_path,
        const SearchPaths search_dir,
        FileWriteStream & file_stream)
    {
        // --- 打开/创建一个文件进行写入 --- //

        // 创建文件的绝对路径
        const RuntimeText _abs_path_name = getAbsPath(rel_path, search_dir);
        INFO(NativeFileSystem, "[Write file]: %s", _abs_path_name.c_str());
        NSString *const _abs_path = [NSString stringWithCString:(const char*)_abs_path_name.c_str()
                                                       encoding:NSUTF8StringEncoding];

        // 检查文件是否存在, 如果不则创建它
        if (!existsFileAbs(_abs_path_name))
        {
            if (!createFileAbs(_abs_path))
            {
                return false;
            }
        }

        // 打开此文件进行写入操作
        NativeFile _file_opened(openFileAbsForHandle(_abs_path, ACCESS_MODES_WRITE_ONLY));
        if (_file_opened.isOpened())
        {
            file_stream.initWithFile(_file_opened);
            return true;
        }
        else
        {
            return false;
        }
    }


    Bool
    NativeFileSystem::openFile (
        const UTF8 *const rel_path,
        const SearchPaths search_dir,
        FileStream &      file_stream)
    {
        // --- 打开/创建一个文件进行读写 --- //

        // 创建文件的绝对路径
        const RuntimeText _abs_path_name = getAbsPath(rel_path, search_dir);
        INFO(NativeFileSystem, "[Open file]: %s", _abs_path_name.c_str());
        NSString *const _abs_path = [NSString stringWithCString:(const char*)_abs_path_name.c_str()
                                                       encoding:NSUTF8StringEncoding];

        // 检查文件是否存在, 如果不则创建它
        if (!existsFileAbs(_abs_path_name))
        {
            if (!createFileAbs(_abs_path))
            {
                return false;
            }
        }

        // 打开此文件进行写入操作
        NativeFile _file_opened(openFileAbsForHandle(_abs_path, ACCESS_MODES_READ_WRITE));
        if (_file_opened.isOpened())
        {
            file_stream.initWithFile(_file_opened);
            return true;
        }
        else
        {
            return false;
        }
    }



    //----------------------------------------------------------------------------------------------
    // PRIVATE
    //----------------------------------------------------------------------------------------------
    // MARK: - Private函数

    Bool
    NativeFileSystem::existsFileAbs (
        const RuntimeText & abs_path)
    {
        // 获得缺省NSFileManager
        NSFileManager *const _file_mgr    = [NSFileManager defaultManager];
        RUNTIME_ASSERT(_file_mgr != nil, "No default NSFIleManager instance");

        // 创建文件的路径
        NSString *const _path_name = [NSString stringWithCString: (const char*)abs_path.c_str()
                                                        encoding: NSUTF8StringEncoding];

        // 检查文件是否存在
        BOOL _is_folder; // 表示所给路径是否为目录
        const BOOL _is_available = [_file_mgr fileExistsAtPath: _path_name
                                                   isDirectory: &_is_folder];
        return (_is_available == YES) && (_is_folder == NO);
    }


    Bool
    NativeFileSystem::deleteFileAbs (
        const RuntimeText & abs_path)
    {
        // 检查文件是否存在
        if (!existsFileAbs(abs_path))
        {
            return false;
        }

        // 获得缺省NSFileManager
        NSFileManager *const _file_mgr  = [NSFileManager defaultManager];
        RUNTIME_ASSERT(_file_mgr != nil, "No default NSFIleManager instance");

        // 创建文件的路径
        NSString *const _path_name = [NSString stringWithCString: (const char*)abs_path.c_str()
                                                        encoding: NSUTF8StringEncoding];

        return [_file_mgr removeItemAtPath:_path_name error:nil] == YES;
    }

} // namespace Core

#endif // #if defined(__APPLE__)
