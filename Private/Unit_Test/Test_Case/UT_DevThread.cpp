// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (DEV_THREAD_CHECKING == 1)
#include "Core/Thread/RS_DevThread.hpp"
#include "Core/Thread/RS_ThreadHelper.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking DevThread", "[DevThread]")
{

    class Example : public Core::DevThread
    {
    public:
        UInt32 IntData;
    public:
        Example()
        :
            Core::DevThread("Example"),
            IntData(0)
        {

        }


    private:
        // The ThreadProc method is called when the thread starts.
        // It loops ten times, writing to the console and yielding
        // the rest of its time slice each time, and then ends.
        virtual
        UInt32
        coRutine () override
        {
            for (int i = 0; i < 10; i++)
            {
                IntData++;
                std::printf("[Example]: Increase IntData: %u\n", IntData);
                // have a nap
                ThreadHelper::sleep(100);
            }
            return 0;
        }
    };


    SECTION("Checking DevThread:")
    {
        std::printf("--- Checking DevThread...\n");

        std::printf("[Main]: Start Example thread\n");

        Example t;
        t.start();
        for (int i = 0; i < 4; i++)
        {
            std::printf("[Main]: Sleep 0.1s\n");
            ThreadHelper::sleep(100);
        }
        
        std::printf("[Main]: Join the Example thread\n");
        t.join();
        REQUIRE(t.IntData == 10);

        std::printf("--- Checking DevThread: OK!\n");
    }
} // TEST_CASE("Checking DevThread", "[DevThread]")

#endif // #if (DEV_THREAD_CHECKING == 1)
