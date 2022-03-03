// Copyright 2022 Peter Lenz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------
#ifndef TMECH_FUNCTION_WRAPPER_H
#define TMECH_FUNCTION_WRAPPER_H

namespace detail {

struct det_wrapper{

    template<typename T>
    constexpr static inline auto apply(T && value){
        return tmech::det(std::forward<T>(value));
    }

    static inline std::string get_string(){
        return std::string("det");
    }
};

struct trace_wrapper{

    template<typename T>
    constexpr static inline auto apply(T && value){
        return tmech::trace(std::forward<T>(value));
    }

    static inline std::string get_string(){
        return std::string("trace");
    }
};


struct cof_wrapper{

    template<typename T>
    constexpr static inline auto apply(T && value){
        return tmech::cof(std::forward<T>(value));
    }

    static inline std::string get_string(){
        return std::string("cof");
    }
};

struct adj_wrapper{

    template<typename T>
    constexpr static inline auto apply(T && value){
        return tmech::adj(std::forward<T>(value));
    }

    static inline std::string get_string(){
        return std::string("adj");
    }
};

}
#endif // TMECH_FUNCTION_WRAPPER_H
