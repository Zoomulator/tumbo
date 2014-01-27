#include "tumbo.hpp"
#include "io.hpp"

#include <gtest/gtest.h>

using namespace tumbo;

TEST( MatOp, M44Inverse )
    {
    fmat44 m0{
        1, 0, 0, 3,
        0, 1, 0, 2,
        0, 0, 1, 7,
        0, 0, 0, 1 };

    auto m1 = inverse(m0);
    auto m2 = inverse(m1);
    for( int i=0; i<16; ++i )
        ASSERT_FLOAT_EQ( m2[i], m0[i] );
    }

TEST( MatOp, M33Inverse )
    {
    fmat33 m0{
        1, 2, 5,
        0, 3, 0,
        0, 0, 1 };

    auto m1 = inverse(m0);
    auto m2 = inverse(m1);
    for( int i=0; i<9; ++i )
        ASSERT_FLOAT_EQ( m2[i], m0[i] );
    }

TEST( MatOp, M22Inverse )
    {
    fmat22 m0{
        96, 48,
        0,  35 };
    fmat22 expect{
        0.010416667, -0.014285715,
        0,          0.028571431 };

    auto m1 = inverse(m0);
    auto m2 = inverse(m1);
    for( int i=0; i<4; ++i )
        {
        ASSERT_FLOAT_EQ( expect[i], m1[i] );
        ASSERT_FLOAT_EQ( m0[i], m2[i] );
        }
    }

TEST( Swizzling, Swizz )
    {
    using namespace tumbo;
    using namespace components;

    fvec2 v0{4,5};
    fvec3 v1{0,0,0};
    swiz(v1,X,Y,Z) = swiz(v0,Y,X,1.0f);
    //v1 = swiz(v0, Y,X,1.0f);
    std::cout << "Original: " << v0 << std::endl;
    std::cout << "Swizzled: " << v1 << std::endl;
    ASSERT_EQ( v1, (fvec3{5,4,1}) );
    }

TEST( Multiplication, Mat22 )
    {
    imat22 A{ 0,1,2,3 };
    auto A2 = A*A;
    imat22 A2_correct{ 2,3,6,11 };
    ASSERT_EQ( A2, A2_correct );
    }
/*
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
    }*/
