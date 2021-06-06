// Precompiled header
#include "PrecompiledH.hpp"


// System headers
// Lib headers
// Self
#include "Core/Thread/RS_MainDevThread.hpp"

namespace Core
{

    // 定义MainThread的全局实例
    const MainDevThread g_Main_Thread;


    MainDevThread::MainDevThread ()
    :
        SUPER("Main")
    {}


    Bool
    MainDevThread::start ()
    { return true; }


    void
    MainDevThread::suspend ()
    {}


    void
    MainDevThread::resume ()
    {}


    void
    MainDevThread::join () const
    {}


    void
    MainDevThread::kill (
        const UInt32)
    {}

} // namespace Core
