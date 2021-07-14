#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"
#include "Core/DataType/RS_Vec3D.hpp"


namespace Core
{

    // NOTE:
    // Pnt3D定义为POD/Aggregate类型：
    // - NO constructor, NO copy constructor, NO operator=(),
    //    NO protect/private/static data, NO base class, NO virtual functions
    // - The default opeartor=() will use memcpy() to do byte by byte data copy
    //
    struct alignas(16) Pnt3D
    {
        static constexpr UInt32 DIMENSION = 3;

        union
        {
            struct
            {
                Real32  x;
                Real32  y;
                Real32  z;
            };
            Real32      e[DIMENSION];
        };

        // 获得给定索引的数值
        Real32
        operator[] (
            const UInt32 idx) const;

        // 获得给定索引的数值
        Real32&
        operator[] (
            const UInt32 idx);

        // 检测此点是否与P相同
        Bool
        operator== (
            const Pnt3D p) const;

        // 检测此点是否与P不同
        Bool
        operator!= (
            const Pnt3D p) const;

        // 求点的和
        Pnt3D
        operator+ (
            const Pnt3D p) const;

        // 获得一个沿着v向量偏移后的点
        Pnt3D
        operator+ (
            const Vec3D v) const;

        // 获得一个沿着-v向量偏移后的点
        Pnt3D
        operator- (
            const Vec3D v) const;

        // 获得一个放大s倍的点
        Pnt3D
        operator* (
            const Real32 s) const;

        // 获得一个缩小s倍的点
        Pnt3D
        operator/ (
            const Real32 s) const;
    };


    // 原点：< 0, 0, 0 >
    constexpr Pnt3D ORIGIN = Pnt3D{0, 0, 0};

} // namespace Core
