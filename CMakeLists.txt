cmake_minimum_required( VERSION 2.8 )
project( TUMBO CXX )

set( TUMBO_HEADERS
    aabb.hpp
    assert.hpp
    cons.hpp
    io.hpp
    lua_binding.hpp
    lua_std_binding.hpp
    lua_cons_binding.hpp
    lua_aabb_binding.hpp
    matrix.hpp
    swizzling.hpp
    tumbo.hpp
    types.hpp
    utility.hpp )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall" )
add_executable( test_suite test.cpp )
target_link_libraries( test_suite gtest gtest_main pthread )

install( FILES ${TUMBO_HEADERS} DESTINATION "include/tumbo" )
