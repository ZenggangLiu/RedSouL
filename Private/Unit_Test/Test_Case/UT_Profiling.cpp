// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (PROFILING_CHECKING == 1)
#include "Core/Profiling/RS_Profiling.hpp"
#include "Core/Thread/RS_ThreadHelper.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking Profiling", "[Profiling]")
{
    SECTION("Checking Profiling:")
    {
        std::printf("--- Checking Profiling...\n");

        // 必须使用{ } 来进行区域分割
        {
            // total branch time: 1 sec + 2 sec + 5 sec = 8 sec
            PROFILING("Branch I");
            {
                // waiting 0.1 second
                PROFILING("First Child");
                ThreadHelper::sleep(100);
            }

            {
                // waiting 0.2 seconds
                PROFILING("Second Child");
                ThreadHelper::sleep(200);
            }

            {
                // 5 times and each time 1 sec
                // totally 0.5 sec
                PROFILING("Recursive Child");
                {
                    for (UInt8 idx = 0; idx < 5; ++idx)
                    {
                        // 使用PROFILING()宏来定义一个Profiling Sample
                        // NOTE： 给定的名字必须时静态字符串
                        PROFILING("Recursion(5 Times)");

                        // waiting 1 second
                        ThreadHelper::sleep(100);
                    }
                }
            }
        }

        {
            // waiting 0.3 second
            PROFILING("Branch II");
            ThreadHelper::sleep(300);
        }

        // 使用PROFILING_DUMP() 来得到Sample Tree的文字描述
        PROFILING_DUMP();
        /*
         - Branch I: [#used]: 1, [Total]: 0.8350, [Avg Total]: 0.8350s, [Self]: 0.0001, [Avg Self]: 0.0001
           - First Child: [#used]: 1, [Total]: 0.1071, [Avg Total]: 0.1071s, [Self]: 0.1071, [Avg Self]: 0.1071
           - Second Child: [#used]: 1, [Total]: 0.2071, [Avg Total]: 0.2071s, [Self]: 0.2071, [Avg Self]: 0.2071
           - Recursive Child: [#used]: 1, [Total]: 0.5206, [Avg Total]: 0.5206s, [Self]: 0.0000, [Avg Self]: 0.0000
             - Recursion(5 Times): [#used]: 5, [Total]: 0.5206, [Avg Total]: 0.5206s, [Self]: 0.5206, [Avg Self]: 0.5206
         - Branch II: [#used]: 1, [Total]: 0.3028, [Avg Total]: 0.3028s, [Self]: 0.3028, [Avg Self]: 0.3028
         */
        std::printf("--- Checking Profiling: OK!\n");
    }
} // TEST_CASE("Checking Profiling", "[Profiling]")

#endif // #if (PROFILING_CHECKING == 1)
