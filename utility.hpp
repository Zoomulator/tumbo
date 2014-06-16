#ifndef TUMBO_UTILITY_HPP
#define TUMBO_UTILITY_HPP

#include <utility>
#include "matrix.hpp"
#include "assert.hpp"

#undef minor

namespace tumbo
    {
    const double PI = 3.1415926535897932384626433832795028841971693993751058;

    /// Returns a copy of the matrix transposed.
    template< class T, size_t M, size_t N > matrix<T,N,M>
    transpose( const matrix<T,M,N>& A )
        {
        matrix<T,N,M> R;

        for( size_t i=0; i<M; ++i )
        for( size_t j=0; j<N; ++j )
            R(j,i) = A(i,j);

        return R;
        }


    /// Multiply matrix by scalar.
    template< class S, class T, size_t M, size_t N>
    typename std::enable_if< std::is_arithmetic<S>::value,
        matrix< typename std::common_type<T,S>::type, M, N > >::type
    operator* ( S s, const matrix<T,M,N>& A )
        {
        matrix< typename std::common_type<T,S>::type, M, N > R;
        for( size_t i=0; i < A.size(); ++i )
            R[i] = s * A[i];
        return R;
        }
        
        
    /// Scalar multiplication is associative.
    template< class T, class S, size_t M, size_t N >
    typename std::enable_if< std::is_arithmetic<S>::value,
        matrix< typename std::common_type<T,S>::type, M, N > >::type
    operator* ( const matrix<T,M,N>& A, S s )
        {
        return s * A;
        }


    /// Matrix division by scalar.
    template< class T, class S, size_t M, size_t N >
    matrix< typename std::common_type<S,T>::type, M, N >
    operator/ (const matrix<T,M,N>& A, S s )
        {
        matrix< typename std::common_type<S,T>::type, M, N > R;
        for( size_t i=0; i < A.size(); ++i )
            R[i] = A[i] / s;
        return R;
        }

    // Element-wise binary operation
    template<class T, size_t M, size_t N, typename Op> matrix<T,M,N>
    ewise(const matrix<T,M,N>& A, const matrix<T,M,N>& B, Op op)
        {
        matrix<T,M,N> R;
        for( size_t i=0; i<M*N; ++i )
            R[i] = op(A[i],B[i]);
        return R;
        }

    template<class T, size_t M, size_t N> matrix<T,M,N>
    emultiply( const matrix<T,M,N>& A, const matrix<T,M,N>& B )
        {
        return ewise(A,B, std::multiplies<T>());
        }

    template<class T, size_t M, size_t N> matrix<T,M,N>
    edivision( const matrix<T,M,N>& A, const matrix<T,M,N>& B )
        {
        return ewise(A,B, std::divides<T>());
        }


    /// Element-wise negation of matrix.
    template< class T, size_t M, size_t N> matrix<T,M,N>
    operator- ( const matrix<T,M,N>& A )
        {
        matrix<T,M,N> R;

        for( size_t i=0; i < A.size(); ++i )
            R[i] = -A[i];
        return R;
        }


    /// Dot product between two equal length vector matrices.
    template< class T, class S, size_t AM, size_t AN, size_t BM, size_t BN >
    typename std::common_type<T,S>::type
    dot( const matrix<T,AM,AN>& A, const matrix<S,BM,BN>& B )
        {
        static_assert( (AM == 1 || AN == 1) && (BM == 1 || BN == 1),
            "Input must be vectors." );
        static_assert( AM*AN == BM*BN,
            "Vectors must be equal in length." );

        typename std::common_type<T,S>::type sum = 0;
        for( size_t i=0; i < A.size(); ++i )
            sum += A[i]*B[i];

        return sum;
        }

    /// Special case of cross product for 2D vectors. Returns scalar.
    template<class T> T
    cross( const matrix<T,2,1>& A, const matrix<T,2,1>& B )
        {
        using namespace tumbo::components;
        return A[X]*B[Y] - A[Y]*B[X];
        }

    /// Returns the cross product vector of two vectors.
    template<class T, size_t D> matrix<T,D,1>
    cross( const matrix<T,D,1>& A, const matrix<T,D,1>& B )
        {
        matrix<T,D,1> R;
        for( size_t d=0; d<D; ++d )
            R[d] = A[(d+1)%D] * B[(d+2)%D] - A[(d+D-1)%D] * B[(d+D-2)%D];
        return R;
        }


    /// Returns the squared length of a vector matrix.
    /**
        Many comparisons and equations require the square length of a vector.
        This function lets you avoid the indirect use of sqrt when using
        the standard length function.
    */
    template< class T, size_t M, size_t N >
    T
    length_sq( const matrix<T,M,N>& A )
        {
        static_assert( M == 1 || N == 1,
           "Length is a vector property. matrix is not a vector." );

        T square_sum = 0;
        for( size_t i=0; i<A.size(); ++i )
            square_sum += A[i]*A[i];
        return square_sum;
        }


    /// Returns the length, or magnitude, of a vector. Uses sqrt
    template< class T, size_t M, size_t N >
    decltype( sqrt( std::declval<T>() ) )
    length( const matrix<T,M,N>& A )
        {
        return sqrt( length_sq(A) );
        }


    template< class T, size_t M, size_t N >
    matrix< decltype(length(std::declval<matrix<T,M,N>>())), M, N >
    normalize( const matrix<T,M,N>& A,
               decltype(length(std::declval<matrix<T,M,N>>()))* len = nullptr )
        {
        static_assert( M == 1 || N == 1,
           "Can only normalize a vector" );
        auto L = length( A );
        if( len ) *len = L;
        return A / L;
        }


    // Reflect A over line L.
    template< class T, size_t M, size_t N >
    matrix< T, M, N >
    reflect( const matrix<T,M,N>& A, const matrix<T,M,N>& L )
        {
        static_assert( M == 1 || N == 1, "Only vectors can be reflected." );
        return 2 * dot(A,L) / length_sq(L) * L - A;
        }


    // Flip a 2D vector 90degrees
    template< class T, size_t M, size_t N >
    matrix< T, M, N >
    orthogonal( const matrix<T,M,N>& A )
        {
        static_assert( ( M == 1 && N == 2 ) || ( M == 2 && N == 1 ),
            "Only 2D vectors can be used with the orthogonal function." );
        return { A[1], -A[0] };
        }


    /// Returns a copy of row i of matrix A as a vector matrix.
    template< class T, size_t M, size_t N > matrix<T,1,N>
    row( const matrix<T,M,N>& A, size_t i )
        {
        matrix<T,1,N> r;
        for( size_t j=0; j < N; ++j )
            r[j] = A(i,j);
        return r;
        }


    /// Returns a copy of column j of matrix A as a vector matrix.
    template< class T, size_t M, size_t N > matrix<T,M,1>
    column( const matrix<T,M,N>& A, size_t j )
        {
        matrix<T,M,1> c;
        for( size_t i = 0; i < M; ++i )
            c[i] = A(i,j);
        return c;
        }


    /// Returns a copy of the specified submatrix in A.
    /** Usage: The size of the new matrix is specified as template arguments.
        The coordinates gives the top left element in A that will be the first
        element of the submatrix.
        submatrix<H,W>( A, y, x )
    */
    template< size_t RM, size_t RN, class T, size_t AM, size_t AN >
    matrix<T,RM,RN>
    submatrix( const matrix<T,AM,AN>& A, size_t oi = 0, size_t oj = 0 )
        {
        matrix<T,RM,RN> R;

        for( size_t i=0; i<RM; ++i )
        for( size_t j=0; j<RN; ++j )
            R(i,j) = A(i+oi, j+oj);

        return R;
        }


    /// Mutates the matrix A at row i to contain the given data.
    /** The data may be any iterable type, including sized c-arrays and
        initializer lists. It may contain less elements than the width of
        the row, in which case it fills the row with the given elements and
        leaves the remaining row elements unchanged.
    */
    template<class T, size_t M, size_t N, class container> matrix<T,M,N>&
    assign_row( matrix<T,M,N>& A, size_t i, const container& vec)
        {
        auto it = std::begin(vec);
        auto end = std::end(vec);
        TUMBO_ASSERT( N >= std::distance(it,end) );
        size_t j = 0;
        while( it != end )
            {
            A(i,j) = *it;
            ++it; ++j;
            }
        return A;
        }


    // Mutates the matrix A at column j to contain the given data.
    /** Refer to assign_row for details. */
    template<class T, size_t M, size_t N, class container> matrix<T,M,N>&
    assign_column( matrix<T,M,N>& A, size_t j, const container& vec)
        {
        auto it = std::begin(vec);
        auto end = std::end(vec);
        TUMBO_ASSERT( M >= std::distance(it,end) );
        size_t i = 0;
        while( it != end )
            {
            A(i,j) = *it;
            ++it; ++i;
            }
        return A;
        }


    /// Combines two matrices of equal height into a larger matrix.
    template< class T, size_t M, size_t N0, size_t N1 > matrix<T,M,N0+N1>
    weld( const matrix<T,M,N0>& A, const matrix<T,M,N1>& B )
        {
        matrix<T,M,N0+N1> result;
        for( size_t i=0; i < M; ++i )
            {
            for( size_t j=0; j < N0; ++j )
                result(i,j) = A(i,j);
            for( size_t j=0; j < N1; ++j )
                result(i,N0+j) = B(i,j);
            }
        return result;
        }


    /// Combines two matrices of equal width into a larger matrix.
    template< class T, size_t M0, size_t M1, size_t N > matrix<T,M0+M1,N>
    weldv( const matrix<T,M0,N>& A, const matrix<T,M1,N>& B )
        {
        matrix<T,M0+M1,N> result;
        for( size_t j=0; j < N; ++j )
            {
            for( size_t i=0; i < M0; ++i )
                result(i,j) = A(i,j);
            for( size_t i=0; i < M1; ++i )
                result(M0+i,j) = B(i,j);
            }
        return result;
        }


    /// Multiplication operator between two matrices.
    /** Matrix B must have the same height as A's width. */
    template< class T, class S, size_t M, size_t N, size_t P >
    matrix< typename std::common_type<T,S>::type, M, P >
    operator * ( const matrix<T,M,N>& A, const matrix<S,N,P>& B )
        {
        matrix< typename std::common_type<T,S>::type, M, P > R;

        for( size_t i=0; i<M; ++i )
        for( size_t j=0; j<P; ++j )
            R(i,j) = dot( row(A,i), column(B,j) );

        return R;
        }


    /// Element-wise comparison between two matrices.
    template< class T, size_t M, size_t N > bool
    operator == ( const matrix<T,M,N>& A, const matrix<T,M,N>& B )
        {
        for( size_t i = 0; i < M*N; ++i )
            if( A[i] != B[i] ) return false;

        return true;
        }


    /// Element-wise addition of matrices.
    template< class T, class S, size_t M, size_t N >
    matrix< typename std::common_type<T,S>::type, M, N >
    operator + ( const matrix<T,M,N>& A, const matrix<S,M,N>& B )
        {
        matrix< typename std::common_type<T,S>::type, M, N > result;
        for( size_t i=0; i < M*N; ++i )
            result[i] = A[i] + B[i];

        return result;
        }


    /// Element-wise subtraction of matrices.
    template< class T, class S, size_t M, size_t N >
    matrix< typename std::common_type<T,S>::type, M, N >
    operator - ( const matrix<T,M,N>& A, const matrix<S,M,N>& B )
        {
        matrix< typename std::common_type<T,S>::type, M, N > result;
        for( size_t i=0; i < M*N; ++i )
            result[i] = A[i] - B[i];

        return result;
        }


    /// Generates a new matrix where a row and column is removed.
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


    /// Returns the matrix minor.
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
            // Flip signs every other element
            result(i,j) = result(i,j) * ((i+j)%2 ? -1.0f : 1.0f );

        return result;
        }


    template< class T, size_t M, size_t N > matrix<T,M,N>
    adjugate( const matrix<T,M,N>& A )
        {
        if( M == 2 && N == 2 )
            {
            matrix<T,M,N> adj{
                A(1,1), -A(0,1),
                -A(1,0), A(0,0)
                };
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


    /// Maps a function over a matrix. Returns the resulting matrix
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
