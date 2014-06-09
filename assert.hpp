/**
    \def TUMBO_ASSERT
    \brief Used by tumbo for runtime assertions.

    Can be disabled by defining NDEBUG, like c assert.
    On failure it throws a std::logic_error.
    This can be replaced by a standard c assert by defining TUMBO_CASSERT
*/

#ifndef TUMBO_ASSERT_HPP
#define TUMBO_ASSERT_HPP


#ifndef NDEBUG
#ifdef TUMBO_CASSERT
    #include <cassert>
    #define TUMBO_ASSERT(x) assert((x))
#else
    #include <stdexcept>
    #define TUMBO_ASSERT(x)\
        if( !(x) ) throw std::logic_error( "Tumbo assertion failed: " #x );
#endif // TUMBO_CASSERT
#else
    #define TUMBO_ASSERT(x)
#endif // NDEBUG

#endif // TUMBO_ASSERT_HPP
