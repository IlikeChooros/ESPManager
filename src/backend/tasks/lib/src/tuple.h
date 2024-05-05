#pragma once

/*

A complete implementation of the `apply` function for tuples.
This implementation is based on the one provided by the C++14 standard library.

*/

#include <tuple>

#include "namespaces.h"

BEGIN_TASKS_NAMESPACE

// Generate an integer sequence
template<int...> struct index_sequence {};

template<int N, int... Is>
struct make_index_sequence : make_index_sequence<N-1, N-1, Is...> {};

template<int... Is>
struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

// Apply function to tuple
template<class F, class Tuple, int... Is>
auto apply_impl(F&& f, Tuple&& t, index_sequence<Is...>) 
-> decltype(std::forward<F>(f)(std::get<Is>(std::forward<Tuple>(t))...)) 
{ 
    return std::forward<F>(f)(std::get<Is>(std::forward<Tuple>(t))...); 
} 

template<class F, class Tuple> 
auto apply(F&& f, Tuple&& t) 
-> decltype(apply_impl(std::forward<F>(f), std::forward<Tuple>(t), 
    make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>())) 
{ 
    return apply_impl(std::forward<F>(f), std::forward<Tuple>(t), 
        make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>()); 
}


END_TASKS_NAMESPACE