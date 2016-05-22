#pragma once

#include "lua.hpp"
#include "LuaStackStream.hpp"
#include "StackStreamCaller.hpp"

namespace luabind
{
    namespace impl
    {
        template<typename ReturnType>
        struct ReturnInfo
        {
            static constexpr int arity = 1;
            static ReturnType get(LuaStackStream &ss)
            {
                ReturnType r;
                ss >> r;
                return r;
            }
        };

        template<>
        struct ReturnInfo<void>
        {
            static constexpr int arity = 0;
            static void get(LuaStackStream &ss) { }
        };

    }

    template<typename ReturnType, typename ...Args>
    ReturnType CallFunction(lua_State* lua, const char* function_name, Args&&... args)
    {
        LuaStackStream ss{ lua };

        int result = lua_getglobal(lua, function_name);

        auto fill_args = { 0,
            ((ss << args), 0)...
        };

        using ReturnInfo = impl::ReturnInfo<ReturnType>;
        static constexpr int arity = sizeof...(args);

        lua_call(lua, arity, ReturnInfo::arity);

        return ReturnInfo::get(ss);
    }
};

#define LB_MAKE_LUA_WRAPPER(x) \
[](lua_State* L) { \
    luabind::LuaStackStream ss{ L }; \
    return CallFromStreamResultToStream(ss, x); \
}

