// Precompiled header
#include "PrecompiledH.hpp"


// System headers
// Lib headers
// Self
#include "Core/Thread/RS_MainDevThread.hpp"


namespace Core
{

    MainDevThread MainDevThread::ms_main_thread;


    DevThread*
    MainDevThread::mainThread ()
    {
        return &ms_main_thread;
    }


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
