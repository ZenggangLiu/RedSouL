#pragma once


// System headers
#include <cmath>                        // std::abs and other math functions in std::
#include <cstring>                      // strlen, std::memcpy, std::memset
#include <cstdio>                       // std::printf
#include <functional>                   // std::hash, std::equal_to
#include <new>                          // placement new: new(...) type;
#include <stdio.h>                      // sscanf, sscanf_s
#include <unordered_map>
#include <utility>                      // std::move
#include <vector>
// Lib headers

// Assert
#include "Core/Assert/RS_RuntimeAssert.hpp"
#include "Core/Assert/RS_StaticAssert.hpp"
// Common
//#include "Common/ListDefs.hpp"
#include "Core/Common/RS_CommonDefs.hpp"
#include "Core/Common/RS_CompilerDefs.hpp"
#include "Core/Common/RS_OsDefs.hpp"

// DataTypes
#include "Core/DataType/RS_DataTypeDefs.hpp"
/*
// Core
#include "Core/ZGEAssert.hpp"
#include "Core/ZGELoggerMgr.hpp"
#include "Core/ZGEMemAllocator.hpp"
#include "Core/ZGEMemoryMgr.hpp"
#include "Core/ZGETime.hpp"
// Geometry
#include "Geometry/ZGEAABB.hpp"
#include "Geometry/ZGEAABB3F.hpp"
#include "Geometry/ZGELine.hpp"
#include "Geometry/ZGEOBB.hpp"
#include "Geometry/ZGEPlane.hpp"
#include "Geometry/ZGERay.hpp"
#include "Geometry/ZGERay2F.hpp"
#include "Geometry/ZGERay3F.hpp"
#include "Geometry/ZGESphere.hpp"
#include "Geometry/ZGESphereF.hpp"
#include "Geometry/ZGETriangle.hpp"
#include "Geometry/ZGETriangleF.hpp"
// Intersection
#include "Intersection/ZGEAABBIntersection.hpp"
#include "Intersection/ZGEIntersectionData.hpp"
#include "Intersection/ZGELineIntersection.hpp"
#include "Intersection/ZGEOBBIntersection.hpp"
#include "Intersection/ZGEPlaneIntersection.hpp"
#include "Intersection/ZGEPointIntersection.hpp"
#include "Intersection/ZGERayIntersection.hpp"
#include "Intersection/ZGESphereIntersection.hpp"
#include "Intersection/ZGETriangleIntersection.hpp"
// Mathe
#include "Mathe/ConstDefs.hpp"
#include "Mathe/ZGEMat4x4.hpp"
#include "Mathe/ZGEVec2.hpp"
#include "Mathe/ZGEVec3.hpp"
#include "Mathe/ZGEVec4.hpp"
#include "Mathe/ZGEQuat.hpp"
#include "Mathe/ZGEMat4x4F.hpp"
#include "Mathe/ZGETransform.hpp"
#include "Mathe/ZGEVec2F.hpp"
#include "Mathe/ZGEVec3F.hpp"
#include "Mathe/ZGEVec4F.hpp"
#include "Mathe/ZGEQuatF.hpp"
// Util
#include "Util/ZGEColorUtil.hpp"
#include "Util/ZGEFloatUtil.hpp"
#include "Util/ZGEHash.hpp"
#include "Util/ZGELerp.hpp"
#include "Util/ZGEMath.hpp"
#include "Util/ZGERandomNumber.hpp"
#include "Util/ZGEStringUtil.hpp"
*/

// --- 基本数据类型(Primary data types)大小检测 --- //
STATIC_ASSERT(sizeof(SInt8 )            == 1);
STATIC_ASSERT(sizeof(UInt8 )            == 1);
STATIC_ASSERT(sizeof(SInt16)            == 2);
STATIC_ASSERT(sizeof(UInt16)            == 2);
STATIC_ASSERT(sizeof(SInt32)            == 4);
STATIC_ASSERT(sizeof(UInt32)            == 4);
STATIC_ASSERT(sizeof(SInt64)            == 8);
STATIC_ASSERT(sizeof(UInt64)            == 8);
STATIC_ASSERT(sizeof(Real16)            == 2);
STATIC_ASSERT(sizeof(Real32)            == 4);
STATIC_ASSERT(sizeof(Real64)            == 8);


// --- 编译环境检测 --- //
STATIC_ASSERT(OS_TYPE != OS_TYPE_UNKNOWN);
#if (OS_TYPE == OS_TYPE_WIN)
    #pragma message("[Compile Env] Build mode: " BUILD_MODE_STR ", OS type: " OS_TYPE_STRING ", OS subtype: " OS_SUB_TYPE_STRING ", CPU type: " CPU_TYPE_STRING)
#elif defined(__APPLE__)
    #pragma message "[Compile Env] Build mode: " BUILD_MODE_STR ", OS type: " OS_TYPE_STRING ", OS subtype: " OS_SUB_TYPE_STRING ", CPU type: " CPU_TYPE_STRING
#else
    #error "[Env Check]: No implementation"
#endif // #if (OS_TYPE == OS_TYPE_WIN)
