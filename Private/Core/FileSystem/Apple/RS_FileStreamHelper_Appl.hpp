#pragma once


// System headers
// Lib headers


#if !defined(IMPLEMENT_COMMON_STREAM_API)
#define IMPLEMENT_COMMON_STREAM_API(TYPE)                                                          \
    TYPE::TYPE ()                                                                                  \
    :                                                                                              \
        m_stream_length(0),                                                                        \
        m_cursor_position(0)                                                                       \
    {                                                                                              \
        /* 清除NativeFile的实例 */                                                                   \
        memset(m_file_instance, 0, sizeof(m_file_instance));                                       \
    }                                                                                              \
                                                                                                   \
    TYPE::~TYPE ()                                                                                 \
    {                                                                                              \
        close();                                                                                   \
    }                                                                                              \
                                                                                                   \
    Bool                                                                                           \
    TYPE::isValid () const                                                                         \
    {                                                                                              \
        return ((NativeFile*)m_file_instance)->is_opened;                                          \
    }                                                                                              \
                                                                                                   \
    UInt32                                                                                         \
    TYPE::getLength () const                                                                       \
    {                                                                                              \
        return m_stream_length;                                                                    \
    }                                                                                              \
                                                                                                   \
    UInt32                                                                                         \
    TYPE::getCursorPostion () const                                                                \
    {                                                                                              \
        RUNTIME_ASSERT(m_cursor_position == ((NativeFile*)m_file_instance)->getCursorPosition(),   \
                       "NO matching cursor position");                                             \
        return m_cursor_position;                                                                  \
    }                                                                                              \
                                                                                                   \
    const TYPE&                                                                                    \
    TYPE::SELF () const                                                                            \
    {                                                                                              \
        return *this;                                                                              \
    }                                                                                              \
                                                                                                   \
    TYPE&                                                                                          \
    TYPE::SELF ()                                                                                  \
    {                                                                                              \
        return *this;                                                                              \
    }                                                                                              \
                                                                                                   \
    SInt32                                                                                         \
    TYPE::seek (                                                                                   \
        const SInt32                      cursor_pos,                                              \
        const NativeFileSystem::SeekModes seek_mode)                                               \
    {                                                                                              \
        NativeFile *const _file_io = (NativeFile*)m_file_instance;                                 \
        RUNTIME_ASSERT(_file_io->is_opened, "NativeFile is not opened");                           \
        SInt32 _cursor_pos = _file_io->seek(cursor_pos, seek_mode);                                \
        if (_cursor_pos >=0 )                                                                      \
        {                                                                                          \
            m_cursor_position = _cursor_pos;                                                       \
        }                                                                                          \
                                                                                                   \
        return _cursor_pos;                                                                        \
    }                                                                                              \
                                                                                                   \
    void                                                                                           \
    TYPE::close ()                                                                                 \
    {                                                                                              \
        /* 尝试着关闭文件 */                                                                          \
        NativeFile *const _file_io = (NativeFile*)m_file_instance;                                 \
        _file_io->close();                                                                         \
                                                                                                   \
        /* 复位所以成员变量 */                                                                        \
        m_stream_length   = 0;                                                                     \
        m_cursor_position = 0;                                                                     \
    }                                                                                              \
                                                                                                   \
    void                                                                                           \
    TYPE::initWithFile (                                                                           \
        NativeFile & file_io)                                                                      \
    {                                                                                              \
        RUNTIME_ASSERT(sizeof(NativeFile) <= sizeof(m_file_instance),                              \
                       "NativeFile class is too large");                                           \
                                                                                                   \
        /* 初始化变量 */                                                                             \
        m_stream_length   = file_io.getLength();                                                   \
        m_cursor_position = 0;                                                                     \
        /* 创建NativeFile的实例 */                                                                   \
        new(m_file_instance)NativeFile(file_io);                                                   \
    }
#endif // #if !defined(IMPLEMENT_COMMON_STREAM_API)


#if !defined(IMPLEMENT_READ_STREAM_API)
#define IMPLEMENT_READ_STREAM_API(TYPE)                                                            \
    SInt32                                                                                         \
    TYPE::read (                                                                                   \
        const UInt32 buffer_size,                                                                  \
        const UInt32 offset,                                                                       \
        const UInt32 count,                                                                        \
        UInt8 *const buffer)                                                                       \
    {                                                                                              \
        NativeFile *const _file_io = (NativeFile*)m_file_instance;                                 \
        RUNTIME_ASSERT(_file_io->is_opened, "NativeFile is not opened");                           \
                                                                                                   \
        /* 检查是否可以读人希望的字节数 */                                                              \
        if (m_cursor_position + count > m_stream_length)                                           \
        {                                                                                          \
            return -1;                                                                             \
        }                                                                                          \
                                                                                                   \
        /* 读人数据 */                                                                               \
        const SInt32 _read_count = _file_io->read(buffer_size, offset, count, buffer);             \
        /* 更新文件流的当前位置 */                                                                     \
        if (_read_count > 0)                                                                       \
        {                                                                                          \
            m_cursor_position += _read_count;                                                      \
        }                                                                                          \
                                                                                                   \
        return _read_count;                                                                        \
    }
#endif // #if !defined(IMPLEMENT_READ_STREAM_API)


#if !defined(IMPLEMENT_WRITE_STREAM_API)
#define IMPLEMENT_WRITE_STREAM_API(TYPE)                                                           \
    void                                                                                           \
    TYPE::setLength (                                                                              \
       const UInt32 length)                                                                        \
    {                                                                                              \
        NativeFile *const _file_io = (NativeFile*)m_file_instance;                                 \
        RUNTIME_ASSERT(_file_io->is_opened, "NativeFile is not opened");                           \
        _file_io->setLength(length);                                                               \
        m_stream_length = length;                                                                  \
    }                                                                                              \
                                                                                                   \
    SInt32                                                                                         \
    TYPE::write (                                                                                  \
       const UInt8 *const buffer,                                                                  \
       const UInt32       buffer_size,                                                             \
       const UInt32       offset,                                                                  \
       const UInt32       count)                                                                   \
    {                                                                                              \
        /* 输出数据 */                                                                               \
        NativeFile *const _file_io = (NativeFile*)m_file_instance;                                 \
        RUNTIME_ASSERT(_file_io->is_opened, "NativeFile is not opened");                           \
        _file_io->write(buffer, buffer_size, offset, count);                                       \
                                                                                                   \
        /* 更新文件流的当前位置 */                                                                     \
        const UInt32 _cursor_pos  = _file_io->getCursorPosition();                                 \
        const SInt32 _write_count = (SInt32)(_cursor_pos - m_cursor_position);                     \
        m_cursor_position = _cursor_pos;                                                           \
                                                                                                   \
        /* 更新文件的总长度 */                                                                        \
        if (m_cursor_position > m_stream_length)                                                   \
        {                                                                                          \
            m_stream_length = m_cursor_position;                                                   \
        }                                                                                          \
                                                                                                   \
        /* 返回输出的字节数目 */                                                                      \
        return _write_count > 0 ? _write_count : -1;                                               \
    }
#endif // #if !defined(IMPLEMENT_WRITE_STREAM_API)
