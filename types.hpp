#ifndef TUMBO_TYPES_HPP
#define TUMBO_TYPES_HPP

#include "matrix.hpp"

namespace tumbo
    {

    /* All types are in separate namespaces so the user can include a certain
        set of types into their own namespace. */
    namespace template_types
        {
        template<typename T> using scalar = matrix<T,1,1>;

        template<typename T,size_t D> using vec = matrix<T,D,1>;
        template<typename T> using vec2 = vec<T,2>;
        template<typename T> using vec3 = vec<T,3>;
        template<typename T> using vec4 = vec<T,4>;

        template<typename T> using mat22 = matrix<T,2,2>;
        template<typename T> using mat33 = matrix<T,3,3>;
        template<typename T> using mat44 = matrix<T,4,4>;
        }

    namespace int_types
        {
        typedef template_types::vec2<int> ivec2;
        typedef template_types::vec3<int> ivec3;
        typedef template_types::vec4<int> ivec4;

        typedef template_types::mat22<int> imat22;
        typedef template_types::mat33<int> imat33;
        typedef template_types::mat44<int> imat44;
        }

    namespace float_types
        {
        typedef template_types::vec2<float> fvec2;
        typedef template_types::vec3<float> fvec3;
        typedef template_types::vec4<float> fvec4;

        typedef template_types::mat22<float> fmat22;
        typedef template_types::mat33<float> fmat33;
        typedef template_types::mat44<float> fmat44;
        }

    namespace double_types
        {
        typedef template_types::vec2<double> dvec2;
        typedef template_types::vec3<double> dvec3;
        typedef template_types::vec4<double> dvec4;

        typedef template_types::mat22<double> dmat22;
        typedef template_types::mat33<double> dmat33;
        typedef template_types::mat44<double> dmat44;
        }

    // Include all types into tumbo namespace.
    namespace types
        {
        using namespace template_types;
        using namespace int_types;
        using namespace float_types;
        using namespace double_types;
        }

    using namespace types;
    } // namespace tumbo

#endif /* TUMBO_TYPES_HPP */
