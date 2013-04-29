#ifndef TUMBO_TUMBO_HPP
#define TUMBO_TUMBO_HPP

#include "matrix.hpp"
#include "utility.hpp"

namespace tumbo
    {
    template<typename T> using vec2 = matrix<T,2,1>;
    template<typename T> using vec3 = matrix<T,3,1>;
    template<typename T> using vec4 = matrix<T,4,1>;

    typedef matrix<int, 2, 1> ivec2;
    typedef matrix<int, 3, 1> ivec3;
    typedef matrix<int, 4, 1> ivec4;

    typedef matrix<int, 2, 2> imat22;
    typedef matrix<int, 3, 3> imat33;
    typedef matrix<int, 4, 4> imat44;


    typedef matrix<float, 2, 1> fvec2;
    typedef matrix<float, 3, 1> fvec3;
    typedef matrix<float, 4, 1> fvec4;

    typedef matrix<float, 2, 2> fmat22;
    typedef matrix<float, 3, 3> fmat33;
    typedef matrix<float, 4, 4> fmat44;

    typedef matrix<double, 2, 1> dvec2;
    typedef matrix<double, 3, 1> dvec3;
    typedef matrix<double, 4, 1> dvec4;

    typedef matrix<double, 2, 2> dmat22;
    typedef matrix<double, 3, 3> dmat33;
    typedef matrix<double, 4, 4> dmat44;
    } // namespace tumbo

#endif // TUMBO_TUMBO_HPP
