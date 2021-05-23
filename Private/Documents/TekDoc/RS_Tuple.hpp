#pragma once


// System headers
#include <type_traits>
// Lib headers
#include "Core/Common/RS_CommonDefs.hpp"


namespace Core
{
    // --------- n元组: tuple< int, std::vector<int>, ClassA, char* > ---------
    // http://www.drdobbs.com/article/print?articleId=184401421&siteSectionName=cpp
    // 参见：Documents/Tuple Types and Multiple Return Values.pdf
    // 1. 定义一个结尾类型：NIL：表示空列表[]
    // 2. 定义列表进行连接操作后的类型：CONS<HEAD_TYPE, REST_TYPE>：表示x:xs，其中x表示第一个元素，xs表示剩余元素
    // 3. 为CONS<HEAD_TYPE, REST_TYPE>定义一个特例：CONS<HEAD_TYPE, NIL>：表示x:[]
    //
    // 这样，描述三元组(UInt32, Real32, Vec3D)的内部类型为：
    // CONS<  UInt32, CONS< Real32, CONS<Vec3D, NIL> >  >
    //          ^      ^      ^     ^              ^ ^
    //          |      |      |     |              | |
    //          |      |      |     +--------------+ |
    //          |      |    HEAD          REST       |
    //          |      +-----------------------------+
    //        HEAD                 REST
    //
    // 4. 定义一个公共接口：TUPLE<...>
    // 5. 定义一个TUPLE<...> 到 CONS<...>的映射
    // 6. 让TUPLE<...>来继承其内部的CONS<...>类型
    //
    //
    // n元组的结尾标记: []
    struct TUPLE_END_MARK
    {
        // 获得n元组结尾标记的参考
        INLINE_FUNCTION
        static
        const TUPLE_END_MARK&
        reference ()
        {
            // 唯一的实例
            static TUPLE_END_MARK SOLO_INSTANCE;
            return SOLO_INSTANCE;
        }
    };


    // 预先声明
    template < typename HeadType, typename RestType > struct TUPLE_CONS;

    // 定义链接操作 x:[]: 将一个元素x链接到一个空列表上
    template < typename HeadType >
    struct TUPLE_CONS < HeadType, TUPLE_END_MARK >
    {
        // templated member func: use the implicit type conversion from T1 to HeadType
        template < typename T1 >
        INLINE_FUNCTION
        TUPLE_CONS (
            T1 &             v1,
            const TUPLE_END_MARK & v2,
            const TUPLE_END_MARK & v3,
            const TUPLE_END_MARK & v4,
            const TUPLE_END_MARK & v5, 
            const TUPLE_END_MARK & v6)
        :
            head(v1)
        {

        }


        // copy constructor
        template < typename U1 >
        INLINE_FUNCTION
        TUPLE_CONS (
            const TUPLE_CONS< U1, TUPLE_END_MARK > & rhs)
        :
            head(rhs.head)
        {

        }


        // assignment operator
        template < typename U1 >
        INLINE_FUNCTION
        TUPLE_CONS&
        operator= (
            const TUPLE_CONS< U1, TUPLE_END_MARK > & rhs)
        {
            head = rhs.head;

            return *this;
        }


        // store this element's value
        HeadType head;
        // NO rest part
    };


    // x:xs: concatenate an element onto an existing list
    template < typename HeadType, typename RestType >
    struct TUPLE_CONS
    {
        // templated member func: use the implicit type conversion from T1 to HeadType and so forth
        template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
        INLINE_FUNCTION
        TUPLE_CONS (
            T1 & v1,
            T2 & v2,
            T3 & v3,
            T4 & v4,
            T5 & v5,
            T6 & v6)
        :
            head(v1),
            rest(v2, v3, v4, v5, v6, TUPLE_END_MARK::reference())
        {

        }


        // copy constructor
        template < typename U1, typename U2 >
        INLINE_FUNCTION
        TUPLE_CONS (
            const TUPLE_CONS< U1, U2 > & rhs)
        :
            head(rhs.head),
            rest(rhs.rest)
        {

        }


        // assignment operator
        template < typename U1, typename U2 >
        INLINE_FUNCTION
        TUPLE_CONS&
        operator= (
            const TUPLE_CONS< U1, U2 > & rhs)
        {
            head = rhs.head;
            rest = rhs.rest;

            return *this;
        }


