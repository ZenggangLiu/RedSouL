#pragma once


// System headers
#include <cstddef>
#include <cstdlib>                          // std::malloc/free
#include <memory>                           // std::allocator
#include "Core/Common/RS_CompilerDefs.hpp"
#if (USE_EXCEPTION == 1)
#include <new>                              // std::bad_alloc
#endif // #if (USE_EXCEPTION == 1)
// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"    // INLINE_FUNCTION


// 定义使用哪个底层内存管理
#define USE_STD_ALLOCATOR               1
#define USE_VMEM_ALLOCATOR              0
// 定义是否使用此Allocator中的构造函数/析构函数: construct/destroy()
//#define USE_CUSTOMIZED_CONSTRUCT        1
#if defined(CATCH_PLATFORM_MAC) || defined(CATCH_PLATFORM_WINDOWS) || defined(CATCH_PLATFORM_LINUX)
    // 测试逻辑
    #if !defined(USE_CUSTOMIZED_CONSTRUCT)
        #define USE_CUSTOMIZED_CONSTRUCT        1
    #endif
    #if (USE_CUSTOMIZED_CONSTRUCT == 0)
        #undef USE_CUSTOMIZED_CONSTRUCT
        #define USE_CUSTOMIZED_CONSTRUCT        1
    #endif // #if (USE_CUSTOMIZED_CONSTRUCT == 0)
#endif // #if defined(CATCH_PLATFORM_MAC) ||...

#if !defined(MEMORY_ALLOCATE)
    #if (USE_STD_ALLOCATOR == 1)
        #define MEMORY_ALLOCATE(type, number)       std::malloc(type*number);
        #define MEMORY_RELEASE(startMem, number)    std::free(static_cast<void*>(startMem))
    #elif (USE_VMEM_ALLOCATOR == 1)
        #warning TODO:
        #define MEMORY_ALLOCATE(type, number)
        #define MEMORY_RELEASE(startMem, number)
    #endif
    #define INSTANCE_RELEASE(startMem, number)                                                     \
        Core::destruct(startMem, number); MEMORY_RELEASE(startMem, number)

#endif // #if !defined(MEMORY_ALLOCATE)


namespace Core
{

    // 析构给定数目的类型为T的实体
    //
    // @param[in] first_instance
    //      第一个实体的地址
    // @param[in] instance_number
    //      实体的个数
    template < typename T >
    INLINE_FUNCTION
    void
    destruct (
        T *const     first_instance,
        const UInt32 instance_number)
    {
        for (UInt32 _idx = 0; _idx < instance_number; ++_idx)
        {
            // 调用析构器
            T *const _cur_instance = first_instance + _idx;
            (_cur_instance)->~T();
        }
    }


    // STL使用的内存Allocator/De-allocator
    // Ref: https://github.com/johannesthoma/mmap_allocator/blob/master/mmap_allocator.h
    // 以及 std::allocator_traits<T>
    //
    template < typename T >
    struct StlAllocator : public std::allocator<T>
    {
        // 通用类型定义：参考<memory>头文件中定义的allocator<T>
        //
        // T的常数指针
        typedef const T*    const_pointer;
        // T的常数参考
        typedef const T&    const_reference;
        // T的指针
        typedef T*          pointer;
        // T的参考
        typedef T&          reference;
        // T的种类
        typedef T           value_type;
        // 正整数类型：用来表示最大可以申请的Object的大小
        typedef size_t      size_type;
        // 有符号整数：用来表示两个Object的指针的差
        typedef ptrdiff_t   difference_type;
        // 不需要定义如下两个类型，因为我们将从std::allocator<T>继承此定义
        // 如果我们不使用继承，我们必须定义如下两个类型
//        typedef std::true_type propagate_on_container_move_assignment;
//        typedef std::true_type is_always_equal;


        // Allocates a node of type U, which will store the object of type T and one/more pointers
        //
        // NOTE: the declare of rebind will require the compiler supports the Member Templates functionality.
        template < typename U >
        struct rebind
        {
            typedef StlAllocator< U > other;
        };


        // Constructors
        // Constructor, which can NOT throw any kind of exception
        // noexcept keyword needs C++ 11 standard
        INLINE_FUNCTION
        StlAllocator () noexcept
        {

        }


        // Copy constructor, which can NOT throw any kind of exception
        INLINE_FUNCTION
        StlAllocator (
            const StlAllocator&) noexcept
        {

        }


        // Converting copy constructor, which creates an allocator of type T from another type U
        template < typename U >
        INLINE_FUNCTION
        StlAllocator (
            const StlAllocator< U >&) noexcept
        {

        }


        // Destructor, which can NOT throw any kind of exception
        INLINE_FUNCTION
        ~StlAllocator () noexcept
        {

        }


