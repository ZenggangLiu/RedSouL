// Precompiled header
#include "PrecompiledH.hpp"
// Lib headers
#include "Core/Mathe/RS_MatheFuncs.hpp"
// Self
#include "Core/Encoding/RS_XXTEA.hpp"


namespace Core
{

    // XXTEA cipher: https://en.wikipedia.org/wiki/XXTEA
#define COPY_KEY(key, key_size)                                                                    \
    UInt32 _key_used[KEY_BYTE_SIZE >> 2] = {0};                                                    \
    std::memcpy(_key_used, key, Mathe::minimum(key_size, (UInt32)sizeof(_key_used)));
#define DELTA 0x9E3779B9
// Key的长度：128位Key: 16字节
#define KEY_BYTE_SIZE 16
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + ((_key_used)[(p&3)^e] ^ z)))



    void
    XXTEA::encode (
        const UInt8 *const key,
        const UInt32       key_size,
        UInt32 *const      data_ptr,
        const UInt32       data_size)
    {
        RUNTIME_ASSERT(data_size >= 2, "Data is too short. It is at least of 2 UInt32");

        // 由于XXTEA使用128位Key，所以此处进行Key的生成
        COPY_KEY(key, key_size);

        // 编码data_ptr中的数据
        UInt32 y, z, sum, p, rounds, e;
        rounds = 6 + 52/data_size;
        sum    = 0;
        z      = data_ptr[data_size-1];
        do
        {
            sum += DELTA;
            e    = (sum >> 2) & 3;
            for (p = 0; p < data_size-1; p++)
            {
                y = data_ptr[p+1];
                z = data_ptr[p] += MX;
            }
            y = data_ptr[0];
            z = data_ptr[data_size-1] += MX;
        } while (--rounds);
    }


    void
    XXTEA::decode (
        const UInt8 *const key,
        const UInt32       key_size,
        UInt32 *const      data_ptr,
        const UInt32       data_size)
    {
        RUNTIME_ASSERT(data_size >= 2, "Data is too short. It is at least of 2 UInt32");

        // 由于XXTEA使用128位Key，所以此处进行Key的生成
        COPY_KEY(key, key_size);

        // 解码data_ptr中的数据
        UInt32 y, z, sum, p, rounds, e;
        rounds = 6 + 52/data_size;
        sum    = rounds*DELTA;
        y      = data_ptr[0];
        do
        {
            e = (sum >> 2) & 3;
            for (p = data_size - 1; p > 0; p--)
            {
                z = data_ptr[p - 1];
                y = data_ptr[p] -= MX;
            }
            z    = data_ptr[data_size - 1];
            y    = data_ptr[0] -= MX;
            sum -= DELTA;
        } while (--rounds);
    }

} // namespace Core
