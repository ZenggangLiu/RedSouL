// third party headers
#include "External/Catch/catch_v2.hpp"
// Lib headers
#include "UT_Config.hpp"

#if (FILE_READ_WRITE_STREAM_CHECKING == 1)
#include "Core/FileSystem/RS_FileStream.hpp"
#include "Core/FileSystem/RS_FileReadStream.hpp"
#include "Core/FileSystem/RS_FileWriteStream.hpp"
#include "Core/FileSystem/RS_NativeFileSystem.hpp"


using namespace Core;


// TEST_CASE( name [, tags ] )
TEST_CASE("Checking FileRead/WriteStream", "[FileRead/WriteStream]")
{
    // 各种类型数据
    const SInt8  si8data   = -127;
    const UInt8  ui8data   = 127;
    const ASCII  asciidata = 'A';
    const SInt16 si16data  = -640;
    const UInt16 ui16data  = 640;
    const UTF16  utf16data = u'編';
    const SInt32 si32data  = -1640;
    const UInt32 ui32data  = 1640;
    const SInt64 si64data  = -2640;
    const UTF32  utf32data = U'編';
    const UInt64 ui64data  = 2640;
//    const Real16 r16data   = 1.23;
    const Real32 r32data   = 2.34;
    const Real64 r64data   = 4.56;

    SECTION("Checking Write Stream:")
    {
        std::printf("--- Checking Write Stream...\n");

        FileWriteStream _write_io;
        NativeFileSystem::openFileWrite((const UTF8*)"Unit_Test_FileRead_WriteStream.dat",
                                        NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                        NativeFileSystem::WRITE_MODES_OVERWRITE,
                                        _write_io);
        _write_io << si8data  << ui8data  << asciidata;
        _write_io << si16data << ui16data << utf16data;
        _write_io << si32data << ui32data << utf32data;
        _write_io << si64data << ui64data;
        _write_io << r32data  << r64data;
        _write_io << LINE_FEED;
        _write_io.close();
        REQUIRE(NativeFileSystem::existsFile((const UTF8*)"Unit_Test_FileRead_WriteStream.dat",
                                             NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER));

        std::printf("--- Checking Write Stream: OK!\n");
    }

    SECTION("Checking Read Stream:")
    {
        std::printf("--- Checking Read Stream...\n");

        // 输入各种类型数据
        SInt8  _si8data;
        UInt8  _ui8data;
        ASCII  _asciidata;
        SInt16 _si16data;
        UInt16 _ui16data;
        UTF16  _utf16data;
        SInt32 _si32data;
        UInt32 _ui32data;
        UTF32  _utf32data;
        SInt64 _si64data;
        UInt64 _ui64data;
        Real32 _r32data;
        Real64 _r64data;
        FileReadStream  _read_io;
        NativeFileSystem::openFileRead((const UTF8*)"Unit_Test_FileRead_WriteStream.dat",
                                       NativeFileSystem::SEARCH_PATH_DOCUMENT_FOLDER,
                                       _read_io);
        _read_io >> _si8data;
        _read_io >> _ui8data;
        _read_io >> _asciidata;
        _read_io >> _si16data;
        _read_io >> _ui16data;
        _read_io >> _utf16data;
        _read_io >> _si32data;
        _read_io >> _ui32data;
        _read_io >> _utf32data;
        _read_io >> _si64data;
        _read_io >> _ui64data;
        _read_io >> _r32data;
        _read_io >> _r64data;

        // 检查数据
        REQUIRE(si8data   == _si8data   );
        REQUIRE(ui8data   == _ui8data   );
        REQUIRE(asciidata == _asciidata );
        REQUIRE(si16data  == _si16data  );
        REQUIRE(ui16data  == _ui16data  );
        REQUIRE(utf16data == _utf16data );
        REQUIRE(si32data  == _si32data  );
        REQUIRE(ui32data  == _ui32data  );
        REQUIRE(utf32data == _utf32data );
        REQUIRE(si64data  == _si64data  );
        REQUIRE(ui64data  == _ui64data  );
        REQUIRE(r32data   == _r32data   );
        REQUIRE(r64data   == _r64data   );

        std::printf("--- Checking Read Stream: OK\n");
    }
} // TEST_CASE("Checking FileRead/WriteStream", "[FileRead/WriteStream]")

#endif // #if (FILE_READ_WRITE_STREAM_CHECKING == 1)
