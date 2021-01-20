#pragma once


#if !defined(OS_TYPE_UNKNOWN)
    #define OS_TYPE_UNKNOWN                     0xFFFF
    #define OS_TYPE_STR_UNKNOWN                 "UNKNOWN"
    #define OS_SUB_TYPE_UNKNOWN                 0xFFFF
    #define OS_SUB_TYPE_STR_UNKNOWN             "UNKNOWN"
    #define CPU_TYPE_UNKNOWN                    0xFFFF
    #define CPU_TYPE_STR_UNKNOWN                "UNKNOWN"
#endif // #if !defined(STATIC_ASSERT)

#if !defined(CPU_TYPE_32)
    #define CPU_TYPE_32                         32
    #define CPU_TYPE_32_STR                     "32Bits"
#endif // #if !defined(CPU_TYPE_32)

#if !defined(CPU_TYPE_64)
    #define CPU_TYPE_64                         64
    #define CPU_TYPE_64_STR                     "64Bits"
#endif // #if !defined(CPU_TYPE_64)


//--------------------------------------------------------------------------------------------------
// WINDOWS DEFINES
//--------------------------------------------------------------------------------------------------
// MARK: - Windows 相关定义

// Windows: 0x0xxx
#if !defined(OS_TYPE_WIN)
    #define OS_TYPE_WIN                         0x0001
    #define OS_TYPE_WIN_STR                     "Windows OS"
#endif // #if !defined(OS_TYPE_WIN)

// Windows PC application
#if !defined(OS_SUB_TYPE_WIN10)
    #define OS_SUB_TYPE_WIN10                   0x0100
    #define OS_SUB_TYPE_WIN10_STR               "Windows10 OS"
#endif // #if !defined(OS_SUB_TYPE_WIN10)

// Windows store application
#if !defined(OS_SUB_TYPE_WIN10_STORE)
    #define OS_SUB_TYPE_WIN10_STORE             0x0101
    #define OS_SUB_TYPE_WIN10_STORE_STR         "Windows Store App"
#endif // #if !defined(OS_SUB_TYPE_WIN10_STORE)

// Windows phone application
#if !defined(OS_SUB_TYPE_WIN_PHONE10)
    #define OS_SUB_TYPE_WIN_PHONE10             0x0102
    #define OS_SUB_TYPE_WIN_PHONE10_STR         "Windows Phone App"
#endif // #if !defined(OS_SUB_TYPE_WIN_PHONE10)



//--------------------------------------------------------------------------------------------------
// APPLE DEFINES
//--------------------------------------------------------------------------------------------------
// MARK: - Apple 相关定义

// Apple: 0x1xxx
#if !defined(OS_TYPE_MAC)
    #define OS_TYPE_MAC                         0x1000
    #define OS_TYPE_MAC_STR                     "Mac OS"
#endif // #if !defined(OS_TYPE_MAC)

#if !defined(OS_TYPE_IOS)
    #define OS_TYPE_IOS                         0x1A00
    #define OS_TYPE_IOS_STR                     "iOS"
#endif // #if !defined(OS_TYPE_IOS)

#if !defined(OS_TYPE_TVOS)
    #define OS_TYPE_TVOS                        0x1B00
    #define OS_TYPE_TVOS_STR                    "tvOS"
#endif // #if !defined(OS_TYPE_TVOS)

#if !defined(OS_SUB_TYPE_MAC_STORE)
    #define OS_SUB_TYPE_MAC_STORE               0xAE  // MAC STORE
    #define OS_SUB_TYPE_MAC_STORE_STR           "MacOS Store App"
#endif // #if !defined(OS_SUB_TYPE_MAC_STORE)

#if !defined(OS_SUB_TYPE_IOS_DEVICE)
    #define OS_SUB_TYPE_IOS_DEVICE              0x1ADE
    #define OS_SUB_TYPE_IOS_DEVICE_STR          "iOS Device"
#endif // #if !defined(OS_SUB_TYPE_IOS_DEVICE)

#if !defined(OS_SUB_TYPE_TVOS_DEVICE)
    #define OS_SUB_TYPE_TVOS_DEVICE             0x1BDE
    #define OS_SUB_TYPE_TVOS_DEVICE_STR         "tvOS Device"
#endif // #if !defined(OS_SUB_TYPE_TVOS_DEVICE)

#if !defined(OS_SUB_TYPE_IOS_SIMULATOR)
    #define OS_SUB_TYPE_IOS_SIMULATOR           0x1A21 // SI --> 2I --> 21
    #define OS_SUB_TYPE_IOS_SIMULATOR_STR       "iOS Simulator"
#endif // #if !defined(OS_SUB_TYPE_IOS_SIMULATOR)

#if !defined(OS_SUB_TYPE_TVOS_SIMULATOR)
    #define OS_SUB_TYPE_TVOS_SIMULATOR          0x1B21 // SI --> 2I --> 21
    #define OS_SUB_TYPE_TVOS_SIMULATOR_STR      "tvOS Simulator"
#endif // #if !defined(OS_SUB_TYPE_TVOS_SIMULATOR)



