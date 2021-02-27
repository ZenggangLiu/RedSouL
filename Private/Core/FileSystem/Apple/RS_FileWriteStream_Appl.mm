// Precompiled header
#include "PrecompiledH.hpp"
#if defined(__APPLE__)


// System headers
// Lib headers
#include "Core/FileSystem/Apple/RS_FileStreamHelper_Appl.hpp"
#include "Core/FileSystem/Apple/RS_NativeFile_Appl.hpp"
// Self
#include "Core/FileSystem/RS_FileWriteStream.hpp"


namespace Core
{

    IMPLEMENT_COMMON_STREAM_API(FileWriteStream);
    IMPLEMENT_WRITE_STREAM_API(FileWriteStream);

} // namespace Core

#endif // #if defined(__APPLE__)
