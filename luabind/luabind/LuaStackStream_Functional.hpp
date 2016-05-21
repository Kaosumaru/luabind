#pragma once
#include "LuaStackStream.hpp"
#include "StackStreamCaller.hpp"
#include <functional>


namespace luabind
{
    //lua function
    LuaStackStream& operator << (LuaStackStream& ss, int(*v) (lua_State *L))
    {
        lua_pushcfunction(ss.LuaState(), v);
        return ss;
    }

    //std::function
    template<typename T>
    LuaStackStream& operator << (LuaStackStream& ss, const std::function<T> &v)
    {
        //auto wrapper = LB_MAKE_LUA_WRAPPER(v);
        //lua_pushcfunction(ss.LuaState(), wrapper);
        return ss;
    }




};



