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
