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
        return MurmurHash3_x86_32(seed, buffer, buffer_len);
    }


    UInt32
    hash32 (
        const UInt32      seed,
        const Char *const text)
    {
        return hash32(seed, (const UInt8 *const)text, (SInt32)strlen(text));
    }


    UInt64
    hash64 (
        const UInt32       seed,
        const UInt8 *const buffer,
        const SInt32       buffer_len)
    {
        return XXH64(seed, buffer, buffer_len);
    }


    UInt64
    hash64 (
        const UInt32      seed,
        const Char *const text)
    {
        return hash64(seed, (const UInt8 *const)text, (SInt32)strlen(text));
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


    void
    hash128 (
        const UInt32      seed,
        const Char *const text,
        UInt64 &          low,
        UInt64 &          high)
    {
        return hash128(seed, (const UInt8 *const)text, (SInt32)strlen(text), low, high);
    }

} // namespace Core
