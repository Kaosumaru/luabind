#pragma once

#include <lua.hpp>
#include "LuaStackStream.hpp"
#include "StackStreamCaller.hpp"

namespace luabind
{
    class LuaState
    {
    public:
        using lua_function = int(*) (lua_State *L);
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

        template<typename T>
        void set_global(const char* name, T&& t)
        {
            stream() << std::forward<T>(t);
            lua_setglobal(_L, name);
        }

        int size()
        {
            return lua_gettop(_L);
        }

        void force_gc()
        {
            lua_gc(_L, LUA_GCCOLLECT, 0);
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
