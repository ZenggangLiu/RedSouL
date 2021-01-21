// 使用如下方式 DUMP 出所有预定义的鸿
// How to dump all predefined marcos:
// -x <language>           Treat subsequent input files as having type <language>
// -dM                     Print macro definitions in -E mode instead of normal output
// -E                      Only run the preprocessor
// ARCH 参数可以取如下数值: arm64, x86_64, i386
//
// /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
// -x c++ -arch ARCH -std=c++11 -stdlib=libc++ -fno-exceptions -fno-rtti -dM -E /dev/null

--- arm 64 ---
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -arch arm64 -std=c++11 -stdlib=libc++ -dM -E /dev/null > predefined-macros_arm64.txt

--- x64 ---
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -arch x86_64 -std=c++11 -stdlib=libc++ -dM -E /dev/null > predefined-macros_x86_64.txt

--- i386 ---
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -arch i386 -std=c++11 -stdlib=libc++ -dM -E /dev/null > predefined-macros_i386.txt



---- USEFUL MACROS ----
#define __VERSION__ "4.2.1 Compatible Apple LLVM 10.0.1 (clang-1001.0.46.4)"
#define __apple_build_version__ 10010046

// in AvailabilityInternal.h
-- arm 64 --
iOS:
compiler sets __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__, when -miphoneos-version-min=y.z is used as on compile option
it has the same value as in "iOS Deployment Target" setting

#define __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ 50000

tvOS: ??
#define __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ 90000


--- x64 --
compiler sets __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__, when -mmacosx-version-min=10.x is used as an compile option
it has the same value as in "macOS Deployment Target" setting

#define __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ 101500

--- arm 64 --
#define __ARM_ARCH 8   : __ARM_ARCH <arm version>
// the default alignment when using: new T
#define __STDCPP_DEFAULT_NEW_ALIGNMENT__ 8UL
-- x64 --
#define __STDCPP_DEFAULT_NEW_ALIGNMENT__ 16UL


in <Availability.h>
we can use this to declare deprecated functions:
__OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_1, __IPHONE_NA, __IPHONE_NA)
