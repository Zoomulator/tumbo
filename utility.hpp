#ifndef TUMBO_UTILITY_HPP
#define TUMBO_UTILITY_HPP

#include "matrix.hpp"

#undef minor

namespace tumbo
	{


	template< class T, size_t M, size_t N > matrix<T,N,M>
	transpose( const matrix<T,M,N>& A )
		{
		matrix<T,N,M> R;

		for( size_t i=0; i<M; ++i )
		for( size_t j=0; j<N; ++j )
			R(j,i) = A(i,j);

		return R;
		}


	template< class T, size_t M, size_t N> matrix<T,M,N>
	operator* ( T s, const matrix<T,M,N>& A )
		{
		matrix<T,M,N> R;
		for( size_t i=0; i < A.size(); ++i )
			R[i] = s * A[i];
		return R;
		}


	template< class T, size_t M, size_t N> matrix<T,M,N>
	operator/ (const matrix<T,M,N>& A, T s )
		{
		matrix<T,M,N> R;
		for( size_t i=0; i < A.size(); ++i )
			R[i] = A[i] / s;
		return R;
		}


	template< class T, size_t M, size_t N> matrix<T,M,N>
	operator- ( const matrix<T,M,N>& A )
		{
		matrix<T,M,N> R;

		for( size_t i=0; i < A.size(); ++i )
			R[i] = -A[i];
		return R;
		}


	template< class T, size_t AM, size_t AN, size_t BM, size_t BN > T
	dot( const matrix<T,AM,AN>& A, const matrix<T,BM,BN>& B )
		{
		static_assert( (AM == 1 || AN == 1) && (BM == 1 || BN == 1), "Input must be vector" );
		static_assert( AM*AN == BM*BN, "Vectors must be equal in size" );

		T sum = 0;
		for( size_t i=0; i < A.size(); ++i )
			sum += A[i]*B[i];

		return sum;
		}


	template< class T, size_t M, size_t N > T
	length_sq( const matrix<T,M,N>& A )
		{
		static_assert( M == 1 || N == 1,
           "Length is a vector property. matrix is not a vector." );

		T square_sum = 0;
		for( size_t i=0; i<A.size(); ++i )
			square_sum += A[i]*A[i];
        return square_sum;
        }


    template< class T, size_t M, size_t N > T
    length( const matrix<T,M,N>& A )
        {
		return sqrt( length_sq(A) );
		}


	template< class T, size_t M, size_t N > matrix<T,1,N>
	row( const matrix<T,M,N>& A, size_t i )
		{
		matrix<T,1,N> r;
		for( size_t j=0; j < N; ++j )
			r[j] = A(i,j);
		return r;
		}


	template< class T, size_t M, size_t N > matrix<T,M,1>
	column( const matrix<T,M,N>& A, size_t j )
		{
		matrix<T,M,1> c;
		for( size_t i = 0; i < M; ++i )
			c[i] = A(i,j);
		return c;
		}


	template< size_t RM, size_t RN, class T, size_t AM, size_t AN > matrix<T,RM,RN>
	submatrix( const matrix<T,AM,AN>& A, size_t oi, size_t oj )
		{
		matrix<T,RM,RN> R;

		for( size_t i=0; i<RM; ++i )
		for( size_t j=0; j<RN; ++j )
			R(i,j) = A(i+oi, j+oj);

		return R;
		}


	template<class T, size_t M, size_t N, class container> matrix<T,M,N>&
	assign_row( matrix<T,M,N>& A, size_t i, const container& vec)
		{
		auto it = std::begin(vec);
		auto end = std::end(vec);
		assert( N == std::distance(it,end) );
		size_t j = 0;
		while( it != end )
			{
			A(i,j) = *it;
			++it; ++j;
			}
		return A;
		}


	template<class T, size_t M, size_t N, class container> matrix<T,M,N>&
	assign_column( matrix<T,M,N>& A, size_t j, const container& vec)
		{
		auto it = std::begin(vec);
		auto end = std::end(vec);
		assert( M == std::distance(it,end) );
		size_t i = 0;
		while( it != end )
			{
			A(i,j) = *it;
			++it; ++i;
			}
		return A;
		}


	template< class T, size_t M, size_t N, size_t P > matrix<T,M,P>
	operator * ( const matrix<T,M,N>& A, const matrix<T,N,P>& B )
		{
		//static_assert( AN == BM, "matrix multiplication can only be done with a M x N and N x O matrices." );

		matrix<T,M,P> R;

		for( size_t i=0; i<M; ++i )
		for( size_t j=0; j<P; ++j )
			R(i,j) = dot( row(A,i), column(B,j) );

		return R;
		}


	template< class T, size_t M, size_t N > bool
	operator == ( const matrix<T,M,N>& A, const matrix<T,M,N>& B )
		{
		for( size_t i = 0; i < M*N; ++i )
			if( A[i] != B[i] ) return false;

		return true;
		}


	template< class T, size_t M, size_t N > matrix<T,M,N>
	operator + ( const matrix<T,M,N>& A, const matrix<T,M,N>& B )
		{
		matrix<T,M,N> result;
		for( size_t i=0; i < M*N; ++i )
			result[i] = A[i] + B[i];

		return result;
		}


	template< class T, size_t M, size_t N > matrix<T,M,N>
	operator - ( const matrix<T,M,N>& A, const matrix<T,M,N>& B )
		{
		matrix<T,M,N> result;
		for( size_t i=0; i < M*N; ++i )
			result[i] = A[i] - B[i];

		return result;
		}


	// Generates a new matrix where a row and column is removed.
	template< class T, size_t M, size_t N > matrix<T, M-1, N-1>
	cross_out( const matrix<T,M,N>& A, size_t r, size_t c )
		{
		matrix<T, M-1, N-1> result;
		size_t rj = 0;
		size_t ri = 0;

		for( size_t i = 0; i < M; ++i )
			{
			if( i == r ) continue; // Skip row

			for( size_t j=0; j<N; ++j )
				{
				if( j == c ) continue; // Skip column
				result( ri,rj ) = A(i,j);
				++rj;
				}
			++ri;
			rj = 0;
			}

		return result;
		}


	// The Determinant_ class does some template magic to avoid static
	// infinite recursion

	// Determinant for a NxN matrix:
	// |A| == + a00 * |A00| - a01 * |A01| + a02 * |A02| ... +- a0n * |A0n|;
	template< class T, size_t M, size_t N >
	struct determinant_
		{
		static T calc( const matrix<T,M,N>& A )
			{
			T sum = 0;
			for( size_t j=0; j<N; ++j )
				sum += ( j%2==1 ? -1 : 1 ) * // Every other is negative
						A(0,j) * determinant(cross_out(A,0,j));
			return sum;
			}
		};


	template< class T >
	struct determinant_<T,1,1>
		{
		static T calc( const matrix<T,1,1>& A )
			{
			return A[0];
			}
		};

	// Determinant for a 2x2 matrix:
	template< class T >
	struct determinant_<T,2,2>
		{
		static T calc( const matrix<T,2,2>& A )
			{
			return A[0] * A[3] - A[1] * A[2];
			}
		};

	template< class T, size_t M, size_t N > T
	determinant( const matrix<T,M,N>& A )
		{
		// Splits up the templates into recursion and base case
		return determinant_<T,M,N>::calc(A);
		}


	template< class T, size_t M, size_t N > matrix<T,M,N>
	minor( const matrix<T,M,N>& A )
		{
		matrix<T,M,N> result;

		for( size_t j=0; j<N; ++j )
		for( size_t i=0; i<M; ++i )
			result(i,j) = determinant( cross_out(A,i,j) );

		return result;
		}


	template< class T, size_t M, size_t N > matrix<T,M,N>
	cofactor( const matrix<T,M,N>& A )
		{
		matrix<T,M,N> result = minor(A);
		for( size_t i = 0; i < M; ++i )
		for( size_t j = 0; j < N; ++j )
			result(i,j) = result(i,j) * ((i+j)%2 ? -1.0f : 1.0f ); // Flip signs every other element

		return result;
		}


	template< class T, size_t M, size_t N > matrix<T,M,N>
	adjugate( const matrix<T,M,N>& A )
		{
		if( M == 2 && N == 2 )
			{
			matrix<T,M,N> adj;
			adj(0,0) = A(1,1);	adj(1,1) = A(0,0);
			adj(0,1) = A(0,1);	adj(1,0) = A(1,0);
			return adj;
			}

		return transpose( cofactor(A) );
		}


	template< class T, size_t M, size_t N > matrix<T,M,N>
	inverse( const matrix<T,M,N>& A )
		{
		static_assert( M == N, "Inverse: matrix must be square." );
		return (1 / determinant(A) ) * adjugate(A);
		}


	template< class T, size_t M, size_t N > bool
	is_singular( const matrix<T,M,N>& A )
		{
		return determinant(A) == 0;
		}


	// Constructor for Identity matrix.
	template< class T, size_t M, size_t N > matrix<T,M,N>
	matrix<T,M,N>::identity()
		{
		matrix<T,M,N> A;
		for( size_t i=0; i<M*N; ++i )
			A[i] = static_cast<T>(i % (N+1) == 0 ? 1 : 0);

		return A;
		}


	// Constructor for a uniform matrix. All elements will have the given value
	template< class T, size_t M, size_t N > matrix<T,M,N>
	matrix<T,M,N>::uniform(typename matrix<T,M,N>::scalar_t s)
		{
		matrix<T,M,N> A;
		for( size_t i=0; i<M*N; ++i )
			A[i] = s;
		return A;
		}


	// Constructor for translation matrix.
	template<class T> matrix<T,4,4>
	translation( T x, T y, T z )
		{
		auto result = matrix<T,4U,4U>::identity();
		result(3,0) = x;
		result(3,1) = y;
		result(3,2) = z;
		return result;
		}

	template<class T> matrix<T,3,3>
	translation( T x, T y )
		{
		auto result = matrix<T,3,3>::identity();
		result(0,2) = x;
		result(1,2) = y;
		return result;
		}


	// Contructor for rotation matrix.
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

	template<class T> matrix<T,3,3>
	scaling( T x, T y )
		{
		matrix<T,3,3> R = matrix<T,3,3>::identity();
		R(0,0) = x;
		R(1,1) = y;
		return R;
		}



	// Maps a function over a matrix. Returns the resulting matrix
	template<class T, size_t M, size_t N, class FuncT> matrix<T,M,N>
	mapf( const matrix<T,M,N>& A, FuncT fun )
		{
		matrix<T,M,N> R;
		for( size_t i=0; i < A.size(); ++i )
			{
			R[i] = fun( A[i] );
			}
		return R;
		}

	} // namespace tumbo


#endif // TUMBO_UTILITY_HPP
