#pragma once


// 通过设置相应的宏为1，来Enable相应的测试
//
// 检查BitOperation
#define BIT_OPERATION_CHECKING              1
// 检查DevThread
#define DEV_THREAD_CHECKING                 1
// 检查FileRead/WriteStream
#define FILE_READ_WRITE_STREAM_CHECKING     1
// 检查所以在RS_FloatPointFunc文件中定义的函数
#define FLOATPOINT_FUNCS_CHECKING           1
// 检查Compiletime/Runtime Murmur3 Hash
#define HASH_CHECKING                       1
// 检查Logging
#define LOGGING_CHECKING                    1
// 检查所以在RS_MatheFunc文件中定义的函数
#define MATHE_FUNCS_CHECKING                1
// 检查所以在RS_MemoryHelper文件中定义的函数
#define MEMORY_HELPER_CHECKING              1
// 检查PPM文件IO
#define PPM_IO_CHECKING                     1
// 检查PRN伪随机发生器
#define PRN_GENERATOR_CHECKING              1
#if (PRN_GENERATOR_CHECKING == 1)
#define PRN_GENERATOR_DISTRIBUTION_CHECKING 0
#endif // #if (PRN_GENERATOR_CHECKING == 1)
// 检查Profiling
#define PROFILING_CHECKING                  1
// 检查StlAllocator
#define STL_ALLOCATOR_CHECKING              1
// 检查TextFileReader/Writer类
#define TEXT_FILE_READ_WRITE_CHECKING       1
// 检测RuntimeText的UTF8函数已经Unicode的转换函数
#define UNICODE_CHECKING                    1
// 检查XXTEA编码
#define XXTEA_CHECKING                      1
