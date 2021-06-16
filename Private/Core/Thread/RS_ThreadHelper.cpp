// Precompiled header
//#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "RS_MainDevThread.hpp"
// Self
#include "RS_ThreadHelper.hpp"


namespace Core
{

    Bool
    ThreadHelper::isMainThread ()
    {
        return MainDevThread::isMainThread();
    }

} // namespace Core