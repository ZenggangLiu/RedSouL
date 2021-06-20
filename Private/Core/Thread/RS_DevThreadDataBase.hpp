#pragma once


#include "Core/Common/RS_CompilerDefs.hpp"
#if (BUILD_MODE == DEBUG_BUILD_MODE)
// System headers
#include <unordered_map>
// Lib headers
#include "Core/Thread/RS_DevThreadDefs.hpp"


namespace Core
{

    class DevThread;

    // 在Debug模式下，用来保存所有创建的线程的参考
    struct DevThreadDataBase
    {
        // <Thread ID，Thread实例>
        static std::unordered_map<DevThreadIdT, DevThread*> thread_database;

        static
        DevThread*
        threadFromId (
            const DevThreadIdT id);

        // 注册指定线程
        static
        void
        add (
            const DevThreadIdT id,
            DevThread* const   thread);

        // 注销指定线程
        static
        void
        remove (
            const DevThreadIdT id);
    };

}

#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
