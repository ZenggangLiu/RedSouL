#pragma once


// Lib headers
#include "Core/Common/RS_OsDefs.hpp"
#include "Core/DataType/RS_DataTypeDefs.hpp"


// API借口函数定义: API_FUNCTION void SomeFunc( UInt32, SInt32);
// NOTE：API_BUILD 必须定义为 0 或者 1
#if !defined(API_FUNCTION)
    #if !defined(API_BUILD)
        #error Please define API_BUILD in the build setting("Preprocessor Macros")
    #endif // #if !defined(API_BUILD)

    #if defined(_MSC_VER)
    // Microsoft：我们需要检测编译配置
        #if   (API_BUILD == 0)
            #define API_FUNCTION __declspec(dllimport)
        #elif (API_BUILD == 1)
            #define API_FUNCTION __declspec(dllexport)
        #else
            #error API_BUILD macro has an UnKnown value
        #endif // #if (API_BUILD == 0)

    #else
    // Apple
        #define API_FUNCTION __attribute__ ((visibility ("default")))
    #endif // #if defined(_MSC_VER)
#endif // #if !defined(API_FUNCTION)


// Inline function
#if !defined(INLINE_FUNCTION)
    // debug build mode
    #if (BUILD_MODE == DEBUG_BUILD_MODE)
        #define INLINE_FUNCTION inline

    // release build mode
    #else
        #if (OS_TYPE == OS_TYPE_WIN && defined(_MSC_VER))
        // Windows using MSVC
            #define INLINE_FUNCTION __forceinline
        #else
        // others
            #define INLINE_FUNCTION inline __attribute__((always_inline))
        #endif // #if (OS_TYPE == OS_TYPE_WIN && defined(_MSC_VER))
    #endif // #if (BUILD_MODE == DEBUG_BUILD_MODE)
#endif // #if !defined(INLINE_FUNCTION)


// 获得对类型 T 的 const reference
#if !defined(CONST_REFERENCE)
    #define CONST_REFERENCE(T)                                                                     \
    (*static_cast< const T* >(0))
#endif // #if !defined(CONST_REFERENCE)


// 获得对类型 T 的 mutable reference
#if !defined(MUTABLE_REFERENCE)
    #define MUTABLE_REFERENCE(T)                                                                   \
    (*static_cast< T* >(0))
#endif // #if !defined(MUTABLE_REFERENCE)


// 获得类型 T 中指定变量成员的大小（Byte size)
#if !defined(MEMBER_SIZE)
    #define MEMBER_SIZE(T, member)                                                                 \
    (sizeof(((T*)0)->member))
#endif // #if !defined(MEMBER_SIZE)


// 获得类型 T 中给定成员变量的64位偏移
// Gets the 64bits offset of a member in the given type T
// Usage:
// class T
// {
// public:
//     T() : iVal(0xC00DDEC0) {}
//     SInt32 iVal;   // 4bytes
//     UTF8 cVal;     // 4bytes: 1byte + 3bytes padding
//     Real32 fVal;   // 4bytes
//     UTF8 var;
// };
//
// MEMBER_OFFSET(T, var) retuns 12: the offset of member var
#if !defined(MEMBER_OFFSET)
// NOTE:
// 由于对C++11标准的支持问题
// C++11 standard(clang follows and msvc doesn't) forbids using reinterpret_cast<> in constant
// expressions.
// MSVC uses reinterpret_cast<> in offsetof macro, while clang uses compiler intrinsic.
// 例如调用：
// static_assert(STRUCT_OFFSET(x, y) == SomeValue)
// 在Windows系统上，Clang将产出错误
// SO, we always using intrinsic, if the code is not being compiled using visual studio
    #if(OS_TYPE == OS_TYPE_WIN && defined(_MSC_VER))
        #define MEMBER_OFFSET(T, member)                                                           \
        ((UInt64)&(((T*)0)->member))
    #else
        #define MEMBER_OFFSET(T, member)                                                           \
        ((UInt64)(__builtin_offsetof(T, member)))
    #endif // #if(OS_TYPE == OS_TYPE_WIN && defined(_MSC_VER))
#endif // #if !defined(MEMBER_OFFSET)


// 获得静态数组成员的个数
#if !defined(ARRAY_ITEM_COUNT)
// 我们定义N个函数参考的数组
// 使用如下方法定义一个对char[N]参考的函数: a function referring to a char[N]
// 使用如下方法定义一个对char[N]参考：
// - char(&reference) [N]: reference is a reference to char[N]
// NOTE：if the array has overloaded operator[], it will change the expected behavior
    template < typename T, UInt32 N >
    char(&_GetArrayItemCount(const T(&)[N]))[N];

