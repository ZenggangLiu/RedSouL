#pragma once


// System headers
// Lib headers
#include "Core/DataType/RS_DataTypeDefs.hpp"


namespace Core
{

    // NOTE:
    // Vec3D定义为POD/Aggregate类型：
    // - NOT constructor, NO copy constructor, NO operator=()
    // - The default opeartor=() will use memcpy() to do byte by byte data copy
    //
    struct alignas(16) Vec3D
    {
        static constexpr UInt32 DIMENSION = 3;

        /// elements
        union
        {
            struct
            {
                Real32 x;
                Real32 y;
                Real32 z;
            };
            Real32 e[DIMENSION];
        };

        // 获得给定索引的数值
        Real32
        operator[] (
            const UInt32 idx) const;

        // 获得给定索引的数值
        Real32&
        operator[] (
            const UInt32 idx);

        // normalize此向量
        void
        normalize ();

        // 计算此向量的长度
        Real32
        length () const;

        // 计算此向量的长度的平方
        Real32
        length_squared () const;
    };


    // 零向量
    constexpr Vec3D ZERO     = Vec3D{ 0,  0,  0};
    // 一向量
    constexpr Vec3D ONE      = Vec3D{ 1,  1,  1};
    // 左向量
    constexpr Vec3D LEFT     = Vec3D{-1,  0,  0};
    // 右向量
    constexpr Vec3D RIGHT    = Vec3D{ 1,  0,  0};
    // 上向量
    constexpr Vec3D UP       = Vec3D{ 0,  1,  0};
    // 下向量
    constexpr Vec3D DOWN     = Vec3D{ 0, -1,  0};
    // 前向量
    constexpr Vec3D FORWARD  = Vec3D{ 0,  0,  1};
    // 后向量
    constexpr Vec3D BACKWARD = Vec3D{ 0,  0, -1};

} // namespace Core
