#pragma once


// 通过设置相应的宏为1，来Enable相应的测试
#define FILE_READ_WRITE_STREAM_CHECKING     0
// 检查Compiletime/Runtime Murmur3 Hash
#define HASH_CHECKING                       1
#define LOGGING_CHECKING                    0
#define MATHE_FUNCS_CHECKING                1
// 检查Profiling模块
#define PROFILING_CHECKING                  1
// 检查TextFileReader/Writer类
#define TEXT_FILE_READ_WRITE_CHECKING       0
// 检测RuntimeText的UTF8编码
#define UTF8_CHECKING                       0
// 检查XXTEA编码模块
#define XXTEA_CHECKING                      1