// 我们使用 sizeof 操作符来获得静态数组的成员个数
// NOTE:
// sizeof 不会真正计算（evaluate）给定的表达式：此处不会真正调用 _GetArrayItemCount 函数
    #define ARRAY_ITEM_COUNT(T)                                                                    \
    (sizeof(_GetArrayItemCount(T)))
    // #define ARRAY_ITEM_COUNT(array) (sizeof(array) / sizeof(array[0]))
#endif  // #if !defined(ARRAY_ITEM_COUNT)


// 链接Token: TOKEN_COMBINE(t1, t2) --> t1t2
#if !defined(TOKEN_COMBINE)
// NOTE: 由于 ## 将抑制对于宏的替代：
// 如果我们直接定义 #define TOKEN_COMBINE(x, y) x ## y
// x 与 y 将不会进行宏扩展
    #define _TOKEN_COMBINE(token1, token2)                                                         \
    token1 ## token2

    #define TOKEN_COMBINE(token1, token2)                                                          \
    _TOKEN_COMBINE(token1, token2)
#endif // #if !defined(TOKEN_COMBINE)


// 形成给定 Token 的字符串：TOKEN_QUOTE(name) --> "name"
#if !defined(TOKEN_QUOTE)
    #define _TOKEN_QUOTE(token)                                                                    \
    #token

    #define TOKEN_QUOTE(token)                                                                     \
    _TOKEN_QUOTE(token)
#endif // #if !defined(TOKEN_QUOTE)


// 使用给定的多行文字形成一个字符串
// Usage：
/*
    const uint8 d3d_obj_viewer_vs_code[] = MULTILINE_TEXT(

        //-------------------- Vertex Shader ------------------

        // Uniforms:
        // Names MUST match with Names given in the Constant Buffer's Declaration
        // HLSL Data Type ---->  GLSL Data Type
        // float2                  vec2
        // float3                  vec3
        // float4                  vec4
        // float2x2                mat2
        // float3x3                mat3
        // float4x4                mat4
        // Texture2D               sampler2D

        // In GLES, We can JUST use void main() as the Entry for VS and PS

        // In GLSL, We use Attribute Names to Match the Data Transferred from APP to the Shader
        // In D3D,  We use Semantic        to Match the Data Transferred from APP to the Shader

        // In GLSL, We can NOT use f for floating point number like: 1.0f we have to use 1.0
        // In GLSL, We can NOT use: vec4 value = 1.0; we have to use vec4 value = vec4(1.0);
        // In GLSL, UN-Used Attribute will be striped away,
        // in this case we can NOT find the location of that attribute any more

        uniform	mat4 mat_world;
        uniform	mat4 mat_world_view_projection;
        uniform	mat4 mat_world_inverse_transpose;

        // all positions/vectors are in world space
        uniform vec4 light1_position_world_space;
        uniform vec4 light2_position_world_space;
        uniform vec4 eye_position_world_space;

        // Application To Vertex Shader: Attributes: MATCH the Name used int Vertex Layout
        attribute vec4 Position;               // Vertex Position: POSITION
        attribute vec4 Normal;                 // Vertex Normal  : NORMAL
        attribute vec4 Tangent;                // Vertex Tangent : TANGENT
        //attribute vec4 DiffuseColor;         //                : COLOR0
        attribute vec2 UV0;	                   // Vertex UV0     : TEXCOORD0

        // Vertex Shader To Pixel Shader: Varying
        varying vec4 v2f_diffuse_color;            // COLOR0
        varying vec2 v2f_uv0;                      // TEXCOORD0
        varying vec3 v2f_light1_vector_ws;         // TEXCOORD1
        varying vec3 v2f_light2_vector_ws;         // TEXCOORD2
        varying vec3 v2f_view_vector_ws;           // TEXCOORD3
        varying vec3 v2f_normal_vector_ws;         // TEXCOORD4
        varying vec3 v2f_tangent_vector_ws;        // TEXCOORD5

        // vs entry
        void main( )
        {
            // transform the vertex's position into clip space
            gl_Position = mat_world_view_projection * Position;

            // vertex position in world space
            vec4 vertex_pos_ws = mat_world * Position;

            // light vector in world space
            v2f_light1_vector_ws = normalize(light1_position_world_space - vertex_pos_ws).xyz;
            v2f_light2_vector_ws = normalize(light2_position_world_space - vertex_pos_ws).xyz;

            // view vector in world space
            v2f_view_vector_ws = normalize(eye_position_world_space - vertex_pos_ws).xyz;

            // normal vector in world space : transpose of inverse of world transform * normal
            v2f_normal_vector_ws = normalize(mat_world_inverse_transpose * Normal).xyz;

            // tangent vector in world space
            v2f_tangent_vector_ws = normalize(mat_world_inverse_transpose * Tangent).xyz;

            // copy over the UV
            v2f_uv0 = UV0;

            // copy over the diffuse color
            v2f_diffuse_color = vec4(1.0, 1.0, 1.0, 1.0); // DiffuseColor;
        }
    );
*/
#if !defined(MULTILINE_TEXT)
// 我们使用 __VA_ARGS__ 来表示变化的参数：variadic arguments
    #define MULTILINE_TEXT(...)                                                                    \
    #__VA_ARGS__
