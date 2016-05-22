#pragma once

#include <lua.hpp>
#include "State.hpp"
#include <vector>

namespace luabind
{


    template<typename T>
    class MetatableCreator
    {
    public:
        using lua_function = int(*) (lua_State *L);
        using FunctionsList = std::vector<std::pair<const char*, lua_function>>;

        MetatableCreator() {}
 


        static void* table_index() { return (void *)&_registry_table_index; }

        static void AssignMetatable(lua_State *L)
        {
            PushOurTable(L);
            lua_setmetatable(L, -2);
        }

        static bool CheckMetatable(lua_State *L)
        {
            lua_getmetatable(L, -1);
            PushOurTable(L);
            bool result = lua_compare(L, -1, -2, LUA_OPEQ) != 0;
            lua_pop(L, 2);
            return result;
        }

        static void AddMetamethod(const char* name, lua_function f)
        {
            s_metamethods.emplace_back(name, f);
        }

        static void AddMethod(const char* name, lua_function f)
        {
            s_methods.emplace_back({ name, f });
        }

        static bool ShouldInit()
        {
            static bool initialized = false;
            auto ret = initialized;
            initialized = true;
            return !ret;
        }
    protected:
        template<typename Field>
        static void SetField(lua_State *L, const char *name, const Field& f)
        {
            //table
            PushOurTable(L);

            //index
            lua_pushstring(L, name);

            //value
            LuaStackStream ss{ L };
            ss << f;

            //assign and pop table
            lua_settable(L, -3);
            lua_pop(L, 1);
        }

        //creates our array in registry
        static void CreateOurTable(lua_State *L)
        {
            lua_pushlightuserdata(L, table_index());
            lua_createtable(L, 0, 0);

            lua_settable(L, LUA_REGISTRYINDEX);

            auto gc_wrapper = LB_MAKE_LUA_WRAPPER([](T *t) { t->~T(); });
            SetField(L, "__gc", gc_wrapper);

            for(auto& m : s_metamethods)
                SetField(L, m.first, m.second);
        }

        //pushes our array at stack
        static void PushOurTable(lua_State *L)
        {
            lua_pushlightuserdata(L, table_index());
            auto type = lua_gettable(L, LUA_REGISTRYINDEX);

            if (type == LUA_TNIL)
            {
                lua_pop(L, 1);
                CreateOurTable(L);
                PushOurTable(L);
            }
        }

        const static int _registry_table_index = 0;

        static FunctionsList s_metamethods;
        static FunctionsList s_methods;
    };

    template<typename T>
    typename MetatableCreator<T>::FunctionsList MetatableCreator<T>::s_metamethods;

    template<typename T>
    typename MetatableCreator<T>::FunctionsList MetatableCreator<T>::s_methods;
}
