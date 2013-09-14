#ifndef TUMBO_LUA_CONS_BINDING_HPP
#define TUMBO_LUA_CONS_BINDING_HPP


#include <tumbo/lua_binding.hpp>

namespace tumbo
    {
namespace lua
    {
    const char* CONS_ARG_ERROR = "%s constructor given wrong arguments. (%d)";

    template<class T> int
    cons_translation( lua_State* L )
        {
        const int max_args = 3;
        int argc = lua_gettop(L);
        T argv[max_args];
        for( size_t i=0; i < argc; ++i )
            argv[i] = luaL_checknumber(L, i+1);

        if( argc == 3 )
            new (bind<matrix<T,4,4>>::push(L)) matrix<T,4,4>(
                translation<T>( vec<T,3>{argv} ) );
        else if( argc == 2 )
            new (bind<matrix<T,3,3>>::push(L)) matrix<T,3,3>(
                translation<T>( vec<T,2>{argv[0], argv[1]} ) );
        else return luaL_error(L, CONS_ARG_ERROR, "Translation", argc );
        return 1;
        }


    template<class T> int
    cons_rotation( lua_State* L )
        {
        const int max_args = 4;
        int argc = lua_gettop(L);
        T args[max_args];
        for( size_t i=0; i < argc; ++i )
            args[i] = luaL_checknumber(L, i+1);

        if( argc == 4 )
            new (bind<matrix<T,4,4>>::push(L)) matrix<T,4,4>(
                rotation<T>( args[0], args[1], args[2], args[3] ) );
        else if( argc == 1 )
            new (bind<matrix<T,3,3>>::push(L)) matrix<T,3,3>(
                rotation<T>( args[0] ) );
        else return luaL_error(L, CONS_ARG_ERROR, "Rotation", argc );

        return 1;
        }


    template<class T> int
    cons_scaling( lua_State* L )
        {
        const int max_args = 3;
        int argc = lua_gettop(L);
        T args[max_args];
        for( size_t i=0; i < argc; ++i )
            args[i] = luaL_checknumber(L, i+1);

        if( argc == 3 )
            new (bind<matrix<T,4,4>>::push(L)) matrix<T,4,4>(
                scaling( vec<T,3>{ args } ) );
        else if( argc == 2 )
            new (bind<matrix<T,3,3>>::push(L)) matrix<T,3,3>(
                scaling( vec<T,2>{ args[0], args[1] } ) );
        else return luaL_error(L, CONS_ARG_ERROR, "Scaling", argc );

        return 1;
        }


    template<class T> int
    cons_ortho( lua_State* L )
        {
        int argc = lua_gettop(L);
        if( argc != 6 )
            return luaL_error(L, CONS_ARG_ERROR, "Ortho", argc );

        new (bind<matrix<T,4,4>>::push(L)) matrix<T,4,4>(
            ortho<T>(
                luaL_checknumber(L, 1),
                luaL_checknumber(L, 2),
                luaL_checknumber(L, 3),
                luaL_checknumber(L, 4),
                luaL_checknumber(L, 5),
                luaL_checknumber(L, 6) ) );
        return 1;
        }

    template<class T> void
    reg_cons( lua_State* L )
        {
        lua_register(L, "translation", cons_translation<T>);
        lua_register(L, "rotation", cons_rotation<T>);
        lua_register(L, "scaling", cons_scaling<T>);
        lua_register(L, "ortho", cons_ortho<T>);
        }

    } /* namespace lua */
    } /* namespace tumbo */

#endif // TUMBO_LUA_CONS_BINDING_HPP
