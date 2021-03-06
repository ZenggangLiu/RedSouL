#pragma once

// System headers
// Lib headers
#include "Core/Assert/RS_RuntimeAssert.hpp"
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"


namespace Core
{

    class  NativeFile;
    // 回车
    struct LineFeed {};

} // namespace Core



// 定义Read/Write文件流共享的接口
#if !defined(DEFINE_COMMON_FILESTREAM_API)
// TYPE: 位文件流的类型：例如FileReadStream
#define DEFINE_COMMON_FILESTREAM_API(TYPE)                                                         \
public:                                                                                            \
    /* 创建一个无效文件流 */                                                                        \
    TYPE ();                                                                                       \
                                                                                                   \
    /* 关闭一个文件流 */                                                                            \
    ~TYPE ();                                                                                      \
                                                                                                   \
    /* 检查文件流是否有效 */                                                                        \
    Bool                                                                                           \
    isValid () const;                                                                              \
                                                                                                   \
    /* 获得文件流的长度 */                                                                          \
    UInt32                                                                                         \
    getLength () const;                                                                            \
                                                                                                   \
    /* 获得读写头的当前位置 */                                                                      \
    UInt32                                                                                         \
    getCursorPostion () const;                                                                     \
                                                                                                   \
    /* --- OS specific methods START --- */                                                        \
    /* Implementations are in RS_TYPE_Appl/Wind/Andr.??? */                                        \
                                                                                                   \
    /* 设定读写头的当前位置 */                                                                      \
    /**/                                                                                           \
    /* @param[in] cursor_pos */                                                                    \
    /*      相对于SeekMode的读写头位置的偏移 */                                                      \
    /* @param[in] seek_mode */                                                                     \
    /*      定义如何改变读写头的位置 */                                                              \
    /* @return */                                                                                  \
    /*      > 0     如果移动成功，返回当前读写头的位置 */                                            \
    /*      < 0     如果移动失败 */                                                                 \
    SInt32                                                                                         \
    seek (                                                                                         \
        const SInt32                      cursor_pos,                                              \
        const NativeFileSystem::SeekModes seek_mode);                                              \
                                                                                                   \
    /* 关闭此文件流 */                                                                              \
    void                                                                                           \
    close ();                                                                                      \
    /* --- OS specific methods END --- */                                                          \
                                                                                                   \
                                                                                                   \
protected:                                                                                         \
    friend struct NativeFileSystem;                                                                \
                                                                                                   \
    /* 获得一个Const Ref */                                                                         \
    const TYPE&                                                                                    \
    SELF () const;                                                                                 \
                                                                                                   \
    /* 获得一个Mutable Ref */                                                                       \
    TYPE&                                                                                          \
    SELF ();                                                                                       \
                                                                                                   \
    /* --- OS specific methods START --- */                                                        \
    /* Implementations are in RS_TYPE_Appl/Wind/Andr.??? */                                        \
    void                                                                                           \
    initWithFile (                                                                                 \
        NativeFile & file_io);                                                                     \
    /* --- OS specific methods END --- */                                                          \
                                                                                                   \
protected:                                                                                         \
    static const UInt8 NATIVE_FILE_INSTANCE_SIZE = 16;                                             \
                                                                                                   \
    /* 文件流的长度 */                                                                              \
    UInt32 m_stream_length;                                                                        \
    /* 读写头的当前位置 */                                                                          \
    UInt32 m_cursor_position;                                                                      \
    /* NativeFile的实例 */                                                                         \
    UInt8  m_file_instance[NATIVE_FILE_INSTANCE_SIZE];
#endif  // #if !defined(DEFINE_COMMON_FILESTREAM_API)


