cmake_minimum_required( VERSION 2.8 )
project( TUMBO CXX )

set( TUMBO_HEADERS
    assert.hpp
    cons.hpp
    io.hpp
    matrix.hpp
    tumbo.hpp
    utility.hpp )

install( FILES ${TUMBO_HEADERS} DESTINATION "include/tumbo" )
