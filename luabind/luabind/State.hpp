#pragma once

#include <lua.hpp>
#include "LuaStackStream.hpp"
#include "StackStreamCaller.hpp"

namespace luabind
{
    class LuaState
    {
    public:
        LuaState()
        {
            _L = luaL_newstate();
            luaL_openlibs(_L);
        }

        LuaState(const std::string& path) : LuaState()
        {
            load(path);
        }

        ~LuaState()
        {
            lua_close(_L);
        }

        auto stream() { return LuaStackStream{ _L }; }
        auto state() { return _L; }

        void load(const std::string& path)
        {
            if (luaL_loadfile(_L, path.c_str()) != LUA_OK)
                report_error();
        }

        void pcall()
        {
            if (lua_pcall(_L, 0, LUA_MULTRET, 0) != LUA_OK)
                report_error();
        }
    protected:
        void report_error()
        {
            std::string message;
            stream() >> message;
            throw std::runtime_error{ message };
        }

        lua_State *_L;
    };
}
