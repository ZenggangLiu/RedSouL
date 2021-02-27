#pragma once


// System headers
// Lib headers
#include "Core/FileSystem/RS_FileStreamHelper.hpp"


namespace Core
{

    // WriteOnly文件流
    class FileWriteStream
    {
        DEFINE_COMMON_FILESTREAM_API(FileWriteStream);
        DEFINE_WRITE_FILESTREAM_API (FileWriteStream);
    };

} // namespace Core
