#ifndef TUMBO_LUA_STD_BIND_HPP
#define TUMBO_LUA_STD_BIND_HPP
#include <string>
//#include <lua.hpp>
#include <tumbo/tumbo.hpp>
#include <tumbo/lua_binding.hpp>
#include <tumbo/lua_cons_binding.hpp>
#include <tumbo/lua_aabb_binding.hpp>


namespace tumbo
    {
namespace lua
    {


    template<class T> void
    reg_std( lua_State* L, const std::string& type_prefix="" )
        {
        auto str_vec2  = type_prefix + "vec2";
        auto str_vec3  = type_prefix + "vec3";
        auto str_vec4  = type_prefix + "vec4";
        auto str_mat22 = type_prefix + "mat22";
        auto str_mat33 = type_prefix + "mat33";
        auto str_mat44 = type_prefix + "mat44";
        bind<mat22<T>>::reg(L, str_mat22);
        bind<mat33<T>>::reg(L, str_mat33);
        bind<mat44<T>>::reg(L, str_mat44);
        bind<vec2<T> >::reg(L, str_vec2);
        bind<vec3<T> >::reg(L, str_vec3);
        bind<vec4<T> >::reg(L, str_vec4);
        reg_cons<T>(L, type_prefix);
        }


    } /* namespace lua */
    } /* namespace tumbo */

#endif /* TUMBO_LUA_STD_BIND_HPP */
