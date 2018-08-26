#pragma once
#include "LuaStackStream.hpp"
#include "StackStreamCaller.hpp"
#include <functional>

#include "MetatableCreator.hpp"
#include <memory>

namespace luabind
{
	template<typename T>
	void push_object(LuaStackStream& ss, const T &v)
	{
		auto L = ss.LuaState();

		void* address = lua_newuserdata(L, sizeof(T));
		new (address) T{ v };

		MetatableCreator<T>::AssignMetatable(L);
	}

	template<typename T>
	void pop_object(LuaStackStream& ss, T &v)
	{
		auto L = ss.LuaState();
		if (!lua_isuserdata(L, -1) || !MetatableCreator<T>::CheckMetatable(L))
		{
			assert(false);
			return;
		}
		v = *(T*)lua_touserdata(L, -1);
		lua_pop(L, 1);
	}

	template<typename T>
	void pop_object_pointer(LuaStackStream& ss, T *&v)
	{
		auto L = ss.LuaState();
		if (!lua_isuserdata(L, -1) || !MetatableCreator<T>::CheckMetatable(L))
		{
			assert(false);
			return;
		}
		v = (T*)lua_touserdata(L, -1);
		lua_pop(L, 1);
	}

	//std::function
	template<typename R, typename ...Args>
	LuaStackStream& operator << (LuaStackStream& ss, const std::function<R(Args...)> &v)
	{
		using function = std::function<R(Args...)>;
		using creator = MetatableCreator<function>;

		if (creator::ShouldInit())
		{
			struct Caller
			{
				static auto Call(function* s, Args... args)
				{
					return (*s)(std::forward<Args>(args)...);
				}
			};

			creator::AddMetamethod("__call", LB_MAKE_LUA_WRAPPER(Caller::Call));
		}

		push_object<function>(ss, v);
		return ss;
	}

	template<typename T>
	LuaStackStream& operator >> (LuaStackStream& ss, std::function<T> &v)
	{
		using function = std::function<T>;
		pop_object<function>(ss, v);
		return ss;
	}

	template<typename T>
	LuaStackStream& operator >> (LuaStackStream& ss, std::function<T> *&v)
	{
		using function = std::function<T>;
		pop_object_pointer<function>(ss, v);
		return ss;
	}


	//std::shared_ptr
	template<typename T>
	LuaStackStream& operator >> (LuaStackStream& ss, std::shared_ptr<T> &v)
	{
		using pointer = std::shared_ptr<T>;
		pop_object<pointer>(ss, v);
		return ss;
	}

	template<typename T>
	LuaStackStream& operator >> (LuaStackStream& ss, std::shared_ptr<T> *&v)
	{
		using pointer = std::shared_ptr<T>;
		pop_object_pointer<pointer>(ss, v);
		return ss;
	}

	template<typename T>
	LuaStackStream& operator << (LuaStackStream& ss, const std::shared_ptr<T> &v)
	{
		using pointer = std::shared_ptr<T>;
		push_object<pointer>(ss, v);
		return ss;
	}

};



