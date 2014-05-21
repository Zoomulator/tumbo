#ifndef TUMBO_TYPES_HPP
#define TUMBO_TYPES_HPP

#include "matrix.hpp"

namespace tumbo
    {
    template<typename T> using scalar = matrix<T,1,1>;

    template<typename T,size_t D> using vec = matrix<T,D,1>;
    template<typename T> using vec2 = vec<T,2>;
    template<typename T> using vec3 = vec<T,3>;
    template<typename T> using vec4 = vec<T,4>;

    template<typename T> using mat22 = matrix<T,2,2>;
    template<typename T> using mat33 = matrix<T,3,3>;
    template<typename T> using mat44 = matrix<T,4,4>;

    typedef vec2<int> ivec2;
    typedef vec3<int> ivec3;
    typedef vec4<int> ivec4;

    typedef matrix<int, 2, 2> imat22;
    typedef matrix<int, 3, 3> imat33;
    typedef matrix<int, 4, 4> imat44;

    typedef vec2<float> fvec2;
    typedef vec3<float> fvec3;
    typedef vec4<float> fvec4;

    typedef matrix<float, 2, 2> fmat22;
    typedef matrix<float, 3, 3> fmat33;
    typedef matrix<float, 4, 4> fmat44;

    typedef vec2<double> dvec2;
    typedef vec3<double> dvec3;
    typedef vec4<double> dvec4;

    typedef matrix<double, 2, 2> dmat22;
    typedef matrix<double, 3, 3> dmat33;
    typedef matrix<double, 4, 4> dmat44;
    } // namespace tumbo

#endif /* TUMBO_TYPES_HPP */
