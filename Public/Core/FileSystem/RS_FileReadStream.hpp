#pragma once


// System headers
// Lib headers
#include "Core/FileSystem/RS_FileStreamHelper.hpp"


namespace Core
{

    // ReadOnly文件流
    class FileReadStream
    {
        DEFINE_COMMON_FILESTREAM_API(FileReadStream);
        DEFINE_READ_FILESTREAM_API  (FileReadStream);
    };

} // namespace Core
