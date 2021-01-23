//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.
#pragma once


#include "Core/DataType/RS_DataTypeDefs.hpp"


//-----------------------------------------------------------------------------

void MurmurHash3_x86_128(
    const UInt32       seed,
    const UInt8 *const buffer,
    const SInt32       buffer_len,
    UInt64 &           low,
    UInt64 &           high);

void MurmurHash3_x64_128(
    const UInt32       seed,
    const UInt8 *const buffer,
    const SInt32       buffer_len,
    UInt64 &           low,
    UInt64 &           high);

//-----------------------------------------------------------------------------
