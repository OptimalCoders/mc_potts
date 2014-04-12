// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    11.04.2014 14:35:43 CEST
// File:    meta_tpl.hpp

#ifndef __META_TPL_HEADER
#define __META_TPL_HEADER

#include <typeinfo>

//------------------- list -------------------
struct node_end;

template<typename T, typename next_ = node_end>
struct list_node {
    using type = T;
    using next = next_;
};
//------------------- create list -------------------
template<typename T1, typename ...TT>
struct list {
    using type = list_node<T1, typename list<TT...>::type>;
};
template<typename T>
struct list<T> {
    using type = list_node<T>;
};
//------------------- get form list -------------------
template<typename T, typename L, int C, int N>
struct get_impl {
    using type = typename get_impl<typename L::type, typename L::next, C+1, N>::type;
};
template<typename T, typename L, int C>
struct get_impl<T, L, C, C> {
    using type = T;
};
template<typename L, int N>
struct get {
    using type = typename get_impl<typename L::type, typename L::next, 0, N>::type;
};
//------------------- print list -------------------
template<typename T>
void print_list() {
    std::cout << typeid(typename T::type).name() << std::endl;
    print_list<typename T::next>();
}
template<>
void print_list<node_end>() {}

//------------------- size -------------------
template<typename L>
struct size {
    enum{value = 1 + size<typename L::next>::value};
};
template<>
struct size<node_end> {
    enum{value = 0};
};

//------------------- concat -------------------
template<typename T, typename N, typename L2>
struct concat_impl {
    using type = list_node<T, typename concat_impl<typename N::type, typename N::next, L2>::type>;
};
template<typename T, typename L2>
struct concat_impl<T, node_end, L2> {
    using type = list_node<T, L2>;
};
template<typename L, typename L2>
struct concat {
    using type = typename concat_impl<typename L::type, typename L::next, L2>::type;
};

//------------------- n copy -------------------
template<typename L, int N>
struct copy_impl {
    using type = typename concat<L, typename copy_impl<L, N-1>::type>::type;
};
template<typename L>
struct copy_impl<L, 1> {
    using type = L;
};
template<typename L, int N>
struct copy {
    using type = typename copy_impl<L, N>::type;
};
//------------------- list n -------------------
template<typename T, int N, typename E = node_end>
struct listn {
    using type = list_node<T, typename listn<T, N-1, E>::type>;
};
template<typename T, typename E>
struct listn<T, 1, E> {
    using type = list_node<T, E>;
};
//------------------- n stretch -------------------
template<typename T, typename L, int N>
struct stretch_impl {
    using type = typename listn<T, N, typename stretch_impl<typename L::type, typename L::next, N>::type>::type;
};
template<typename T, int N>
struct stretch_impl<T, node_end, N> {
    using type = typename listn<T, N>::type;
};
template<typename L, int N>
struct stretch {
    using type = typename stretch_impl<typename L::type, typename L::next, N>::type;
};
//------------------- splice -------------------
template< typename T1
        , typename N1
        , typename T2
        , typename N2
        , typename T3
        , typename N3
        , typename T4
        , typename N4
        , typename T5
        , typename N5
        , typename T6
        , typename N6>
struct splice_impl {
    using type = list_node<typename list<T1, T2, T3, T4, T5, T6>::type, typename splice_impl< typename N1::type
                                                                                    , typename N1::next
                                                                                    , typename N2::type
                                                                                    , typename N2::next
                                                                                    , typename N3::type
                                                                                    , typename N3::next
                                                                                    , typename N4::type
                                                                                    , typename N4::next
                                                                                    , typename N5::type
                                                                                    , typename N5::next
                                                                                    , typename N6::type
                                                                                    , typename N6::next
                                                                     >::type
                          >;
};
template< typename T1
        , typename T2
        , typename T3
        , typename T4
        , typename T5
        , typename T6>
struct splice_impl<T1, node_end, T2, node_end, T3, node_end, T4, node_end, T5, node_end, T6, node_end> {
    using type = list_node<typename list<T1, T2, T3, T4, T5, T6>::type>;
};
template<typename L1, typename L2, typename L3, typename L4, typename L5, typename L6>
struct splice {
    using type = typename splice_impl<typename L1::type
                                    , typename L1::next
                                    , typename L2::type
                                    , typename L2::next
                                    , typename L3::type
                                    , typename L3::next
                                    , typename L4::type
                                    , typename L4::next
                                    , typename L5::type
                                    , typename L5::next
                                    , typename L6::type
                                    , typename L6::next>::type;
};
//------------------- permute -------------------
template<typename L1, typename L2, typename L3, typename L4, typename L5, typename L6>
struct permut_list {
    using f2 = typename copy<typename stretch<L6, 1>::type, size<L1>::value * size<L2>::value * size<L3>::value * size<L4>::value * size<L5>::value>::type;
    using e2 = typename copy<typename stretch<L5, size<L6>::value>::type, size<L1>::value * size<L2>::value * size<L3>::value * size<L4>::value>::type;
    using d2 = typename copy<typename stretch<L4, size<L6>::value * size<L5>::value>::type, size<L1>::value * size<L2>::value * size<L3>::value>::type;
    using c2 = typename copy<typename stretch<L3, size<L4>::value * size<L5>::value * size<L6>::value>::type, size<L1>::value * size<L2>::value>::type;
    using b2 = typename copy<typename stretch<L2, size<L4>::value * size<L3>::value * size<L5>::value * size<L6>::value>::type, size<L1>::value>::type;
    using a2 = typename copy<typename stretch<L1, size<L4>::value * size<L3>::value * size<L2>::value * size<L5>::value * size<L6>::value>::type, 1>::type;
    
    using type = typename splice<a2, b2, c2, d2, e2, f2>::type;
};
//------------------- int_type -------------------
template<int N>
struct int_type {
    static const int value = N;
};
//------------------- rational_type -------------------
template<int N, int D>
struct rational_type {
    static const int nom = N;
    static const int div = D;
    static double get() {
        return double(nom) / div;
    }
};

#endif //__META_TPL_HEADER