        // 列表的头部：x
        HeadType head;
        // 列表中余下的部分：xs
        RestType rest;
    };


    // used to get the reference to a given type
    template < typename ClassType > struct GETREFTYPE;
    template < typename ClassType >
    struct GETREFTYPE< ClassType& >
    {
        // constant reference
        typedef ClassType EvalType_CREF;
        // normal reference
        typedef ClassType EvalType_REF;
    };


    template < typename ClassType >
    struct GETREFTYPE
    {
        typedef const ClassType& EvalType_CREF;
        typedef ClassType& EvalType_REF;
    };


    // used to get the default value of a given class: the given class MUST have a default constructor
    // If the get() is instantiated, the default constructor MUST be public
    template < typename ClassType > struct DEFAULT_VALUE;
    template < typename ClassType >
    struct DEFAULT_VALUE< ClassType& >
    {
        // NO implementation of get() for reference type: linking error for this struct means the given ClassType does NOT have a PUBLIC default constructor
    };


    template < >
    struct DEFAULT_VALUE < TUPLE_END_MARK >
    {
        INLINE_FUNCTION
        static
        TUPLE_END_MARK
        get ()
        {
            return TUPLE_END_MARK::reference();
        }
    };


    template < typename ClassType >
    struct DEFAULT_VALUE
    {
        INLINE_FUNCTION
        static
        ClassType
        get ()
        {
            return ClassType();
        }
    };


    // --- TUPLE<...> 到 CONS<...>的映射 ---
    // MARK: -
    // MARK: - 获得TUPLE<...> 到 CONS<...>的映射
    //
    // 预先声明
    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    struct MAP_TUPLE_2_CONS;

    // 将tuple<T>映射到TUPLE_CONS<T, TUPLEEND>
    template< typename T1 >
    struct MAP_TUPLE_2_CONS < T1,
                              TUPLE_END_MARK,
                              TUPLE_END_MARK,
                              TUPLE_END_MARK,
                              TUPLE_END_MARK,
                              TUPLE_END_MARK >
    {
        // MARK: - EvalType定义为T1:[]
        typedef TUPLE_CONS < T1, TUPLE_END_MARK> EvalType;
        // MARK: -
    };


    // 将tuple<T1, T2, T3, T4, T5, T6>映射到TUPLE_CONS<...>
    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    struct MAP_TUPLE_2_CONS
    {
        // MARK: - EvalType定义为T1:xs, 这里xs为T2:T3:T4:T5:T6:[]
        typedef TUPLE_CONS< T1,
                            typename MAP_TUPLE_2_CONS< T2, T3, T4, T5, T6,
                                                       TUPLE_END_MARK >::EvalType > EvalType;
        // MARK: -
    };
    // MARK: -
    // MARK: -



    // Public Interface of tuple: tuple
    // maximal 30 members: without existing of variadic template argument,
    // we can NOT define multiple templates with the same name, but different argument lists: we have to use default arguments
    template< typename T1,
              typename T2 = TUPLE_END_MARK,
              typename T3 = TUPLE_END_MARK,
              typename T4 = TUPLE_END_MARK,
              typename T5 = TUPLE_END_MARK,
              typename T6 = TUPLE_END_MARK >
    struct tuple : public MAP_TUPLE_2_CONS< T1, T2, T3, T4, T5, T6 >::EvalType
    {
        typedef typename MAP_TUPLE_2_CONS< T1, T2, T3, T4, T5, T6 >::EvalType Super;

        // constructor
        INLINE_FUNCTION
        tuple (
            typename GETREFTYPE<T1>::EvalType_CREF & v1 = DEFAULT_VALUE<T1>::get(),
            typename GETREFTYPE<T2>::EvalType_CREF & v2 = DEFAULT_VALUE<T2>::get(),
            typename GETREFTYPE<T3>::EvalType_CREF & v3 = DEFAULT_VALUE<T3>::get(),
            typename GETREFTYPE<T4>::EvalType_CREF & v4 = DEFAULT_VALUE<T4>::get(),
            typename GETREFTYPE<T5>::EvalType_CREF & v5 = DEFAULT_VALUE<T5>::get(),
            typename GETREFTYPE<T6>::EvalType_CREF & v6 = DEFAULT_VALUE<T6>::get())
        :
            // let the TUPLE_CONS store all given values
            Super(v1, v2, v3, v4, v5, v6)
        {

        }


