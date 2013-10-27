#ifndef TUMBO_QUATERNION_HPP
#define TUMBO_QUATERNION_HPP

#include <cmath>
#include "tumbo.hpp"

/* TODO: Verify that these functions work. They are untested. */

namespace tumbo
    {
    /* The quaternion is stored in a vec4. The vector is structured {x,y,z,w},
       where w is the rotation scalar and x,y,z is the axis vector.

       Remember to keep quaternions normalized if they're used for object
       orientation.
       auto mag_sq = tumbo:length_sq( q );
       if( max_sq >= (1+some_threshold) )
          q = q / sqrt(mag_sq);
       */
    typedef vec4 quaternion;
    typedef quaternion<float>  fquaternion;
    typedef quaternion<double> dquaternion;

    template<class T> quaternion<T>
    qmul( const quaternion<T>& A, const quaternion<T>& B )
        {
        using namespace components;
        quaternion<T> R;
        R[W] = (A[W]*B[W] - A[X]*B[X] - A[Y]*B[Y] - A[Z]*B[Z]);
        R[X] = (A[W]*B[X] + A[X]*B[W] + A[Y]*B[Z] - A[Z]*B[Y]);
        R[Y] = (A[W]*B[Y] - A[X]*B[Z] + A[Y]*B[W] + A[Z]*B[X]);
        R[Z] = (A[W]*B[Z] + A[X]*B[Y] - A[Y]*B[X] + A[Z]*B[W]);
        return R;
        }

    /* Creates a difference quaternion, also called a local quaternion, for
     * representing a change in rotation that can be multiplied onto another
     * quaternion. */
    template<class T> quaternion<T>
    qdiff( const quaternion<T>& A )
        {
        using std::sin;
        using std::cos;
        using namespace components;
        quaternion R;
        T angle = A[W]/T(2);
        T sinangle = sin( angle );
        R[W] = cos( angle );
        R[X] = A[X] * sinangle;
        R[Y] = A[Y] * sinangle;
        R[Z] = A[Z] * sinangle;
        return R;
        }

    template<class T> quaternion<T>
    qdiff( const vec3<T>& v, T s )
        {
        return qdiff( vweld(v, scalar<T>(s)) );
        }

    /* Matrix for any quaternion. */
    template<class T> mat4<T>
    qmat( const quaternion& A )
        {
        using namespace components;
        T x = A[X];
        T y = A[Y];
        T z = A[Z];
        T w = A[W];
        T w2 = A[W]*A[W];
        T x2 = A[X]*A[X];
        T y2 = A[Y]*A[Y];
        T z2 = A[Z]*A[Z];
        return mat4<T>{
            w2+x2-y2-z2, 2*(w*y-w*z), 2*(x*z+w*y), 0,
            2*(x*y+w*z), w2-x2+y2-z2, 2*(y*z+w*x), 0,
            2*(x*z-w*y), 2*(y*z-w*x), w2-x2-y2+z2, 0,
            0,           0,           0,           1 };
        }

    /* Matrix for unit quaternion. */
    template<class T> mat4<T>
    qmatu( const quaternion& A )
        {
        T x = A[X];
        T y = A[Y];
        T z = A[Z];
        T w = A[W];
        T w2 = A[W]*A[W];
        T x2 = A[X]*A[X];
        T y2 = A[Y]*A[Y];
        T z2 = A[Z]*A[Z];
        return mat4<T>{
            1+2*(-y2-z2), 2*(x*y-w*z), 2*(x*z+w*y),    0,
            2*(x*y+w*z), 1+2*(-x2-z2), 2*(y*z+w*x),    0,
            2*(x*z-w*y), 2*(y*z-w*x),  1+2*(-x2-y2),   0,
            0,           0,            0,              1 };
        }



    } /* namespace tumbo */

#endif /* TUMBO_QUATERNION_HPP */
