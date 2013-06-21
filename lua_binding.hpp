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

    /* Check that the lua stack is of the correct size upon leaving
        the function. */
    class stackassert
        {
        public:
            stackassert( lua_State* L, int sizechange) :
                L(L)
                {
                stacksize = lua_gettop(L) + sizechange;
                }

            ~stackassert()
                {
                assert( lua_gettop(L) == stacksize );
                }

        private:
            lua_State* L;
            int stacksize;
        };

    #ifndef NDEBUG
    #define TUMBO_LUA_STACKASSERT( L, sizechange )\
        stackassert stackguard((L), (sizechange));
    #else
    #define TUMBO_LUA_STACKASSERT( L, sizechange )
    #endif


    template<class T>
    struct bind
        {
        static const char* NAME;

        static T*
        lua_cast( lua_State* L, int index );

        static T*
        lua_check( lua_State* L, int index );

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
        mat_assign_index( lua_State* L );

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
        TUMBO_LUA_STACKASSERT(L,0);
        T* ptr = nullptr;
        lua_getmetatable( L, index );
        lua_getfield( L, LUA_REGISTRYINDEX, NAME );
        if( lua_rawequal( L, -1, -2 ) )
            ptr = static_cast<T*>( lua_touserdata( L, index ) );

        lua_pop( L, 2 );
        return ptr;
        }


    template<class T> T*
    bind<T>::lua_check( lua_State* L, int index )
        {
        TUMBO_LUA_STACKASSERT(L,0);
        auto ptr = lua_cast(L,index);
        if( ptr )
            return ptr;
        else
            {
            luaL_error(L,
                "Bad type for argument %d: Type %s required",
                index, NAME );
            return nullptr;
            }
        }


    /* Pushes the registered type T onto the lua stack. Does not initialize. */
    template<class T> T*
    bind<T>::push( lua_State* L )
        {
        TUMBO_LUA_STACKASSERT(L,1);
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
        auto A = lua_check(L,1);
        int i = lua_tointegerx(L,2,&isnum);
        if( isnum == 0 )
            {
            /* Get methods table. */
            lua_getmetatable(L,1);
            lua_pushstring(L, "methods");
            lua_rawget(L,-2);
            /* copy the index value. */
            lua_pushvalue(L,2);
            /* Get the value in the methods table. */
            lua_gettable(L,-2);
            return 1;
            //return luaL_error(L,"Invalid index type.");
            }
        if( i < (int)T::size() )
            {
            lua_pushnumber( L, (*A)[i] );
            return 1;
            }
        return luaL_error(L, "Index out of bounds.");
        }


    template<class T> int
    bind<T>::mat_assign_index( lua_State* L )
        {
        auto A = lua_check(L,1);
        auto k = luaL_checknumber(L,2);
        auto v = luaL_checknumber(L,3);
        (*A)[k] = static_cast<typename T::scalar_t>(v);
        return 0;
        }


    template<class T> int
    bind<T>::mat_call( lua_State* L )
        {
        T* A = lua_check(L,1);
        int i_num=0,j_num=0;
        int i = lua_tointegerx(L,2,&i_num);
        int j = lua_tointegerx(L,3,&j_num);
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
        TUMBO_LUA_STACKASSERT(L,0);
        NAME = name;

        luaL_Reg meta[] =
            {
            { "__add", add },
            { "__unm", unary_minus },
            { "__sub", sub },
            { "__mul", mul },
            { "__call", mat_call },
            { "__tostring", tostr },
            { "__index", mat_index },
            { "__newindex", mat_assign_index },
            { NULL, NULL }
            };
        luaL_Reg meth[] =
            {
            { NULL, NULL }
            };
        int metatable, methodtable;
        /* Create the metatable. */
        luaL_newmetatable( L, NAME );
        metatable = lua_gettop(L);
        luaL_setfuncs( L, meta, 0 );

        /* Create methods table. */
        lua_newtable( L );
        methodtable = lua_gettop(L);
        luaL_setfuncs( L, meth, 0 );
        /* Set the method table as the meta __index table. */
        lua_pushstring(L, "methods");
        lua_pushvalue(L, methodtable);
        lua_rawset(L, metatable);

        /* Make the constructor available as a global. */
        lua_register(L, NAME, create);
        /* Remove the metatable. */
        lua_pop(L,2);
        }


    } // namespace lua
    } // namespace tumbo

#endif // TUMBO_BIND_MATRIX_HPP