        // copy constructor
        template < typename U1, typename U2 >
        INLINE_FUNCTION
        tuple (
            const TUPLE_CONS< U1, U2 > & rhs)
        :
            Super(rhs)
        {

        }


        // assignment operator
        template < typename U1, typename U2 >
        INLINE_FUNCTION
        tuple&
        operator= (
            const TUPLE_CONS< U1, U2 > & rhs)
        {
            ((Super*)this)->operator=(rhs);

            return *this;
        }
    };


    // get the Nth: 0-based tuple member's type: TupleType: must be TUPLE_CONS<.....>
    template < int zero_based_index, typename TupleType > struct GETRETTYPE;


    // GETRETTYPE(0, x:xs)
    template < typename HeadType, typename RestType >
    struct GETRETTYPE< 0, TUPLE_CONS< HeadType, RestType > >
    {
        typedef HeadType EvalType;
    };


    // GETRETTYPE(n, x:xs): n > 0, do the same operation on rest part of the given tuple
    template < int zero_based_index, typename HeadType, typename RestType >
    struct GETRETTYPE< zero_based_index, TUPLE_CONS< HeadType, RestType > >
    {
        typedef typename GETRETTYPE< zero_based_index - 1, RestType >::EvalType EvalType;
    };


    // NO way to specialize a function, we have to specialize a data type: struct
    template < int zero_based_index >
    struct GETMEMBER
    {
        template < typename HeadType, typename RestType >
        INLINE_FUNCTION
        static
        typename std::remove_reference< typename GETRETTYPE< zero_based_index, TUPLE_CONS< HeadType, RestType > >::EvalType >::type&
        get (
            TUPLE_CONS< HeadType, RestType >& tuple_instance)
        {
            return GETMEMBER< zero_based_index - 1>::get(tuple_instance.rest);
        }
    };


    // for zero_based_index == 0
    template <>
    struct GETMEMBER < 0 >
    {
        template < typename HeadType, typename RestType >
        INLINE_FUNCTION
        static
        typename std::remove_reference< typename GETRETTYPE< 0, TUPLE_CONS< HeadType, RestType > >::EvalType >::type&
        get (
            TUPLE_CONS< HeadType, RestType >& tuple_instance)
        {
            return tuple_instance.head;
        }
    };


    // get the Nth member's reference
    // NthType get<N>(tuple&)
    template < int zero_based_index, typename HeadType, typename RestType >
    INLINE_FUNCTION
    typename std::remove_reference< typename GETRETTYPE< zero_based_index,
                                                         TUPLE_CONS< HeadType, RestType > >::EvalType
                                  >::type&
    get (
        TUPLE_CONS< HeadType, RestType > & tuple_instance)
    {
        return GETMEMBER<zero_based_index>::get(tuple_instance);
    }


    // make_tuple(...)
    //
    // 例如：
    // make_tuple(int_var, real_var, classA_var);
    // 相当于定义tuple<int, float, classA>(int_var, real_var, classA_var);
    template< typename T1 >
    INLINE_FUNCTION
    tuple< typename std::remove_all_extents<T1>::type >
    make_tuple (
        const T1 & v1)
    {
        return tuple< typename std::remove_all_extents<T1>::type >(v1);
    }


    template< typename T1, typename T2 >
    INLINE_FUNCTION
    tuple< typename std::remove_all_extents<T1>::type,
           typename std::remove_all_extents<T2>::type >
    make_tuple (
        const T1 & v1,
        const T2 & v2)
    {
        return tuple< typename std::remove_all_extents<T1>::type,
                      typename std::remove_all_extents<T2>::type >(v1, v2);
    }


    template< typename T1, typename T2, typename T3 >
    INLINE_FUNCTION
    tuple< typename std::remove_all_extents<T1>::type,
           typename std::remove_all_extents<T2>::type,
           typename std::remove_all_extents<T3>::type >
    make_tuple (
        const T1 & v1,
        const T2 & v2,
        const T3 & v3)
    {
        return tuple< typename std::remove_all_extents<T1>::type,
                      typename std::remove_all_extents<T2>::type,
                      typename std::remove_all_extents<T3>::type >(v1, v2, v3);
    }


