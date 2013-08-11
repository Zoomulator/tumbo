#ifndef TUMBO_CONS_HPP
#define TUMBO_CONS_HPP

#include "matrix.hpp"


/**
    \file cons.hpp
    \brief Contains various constructor functions for affine matrices.
*/

namespace tumbo
    {


	/// Constructor for an affine 3D translation matrix.
	template<class T> matrix<T,4,4>
	translation( T x, T y, T z )
		{
		auto result = matrix<T,4U,4U>::identity();
		result(0,3) = x;
		result(1,3) = y;
		result(2,3) = z;
		return result;
		}


	/// Constructor for an affine 2D translation matrix.
	template<class T> matrix<T,3,3>
	translation( T x, T y )
		{
		auto result = matrix<T,3,3>::identity();
		result(0,2) = x;
		result(1,2) = y;
		return result;
		}


	/// Contructor for an affine 3D rotation matrix.
	template<class T> matrix<T,4,4>
	rotation( T rad, T x, T y, T z )
		{
		T mag = T( sqrt( x*x + y*y + z*z ) );
		if( mag == 0.0f )
			return matrix<T,4,4>::identity();

		x /= mag; y /= mag; z /= mag;

		T s = sin(rad);
		T c = cos(rad);
		T one_c = 1.0f - c;

		const T data[16] =
			{
			(one_c *x*x) + c,	(one_c *x*y) + z*s, (one_c *z*x) - y*s,		0,
			(one_c *x*y) - z*s,	(one_c *y*y) + c,	(one_c *y*z) + x*s,		0,
			(one_c *z*x) + y*s,	(one_c *y*z) -x*s,	(one_c *z*z) + c,		0,
			0,					0,					0,						1 };

		matrix<T,4,4> R;
		R = data;
		return R;
		}


	/// Contructor for an affine 2D rotation matrix.
	template<class T> matrix<T,3,3>
	rotation( T rad )
		{
		matrix<T,3,3> R = matrix<T,3,3>::identity();
		R(0,0) = cos(rad);
		R(1,0) = sin(rad);
		R(0,1) = -R(1,0);
		R(1,1) = R(0,0);
		return R;
		}


    /// Constructor for an affine 2D scale matrix.
	template<class T> matrix<T,3,3>
	scaling( T x, T y )
		{
		matrix<T,3,3> R = matrix<T,3,3>::identity();
		R(0,0) = x;
		R(1,1) = y;
		return R;
		}


    template<class T> matrix<T,4,4>
    scaling( T x, T y, T z )
        {
        matrix<T,4,4> R = matrix<T,4,4>::identity();
        R(0,0) = x;
        R(1,1) = y;
        R(2,2) = z;
        return R;
        }


    template<class T> matrix<T,4,4>
    ortho(
        T left, T right,
        T bottom, T top,
        T near, T far )
        {
        T x_ortho = T( 2) / (right - left);
        T y_ortho = T( 2) / (top - bottom);
        T z_ortho = T(-2) / (far - near);
        T tx = -(right + left) / (right - left);
        T ty = -(top + bottom) / (top - bottom);
        T tz = -(far + near)   / (far - near);
        return matrix<T,4,4>{
            x_ortho, 0,       0,       tx,
            0,       y_ortho, 0,       ty,
            0,       0,       z_ortho, tz,
            0,       0,       0,       0   };
        }

    } // namespace tumbo

#endif // TUMBO_CONS_HPP
