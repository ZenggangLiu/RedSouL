// Precompile header
#include "PrecompiledH.hpp"
#if (OS_TYPE == OS_TYPE_WIN)


// System headers
// Lib headers
#include "Core/Thread/RS_DevThreadDataBase.hpp"
// Self
#include "Core/Thread/RS_AutoResetEvent.hpp"


namespace Core
{

    AutoResetEvent::AutoResetEvent (
        const Bool         set_on_create,
        const ASCII *const name)
#if (BUILD_MODE == DEBUG_BUILD_MODE)
    :
        m_handle((HANDLE)-1),
        m_name(name ? name : "Un-named AutoResetEvent")
#endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
    {
        m_handle = CreateEventA(nullptr, FALSE, set_on_create ? TRUE : FALSE, name);
    }


    AutoResetEvent::~AutoResetEvent ()
    {
        close();
    }


    Bool
    AutoResetEvent::set ()
    {
        // SetEvent(): 成功放回非零
        const Bool _success_set = SetEvent(m_handle);
        return _success_set;
    }


    Bool
    AutoResetEvent::wait ()
    {
        // WaitForSingleObject()：返回WAIT_OBJECT_0，如果AutoResetEvent在Signal状态
        const Bool _success_wait = WaitForSingleObject(
            m_handle, INFINITE) == WAIT_OBJECT_0;
        return _success_wait;

    }


    Bool
    AutoResetEvent::wait (
        const UInt32 wait_time)
    {
        // WaitForSingleObject()：返回WAIT_OBJECT_0，如果AutoResetEvent在Signal状态
        const Bool _success_wait = WaitForSingleObject(
            m_handle, (DWORD)wait_time) == WAIT_OBJECT_0;
        return _success_wait;
    }


    void
    AutoResetEvent::close ()
    {
        CloseHandle(m_handle);
        m_handle = (HANDLE)-1;
    }

}

#endif // #if (OS_TYPE == OS_TYPE_WIN)


/*
using System;
using System.Threading;

// Visual Studio: Replace the default class in a Console project with
//                the following class.
class Example
{
    private static AutoResetEvent event_1 = new AutoResetEvent(true);
    private static AutoResetEvent event_2 = new AutoResetEvent(false);

    static void Main()
    {
        Console.WriteLine("Press Enter to create three threads and start them.\r\n" +
                          "The threads wait on AutoResetEvent #1, which was created\r\n" +
                          "in the signaled state, so the first thread is released.\r\n" +
                          "This puts AutoResetEvent #1 into the unsignaled state.");
        Console.ReadLine();

        for (int i = 1; i < 4; i++)
        {
            Thread t = new Thread(ThreadProc);
            t.Name = "Thread_" + i;
            t.Start();
        }
        Thread.Sleep(250);

        for (int i = 0; i < 2; i++)
        {
            Console.WriteLine("Press Enter to release another thread.");
            Console.ReadLine();
            event_1.Set();
            Thread.Sleep(250);
        }

        Console.WriteLine("\r\nAll threads are now waiting on AutoResetEvent #2.");
        for (int i = 0; i < 3; i++)
        {
            Console.WriteLine("Press Enter to release a thread.");
            Console.ReadLine();
            event_2.Set();
            Thread.Sleep(250);
        }

        // Visual Studio: Uncomment the following line.
        //Console.Readline();
    }

    static void ThreadProc()
    {
        string name = Thread.CurrentThread.Name;

        Console.WriteLine("{0} waits on AutoResetEvent #1.", name);
        event_1.WaitOne();
        Console.WriteLine("{0} is released from AutoResetEvent #1.", name);

        Console.WriteLine("{0} waits on AutoResetEvent #2.", name);
        event_2.WaitOne();
        Console.WriteLine("{0} is released from AutoResetEvent #2.", name);

        Console.WriteLine("{0} ends.", name);
    }
}

/* This example produces output similar to the following:

Press Enter to create three threads and start them.
The threads wait on AutoResetEvent #1, which was created
in the signaled state, so the first thread is released.
This puts AutoResetEvent #1 into the unsignaled state.

Thread_1 waits on AutoResetEvent #1.
Thread_1 is released from AutoResetEvent #1.
Thread_1 waits on AutoResetEvent #2.
Thread_3 waits on AutoResetEvent #1.
Thread_2 waits on AutoResetEvent #1.
Press Enter to release another thread.

Thread_3 is released from AutoResetEvent #1.
Thread_3 waits on AutoResetEvent #2.
Press Enter to release another thread.

Thread_2 is released from AutoResetEvent #1.
Thread_2 waits on AutoResetEvent #2.

All threads are now waiting on AutoResetEvent #2.
Press Enter to release a thread.

Thread_2 is released from AutoResetEvent #2.
Thread_2 ends.
Press Enter to release a thread.

Thread_1 is released from AutoResetEvent #2.
Thread_1 ends.
Press Enter to release a thread.

Thread_3 is released from AutoResetEvent #2.
Thread_3 ends.
 */
