#pragma once


// C++11 性能支持检测
#if !defined(CXX_STANDARD_VER)

    // Microsoft
    // - VS2015 defines __cplusplus as 199711L, which has support on c++14 in addition to c++11
    #if defined(_MSC_VER) && _MSC_VER >= 1900
        // 所以，从VS2015起，我们认为 C++11 完全支持：MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
        //we consider starting from vs 2015 a full support on c++ 11
        #define CXX_STANDARD_VER 2011

    #else
        // other compilers
        #if defined(__cplusplus)
            #if __cplusplus == 199711L
                #define CXX_STANDARD_VER 1998

            #elif __cplusplus == 201103L
                #define CXX_STANDARD_VER 2011
            #else
                #error No logic to check c++ standard later than C++11
            #endif // #if __cplusplus == 199711L

        #else
           // if the file is compiled by objc for instance, we just use old c++ 98
            #define CXX_STANDARD_VER 1998
        #endif // #if defined(__cplusplus)

    #endif // #if defined(_MSC_VER) && _MSC_VER >= 1900

#endif // #if !defined(CXX_STANDARD_VER)


// Exception 使用情况检测
#if !defined(USE_EXCEPTION)

    #if defined(__APPLE__)
        // Apple
        // NOTE: __EXCEPTIONS 定义为1，如果 C++ 或者 Objective-C 任何一方使用异常
        #if (__EXCEPTIONS == 1)
            #define USE_EXCEPTION 1
        #else
            #define USE_EXCEPTION 0
        #endif

    #elif defined(_MSC_VER)
        // Microsoft
        #if defined(_CPPUNWIND) && (_CPPUNWIND == 1)
            #define USE_EXCEPTION 1
        #else
            #define USE_EXCEPTION 0
        #endif

    #else
        // Linux
        #error TODO: No implementation
    #endif // #if defined(__APPLE__)

#endif // #if !defined(USE_EXCEPTION)


// RTTI 使用情况监测
#if !defined(USE_RTTI)

    #if defined(__APPLE__)
        #if (__GXX_RTTI == 1)
            #define USE_RTTI 1
        #else
            #define USE_RTTI 0
        #endif // #if (__GXX_RTTI == 1)

    #else
        #error TODO: No implementation
    #endif // #if defined(__APPLE__)

#endif // #if !defined(USE_RTTI)


// Build mode 检测
#if !defined(BUILD_MODE)
    #define DEBUG_BUILD_MODE    1
    #define RELEASE_BUILD_MODE  0

    // Debug mode
    #if !defined(NDEBUG) && (defined(DEBUG) || defined(_DEBUG))
        #define BUILD_MODE DEBUG_BUILD_MODE
        #define BUILD_MODE_STR "Debug build"

    // Release mode
    #else
        #define BUILD_MODE RELEASE_BUILD_MODE
        #define BUILD_MODE_STR "Release build"
    #endif // #if !defined(NDEBUG) && (defined(DEBUG) || defined(_DEBUG))
#endif // #if !defined(BUILD_MODE)