        // 申请(不构造)内存空间来存放instance_count个实例
        //
        // @param[in] instance_count
        //      T类型的实例数目
        // @param[in] locality_hint_ptr
        //      指向一个已有的由此Allocator分配的实例来提高Locality
        // @return
        //      指向第一个实例的指针
        // @throw
        //      bad_alloc异常如果内存不足
        //
        INLINE_FUNCTION
        static
        pointer allocate (
            size_type         instance_count,
            const void *const locality_hint_ptr = nullptr)
        {
            if (instance_count == 0)
            {
                return nullptr;
            }

            pointer const _alloc_addr = (pointer)MEMORY_ALLOCATE(sizeof(T), instance_count);
            if (!_alloc_addr)
            {
#if (USE_EXCEPTION == 1)
                throw std::bad_alloc();
#endif // #if (USE_EXCEPTION == 1)
            }

#if defined(CATCH_PLATFORM_MAC) || defined(CATCH_PLATFORM_WINDOWS) || defined(CATCH_PLATFORM_LINUX)
            // 测试逻辑
            const UInt8 MAGIC_NUMBER[] = { 0xA0, 0x0B };
            std::memcpy(_alloc_addr, MAGIC_NUMBER,
                        sizeof(T) < sizeof(MAGIC_NUMBER) ? sizeof(T) : sizeof(MAGIC_NUMBER));
#endif
            return _alloc_addr;
        }


        // 释放(不析构)alloc_addr所指向的内存
        //
        // @param[in] start_addr
        //      指向第一个实例的指针
        // @param[in] instance_count
        //      此地址存放的实例数目
        INLINE_FUNCTION
        static
        void
        deallocate (
            pointer   start_addr,
            size_type instance_count) noexcept
        {
            MEMORY_RELEASE(start_addr, instance_count);
        }


#if (USE_CUSTOMIZED_CONSTRUCT == 1)
        // NOTE: 由于从C++11开始，construct/destroy()是可选的
        // 所以如果我们要定义自己的construct/destroy()，需要定义USE_CUSTOMIZED_CONSTRUCT
        //
        //  如果construct/destroy()没有定义则使用：
        // std::allocator的
        // #if !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)
        //    template <class _Up, class... _Args>
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(_Up* __p, _Args&&... __args)
        //    {
        //        ::new((void*)__p) _Up(_VSTD::forward<_Args>(__args)...);
        //    }
        // #else
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(pointer __p)
        //    {
        //        ::new((void*)__p) _Tp();
        //    }
        // # if defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)
        //    template <class _A0>
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(pointer __p, _A0& __a0)
        //    {
        //        ::new((void*)__p) _Tp(__a0);
        //    }
        //    template <class _A0>
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(pointer __p, const _A0& __a0)
        //    {
        //        ::new((void*)__p) _Tp(__a0);
        //    }
        // # endif  // defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)
        //    template <class _A0, class _A1>
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(pointer __p, _A0& __a0, _A1& __a1)
        //    {
        //        ::new((void*)__p) _Tp(__a0, __a1);
        //    }
        //    template <class _A0, class _A1>
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(pointer __p, const _A0& __a0, _A1& __a1)
        //    {
        //        ::new((void*)__p) _Tp(__a0, __a1);
        //    }
        //    template <class _A0, class _A1>
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(pointer __p, _A0& __a0, const _A1& __a1)
        //    {
        //        ::new((void*)__p) _Tp(__a0, __a1);
        //    }
        //    template <class _A0, class _A1>
        //    _LIBCPP_INLINE_VISIBILITY
        //    void
        //    construct(pointer __p, const _A0& __a0, const _A1& __a1)
        //    {
        //        ::new((void*)__p) _Tp(__a0, __a1);
        //    }
        // #endif  // !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) && !defined(_LIBCPP_HAS_NO_VARIADICS)

        // 构建指定实例
        //
        // @param[in] alloc_addr
        //      用来构建实例的内存空间
        // @param[in] source_obj
        //      构建时用的参考实例
        template < typename... Args >
        INLINE_FUNCTION
        static
        void
        construct (
            pointer   alloc_addr,
            Args&&... args)
        {
            // 使用placement new来调用构造器
            ::new(reinterpret_cast<void*>(alloc_addr)) value_type(std::forward<Args>(args)...);
#if defined(CATCH_PLATFORM_MAC) || defined(CATCH_PLATFORM_WINDOWS) || defined(CATCH_PLATFORM_LINUX)
            // 测试逻辑
            const UInt8 MAGIC_NUMBER[] = { 0xC0, 0x0B };
            std::memcpy(alloc_addr, MAGIC_NUMBER,
                        sizeof(T) < sizeof(MAGIC_NUMBER) ? sizeof(T) : sizeof(MAGIC_NUMBER));
#endif
        }


        // 销毁给定内存空间的实例
        //
        // @param[in] object_ptr
        //      实例的内存空间
        INLINE_FUNCTION
        static
        void
        destroy (
            pointer object_ptr)
        {
            // 调用析构器
            (object_ptr)->~value_type();
        }
#endif // #if (USE_CUSTOMIZED_CONSTRUCT == 1)
    };

} // namespace Core
