#define RED_SOUL_CHECKING       1


#if (RED_SOUL_CHECKING == 1)
#include "RS_Checksuite.cpp"
#endif


int
main (
    int               argc,
    const char *const argv[])
{
#if (RED_SOUL_CHECKING == 1)
    Check();
#endif

    return 0;
}

