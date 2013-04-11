cmake_minimum_required( VERSION 2.8 )
project( TUMBO CXX )

set( TUMBO_HEADERS
    tumbo.hpp
    matrix.hpp
    utility.hpp )

install( FILES ${TUMBO_HEADERS} DESTINATION "include/tumbo" )
