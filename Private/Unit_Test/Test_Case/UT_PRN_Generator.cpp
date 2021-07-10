// System headers
#include <cstdlib>          // std::rand
// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (PRN_GENERATOR_CHECKING == 1)
#include "Core/Mathe/RS_PRN_Generator.hpp"
#include "Core/Time/RS_Time.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking PRN generator", "[PRN genrator]")
{

    static const UInt32 GENERATE_COUNT = 100000000;

    PRN_Generator      _regular_prn_gen;
    PRN_Generator_Fast _fast_prn_gen;

    SECTION("Checking PRN generator:")
    {
        std::printf("--- Checking PRN generator...\n");
        std::printf("Generating %u numbers...\n", GENERATE_COUNT);

        Real64 _start_time, _end_time;

        {
            SInt16 _rand_num = 0;
            _start_time = getCurTimeSec();
            for (UInt32 c = 0; c < GENERATE_COUNT; ++c)
            {
                _rand_num = std::rand();
            }
            _end_time = getCurTimeSec();
            std::printf("[Used seconds]: [Std]:      %fs, [Last]: %u\n",
                        (Real32)(_end_time - _start_time), _rand_num);
        }

        {
            UInt64 _rand_num = 0;
            _start_time = getCurTimeSec();
            for (UInt32 c = 0; c < GENERATE_COUNT; ++c)
            {
                _rand_num = _regular_prn_gen.getUInt64();
            }
            _end_time = getCurTimeSec();
            std::printf("[Used seconds]: [PRN]:      %fs, [Last]: %llu\n",
                        (Real32)(_end_time - _start_time), _rand_num);
        }

        {
            UInt64 _rand_num = 0;
            _start_time = getCurTimeSec();
            for (UInt32 c = 0; c < GENERATE_COUNT; ++c)
            {
                _rand_num = _fast_prn_gen.getUInt64();
            }
            _end_time = getCurTimeSec();
            std::printf("[Used seconds]: [PRN_Fast]: %fs, [Last]: %llu\n",
                        (Real32)(_end_time - _start_time), _rand_num);
        }

        std::printf("--- Checking PRN generator: OK!\n");
    }

#if (PRN_GENERATOR_DISTRIBUTION_CHECKING == 1)
    SECTION("Checking PRN generator distribution:")
    {
        std::printf("--- Checking PRN generator distribution...\n");
        const UInt32 ROW_COUNT = 16;
        const UInt32 COL_COUNT = 16;

        std::printf("---------- STD::RAND ----------\n");
        for (UInt32 i = 0; i < ROW_COUNT; ++i)
        {
            for (UInt32 j = 0; j < COL_COUNT; ++j)
            {
                //std::rand(): [0, RAND_MAX] := [0, 32767]
                const Real32 r1 = (Real32)std::rand() / RAND_MAX;
                const Real32 r2 = (Real32)std::rand() / RAND_MAX;
                std::printf("%f, %f\n", r1, r2);
            }
        }

        std::printf("---------- PRN ----------\n");
        for (UInt32 i = 0; i < ROW_COUNT; ++i)
        {
            for (UInt32 j = 0; j < COL_COUNT; ++j)
            {
                const Real32 r1 = _regular_prn_gen.getReal32();
                const Real32 r2 = _regular_prn_gen.getReal32();
                std::printf("%f, %f\n", r1, r2);
            }
        }

        std::printf("---------- PRN FAST ----------\n");
        for (UInt32 i = 0; i < ROW_COUNT; ++i)
        {
            for (UInt32 j = 0; j < COL_COUNT; ++j)
            {
                const Real32 r1 = _fast_prn_gen.getReal32();
                const Real32 r2 = _fast_prn_gen.getReal32();
                std::printf("%f, %f\n", r1, r2);
            }
        }

        std::printf("--- Checking PRN generator distribution: OK!\n");
    }
#endif // #if (PRN_GENERATOR_DISTRIBUTION_CHECKING == 1)
    
} // TEST_CASE("Checking PRN generator", "[PRN generator]")

#endif // #if (PRN_GENERATOR_CHECKING == 1)
