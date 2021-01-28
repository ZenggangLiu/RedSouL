#pragma once


// Lib headers
#include "Core/Common/RS_CompilerDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


// 编译时的Assert
#if !defined(COMPILE_TIME_ASSERT)
    // C++ 11 up
    #if CXX_STANDARD_VER >= 2011
        #define COMPILE_TIME_ASSERT(cond)                                                          \
        COMPILE_TIME_ASSERT_MSG(cond, "condition: " #cond " failed!")
        #define COMPILE_TIME_ASSERT_MSG(cond, msg)                                                 \
        static_assert(cond, msg)

    // C++ 1998
    #else
        #define COMPILE_TIME_ASSERT(cond)                                                          \
        COMPILE_TIME_ASSERT_MSG(cond, "condition: " #cond " failed!")

        // ASSERT ON PREDICATE : true / false condition：使用负数数组大小
        // compile time assert: negative array size: typedef char COMPILERASSERT[-1];
        // we can typedef using the same variable name with the same type:
        // 同一文件中可以多次使用同一类型的多个typedef定义
        // JUST IN GLOBAL SCOPE OR IN CPP FILE
        // in the same file: CAN JUST BE USED IN GLOBAL SCOPE
        // typedef char abc[1];
        // typedef char abc[1];
        // CAN JUST BE USED IN GLOBAL SCOPE: otherwise, __CTASSERT redefined error
        #define COMPILE_TIME_ASSERT(cond, msg)                                                     \
        typedef char __COMPILE_TIME_ASSERT[cond ? 1 : -1];
    #endif // #if CXX_STANDARD_VER >= 2011
#endif // #if !defined(COMPILE_TIME_ASSERT)


// 编译时对于一个类是否具有指定函数的定义的Assert
// check if a type T has a method called someFunc():
// COMPILE_TIME_ASSERT_METHOD(MUTABLEREF(T).somFunc());
//
// check if a type T has a operator++() or operator++(int) defined: ++T and T++
// COMPILE_TIME_ASSERT_METHOD(++MUTABLEREF(T)  );
// COMPILE_TIME_ASSERT_METHOD(  MUTABLEREF(T)++);
// NOTE: CAN JUST BE USED IN GLOBAL SCOPE: otherwise, COMPILE_TIME_ASSERT_METHOD has redefined error
#if !defined(COMPILE_TIME_ASSERT_METHOD)
// Note: implementation uses the feature sizeof() on an expression:
// - NOT evaluates that expression
// - returns the size in byte if the given expression would be evaluated
//   + if a type T has a methode: T operator++(),
//     then: sizeof((*(T*)0)++) will be the sizeof the returned type: sizeof(T)
    #define COMPILE_TIME_ASSERT_METHOD(meth)                                                       \
    typedef char __COMPILE_TIME_ASSERT_METHOD[sizeof(meth)]
#endif // #if !define(COMPILE_TIME_ASSERT_METHOD)
