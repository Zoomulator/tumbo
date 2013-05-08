/**
*/
#ifndef TUMBO_MATRIX_HPP
#define TUMBO_MATRIX_HPP

#include <algorithm>
#include <cassert>


namespace tumbo
	{

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

			static matrix identity();
			static matrix uniform(scalar_t);

			matrix() {}
            matrix( std::initializer_list<T> l );

			template<class container>
			matrix( const container& new_data );

			template<class container> matrix&
			operator = ( const container& new_data );

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

			static size_t
            size()
				{ return M*N; }

			static size_t
            height()
				{ return M; }

			static size_t
            width()
				{ return N; }

		private:
			scalar_t data_[ M*N ];
		};


	/// Constructor for Identity matrix.
	template< class T, size_t M, size_t N > matrix<T,M,N>
	matrix<T,M,N>::identity()
		{
		matrix<T,M,N> A;
		for( size_t i=0; i<M*N; ++i )
			A[i] = static_cast<T>(i % (N+1) == 0 ? 1 : 0);

		return A;
		}


	/// Creates a uniform matrix. All elements will have the given value
	template< class T, size_t M, size_t N > matrix<T,M,N>
	matrix<T,M,N>::uniform(typename matrix<T,M,N>::scalar_t s)
		{
		matrix<T,M,N> A;
		for( size_t i=0; i<M*N; ++i )
			A[i] = s;
		return A;
		}


    template < class T, size_t M, size_t N >
    matrix<T,M,N>::matrix( std::initializer_list<T> l )
        {
        *this = l;
        }


    template < class T, size_t M, size_t N >
    template<class container>
    matrix<T,M,N>::matrix( const container& new_data )
        {
        *this = new_data;
        }


    template < class T, size_t M, size_t N >
    template<class container>
    matrix<T,M,N>&
    matrix<T,M,N>::operator = ( const container& new_data )
        {
        auto it = std::begin(new_data);
        auto end = std::end(new_data);
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
        return data_[ j + i*N ];
        }


    template < class T, size_t M, size_t N >
    const typename matrix<T,M,N>::scalar_t&
    matrix<T,M,N>::operator() ( size_t i, size_t j ) const
        {
        return data_[ j + i*N ];
        }


    template < class T, size_t M, size_t N >
    typename matrix<T,M,N>::scalar_t&
    matrix<T,M,N>::operator[] ( size_t i )
        {
        return data_[i];
        }


    template < class T, size_t M, size_t N >
    const typename matrix<T,M,N>::scalar_t&
    matrix<T,M,N>::operator[] ( size_t i ) const
        {
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
