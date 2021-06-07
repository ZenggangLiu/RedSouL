// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (MATHE_FUNCS_CHECKING == 1)
#include "Core/Mathe/RS_MatheDefs.hpp"
#include "Core/Mathe/RS_MatheFuncs.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking MatheFuncs", "[MatheFunc]")
{


    SECTION("Checking MatheFunc:")
    {
        std::printf("--- Checking MatheFunc...\n");

        const SInt32 _minni = -567;
        const SInt32 _maxni = -345;
        const SInt32 _minpi =  345;
        const SInt32 _maxpi =  567;

        const Real32 _minnr = -3.1425;
        const Real32 _maxnr = -2.7777;
        const Real32 _minpr =  2.7777;
        const Real32 _maxpr =  3.1425;

        // clamp()
        REQUIRE(Mathe::clamp(-570, _minni, _maxni) == _minni);
        REQUIRE(Mathe::clamp(-560, _minni, _maxni) == -560);
        REQUIRE(Mathe::clamp(-340, _minni, _maxni) == _maxni);
        REQUIRE(Mathe::clamp( 340, _minni, _maxni) == _maxni);
        REQUIRE(Mathe::clamp( 340, _minpi, _maxpi) == _minpi);
        REQUIRE(Mathe::clamp( 350, _minpi, _maxpi) == 350);
        REQUIRE(Mathe::clamp( 570, _minpi, _maxpi) == _maxpi);

        REQUIRE(Mathe::clamp(-3.2425f, _minnr, _maxnr) == _minnr);
        REQUIRE(Mathe::clamp(-3.0425f, _minnr, _maxnr) == -3.0425f);
        REQUIRE(Mathe::clamp(-2.6777f, _minnr, _maxnr) == _maxnr);
        REQUIRE(Mathe::clamp( 2.7777f, _minnr, _maxnr) == _maxnr);
        REQUIRE(Mathe::clamp( 2.6777f, _minpr, _maxpr) == _minpr);
        REQUIRE(Mathe::clamp( 2.8777f, _minpr, _maxpr) == 2.8777f);
        REQUIRE(Mathe::clamp( 3.2425f, _minpr, _maxpr) == _maxpr);

        // maximum()
        REQUIRE(Mathe::maximum(_minni, _maxni) == _maxni);
        REQUIRE(Mathe::maximum(_minpi, _maxpi) == _maxpi);
        REQUIRE(Mathe::maximum(_minni, _minpi) == _minpi);
        REQUIRE(Mathe::maximum(_maxni, _maxpi) == _maxpi);
        REQUIRE(Mathe::maximum(_minnr, _maxnr) == _maxnr);
        REQUIRE(Mathe::maximum(_minpr, _maxpr) == _maxpr);
        REQUIRE(Mathe::maximum(_minnr, _minpr) == _minpr);
        REQUIRE(Mathe::maximum(_maxnr, _maxpr) == _maxpr);

        // maxMinimum()
        SInt32 _mini, _maxi;
        Real32 _minr, _maxr;
        std::tie(_mini, _maxi) = Mathe::maxMinimum(_minni, _maxni);
        REQUIRE(((_mini == _mini) && (_maxi == _maxni)));
        std::tie(_mini, _maxi) = Mathe::maxMinimum(_maxni, _minni);
        REQUIRE(((_mini == _mini) && (_maxi == _maxni)));
        std::tie(_mini, _maxi) = Mathe::maxMinimum(_minpi, _maxpi);
        REQUIRE(((_mini == _minpi) && (_maxi == _maxpi)));
        std::tie(_mini, _maxi) = Mathe::maxMinimum(_maxpi, _minpi);
        REQUIRE(((_mini == _minpi) && (_maxi == _maxpi)));
        std::tie(_mini, _maxi) = Mathe::maxMinimum(_minni, _maxpi);
        REQUIRE(((_mini == _mini) && (_maxi == _maxpi)));
        std::tie(_mini, _maxi) = Mathe::maxMinimum(_maxpi, _minni);
        REQUIRE(((_mini == _mini) && (_maxi == _maxpi)));
        std::tie(_minr, _maxr) = Mathe::maxMinimum(_minnr, _maxnr);
        REQUIRE(((_minr == _minr) && (_maxr == _maxnr)));
        std::tie(_minr, _maxr) = Mathe::maxMinimum(_maxnr, _minnr);
        REQUIRE(((_minr == _minr) && (_maxr == _maxnr)));
        std::tie(_minr, _maxr) = Mathe::maxMinimum(_minpr, _maxpr);
        REQUIRE(((_minr == _minpr) && (_maxr == _maxpr)));
        std::tie(_minr, _maxr) = Mathe::maxMinimum(_maxpr, _minpr);
        REQUIRE(((_minr == _minpr) && (_maxr == _maxpr)));
        std::tie(_minr, _maxr) = Mathe::maxMinimum(_minnr, _maxpr);
        REQUIRE(((_minr == _minr) && (_maxr == _maxpr)));
        std::tie(_minr, _maxr) = Mathe::maxMinimum(_maxpr, _minnr);
        REQUIRE(((_minr == _minr) && (_maxr == _maxpr)));

        // minimum
        REQUIRE(Mathe::minimum(_minni, _maxni) == _minni);
        REQUIRE(Mathe::minimum(_minpi, _maxpi) == _minpi);
        REQUIRE(Mathe::minimum(_minni, _minpi) == _minni);
        REQUIRE(Mathe::minimum(_maxni, _maxpi) == _maxni);
        REQUIRE(Mathe::minimum(_minnr, _maxnr) == _minnr);
        REQUIRE(Mathe::minimum(_minpr, _maxpr) == _minpr);
        REQUIRE(Mathe::minimum(_minnr, _minpr) == _minnr);
        REQUIRE(Mathe::minimum(_maxnr, _maxpr) == _maxnr);

        std::printf("--- Checking MatheFunc: OK!\n");
    }
} // TEST_CASE("Checking MatheFuncs", "[MatheFunc]")

#endif // #if (MATHE_FUNCS_CHECKING == 1)
