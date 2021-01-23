#include "Core/Assert/RS_RuntimeAssert.hpp"


int
main (
    int               argc,
    const char *const argv[])
{
    RUNTIME_ASSERT(false, "False Codition check");
    return 0;
}