#endif // #if !defined(MULTILINE_TEXT)


// 产生一个 FourCC：an integer representing characters C0, C1 , C2, C3
// NOTE：产生的正数为 0xC3_C2_C1_C0
// 如果讲次 FourCC 存入文件中， 我们将得到如下字符序列：C0 C1 C2 C3
#if !defined(FOUR_CC)
    #define FOUR_CC(c0, c1, c2, c3)                                                                \
    (((UInt32)(c3) << 24 ) | ((UInt32)(c2) << 16) | ((UInt32)(c1) << 8) | ((UInt32)(c0)))
#endif // #if !defined(FOUR_CC)


// MARK: - TO BE CHANGED
// 声明 Serialization 使用的所有函数
#if !defined(DECLARE_SERIALIZABLE_TYPE)
    // forwards declare the archiver class
    namespace Core { class Archiver; class UnArchiver; }
    typedef bool(*DeSerializeFunc)(Core::UnArchiver&, const UInt8, void*const);
    #define DECLARE_SERIALIZABLE_TYPE(type)                                                        \
    public:                                                                                        \
        /* Gets the type's hash code */                                                            \
        static                                                                                     \
        UInt32                                                                                     \
        _typeHash ();                                                                              \
                                                                                                   \
        /* Gets the number of serializable members */                                              \
        static                                                                                     \
        UInt8                                                                                      \
        _serializableMemberCount ();                                                               \
                                                                                                   \
        /* Serializes the given instance using the given io */                                     \
        static                                                                                     \
        void                                                                                       \
        _serialize (                                                                               \
            const type& instance,                                                                  \
            Archiver&   io);                                                                       \
                                                                                                   \
        /* Initializes an instance in the given memory location using the given io */              \
        static                                                                                     \
        bool                                                                                       \
        _deserialize (                                                                             \
            UnArchiver& io,                                                                        \
            const UInt8 member_count,                                                              \
            void*const  mem_ptr);                                                                  \

#endif // #if !defined(DECLARE_SERIALIZABLE_TYPE)


#if !defined(SERIALIZABLE)
    #define SERIALIZABLE
    #define SHARED
    // bool
    #define SERIALIZABLE_BOOL(...)
    // char*
    #define SERIALIZABLE_CHAR_LITERAL(...)
    // enum
    #define SERIALIZABLE_ENUM(...)
    // float32
    #define SERIALIZABLE_FLOAT32(...)
    // float64
    #define SERIALIZABLE_FLOAT64(...)
    // int8
    #define SERIALIZABLE_INT8(...)
    // int16
    #define SERIALIZABLE_INT16(...)
    // int32
    #define SERIALIZABLE_INT32(...)
    // int64
    #define SERIALIZABLE_INT64(...)
    // handle
    #define SERIALIZABLE_SHARED_HANDLE(...)
    // std::vector<Type>
    #define SERIALIZABLE_STD_VEC(...)
    // RuntimeText
    #define SERIALIZABLE_TEXT(...)
    // uint8
    #define SERIALIZABLE_UINT8(...)
    // uint16
    #define SERIALIZABLE_UINT16(...)
    // uint32
    #define SERIALIZABLE_UINT32(...)
    // uint64
    #define SERIALIZABLE_UINT64(...)
    // Class
    #define SERIALIZABLE_USER_DEFINED_TYPE(...)
#endif // #if !defined(SERIALIZABLE)
