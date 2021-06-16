// Precompiled header
#include "PrecompiledH.hpp"
// System headers
#if (BUILD_MODE == DEBUG_BUILD_MODE)
// System headers
#include <mutex>
// Lib headers
#include "Core/Thread/RS_MainDevThread.hpp"
// Self
#include "Core/Thread/RS_DevThreadDataBase.hpp"


namespace Core
{

    // 定义Database的访问锁
    static std::mutex g_database_lock;


    std::unordered_map<DevThreadIdT, DevThread*> DevThreadDataBase::thread_database;


    DevThread*
    DevThreadDataBase::threadFromId (
        const DevThreadIdT id)
    {
        if (MainDevThread::isMainThread())
        {
            return MainDevThread::mainThread();
        }
        else
        {
            g_database_lock.lock();
            RUNTIME_ASSERT(thread_database.find(id) != thread_database.end(),
                           "We can not find the expecting thread");
            DevThread* _found_thread = thread_database.find(id)->second;
            g_database_lock.unlock();
            return _found_thread;
        }
    }


    void
    DevThreadDataBase::add (
        const DevThreadIdT id,
        DevThread* const   thread)
    {
        RUNTIME_ASSERT(thread_database.find(id) == thread_database.end(),
                       "We have to remove this thread before adding it again");

        g_database_lock.lock();
        thread_database[id] = thread;
        g_database_lock.unlock();
    }


    void
    DevThreadDataBase::remove (
        const DevThreadIdT id)
    {
        RUNTIME_ASSERT(thread_database.find(id) != thread_database.end(),
                       "We have to add this thread before removing it");

        g_database_lock.lock();
        thread_database.erase(id);
        g_database_lock.unlock();
    }


} // namespace Core

#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
