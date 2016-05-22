#pragma once
#include "lua.hpp"
#include <cassert>
#include <string>
#include <vector>
#include <map>



namespace luabind
{
    class LuaStackStream
    {
    public:
        LuaStackStream(lua_State* lua) : m_lua(lua) {}

        lua_State* LuaState() { return m_lua; }
    protected:
        lua_State* m_lua;
    };


    //int
    inline LuaStackStream& operator >> (LuaStackStream& ss, int& v)
    {
        auto L = ss.LuaState();
        assert(lua_isnumber(L, -1));

        v = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        return ss;
    }

    inline LuaStackStream& operator << (LuaStackStream& ss, const int& v)
    {
        lua_pushinteger(ss.LuaState(), v);
        return ss;
    }


    //string
    inline LuaStackStream& operator >> (LuaStackStream& ss, std::string& v)
    {
        auto L = ss.LuaState();
        assert(lua_isstring(L, -1));

        auto str = lua_tostring(L, -1);
        auto str_len = lua_rawlen(L, -1);

        v = std::string{ str, str_len };
        lua_pop(L, 1);
        return ss;
    }

    inline LuaStackStream& operator << (LuaStackStream& ss, const std::string& v)
    {
        lua_pushlstring(ss.LuaState(), v.c_str(), v.size());
        return ss;
    }

    //lua function
    inline LuaStackStream& operator << (LuaStackStream& ss, int(*v) (lua_State *L))
    {
        lua_pushcfunction(ss.LuaState(), v);
        return ss;
    }

    //std::vector
    template<typename T>
    LuaStackStream& operator >> (LuaStackStream& ss, std::vector<T>& v)
    {
        auto L = ss.LuaState();
        assert(lua_istable(L, -1));

        auto size = luaL_getn(L, -1);
        v.resize(size);

        for (int i = 1; i <= size; i++)
        {
            lua_rawgeti(L, -1, i);
            ss >> v[i-1];
        }
        lua_pop(L, 1);
        return ss;
    }

    template<typename T>
    LuaStackStream& operator << (LuaStackStream& ss, const std::vector<T>& v)
    {
        auto L = ss.LuaState();

        //create array
        lua_createtable(L, (int)v.size(), 0);

        for (int i = 0; i < v.size(); i++)
        {
            //push value at stack
            ss << v[i];
            //assign key-value pair to newly created array
            lua_rawseti( L, -2, i + 1 );
        }

        return ss;
    }

    //std::map
    template<typename T1, typename T2>
    LuaStackStream& operator >> (LuaStackStream& ss, std::map<T1, T2>& v)
    {
        auto L = ss.LuaState();
        assert(lua_istable(L, -1));

        lua_pushnil(L);  /* first key */
        while (lua_next(L, -2) != 0) {
            T1 key;
            T2 value;

            ss >> value;

            //copy key
            lua_pushvalue(L, -1);
            //consume copy
            ss >> key;

            v.emplace(std::move(key), std::move(value));
        }
        lua_pop(L, 1);
        return ss;
    }

    template<typename T1, typename T2>
    LuaStackStream& operator << (LuaStackStream& ss, const std::map<T1, T2>& v)
    {
        auto L = ss.LuaState();

        //create array

        //TODO if T1 is a number, perhaps we should call it with size, 0?
        lua_createtable(L, 0, (int)v.size());

        for (auto &pair : v)
        {
            //push index at stack
            ss << pair.first;
            //push value at stack
            ss << pair.second;
            //assign key-value pair to newly created array
            lua_settable( L, -3 );
        }

        return ss;
    }




    //custom struct
    namespace impl
    {
        template <typename T>
        struct has_serialize_method
        {
            struct dummy { /* something */ };

            template <typename C, typename P>
            static auto test(P * p) -> decltype(std::declval<C>().MapToLUA(*p), std::true_type());

            template <typename, typename>
            static std::false_type test(...);

            typedef decltype(test<T, dummy>(nullptr)) type;
            static const bool value = std::is_same<std::true_type, decltype(test<T, dummy>(nullptr))>::value;
        };

    }

    template<typename T, typename =
        std::enable_if_t< impl::has_serialize_method<T>::value >
    >
        LuaStackStream& operator >> (LuaStackStream& ss, T& v)
    {
        auto L = ss.LuaState();
        CLAW_ASSERT(lua_istable(L, -1));

        v.MapToLUA([&](const std::string& name, auto &v) mutable
        {
            lua_getfield(L, -1, name.c_str());
            ss >> v;
        });

        return ss;
    }

    template<typename T, typename =
        std::enable_if_t< impl::has_serialize_method<T>::value >
    >
    LuaStackStream& operator << (LuaStackStream& ss, T& v)
    {
        auto L = ss.LuaState();
        lua_createtable(L, 0, 0);

        v.MapToLUA( [&]( const std::string& name, auto &v ) mutable
        {
            ss << name;
            ss << v;
            lua_settable( L, -3 );
        } );

        return ss;
    }



};



