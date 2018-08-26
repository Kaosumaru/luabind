#pragma once

#include "lua.hpp"
#include "LuaStackStream.hpp"
#include "StackStreamCaller.hpp"
#include "mem_fn_shared_ptr.hpp"

namespace luabind
{
	template<typename ReturnType, typename ...Args>
	ReturnType CallFunction(lua_State* lua, const char* function_name, Args&&... args)
	{
		LuaStackStream ss{ lua };

		int result = lua_getglobal(lua, function_name);

		// fill lua stack with arguments
		static constexpr int arity = sizeof...(args);
		(void(ss << args), ...);

		auto return_arity = std::is_void_v<ReturnType> ? 0 : 1;
		lua_call(lua, arity, return_arity);

		//return expected value, if not void
		if constexpr (!std::is_void_v<ReturnType>)
		{
			ReturnType r;
			ss >> r;
			return r;
		}
	}
};

#define LB_MAKE_LUA_WRAPPER(x) \
[](lua_State* L) { \
	luabind::LuaStackStream ss{ L }; \
	return CallFromStreamResultToStream(ss, x); \
}

#define LB_WRAP_MEMBER(x) \
[](lua_State* L) { \
	luabind::LuaStackStream ss{ L }; \
	return CallFromStreamResultToStream(ss, mtl::mem_fn_shared_ptr(x)); \
}

