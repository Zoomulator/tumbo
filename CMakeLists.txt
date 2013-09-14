cmake_minimum_required( VERSION 2.8 )
project( TUMBO CXX )

set( TUMBO_HEADERS
    aabb.hpp
    assert.hpp
    cons.hpp
    io.hpp
    lua_binding.hpp
    lua_cons_binding.hpp
    lua_aabb_binding.hpp
    matrix.hpp
    tumbo.hpp
    utility.hpp )

install( FILES ${TUMBO_HEADERS} DESTINATION "include/tumbo" )
