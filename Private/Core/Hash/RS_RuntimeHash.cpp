// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
#include "Core/External/Hash/murmurhash3.hpp"
#include "Core/External/Hash/xxhash.hpp"
// Self
#include "Core/Hash/RS_RuntimeHash.hpp"


namespace Core
{

    UInt32
    hash32 (
        const UInt32 key)
    {
        UInt32 _result = key;
        _result = (_result + 0x479ab41d) + (_result << 8);
        _result = (_result ^ 0xe4aa10ce) ^ (_result >> 5);
        _result = (_result + 0x9942f0a6) - (_result << 14);
        _result = (_result ^ 0x5aedd67d) ^ (_result >> 3);
        _result = (_result + 0x17bea992) + (_result << 7);
        return _result;
    }


    UInt32
    hash32 (
        const UInt64 key)
    {
        UInt64 _result = key;
        _result = (~_result) + (_result << 18); // _result = (_result << 18) - _result - 1;
        _result =    _result ^ (_result >> 31);
        _result =   (_result + (_result << 2)) + (_result << 4);
        _result =    _result ^ (_result >> 11);
        _result =    _result + (_result << 6);
        _result =    _result ^ (_result >> 22);
        return (UInt32)_result;
    }


    UInt32
    hash32 (
        const UInt32       seed,
        const UInt8 *const buffer,
        const SInt32       buffer_len)
    {
#if (CPU_TYPE == CPU_TYPE_32)
        return XXH32(buffer, buffer_len, seed);
#else
        return (UInt32)hash64(seed, buffer, buffer_len);
#endif
    }


    UInt64
    hash64 (
        const UInt32       seed,
        const UInt8 *const buffer,
        const SInt32       buffer_len)
    {
        return XXH64(buffer, buffer_len, seed);
    }


    void
    hash128 (
        const UInt32       seed,
        const UInt8 *const buffer,
        const SInt32       buffer_len,
        UInt64 &           low,
        UInt64 &           high)
    {
#if (CPU_TYPE == CPU_TYPE_32)
        return MurmurHash3_x86_128(seed, buffer, buffer_len, low, high);
#else
        return MurmurHash3_x64_128(seed, buffer, buffer_len, low, high);
#endif
    }

} // namespace Core