//--------------------------------------------------------------------------------------------------
// LINUX DEFINES
//--------------------------------------------------------------------------------------------------
// MARK: - Linux 相关定义

// Linux: 0x2xxx
#if !defined(OS_TYPE_LINUX)
    #define OS_TYPE_LINUX                       0x2000
    #define OS_TYPE_LINUX_STR                   "Linux OS"
#endif // #if !defined(OS_TYPE_LINUX)



//--------------------------------------------------------------------------------------------------
// ANDROID DEFINES
//--------------------------------------------------------------------------------------------------
// MARK: - Android 相关定义

// Android: 0x3xxx
#if !defined(OS_TYPE_ANDROID)
    #define OS_TYPE_ANDROID                     0x3000
    #define OS_TYPE_ANDROID_STR                 "Andriod OS"
#endif // #if !defined(OS_TYPE_ANDROID)



//--------------------------------------------------------------------------------------------------
// DEFAULTS
//--------------------------------------------------------------------------------------------------
// MARK: - Defaults 缺省定义

// defines all defaults
#if !defined(OS_TYPE)
    #undef  OS_TYPE
    #define OS_TYPE OS_TYPE_UNKNOWN
    #undef  OS_TYPE_STRING
    #define OS_TYPE_STRING OS_TYPE_STR_UNKNOWN
    #undef  OS_SUB_TYPE
    #define OS_SUB_TYPE OS_SUB_TYPE_UNKNOWN
    #undef  OS_SUB_TYPE_STRING
    #define OS_SUB_TYPE_STRING OS_SUB_TYPE_STR_UNKNOWN
    #undef  CPU_TYPE
    #define CPU_TYPE CPU_TYPE_UNKNOWN
    #undef  CPU_TYPE_STRING
    #define CPU_TYPE_STRING CPU_TYPE_STR_UNKNOWN
#endif // #if !defined(OS_TYPE)



//--------------------------------------------------------------------------------------------------
// WINDOWS CHECKING
//--------------------------------------------------------------------------------------------------
// MARK: - Windows 检测

// ------------ Windows Family ------------ //
// --- Checks OS_TYPE --- //
// 如果 OS_TYPE 使用缺省值（OS_TYPE_UNKNOWN）: 表示无意义的OS类型
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
        #undef  OS_TYPE
        #define OS_TYPE OS_TYPE_WIN
        #undef  OS_TYPE_STRING
        #define OS_TYPE_STRING OS_TYPE_WIN_STR
    #endif // defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#endif // #if (OS_TYPE == OS_TYPE_UNKNOWN)

// --- Checks OS_SUB_TYPE --- //
// 如果 OS_SUB_TYPE 为缺省值（OS_SUB_TYPE_UNKNOWN）进行如下诸多检测

#if (OS_SUB_TYPE == OS_SUB_TYPE_UNKNOWN)
    #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
        #undef  OS_SUB_TYPE
        #define OS_SUB_TYPE OS_SUB_TYPE_WIN10_STORE
        #undef  OS_SUB_TYPE_STRING
        #define OS_SUB_TYPE_STRING OS_SUB_TYPE_WIN10_STORE_STR
    #endif // #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#endif // #if (OS_SUB_TYPE == OS_TYPE_UNKNOWN)

#if (OS_SUB_TYPE == OS_TYPE_UNKNOWN)
    #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
        #undef  OS_SUB_TYPE
        #define OS_SUB_TYPE OS_SUB_TYPE_WIN_PHONE10
        #undef  OS_SUB_TYPE_STRING
        #define OS_SUB_TYPE_STRING OS_SUB_TYPE_WIN_PHONE10_STR
    #endif // #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#endif // #if (OS_SUB_TYPE == OS_TYPE_UNKNOWN)

#if (OS_SUB_TYPE == OS_TYPE_UNKNOWN)
    #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
        #undef  OS_SUB_TYPE
        #define OS_SUB_TYPE OS_SUB_TYPE_WIN10
        #undef  OS_SUB_TYPE_STRING
        #define OS_SUB_TYPE_STRING OS_SUB_TYPE_WIN10_STR
    #endif // #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#endif // #if (OS_SUB_TYPE == OS_TYPE_UNKNOWN)

// --- Checks CPU_TYPE --- //
// 首先检测 WIN64，由于 Win64 以及 Win32 在 x64 平台上同时定义
// at first check on WIN64, because WIN64 and WIN32 both will be defined on x64 platform

#if (CPU_TYPE == CPU_TYPE_UNKNOWN)
    #if defined(WIN64) || defined(_WIN64)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_64
        #undef  CPU_TYPE_STRING
        #define CPU_TYPE_STRING CPU_TYPE_64_STR
    #elif defined(WIN32) || defined(_WIN32)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_32
        #undef  CPU_TYPE_STRING
        #define CPU_TYPE_STRING CPU_TYPE_32_STR
    #endif // #if defined(WIN64) || defined(_WIN64)
#endif // #if (CPU_TYPE ==  CPU_TYPE_UNKNOWN)



