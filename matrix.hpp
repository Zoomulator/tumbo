/**
*/
#ifndef TUMBO_MATRIX_HPP
#define TUMBO_MATRIX_HPP

#include <iterator>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "assert.hpp"

#ifdef _WIN32
#define constexpr
#endif

namespace tumbo
	{
    namespace components
        {
        enum cmp { X=0, Y, Z, W };
        }

	/**
        \class matrix
        \brief Statically sized matrix type

        The matrix index is ordered (row, column) and is stored
        in a row major order.

        Convinient typedefs such as mat44 and vec3 are defined in tumbo.hpp
	*/
	template < class T, size_t M, size_t N >
	class matrix
		{
		public:
			typedef T scalar_t;

			matrix() {}
            matrix( const matrix& );
            matrix( std::initializer_list<T> l );

            template<class Iter>
            matrix( Iter first, Iter end );

            template<class Iter> matrix&
            assign( Iter first, Iter end );

            matrix&
            operator = ( const matrix& );

			scalar_t&
            operator() ( size_t i, size_t j );

			const scalar_t&
            operator() ( size_t i, size_t j ) const;

			scalar_t&
            operator[] ( size_t i );

			const scalar_t&
            operator[] ( size_t i ) const;

			const scalar_t*
            data() const;

			scalar_t*
            begin();

			const scalar_t*
            begin() const;

			scalar_t*
            end();

			const scalar_t*
            end() const;

			static constexpr size_t
            size()
				{ return M*N; }

			static constexpr size_t
            height()
				{ return M; }

			static constexpr size_t
            width()
				{ return N; }

		private:
			scalar_t data_[ M*N ];
		};


    template< class T, size_t M, size_t N >
    matrix<T,M,N>::matrix( const matrix<T,M,N>& other )
        {
        std::memcpy( this, &other, sizeof(matrix<T,M,N>) );
        }


    template < class T, size_t M, size_t N >
    matrix<T,M,N>::matrix( std::initializer_list<T> l )
        {
        assign(l.begin(), l.end());
        }


    template< class T, size_t M, size_t N>
    template<class Iter>
    matrix<T,M,N>::matrix( Iter it, Iter end )
        {
        assign(it,end);
        }


    template < class T, size_t M, size_t N >
    matrix<T,M,N>&
    matrix<T,M,N>::operator = ( const matrix<T,M,N>& other )
        {
        auto it = std::begin(other);
        auto end = std::end(other);
        return assign(it,end);
        }


    template < class T, size_t M, size_t N >
    template<class Iter>
    matrix<T,M,N>&
    matrix<T,M,N>::assign ( Iter it, Iter end )
        {
        assert( size() == std::distance(it,end) );
        size_t i=0;
        while( it != end )
            {
            (*this)[i] = *it;
            ++it; ++i;
            }
        return *this;
        }


    template < class T, size_t M, size_t N >
    typename matrix<T,M,N>::scalar_t&
    matrix<T,M,N>::operator() ( size_t i, size_t j )
        {
        TUMBO_ASSERT( i < M )
        TUMBO_ASSERT( j < N )
        return data_[ j + i*N ];
        }


    template < class T, size_t M, size_t N >
    const typename matrix<T,M,N>::scalar_t&
    matrix<T,M,N>::operator() ( size_t i, size_t j ) const
        {
        TUMBO_ASSERT( i < M )
        TUMBO_ASSERT( j < N )
        return data_[ j + i*N ];
        }


    template < class T, size_t M, size_t N >
    typename matrix<T,M,N>::scalar_t&
    matrix<T,M,N>::operator[] ( size_t i )
        {
        TUMBO_ASSERT( i < M*N );
        return data_[i];
        }


    template < class T, size_t M, size_t N >
    const typename matrix<T,M,N>::scalar_t&
    matrix<T,M,N>::operator[] ( size_t i ) const
        {
        TUMBO_ASSERT( i < M*N );
        return data_[i];
        }



    template < class T, size_t M, size_t N >
    const typename matrix<T,M,N>::scalar_t*
    matrix<T,M,N>::data() const
        {
        return data_;
        }


    template < class T, size_t M, size_t N >
    typename matrix<T,M,N>::scalar_t*
    matrix<T,M,N>::begin()
        {
        return data_;
        }



    template < class T, size_t M, size_t N >
    const typename matrix<T,M,N>::scalar_t*
    matrix<T,M,N>::begin() const
        {
        return data_;
        }


    template < class T, size_t M, size_t N >
    typename matrix<T,M,N>::scalar_t*
    matrix<T,M,N>::end()
        {
        return data_+size();
        }


    template < class T, size_t M, size_t N >
    const typename matrix<T,M,N>::scalar_t*
    matrix<T,M,N>::end() const
        {
        return data_+size();
        }

	} // namespace tumbo

#endif //TUMBO_MATRIX_HPP
