// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
#pragma once

#if defined(__NEC__)


#include <tuple>
#include <type_traits> 

namespace nec_helpers{

//sfinae friendly result_of
namespace detail_sfinae {
template <class T>
struct is_reference_wrapper : std::false_type {};
template <class U>
struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
 
template<class T>
struct invoke_impl {
    template<class F, class... Args>
    static auto call(F&& f, Args&&... args)
        -> decltype(std::forward<F>(f)(std::forward<Args>(args)...));
};
 
template<class B, class MT>
struct invoke_impl<MT B::*> {
    template<class T, class Td = typename std::decay<T>::type,
        class = typename std::enable_if<std::is_base_of<B, Td>::value>::type
    >
    static auto get(T&& t) -> T&&;
 
    template<class T, class Td = typename std::decay<T>::type,
        class = typename std::enable_if<is_reference_wrapper<Td>::value>::type
    >
    static auto get(T&& t) -> decltype(t.get());
 
    template<class T, class Td = typename std::decay<T>::type,
        class = typename std::enable_if<!std::is_base_of<B, Td>::value>::type,
        class = typename std::enable_if<!is_reference_wrapper<Td>::value>::type
    >
    static auto get(T&& t) -> decltype(*std::forward<T>(t));
 
    template<class T, class... Args, class MT1,
        class = typename std::enable_if<std::is_function<MT1>::value>::type
    >
    static auto call(MT1 B::*pmf, T&& t, Args&&... args)
        -> decltype((invoke_impl::get(std::forward<T>(t)).*pmf)(std::forward<Args>(args)...));
 
    template<class T>
    static auto call(MT B::*pmd, T&& t)
        -> decltype(invoke_impl::get(std::forward<T>(t)).*pmd);
};
 
template<class F, class... Args, class Fd = typename std::decay<F>::type>
auto INVOKE(F&& f, Args&&... args)
    -> decltype(invoke_impl<Fd>::call(std::forward<F>(f), std::forward<Args>(args)...));
 
} // namespace detail_sfinae
 
 
// Conforming C++14 implementation (is also a valid C++11 implementation):
namespace detail_sfinae {
template <typename AlwaysVoid, typename, typename...>
struct invoke_result { };
template <typename F, typename...Args>
struct invoke_result<decltype(void(detail_sfinae::INVOKE(std::declval<F>(), std::declval<Args>()...))),
                 F, Args...> {
    using type = decltype(detail_sfinae::INVOKE(std::declval<F>(), std::declval<Args>()...));
};
} // namespace detail_sfinae
 
template <typename> struct result_of;
template <typename F, typename... ArgTypes>
struct result_of<F(ArgTypes...)> : detail_sfinae::invoke_result<void, F, ArgTypes...> {};
 
template <typename F, typename... ArgTypes>
struct invoke_result_sf : detail_sfinae::invoke_result<void, F, ArgTypes...> {};

template <typename Signature>
using result_of_t = typename result_of<Signature>::type;


//https://stackoverflow.com/questions/47496358/c-lambdas-how-to-capture-variadic-parameter-pack-from-the-upper-scope
// Implementation detail of a simplified std::apply from C++17
template < typename F, typename Tuple, std::size_t ... I >
constexpr decltype(auto)
apply_impl(F&& f, Tuple&& t, std::index_sequence< I ... >){
    return static_cast< F&& >(f)(std::get< I >(static_cast< Tuple&& >(t)) ...);
}

// Implementation of a simplified std::apply from C++17
template < typename F, typename Tuple >
constexpr decltype(auto) apply(F&& f, Tuple&& t){
    return apply_impl(
        static_cast< F&& >(f), static_cast< Tuple&& >(t),
        std::make_index_sequence< std::tuple_size<
            std::remove_reference_t< Tuple > >::value >{});
}

 

}
#endif
