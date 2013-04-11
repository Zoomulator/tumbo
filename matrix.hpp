#ifndef TUMBO_MATRIX_HPP
#define TUMBO_MATRIX_HPP

#include <algorithm>
#include <cassert>


namespace tumbo
	{

	/*
	The matrix index is ordered (row, column) and is stored
	in a row major order.

	Use matrix with desired size by using a typedef and naming your setup.
	typedef matrix<float, 4,4> matrix44
	typedef matrix<float, 1,4> Vec4
	*/
	template < class T, size_t M, size_t N >
	class matrix
		{
		public:
			typedef T scalar_t;

			static matrix identity();
			static matrix uniform(scalar_t);

			matrix() {}
            matrix( std::initializer_list<T> l )
                { *this = l; }

			template<class container>
			matrix( const container& data )
				{ *this = data;	}

			template<class container> matrix&
			operator = ( const container& data )
				{
				auto it = std::begin(data);
				auto end = std::end(data);
				assert( size() == std::distance(it,end) );
				size_t i=0;
				while( it != end )
					{
					(*this)[i] = *it;
					++it; ++i;
					}
				return *this;
				}

			scalar_t&
            operator() ( size_t i, size_t j )
				{ return data[ j + i*N ]; }

			const scalar_t&
            operator() ( size_t i, size_t j ) const
				{ return data[ j + i*N ]; }

			scalar_t&
            operator[] ( size_t i )
				{ return data[i]; }

			const scalar_t&
            operator[] ( size_t i ) const
				{ return data[i]; }

			const scalar_t*
            data() const
				{ return data; }

			scalar_t*
            begin()
                { return data; }

			const scalar_t*
            begin() const
                { return data; }

			scalar_t*
            end()
                { return data+M*N; }

			const scalar_t*
            end() const
                { return data+M*N; }

			static size_t
            size()
				{ return M*N; }

			static size_t
            height()
				{ return M; }

			static size_t
            width()
				{ return N; }

			static bool
            is_vector()
				{ return M == 1 || N == 1; }

		private:
			scalar_t data[ M*N ];
		};


	} // namespace tumbo

#endif //TUMBO_MATRIX_HPP