//--------------------------------------------------------------------------------------------------
// APPLE CHECKING
//--------------------------------------------------------------------------------------------------
// MARK: - Apple 检测

// 使用如下方式 DUMP 出所有预定义的鸿
// How to dump all predefined marcos:
// -x <language>           Treat subsequent input files as having type <language>
// -dM                     Print macro definitions in -E mode instead of normal output
// -E                      Only run the preprocessor
// ARCH 参数可以取如下数值: arm64, x86_64, i386
//
// /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
// -x c++ -arch ARCH -std=c++11 -stdlib=libc++ -fno-exceptions -fno-rtti -dM -E /dev/null
// SEE: Doc/XCode/Predefined_Macros
//
// ------------ Apple Family ------------ //
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    // --- Checks OS_TYPE and OS_SUB_TYPE --- //
    #if __APPLE__
        #include <TargetConditionals.h>
        // +----------------------------------------------------------------+
        // |                TARGET_OS_MAC                                   |
        // | +---+  +-----------------------------------------------------+ |
        // | |   |  |          TARGET_OS_IPHONE                           | |
        // | |OSX|  | +-----+ +----+ +-------+ +--------+ +-------------+ | |
        // | |   |  | | IOS | | TV | | WATCH | | BRIDGE | | MACCATALYST | | |
        // | |   |  | +-----+ +----+ +-------+ +--------+ +-------------+ | |
        // | +---+  +-----------------------------------------------------+ |
        // +----------------------------------------------------------------+

        #if TARGET_OS_IOS && TARGET_OS_SIMULATOR
            // iOS 模拟器（Simulator）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_IOS
            #undef  OS_TYPE_STRING
            #define OS_TYPE_STRING OS_TYPE_IOS_STR
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_IOS_SIMULATOR
            #undef  OS_SUB_TYPE_STRING
            #define OS_SUB_TYPE_STRING OS_SUB_TYPE_IOS_SIMULATOR_STR

        #elif TARGET_OS_IOS
            // iOS 硬件设备（device）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_IOS
            #undef  OS_TYPE_STRING
            #define OS_TYPE_STRING OS_TYPE_IOS_STR
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_IOS_DEVICE
            #undef  OS_SUB_TYPE_STRING
            #define OS_SUB_TYPE_STRING OS_SUB_TYPE_IOS_DEVICE_STR

        #elif TARGET_OS_TV && TARGET_OS_SIMULATOR
            // tvOS 模拟器（Simulator）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_TVOS
            #undef  OS_TYPE_STRING
            #define OS_TYPE_STRING OS_TYPE_TVOS_STR
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_TVOS_SIMULATOR
            #undef  OS_SUB_TYPE_STRING
            #define OS_SUB_TYPE_STRING OS_SUB_TYPE_TVOS_SIMULATOR_STR

        #elif TARGET_OS_TV
            // tvOS 硬件设备（device）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_TVOS
            #undef  OS_TYPE_STRING
            #define OS_TYPE_STRING OS_TYPE_TVOS_STR
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_TVOS_DEVICE
            #undef  OS_SUB_TYPE_STRING
            #define OS_SUB_TYPE_STRING OS_SUB_TYPE_TVOS_DEVICE_STR

        #else
            // macOS
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_MAC
            #undef  OS_TYPE_STRING
            #define OS_TYPE_STRING OS_TYPE_MAC_STR
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_MAC_STORE
            #undef  OS_SUB_TYPE_STRING
            #define OS_SUB_TYPE_STRING OS_SUB_TYPE_MAC_STORE_STR
        #endif // #if TARGET_OS_IOS && TARGET_IPHONE_SIMULATOR
    #endif // #if __APPLE__

    // --- Checks CPU_TYPE --- //
    #if defined(_LP64) || defined(__LP64__)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_64
        #undef  CPU_TYPE_STRING
        #define CPU_TYPE_STRING CPU_TYPE_64_STR
    #else
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_32
        #undef  CPU_TYPE_STRING
        #define CPU_TYPE_STRING CPU_TYPE_32_STR
    #endif // #if defined(__x86_64) || defined(__x86_64__)
#endif // #if (OS_TYPE == OS_TYPE_UNKNOWN)



//--------------------------------------------------------------------------------------------------
// LINUX CHECKING
//--------------------------------------------------------------------------------------------------
// MARK: - Linux检测

// ------------ Linux Family ------------ //
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    #error No Implementation
#endif // #if (OS_TYPE == OS_TYPE_UNKNOWN)



//--------------------------------------------------------------------------------------------------
// ANDROID CHECKING
//--------------------------------------------------------------------------------------------------
// MARK: - Android检测

// ------------ Android Family ------------ //
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    #if defined(PLATFORM_ANDROID)
        #undef  OS_TYPE
        #define OS_TYPE OS_TYPE_ANDROID
        #undef  OS_TYPE_STRING
        #define OS_TYPE_STRING OS_TYPE_ANDROID_STR
    #endif // #if defined(PLATFORM_ANDROID)
#endif // #if (OS_TYPE == OS_TYPE_UNKNOWN)
