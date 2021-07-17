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

#include <type_traits> 

//sfinae friendly result_of
namespace detail {
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
 
} // namespace detail
 
 
// Conforming C++14 implementation (is also a valid C++11 implementation):
namespace detail {
template <typename AlwaysVoid, typename, typename...>
struct invoke_result { };
template <typename F, typename...Args>
struct invoke_result<decltype(void(detail::INVOKE(std::declval<F>(), std::declval<Args>()...))),
                 F, Args...> {
    using type = decltype(detail::INVOKE(std::declval<F>(), std::declval<Args>()...));
};
} // namespace detail
 
template <typename> struct result_of_sfinae;
template <typename F, typename... ArgTypes>
struct result_of_sfinae<F(ArgTypes...)> : detail::invoke_result<void, F, ArgTypes...> {};
 
template <typename F, typename... ArgTypes>
struct invoke_result_sf : detail::invoke_result<void, F, ArgTypes...> {};

template <typename Signature>
using result_of_t_sfinae = typename result_of_sfinae<Signature>::type;

#endif