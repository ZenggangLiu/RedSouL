//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.
// Ref: https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
// Precompiled header
#include "PrecompiledH.hpp"
// System headers
// Lib headers
// Self
#include "Core/External/Hash/murmurhash3.hpp"


//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER)

#define FORCE_INLINE	__forceinline

#define ROTL32(x,y)	_rotl(x,y)
#define ROTL64(x,y)	_rotl64(x,y)

#define BIG_CONSTANT(x) (x)

// Other compilers

#else	// defined(_MSC_VER)

#define	FORCE_INLINE inline __attribute__((always_inline))

inline UInt32 rotl32 ( UInt32 x, SInt8 r )
{
  return (x << r) | (x >> (32 - r));
}

inline UInt64 rotl64 ( UInt64 x, SInt8 r )
{
  return (x << r) | (x >> (64 - r));
}

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

FORCE_INLINE UInt32 getblock32 ( const UInt32 * p, SInt32 i )
{
  return p[i];
}

FORCE_INLINE UInt64 getblock64 ( const UInt64 * p, SInt32 i )
{
  return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

FORCE_INLINE UInt32 fmix32 ( UInt32 h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

//----------

FORCE_INLINE UInt64 fmix64 ( UInt64 k )
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}


//-----------------------------------------------------------------------------

UInt32
MurmurHash3_x86_32 (
    const UInt32       seed,
    const UInt8 *const buffer,
    const SInt32       buffer_len)
{
  const SInt32 nblocks = buffer_len / 4;

  UInt32 h1 = seed;

  const UInt32 c1 = 0xcc9e2d51;
  const UInt32 c2 = 0x1b873593;

  //----------
  // body

  const UInt32 *const  blocks = (const UInt32 *)(buffer + nblocks*4);

  for(SInt32 i = -nblocks; i; i++)
  {
    UInt32 k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const UInt8 *const tail = (const UInt8*)(buffer + nblocks*4);

  UInt32 k1 = 0;

  switch(buffer_len & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= buffer_len;

  h1 = fmix32(h1);

  return h1;
}


//-----------------------------------------------------------------------------

void
MurmurHash3_x86_128 (
    const UInt32       seed,
    const UInt8 *const buffer,
    const SInt32       buffer_len,
    UInt64 &           low,
    UInt64 &           high)
{
  const SInt32 nblocks = buffer_len / 16;

  UInt32 h1 = seed;
  UInt32 h2 = seed;
  UInt32 h3 = seed;
  UInt32 h4 = seed;

  const UInt32 c1 = 0x239b961b;
  const UInt32 c2 = 0xab0e9789;
  const UInt32 c3 = 0x38b34ae5;
  const UInt32 c4 = 0xa1e38b93;

  //----------
  // body

  const UInt32 *const blocks = (const UInt32 *)(buffer + nblocks*16);

  for(SInt32 i = -nblocks; i; i++)
  {
    UInt32 k1 = getblock32(blocks,i*4+0);
    UInt32 k2 = getblock32(blocks,i*4+1);
    UInt32 k3 = getblock32(blocks,i*4+2);
    UInt32 k4 = getblock32(blocks,i*4+3);

    k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;

    h1 = ROTL32(h1,19); h1 += h2; h1 = h1*5+0x561ccd1b;

    k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

    h2 = ROTL32(h2,17); h2 += h3; h2 = h2*5+0x0bcaa747;

    k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

    h3 = ROTL32(h3,15); h3 += h4; h3 = h3*5+0x96cd1c35;

    k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

    h4 = ROTL32(h4,13); h4 += h1; h4 = h4*5+0x32ac3b17;
  }

  //----------
  // tail

  const UInt8 *const tail = (const UInt8*)(buffer + nblocks*16);

  UInt32 k1 = 0;
  UInt32 k2 = 0;
  UInt32 k3 = 0;
  UInt32 k4 = 0;

  switch(buffer_len & 15)
  {
  case 15: k4 ^= tail[14] << 16;
  case 14: k4 ^= tail[13] << 8;
  case 13: k4 ^= tail[12] << 0;
           k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

  case 12: k3 ^= tail[11] << 24;
  case 11: k3 ^= tail[10] << 16;
  case 10: k3 ^= tail[ 9] << 8;
  case  9: k3 ^= tail[ 8] << 0;
           k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

  case  8: k2 ^= tail[ 7] << 24;
  case  7: k2 ^= tail[ 6] << 16;
  case  6: k2 ^= tail[ 5] << 8;
  case  5: k2 ^= tail[ 4] << 0;
           k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

  case  4: k1 ^= tail[ 3] << 24;
  case  3: k1 ^= tail[ 2] << 16;
  case  2: k1 ^= tail[ 1] << 8;
  case  1: k1 ^= tail[ 0] << 0;
           k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= buffer_len; h2 ^= buffer_len; h3 ^= buffer_len; h4 ^= buffer_len;

  h1 += h2; h1 += h3; h1 += h4;
  h2 += h1; h3 += h1; h4 += h1;

  h1 = fmix32(h1);
  h2 = fmix32(h2);
  h3 = fmix32(h3);
  h4 = fmix32(h4);

  h1 += h2; h1 += h3; h1 += h4;
  h2 += h1; h3 += h1; h4 += h1;

  low  = (UInt64)h2 << 32 | h1;
  high = (UInt64)h4 << 32 | h3;
}


//-----------------------------------------------------------------------------

void
MurmurHash3_x64_128 (
    const UInt32       seed,
    const UInt8 *const buffer,
    const SInt32       buffer_len,
    UInt64 &           low,
    UInt64 &           high)
{
  const SInt32 nblocks = buffer_len / 16;

  UInt64 h1 = seed;
  UInt64 h2 = seed;

  const UInt64 c1 = BIG_CONSTANT(0x87c37b91114253d5);
  const UInt64 c2 = BIG_CONSTANT(0x4cf5ad432745937f);

  //----------
  // body

  const UInt64 *const blocks = (const UInt64 *)(buffer);

  for(SInt32 i = 0; i < nblocks; i++)
  {
    UInt64 k1 = getblock64(blocks,i*2+0);
    UInt64 k2 = getblock64(blocks,i*2+1);

    k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

    h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

    k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

    h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
  }

  //----------
  // tail

  const UInt8 *const tail = (const UInt8*)(buffer + nblocks*16);

  UInt64 k1 = 0;
  UInt64 k2 = 0;

  switch(buffer_len & 15)
  {
  case 15: k2 ^= ((UInt64)tail[14]) << 48;
  case 14: k2 ^= ((UInt64)tail[13]) << 40;
  case 13: k2 ^= ((UInt64)tail[12]) << 32;
  case 12: k2 ^= ((UInt64)tail[11]) << 24;
  case 11: k2 ^= ((UInt64)tail[10]) << 16;
  case 10: k2 ^= ((UInt64)tail[ 9]) << 8;
  case  9: k2 ^= ((UInt64)tail[ 8]) << 0;
           k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

  case  8: k1 ^= ((UInt64)tail[ 7]) << 56;
  case  7: k1 ^= ((UInt64)tail[ 6]) << 48;
  case  6: k1 ^= ((UInt64)tail[ 5]) << 40;
  case  5: k1 ^= ((UInt64)tail[ 4]) << 32;
  case  4: k1 ^= ((UInt64)tail[ 3]) << 24;
  case  3: k1 ^= ((UInt64)tail[ 2]) << 16;
  case  2: k1 ^= ((UInt64)tail[ 1]) << 8;
  case  1: k1 ^= ((UInt64)tail[ 0]) << 0;
           k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= buffer_len; h2 ^= buffer_len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  h2 += h1;

  low  = h1;
  high = h2;
}
