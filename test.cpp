#include "tumbo.hpp"
#include "io.hpp"




int
main( int argc, char* argv[] )
    {
    using namespace tumbo;
    imat22 A{ 0,1,2,3 };
    std::cout << A << std::endl;

    // Multiplication
    auto A2 = A*A;
    imat22 A2_correct{ 2,3,6,11 };
    TUMBO_ASSERT( A2 == A2_correct );
    return 0;
    }
