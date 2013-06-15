#ifndef TUMBO_BIND_MATRIX_HPP
#define TUMBO_BIND_MATRIX_HPP


#include <lua.hpp>
#include <typeindex>
#include <map>
#include <string>
#include <sstream>
#include "tumbo.hpp"
#include "io.hpp"


/* Call tumbo::lua::reg<T>( lua_State*, std::string type_name ) on the
    matrix type T you wish to register. */

namespace tumbo
    {
namespace lua
    {
    template<class T>
    struct bind
        {
        static const char* NAME;

        static T*
        lua_cast( lua_State* L, int index );

        static T*
        push( lua_State* L );

        static int
        create( lua_State* L );

        static int
        add( lua_State* L );

        static int
        unary_minus( lua_State* L );

        static int
        sub( lua_State* L );

        static int
        mat_index( lua_State* L );

        static int
        mat_call( lua_State* L );

        static int
        tostr( lua_State* L );

        static void
        reg( lua_State* L, const char* name );
        };

    template<class T> const char* bind<T>::NAME = "undef";

    // Attempts to cast userdata to the given type.
    // Returns a pointer to the userdata with the correct type. null on failure
    template<class T> T*
    bind<T>::lua_cast( lua_State* L, int index )
        {
        T* ptr = nullptr;
        lua_getmetatable( L, index );
        lua_getfield( L, LUA_REGISTRYINDEX, NAME );
        if( lua_rawequal( L, -1, -2 ) )
            ptr = static_cast<T*>( lua_touserdata( L, index ) );

        lua_pop( L, 2 );
        return ptr;
        }


    /* Pushes the registered type T onto the lua stack. Does not initialize. */
    template<class T> T*
    bind<T>::push( lua_State* L )
        {
        void* userdata = lua_newuserdata( L, sizeof(T) );
        luaL_setmetatable( L, NAME );
        return static_cast<T*>(userdata);
        }


    /* Lua constructor for matrix T. */
    template<class T> int
    bind<T>::create( lua_State* L )
        {
        int argc = lua_gettop(L);
        /* Default construct. */
        if( argc == 0 )
            {
            new (push(L)) T();
            return 1;
            }
        /* Must provide all elements of the matrix. */
        if( argc == (int)T::size() )
            {
            typename T::scalar_t data[ T::size() ];
            for( size_t i=1; i < T::size()+1; ++i )
                data[i-1] = static_cast<typename T::scalar_t>(
                    luaL_checknumber( L, i ) );

            new (push(L)) T(data, data+T::size());
            return 1;
            }
        return luaL_error(L, "Bad argument count for constructor." );
        }


    template<class T> int
    bind<T>::add( lua_State* L )
        {
        int n = lua_gettop( L ); // passed arguments
        if( n != 2 )
            luaL_error(L, "bad argument count");

        T* a = static_cast<T*>( luaL_checkudata(L, 1, NAME) );
        T* b = static_cast<T*>( luaL_checkudata(L, 2, NAME) );
        if( a == nullptr || b == nullptr )
            luaL_error(L, "bad argument types");

        *push(L) = *a + *b;
        return 1;
        }


    template<class T> int
    bind<T>::unary_minus( lua_State* L )
        {
        int n = lua_gettop( L );
        if( n != 1 )
            luaL_error(L, "bad argument count");

        T* a = static_cast<T*>( luaL_checkudata(L, 1, NAME) );
        if( a == nullptr )
            luaL_error(L, "bad argument types");

        *push(L) = - (*a);
        return 1;
        }


    template<class T> int
    bind<T>::sub( lua_State* L )
        {
        int n = lua_gettop( L ); // passed arguments
        if( n != 2 )
            luaL_error(L, "bad argument count");
        T* a = static_cast<T*>( luaL_checkudata(L, 1, NAME) );
        T* b = static_cast<T*>( luaL_checkudata(L, 2, NAME) );
        if( a == nullptr || b == nullptr )
            luaL_error(L, "bad argument types");

        *push(L) = *a - *b;
        return 1;
        }


    inline int
    mul( lua_State* L )
        {
        int n = lua_gettop( L );
        if( n != 2 )
            luaL_error(L, "bad argument count");

        fmat44* a = static_cast<fmat44*>( luaL_checkudata(L, 1, "mat44" ) );
        fvec4* b_v = bind<fvec4>::lua_cast(L,2);
        fmat44* b_m = bind<fmat44>::lua_cast(L,2);

        if( a == nullptr || (b_v == nullptr && b_m == nullptr) )
            luaL_error(L, "bad argument types");

        if( b_v )
            {
            *bind<fvec4>::push(L) = (*a) * (*b_v);
            return 1;
            }
        else if( b_m )
            {
            *bind<fmat44>::push(L) = (*a) * (*b_m);
            return 1;
            }
        return 0;
        }


    template<class T> int
    bind<T>::mat_index( lua_State* L )
        {
        int isnum = 0;
        T* A = lua_cast(L,1);
        int i = lua_tointegerx(L,2,&isnum);
        if( isnum == 0 )
            return luaL_error(L,"Invalid index type.");
        if( i < (int)T::size() )
            {
            lua_pushnumber( L, (*A)[i] );
            return 1;
            }
        return luaL_error(L, "Index out of bounds.");
        }


    template<class T> int
    bind<T>::mat_call( lua_State* L )
        {
        if( lua_gettop(L) != 2 )
            return luaL_error(L,"Matrix type table access takes two integers");
        T* A = lua_cast(L,1);
        int i_num=0,j_num=0;
        int i = lua_tointegerx(L,1,&i_num);
        int j = lua_tointegerx(L,2,&j_num);
        if( i_num == 0 || j_num == 0 )
            return luaL_error(L,"Invalid types for matrix table access");
        if( i < (int)T::height() && j < (int)T::width() )
            {
            lua_pushnumber( L, (*A)(i,j) );
            return 1;
            }
        return luaL_error(L, "Index out of bounds." );
        }


    template<class T> int
    bind<T>::tostr( lua_State* L )
        {
        auto v = static_cast<T*>( luaL_checkudata(L, 1, NAME) );
        std::stringstream ss;
        ss << *v;
        std::string str = ss.str();
        lua_pushstring( L, str.c_str() );
        return 1;
        }


    template<class T> void
    bind<T>::reg( lua_State* L, const char* name )
        {
        NAME = name;

        //int methods, metatable;

        luaL_Reg meta[] =
            {
            { "__add", add },
            { "__unm", unary_minus },
            { "__sub", sub },
            { "__mul", mul },
            { "__index", mat_index },
            { "__call", mat_call },
            { "__tostring", tostr },
            { NULL, NULL }
            };
        luaL_Reg meth[] =
            {
            { "create", create },
            { NULL, NULL }
            };
        // Put a new table on the stack and retrieve its index.
        luaL_newmetatable( L, NAME );
        //metatable = lua_gettop( L );
        luaL_setfuncs( L, meta, 0 );

        lua_newtable( L );
        luaL_setfuncs( L, meth, 0 );
        lua_setglobal( L, NAME );
        lua_pop(L,1); // Remove the metatable.
        }


    } // namespace lua
    } // namespace tumbo

#endif // TUMBO_BIND_MATRIX_HPP
