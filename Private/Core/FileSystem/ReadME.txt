--- FileSystem 设计 ---
1. 为目录，文件提供查询借口
2. 提供如下的几种FileSystem：NativeFileSystem， PakFileSystem， NetFileSystem
  - NativeFileSystem：一个跨平台的OS的原生文件系统
    关系图：
                     使用             使用             使用
    NativeFileSystem ---> FileStream ---> NativeFile ---> Read/Write OS file
  - PakFileSystem：表示一个Pak压缩文件。Pak文件是对一个目录进行的压缩。
    因此PakFileSystem将提供基本的目录查询和文件操作（目前不支持文件写入操作）
    关系图：
                  使用                   操作         使用
    PakFileSystem ---> NativeFileStream ---> PakFile ---> Read/Write OS file
  - NetFileSytem表述了一个通过Server-Client的形式进行目录文件操作的系统。
    他由两部分构成：
    + NetFileSystemClient：发送文件请求方。例如场景编辑器
    + NetFileSystemServer：服务文件请求方：他将使用PrimFileStemstem来实现文件目录的操作。例如程序的App
3. FileStream/FileReadStream/FileWriteStream
  - 提供对于文件(NativeFile, PakFile)提过简单的流的操作支持
4. TextFileReader/TextFileWriter
  - 对文件的内容提供UTF8/16的编码支持，相当于认为文件的内容为UTF8/16编码的字符串
