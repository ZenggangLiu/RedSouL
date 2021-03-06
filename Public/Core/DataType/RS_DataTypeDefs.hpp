#pragma once


// System headers
#include <stdint.h>                     // C++11 standard types
// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"


// 我们至少需要C++11的支持,由于char16_t, char32_t在C++11中才定义
#if CXX_STANDARD_VER < 2011
#error "We need C++11 compatible compiler"
#endif // #if CXX_STANDARD_VER < 2011


// --- Scalar types --- //
// boolean
typedef bool                            Bool;


// characters
// ASCII character
typedef char                            ASCII;
// UTF8 character
typedef unsigned char                   UTF8;
// UTF16 character: 16bits长
// NOTE: 我们无法使用 UTF16 _utf16_code = u'🀀';
//       因为此字符u'🀀'需要两个UTF16编码
typedef char16_t                        UTF16;
// Unicode(UTF32) character
typedef char32_t                        UTF32;


// signed integer numbers
typedef int8_t                          SInt8;
typedef int16_t                         SInt16;
typedef int32_t                         SInt32;
typedef int64_t                         SInt64;


// unsigned integer numbers
typedef uint8_t                         UInt8;
typedef uint16_t                        UInt16;
typedef uint32_t                        UInt32;
typedef uint64_t                        UInt64;


// floating point real numbers
typedef struct Real16 { UInt16 val; }   Real16;
typedef float                           Real32;
typedef double                          Real64;


// --- Vector types --- //
// Vec2, Vec3, Vec4, Matx33, Matx34, Matx44



// --- Type alias define --- //
// 内存地址(64bits)：memory address 64bits
#if !defined(MEMADDR)
    #define MEMADDR UInt64
#endif // #if !defined(MEMADDR)


// NULL
#if !defined(NULL)
    #define NULL 0
#endif // #if !defined(NULL)


// nullptr
#if CXX_STANDARD_VER == 1998
    #define nullptr 0
#endif // #if CXX_STANDARD_VER == 1998


// BYTE
#if !defined(BYTE)
    #define BYTE UInt8
#endif // #if !defined(BYTE)


// WORD
#if !defined(WORD)
    #define WORD UInt32
#endif // #if !defined(WORD)


// DWORD
#if !defined(DWORD)
    #define DWORD UInt32
#endif // #if !defined(DWORD)
