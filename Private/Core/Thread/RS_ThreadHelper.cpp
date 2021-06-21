// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/Thread/RS_MainDevThread.hpp"
// Self
#include "Core/Thread/RS_ThreadHelper.hpp"


namespace Core
{

    Bool
    ThreadHelper::isMainThread ()
    {
        return MainDevThread::isMainThread();
    }

} // namespace Core