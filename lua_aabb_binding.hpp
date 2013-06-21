#ifndef TUMBO_AABB_LUA_BINDING_HPP
#define TUMBO_AABB_LUA_BINDING_HPP

#include "lua_binding.hpp"
#include "aabb.hpp"

namespace tumbo
    {
namespace lua
    {

    template<class T>
    struct bind_aabb
        {};

    template<class T, size_t D>
    struct bind_aabb<aabb<T,D>> : public bind<aabb<T,D>>
        {
        static int
        contains( lua_State* L );

        static int
        overlaps( lua_State* L );

        static int
        dimensions( lua_State* L );

        static int
        center( lua_State* L );

        static int
        translate( lua_State* L );

        static int
        place( lua_State* L );

        static int
        volume( lua_State* L );

        static void
        reg( lua_State* L, const char* name );
        };


    template<class T, size_t D> int
    bind_aabb<aabb<T,D>>::contains( lua_State* L )
        {
        bool result;
        auto A = bind<aabb<T,D>>::lua_check(L,1);

        if( auto B = bind<aabb<T,D>>::lua_cast(L,2) )
            result = ::tumbo::contains( *A, *B );
        else if( auto P = bind<vec<T,D>>::lua_cast(L,2) )
            result = ::tumbo::contains( *A, *P );
        else luaL_error(L, "Bad type for argument 2");

        lua_pushboolean(L, result);
        return 1;
        }


    template<class T, size_t D> int
    bind_aabb<aabb<T,D>>::overlaps( lua_State* L )
        {
        auto A = bind<aabb<T,D>>::lua_check(L, 1);
        auto B = bind<aabb<T,D>>::lua_check(L, 2);
        lua_pushboolean(L,
            ::tumbo::overlaps(*A,*B) );
        return 1;
        }


    template<class T, size_t D> int
    bind_aabb<aabb<T,D>>::dimensions( lua_State* L )
        {
        auto self = bind<aabb<T,D>>::lua_check(L,1);
        auto v = bind<vec<T,D>>::push(L);
        *v = ::tumbo::dimensions(*self);
        return 1;
        }


    template<class T, size_t D> int
    bind_aabb<aabb<T,D>>::center( lua_State* L )
        {
        auto self = bind<aabb<T,D>>::lua_check(L,1);
        auto v = bind<vec<T,D>>::push(L);
        *v = ::tumbo::center(*self);
        return 1;
        }


    template<class T, size_t D> int
    bind_aabb<aabb<T,D>>::translate( lua_State* L )
        {
        auto self = bind<aabb<T,D>>::lua_check(L,1);
        auto v = bind<vec<T,D>>::lua_check(L,2);
        auto result = bind<aabb<T,D>>::push(L);
        *result = ::tumbo::translate(*self,*v);
        return 1;
        }


    template<class T, size_t D> int
    bind_aabb<aabb<T,D>>::place( lua_State* L )
        {
        auto self = bind<aabb<T,D>>::lua_check(L,1);
        auto v = bind<vec<T,D>>::lua_check(L,2);
        auto result = bind<aabb<T,D>>::push(L);
        *result = ::tumbo::place(*self,*v);
        return 1;
        }


    template<class T, size_t D> int
    bind_aabb<aabb<T,D>>::volume( lua_State* L )
        {
        auto self = bind<aabb<T,D>>::lua_check(L,1);
        auto d = ::tumbo::volume(*self);
        lua_pushnumber(L, d);
        return 1;
        }


    template<class T, size_t D> void
    bind_aabb<aabb<T,D>>::reg( lua_State* L, const char* name )
        {
        TUMBO_LUA_STACKASSERT(L,0);
        /* Register the underlying type as normal. */
        bind<aabb<T,D>>::reg(L, name);

        int metatable;
        luaL_Reg aabb_methods[] =
            {
            {"contains",contains},
            {"overlaps",overlaps},
            {"dimensions",dimensions},
            {"center",center},
            {"translate",translate},
            {"place",place},
            {"volume",volume},
            {NULL,NULL}
            };

        /* Add additional methods to the method table. */
        luaL_getmetatable(L, (bind<aabb<T,D>>::NAME));
        metatable = lua_gettop(L);
        /* Get the method table from the metatable. */
        lua_pushstring(L, "methods");
        lua_rawget(L, metatable);

        luaL_setfuncs(L, aabb_methods, 0);
        lua_pop(L,2);
        }

    } // namespace lua
    } // namespace tumbo


#endif // TUMBO_AABB_LUA_BINDING_HPP
