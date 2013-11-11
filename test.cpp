#include "tumbo.hpp"
#include "io.hpp"



void
test_swizzling()
    {
    using namespace tumbo;
    using namespace components;

    fvec2 v0{4,5};
    fvec3 v1{0,0,0};
    swiz(v1,X,Y,Z) = swiz(v0,Y,X,1.0f);
    //v1 = swiz(v0, Y,X,1.0f);
    std::cout << "Original: " << v0 << std::endl;
    std::cout << "Swizzled: " << v1 << std::endl;
    TUMBO_ASSERT( (v1 == fvec3{5,4,1}) );
    }

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

    // Submatrix
    auto v0 = fvec4{1,2,3,4};
    auto v1 = submatrix<3,1>(v0);
    for( size_t i=0; i<v1.size(); ++i )
        TUMBO_ASSERT( v1[i] == v0[i] );

    auto v2 = weldv( v1, scalar<float>{1} );
    std::cout << v2 << std::endl;

    std::cout << identity<fmat33>() << std::endl;
    std::cout << uniform<fvec4>(3) << std::endl;

    test_swizzling();

    std::cout << "Test finished." << std::endl;
    return 0;
    }
