#pragma once

#include <lua.hpp>
#include "State.hpp"
#include <map>
#include <set>
#include <memory>

namespace luabind
{
	template<typename T>
	class MetatableCreator
	{
	public:
		template<typename O>
		friend class MetatableCreator;

		using lua_function = int(*) (lua_State *L);
		using FunctionsList = std::map<const char*, lua_function>;

		MetatableCreator() {}
 


		static void* table_index() { return (void *)&_registry_table_index; }

		static void AssignMetatable(lua_State *L)
		{
			//assign metatable to obj
			PushOurTable(L);
			lua_setmetatable(L, -2);

			//assign light userdata to obj
			lua_pushlightuserdata(L, (void*)&_registry_table_index);
			lua_setuservalue(L, -2);
		}

		static bool CheckMetatable(lua_State *L)
		{
			lua_getuservalue(L, -1);
			auto pointer = (void*)lua_topointer(L, -1);
			lua_pop(L, 1);

			if (pointer == table_index())
				return true;

			if (_inheritance_children_ids.find(pointer) != _inheritance_children_ids.end())
				return true;

			return false;
		}

		static void AddMetamethod(const char* name, lua_function f)
		{
			s_metamethods.emplace( name, f );
		}

		static void AddMethod(const char* name, lua_function f)
		{
			s_methods.emplace( name, f );
		}

		static bool ShouldInit()
		{
			static bool initialized = false;
			auto ret = initialized;
			initialized = true;
			return !ret;
		}

		template<typename ParentCreator>
		static void InheritFrom()
		{

			for (auto& method : ParentCreator::s_methods)
				s_methods.insert(method);

			_parent_AddChildID.insert(&ParentCreator::AddChildID);
			ParentCreator::AddChildID(table_index());
		}
	protected:
		static void AddChildID(void* id)
		{
			_inheritance_children_ids.insert(id);

			for (auto addChildId : _parent_AddChildID)
				addChildId(id);
		}

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

			if (!s_methods.empty())
				SetField(L, "__index", s_methods);
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
		static std::set<void*> _inheritance_children_ids;
		static std::set<void(*)(void*)> _parent_AddChildID;

		static FunctionsList s_metamethods;
		static FunctionsList s_methods;
	};

	template<typename T>
	struct RegisterObject
	{
		using creator = MetatableCreator<std::shared_ptr<T>>;
		using lua_function = int(*) (lua_State *L);

		auto& method(const char* name, lua_function f)
		{
			creator::AddMethod(name, f);
			return *this;
		}

		template<typename ParentType>
		auto& parent()
		{
			static_assert(std::is_base_of<ParentType, T>::value, "ParentType isn't parent of registered object");
			using parent_creator = MetatableCreator<std::shared_ptr<ParentType>>;
			creator::InheritFrom<parent_creator>();
			
			return *this;
		}
	};

	template<typename T>
	typename std::set<void(*)(void*)> MetatableCreator<T>::_parent_AddChildID;

	template<typename T>
	typename std::set<void*> MetatableCreator<T>::_inheritance_children_ids;

	template<typename T>
	typename MetatableCreator<T>::FunctionsList MetatableCreator<T>::s_metamethods;

	template<typename T>
	typename MetatableCreator<T>::FunctionsList MetatableCreator<T>::s_methods;
}
