#ifndef TUMBO_CONS_HPP
#define TUMBO_CONS_HPP

#include "matrix.hpp"
#include "utility.hpp"
#include <cmath>

/**
    \file cons.hpp
    \brief Contains various constructor functions for affine matrices.
*/

#ifdef _WIN32
#define static_assert(...)
#endif

namespace tumbo
    {
    template<typename matrix> matrix
    identity()
        {
        static_assert( matrix::width() == matrix::height(),
            "Only square matrix types has identity." );
        matrix A;
        for( size_t i=0; i<matrix::size(); ++i )
            A[i] = static_cast<typename matrix::scalar_t>(
                i % (matrix::width()+1) == 0 ? 1 : 0);
        return A;
        }


    template<typename matrix> matrix
    uniform(typename matrix::scalar_t s)
        {
        matrix A;
        for( size_t i=0; i<matrix::size(); ++i )
            A[i] = s;
        return A;
        }


    template<class T, size_t D> matrix<T,D+1,D+1>
    translation( matrix<T,D,1> v )
        {
        auto r = identity<matrix<T,D+1,D+1>>();
        assign_column( r, D, v );
        return r;
        }


    /// Contructor for an affine 3D rotation matrix.
    template<class T> matrix<T,4,4>
    rotation( T rad, T x, T y, T z )
        {
        T mag = T( std::sqrt( x*x + y*y + z*z ) );
        if( mag == 0.0f )
            return identity<matrix<T,4,4>>();

        x /= mag; y /= mag; z /= mag;

        T s = std::sin(rad);
        T c = std::cos(rad);
        T one_c = 1.0f - c;

        const T data[16] =
            {
            (one_c *x*x) + c,    (one_c *x*y) + z*s, (one_c *z*x) - y*s,        0,
            (one_c *x*y) - z*s,    (one_c *y*y) + c,    (one_c *y*z) + x*s,        0,
            (one_c *z*x) + y*s,    (one_c *y*z) -x*s,    (one_c *z*z) + c,        0,
            0,                    0,                    0,                        1 };

        matrix<T,4,4> R;
        R.assign( data, data+16 );
        return R;
        }


    /// Contructor for an affine 2D rotation matrix.
    template<class T> matrix<T,3,3>
    rotation( T rad )
        {
        matrix<T,3,3> R = identity<matrix<T,3,3>>();
        R(0,0) = std::cos(rad);
        R(1,0) = std::sin(rad);
        R(0,1) = -R(1,0);
        R(1,1) = R(0,0);
        return R;
        }


    template<class T, size_t D> matrix<T,D+1,D+1>
    scaling( matrix<T,D,1> v )
        {
        auto R = identity<matrix<T,D+1,D+1>>();
        for( size_t d = 0; d < D; ++d )
            R(d,d) = v[d];
        return R;
        }


    template<class T> matrix<T,4,4>
    orthographic(
        T left, T right,
        T bottom, T top,
        T near, T far )
        {
        T x_ortho = T( 2) / (right - left);
        T y_ortho = T( 2) / (top - bottom);
        T z_ortho = T(-2) / (far - near);
        T tx = -(right + left) / (right - left);
        T ty = -(top + bottom) / (top - bottom);
        T tz = (far + near)   / (far - near);
        return matrix<T,4,4>{
            x_ortho, 0,       0,       tx,
            0,       y_ortho, 0,       ty,
            0,       0,       z_ortho, tz,
            0,       0,       0,       1   };
        }


    // Near and far are entered as positive values.
    template<class T> matrix<T,4,4>
    perspective(
        T l, T r,       /* left, right */
        T b, T t,       /* bottom, top */
        T n, T f )      /* near, far */
        {
        return {
            2*n/(r-l),  0,          (r+l)/(r-l),            0,
            0,          2*n/(t-b),  (t+b)/(t-b),            0,
            0,          0,          -(f+n)/(f-n),           -2*f*n/(f-n),
            0,          0,          -1,                     0 };
        }


    template<class T> matrix<T,4,4>
    perspective(T fov, T aspect, T near, T far )
        {
        auto f = 1 / std::tan( fov/2 );
        auto depth = far - near;
        return matrix<T,4,4>
            {
            f/aspect,  0,  0,  0,
            0,         f,  0,  0,
            0,         0,  -far/depth,  -far*near/depth,
            0,         0,  -1,  0
            };
        }


    /* Calculates a transform with position at eye_position, pointing its
        Z-axis towards the target point.
        If the eye_position should be in world coordinates. To get the
        correct local transform you just do:
            inverse( parent_world_transform ) * look_at( [...] ) */
    template<class T> matrix<T,4,4>
    look_at(
        const matrix<T,3,1> eye_position,
        const matrix<T,3,1> target,
        const matrix<T,3,1> aprox_up )
        {
        using namespace tumbo;
        matrix<T,3,1> forward = normalize( target - eye_position );
        matrix<T,3,1> right   = normalize( cross( forward, aprox_up ) );
        matrix<T,3,1> up      = normalize( cross( right, forward ) );

        /* Weld together the three vectors, resulting in the rotation. */
        auto m0 = weld( right, weld( up, weld( -forward, eye_position) ) );
        /* Fill in the bottom row. */
        auto m1 = weldv( m0, transpose( matrix<T,4,1>{0,0,0,1} ) );
        return m1;
        }

    } // namespace tumbo

#endif // TUMBO_CONS_HPP
