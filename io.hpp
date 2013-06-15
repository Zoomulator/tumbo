#ifndef TUMBO_IO_HPP
#define TUMBO_IO_HPP

#include "matrix.hpp"
#include <iostream>

namespace tumbo
    {


    template < typename T, size_t M, size_t N > std::ostream&
    operator << ( std::ostream& os, const matrix<T,M,N>& A )
        {
        auto const delim = ',';
        for( size_t i=0; i<M; ++i )
            {
            os << '[';
            bool first = true;
            for( size_t j=0; j<N; ++j )
                {
                if(!first)
                    os << delim;
                os << A(i,j);
                first = false;
                }
            os << ']';
            }
        return os;
        }


    } // namespace tumbo

#endif // TUMBO_IO_HPP
