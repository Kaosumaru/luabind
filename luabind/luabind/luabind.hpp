#pragma once

#include "lua.hpp"
#include "LuaStackStream.hpp"
#include "StackStreamCaller.hpp"

namespace luabind
{
    template<typename ReturnType, typename ...Args>
    ReturnType CallFunction(lua_State* lua, const char* function_name, Args&&... args)
    {
        LuaStackStream ss{ lua };

        lua_getglobal(lua, function_name);

        auto fill_args = { 0,
            ((ss << args), 0)...
        };

        static constexpr int arity = sizeof...(args);
        lua_call(lua, arity, 1);

        ReturnType r;
        ss >> r;
        return r;
    }

};

#define LB_MAKE_LUA_WRAPPER(x) \
[](lua_State* L) { \
    luabind::LuaStackStream ss{ L }; \
    ss << CallFromStream(ss, x); \
    return 1; \
}