    template< typename T1, typename T2, typename T3, typename T4 >
    INLINE_FUNCTION
    tuple< typename std::remove_all_extents<T1>::type,
           typename std::remove_all_extents<T2>::type,
           typename std::remove_all_extents<T3>::type,
           typename std::remove_all_extents<T4>::type >
    make_tuple (
        const T1 & v1,
        const T2 & v2,
        const T3 & v3,
        const T4 & v4)
    {
        return tuple< typename std::remove_all_extents<T1>::type,
                      typename std::remove_all_extents<T2>::type,
                      typename std::remove_all_extents<T3>::type,
                      typename std::remove_all_extents<T4>::type >(v1, v2, v3, v4);
    }


    template< typename T1, typename T2, typename T3, typename T4, typename T5 >
    INLINE_FUNCTION
    tuple< typename std::remove_all_extents<T1>::type,
           typename std::remove_all_extents<T2>::type,
           typename std::remove_all_extents<T3>::type,
           typename std::remove_all_extents<T4>::type,
           typename std::remove_all_extents<T5>::type >
    make_tuple (
        const T1 & v1,
        const T2 & v2,
        const T3 & v3,
        const T4 & v4,
        const T5 & v5)
    {
        return tuple< typename std::remove_all_extents<T1>::type,
                      typename std::remove_all_extents<T2>::type,
                      typename std::remove_all_extents<T3>::type,
                      typename std::remove_all_extents<T4>::type,
                      typename std::remove_all_extents<T5>::type >(v1, v2, v3, v4, v5);
    }


    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    INLINE_FUNCTION
    tuple< typename std::remove_all_extents<T1>::type,
           typename std::remove_all_extents<T2>::type,
           typename std::remove_all_extents<T3>::type,
           typename std::remove_all_extents<T4>::type,
           typename std::remove_all_extents<T5>::type,
           typename std::remove_all_extents<T6>::type >
    make_tuple (
        const T1 & v1,
        const T2 & v2,
        const T3 & v3,
        const T4 & v4,
        const T5 & v5,
        const T6 & v6)
    {
        return tuple< typename std::remove_all_extents<T1>::type,
                      typename std::remove_all_extents<T2>::type,
                      typename std::remove_all_extents<T3>::type,
                      typename std::remove_all_extents<T4>::type,
                      typename std::remove_all_extents<T5>::type,
                      typename std::remove_all_extents<T6>::type >(v1, v2, v3, v4, v5, v6);
    }


    // bind(...)
    template < typename T1 >
    INLINE_FUNCTION
    tuple< T1& >
    bind (T1 & v1)
    {
        return tuple< T1& >(v1);
    }


    template < typename T1, typename T2 >
    INLINE_FUNCTION
    tuple< T1&, T2& >
    bind (
        T1 & v1,
        T2 & v2)
    {
        return tuple< T1&, T2& >(v1, v2);
    }


    template < typename T1, typename T2, typename T3 >
    INLINE_FUNCTION
    tuple< T1&, T2&, T3& >
    bind (
        T1 & v1,
        T2 & v2,
        T3 & v3)
    {
        return tuple< T1&, T2&, T3& >(v1, v2, v3);
    }


    template < typename T1, typename T2, typename T3, typename T4 >
    INLINE_FUNCTION
    tuple< T1&, T2&, T3&, T4& >
    bind (
        T1 & v1,
        T2 & v2,
        T3 & v3,
        T4 & v4)
    {
        return tuple< T1&, T2&, T3&, T4& >(v1, v2, v3, v4);
    }


    template < typename T1, typename T2, typename T3, typename T4, typename T5 >
    tuple< T1&, T2&, T3&, T4&, T5& >
    INLINE_FUNCTION
    bind (
        T1 & v1,
        T2 & v2,
        T3 & v3,
        T4 & v4, 
        T5 & v5)
    {
        return tuple< T1&, T2&, T3&, T4&, T5& >(v1, v2, v3, v4, v5);
    }


    template < typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    INLINE_FUNCTION
    tuple< T1&, T2&, T3&, T4&, T5&, T6& >
    bind (
        T1 & v1,
        T2 & v2, 
        T3 & v3, 
        T4 & v4, 
        T5 & v5, 
        T6 & v6)
    {
        return tuple< T1&, T2&, T3&, T4&, T5&, T6& >(v1, v2, v3, v4, v5, v6);
    }

} // namespace Core
