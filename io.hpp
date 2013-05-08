#ifndef TUMBO_IO_HPP
#define TUMBO_IO_HPP

#include "matrix.hpp"
#include <iostream>

namespace tumbo
    {


    template < typename T, size_t M, size_t N > std::ostream&
    operator << ( std::ostream& os, const matrix<T,M,N>& A )
        {
        for( size_t i=0; i<M; ++i )
            {
            os << '[';
            char delim = '\0';
            for( size_t j=0; j<N; ++j )
                {
                os << delim << A(i,j);
                delim = ',';
                }
            os << ']';
            }
        return os;
        }


    } // namespace tumbo

#endif // TUMBO_IO_HPP
