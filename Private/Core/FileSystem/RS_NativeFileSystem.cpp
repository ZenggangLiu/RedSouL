// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
// Self
#include "Core/FileSystem/RS_NativeFileSystem.hpp"


namespace Core
{

    RuntimeText
    NativeFileSystem::convFrontSlashToBack (
        const UTF8 *const path)
    {
        // 复制传入的路径名称
        RuntimeText _conv_path(path);

        // 检查当前字符是否为Slash(/)
        for (UInt32 _idx = 0; _idx < _conv_path.size(); ++_idx)
        {
            if (_conv_path[_idx] == '/')
            {
                _conv_path[_idx] = '\\';
            }
        }

        return  _conv_path;
    }


    RuntimeText
    NativeFileSystem::convBackSlashToFront (
        const UTF8 *const path)
    {
        // 复制传入的路径名称
        RuntimeText _conv_path(path);

        // 检查当前字符是否为Back Slash(\)
        for (UInt32 _idx = 0; _idx < _conv_path.size(); ++_idx)
        {
            if (_conv_path[_idx] == '\\')
            {
                _conv_path[_idx] = '/';
            }
        }

        return  _conv_path;
    }


    const UTF8*
    NativeFileSystem::getBundleDir ()
    {
        return ms_bundle_dir.c_str();
    }


    const UTF8*
    NativeFileSystem::getCacheDir ()
    {
        return ms_cache_dir.c_str();
    }


    const UTF8*
    NativeFileSystem::getDocumentDir ()
    {
        return ms_document_dir.c_str();
    }


    const UTF8*
    NativeFileSystem::getTemporaryDir ()
    {
        return ms_temporary_dir.c_str();
    }


    const UTF8*
    NativeFileSystem::getWorkingDir ()
    {
        return ms_working_dir.c_str();
    }


    RuntimeText
    NativeFileSystem::baseName (
        const RuntimeText & rel_name)
    {
        // 寻找'.'：Base name与Extension的分隔符
        size_t _period_pos;

        // 找不到扩展名
        if (rel_name.empty() || (_period_pos = rel_name.rfind('.')) == RuntimeText::npos)
        {
            return rel_name;
        }
        else
        {
            return rel_name.substr(0, _period_pos).c_str();
        }
    }


    RuntimeText
    NativeFileSystem::extension (
        const RuntimeText & rel_name)
    {
        // return [[NSString stringWithUTF8String:rel_filename.c_str()].pathExtension UTF8String];
        size_t _period_pos;
        // 找到'.'
        if (!rel_name.empty() && (_period_pos = rel_name.rfind('.')) != RuntimeText::npos)
        {
            // 返回'.'后面的子字符串
            return rel_name.substr(_period_pos+1).c_str();
        }
        else
        {
            return RuntimeText("");
        }
    }


    Bool
    NativeFileSystem::existsFile (
        const UTF8 *const rel_path,
        const SearchPaths search_dir)
    {
        // 使用绝对路径来检查文件是否存在
        const RuntimeText _abs_path = getAbsPath(rel_path, search_dir);
        return existsFileAbs(_abs_path);
    }


    Bool
    NativeFileSystem::deleteFile (
        const UTF8 *const rel_path,
        const SearchPaths search_dir)
    {
        // 无法删除Bundle目录下的文件
        if (search_dir == SEARCH_PATH_BUNDLE_FOLDER)
        {
            return false;
        }

        // 使用绝对路径来删除文件
        const RuntimeText _abs_path = getAbsPath(rel_path, search_dir);
        return deleteFileAbs(_abs_path);
    }



    //----------------------------------------------------------------------------------------------
    // PRIVATE
    //----------------------------------------------------------------------------------------------
    // MARK: - Private函数

    RuntimeText
    NativeFileSystem::getAbsPath (
        const UTF8 *const rel_path,
        const SearchPaths search_dir)
    {
        // 存储绝对路径名
        UTF8 _abs_path[512];
        // 表示从哪个目录下开始操作
        const UTF8 * search_dir_name;
        switch (search_dir)
        {
            case SEARCH_PATH_BUNDLE_FOLDER:
            {
                search_dir_name = getBundleDir();
                break;
            }

            case SEARCH_PATH_CACHE_FOLDER:
            {
                search_dir_name = getCacheDir();
                break;
            }

            case SEARCH_PATH_DOCUMENT_FOLDER:
            {
                search_dir_name = getDocumentDir();
                break;
            }

            case SEARCH_PATH_TEMPORARY_FOLDER:
            {
                search_dir_name = getTemporaryDir();
                break;
            }

            case  SEARCH_PATH_WORKING_FOLDER:
            {
                search_dir_name = getWorkingDir();
                break;
            }

            default:
            {
                RUNTIME_ASSERT(false, "Unknown search path");
                break;
            }
        }

        TextHelper::sprintfSafe(_abs_path, sizeof(_abs_path), "%s/%s",
                                search_dir_name, rel_path);

        return RuntimeText(_abs_path);
    }

} // namespace Core