// 定义Read文件流使用的接口
#if !defined(DEFINE_READ_FILESTREAM_API)
#define DEFINE_READ_FILESTREAM_API(TYPE)                                                           \
public:                                                                                            \
    /* 读入一个字节 */                                                                              \
    /**/                                                                                           \
    /* @return */                                                                                  \
    /*      >= 0    如果读入成功。读入的字节Cast成为SInt32 */                                        \
    /*      <  0    如果读入失败 */                                                                 \
    INLINE_FUNCTION                                                                                \
    SInt32                                                                                         \
    readByte ()                                                                                    \
    {                                                                                              \
        UInt8 _byte;                                                                               \
        const SInt32 _read_count = read(1, 0, 1, &_byte);                                          \
        return (_read_count == 1) ? (SInt32)_byte : -1;                                            \
    }                                                                                              \
                                                                                                   \
    /* 读入指定字节数，并返回真正读入的字节数目 */                                                   \
    /**/                                                                                           \
    /* @param[in] buffer_size */                                                                   \
    /*      缓存的大小 */                                                                           \
    /* @param[in] offset */                                                                        \
    /*      缓存中的起始位置 */                                                                     \
    /* @param[in] count */                                                                         \
    /*      希望读入的字节数 */                                                                     \
    /* @param[out] buffer */                                                                       \
    /*      数据存入的缓存 */                                                                       \
    /* @return */                                                                                  \
    /*      > 0     如果读入成功，读入的字节数  */                                                   \
    /*      < 0     如果读入失败 */                                                                 \
    SInt32                                                                                         \
    read (                                                                                         \
        const UInt32 buffer_size,                                                                  \
        const UInt32 offset,                                                                       \
        const UInt32 count,                                                                        \
        UInt8 *const buffer);                                                                      \
                                                                                                   \
    /* 读入操作符 */                                                                                \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        SInt8 & data)                                                                              \
    {                                                                                              \
        return (SELF()) >> (UInt8&)data;                                                           \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        UInt8 & data)                                                                              \
    {                                                                                              \
        read(sizeof(UInt8), 0, sizeof(UInt8), &data);                                              \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        ASCII & data)                                                                              \
    {                                                                                              \
        return (SELF()) >> (UInt8&)data;                                                           \
    }                                                                                              \
                                                                                                   \
    /* NOTE: no operator >> defined for UTF8, because UTF8 is SAME as UInt8 */                     \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        SInt16 & data)                                                                             \
    {                                                                                              \
        return (SELF()) >> (UInt16&)data;                                                          \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        UInt16 & data)                                                                             \
    {                                                                                              \
        read(sizeof(UInt16), 0, sizeof(UInt16), (UInt8*)&data);                                    \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        UTF16 & data)                                                                              \
    {                                                                                              \
        return (SELF()) >> (UInt16&)data;                                                          \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        SInt32 & data)                                                                             \
    {                                                                                              \
        return (SELF()) >> (UInt32&)data;                                                          \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        UInt32 & data)                                                                             \
    {                                                                                              \
        read(sizeof(UInt32), 0, sizeof(UInt32), (UInt8*)&data);                                    \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        UTF32 & data)                                                                              \
    {                                                                                              \
        return (SELF()) >> (UInt32&)data;                                                          \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        SInt64 & data)                                                                             \
    {                                                                                              \
        return (SELF()) >> (UInt64&)data;                                                          \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        UInt64 & data)                                                                             \
    {                                                                                              \
        read(sizeof(UInt64), 0, sizeof(UInt64), (UInt8*)&data);                                    \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        Real32 & data)                                                                             \
    {                                                                                              \
        read(sizeof(Real32), 0, sizeof(Real32), (UInt8*)&data);                                    \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator >> (                                                                                  \
        Real64 & data)                                                                             \
    {                                                                                              \
        read(sizeof(Real64), 0, sizeof(Real64), (UInt8*)&data);                                    \
        return SELF();                                                                             \
    }
#endif // #if !defined(DEFINE_READ_FILESTREAM_API)


// 定义Write文件流使用的接口
#if !defined(DEFINE_WRITE_FILESTREAM_API)
#define DEFINE_WRITE_FILESTREAM_API(TYPE)                                                          \
public:                                                                                            \
    /* --- OS specific methods START --- */                                                        \
    /* Implementations are in RS_TYPE_Appl/Wind/Andr.??? */                                        \
    /* 设置文件流的长度 */                                                                          \
    void                                                                                           \
    setLength (                                                                                    \
        const UInt32 length);                                                                      \
                                                                                                   \
    /* 输出一个字节的数据 */                                                                        \
    /**/                                                                                           \
    /* @return */                                                                                  \
    /*      TRUE    如果输出成功 */                                                                 \
    /*      FALSE   如果输出失败 */                                                                 \
    INLINE_FUNCTION                                                                                \
    Bool                                                                                           \
    writeByte (                                                                                    \
        const UInt8 data)                                                                          \
    {                                                                                              \
        return write(&data, 1, 0, 1) == 1;                                                         \
    }                                                                                              \
                                                                                                   \
    /* 输出指定字节数 */                                                                            \
    /**/                                                                                           \
    /* @param[in] buffer */                                                                        \
    /*      数据的缓存 */                                                                           \
    /* @param[in] buffer_size */                                                                   \
    /*      缓存的大小 */                                                                           \
    /* @param[in] offset */                                                                        \
    /*      缓存中的起始位置 */                                                                     \
    /* @param[in] count */                                                                         \
    /*      希望输出的字节数 */                                                                     \
    /* @return */                                                                                  \
    /*      > 0     如果输出成功，输出的字节数目 */                                                  \
    /*      < 0     如果输出失败 */                                                                 \
    SInt32                                                                                         \
    write (                                                                                        \
       const UInt8 *const buffer,                                                                  \
       const UInt32       buffer_size,                                                             \
       const UInt32       offset,                                                                  \
       const UInt32       count);                                                                  \
                                                                                                   \
    /* 输出操作符 */                                                                                \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const SInt8 data)                                                                          \
    {                                                                                              \
        return (SELF()) << (UInt8)data;                                                            \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const UInt8 data)                                                                          \
    {                                                                                              \
        write(&data, sizeof(UInt8), 0, sizeof(UInt8));                                             \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const ASCII data)                                                                          \
    {                                                                                              \
        return (SELF()) << (UInt8)data;                                                            \
    }                                                                                              \
                                                                                                   \
    /* NOTE: no operator << defined for UTF8, because UTF8 is SAME as UInt8 */                     \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const SInt16 data)                                                                         \
    {                                                                                              \
        return (SELF()) << (UInt16)data;                                                           \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const UInt16 data)                                                                         \
    {                                                                                              \
        write((const UInt8*)&data, sizeof(UInt16), 0, sizeof(UInt16));                             \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const UTF16 data)                                                                          \
    {                                                                                              \
        return (SELF()) << (UInt16)data;                                                           \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const SInt32 data)                                                                         \
    {                                                                                              \
        return (SELF()) << (UInt32)data;                                                           \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const UInt32 data)                                                                         \
    {                                                                                              \
        write((const UInt8*)&data, sizeof(UInt32), 0, sizeof(UInt32));                             \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const UTF32 data)                                                                          \
    {                                                                                              \
        return (SELF()) << (UInt32)data;                                                           \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const SInt64 data)                                                                         \
    {                                                                                              \
        return (SELF()) << (UInt64)data;                                                           \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const UInt64 data)                                                                         \
    {                                                                                              \
        write((const UInt8*)&data, sizeof(UInt64), 0, sizeof(UInt64));                             \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const Real32 data)                                                                         \
    {                                                                                              \
        write((const UInt8*)&data, sizeof(Real32), 0, sizeof(Real32));                             \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const Real64 data)                                                                         \
    {                                                                                              \
        write((const UInt8*)&data, sizeof(Real64), 0, sizeof(Real64));                             \
        return SELF();                                                                             \
    }                                                                                              \
                                                                                                   \
    INLINE_FUNCTION                                                                                \
    TYPE&                                                                                          \
    operator << (                                                                                  \
        const LineFeed &)                                                                          \
    {                                                                                              \
        return (SELF()) << (UInt8)'\n';                                                            \
    }
#endif // #if !defined(DEFINE_WRITE_FILESTREAM_API)


#if !defined(LINE_FEED)
#define LINE_FEED (Core::LineFeed{})
#endif // #if !defined(LINE_FEED)
