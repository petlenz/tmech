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
