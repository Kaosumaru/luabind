#include "lest_helper.hpp"
#include "luabind/LuaStackStream_Functional.hpp"

namespace
{
	template<auto x>
	auto test_obj()
	{
		return [](lua_State* L) {
			luabind::LuaStackStream ss {L};
			return CallFromStreamResultToStream(ss, x);
		};
	}


	CASE("object_tests")
	{
		struct TestObj
		{
			int return_5()
			{
				return 5;
			}

			int v = 123;
		};
		using pointer = std::shared_ptr<TestObj>;

		auto return_v = LB_MAKE_LUA_WRAPPER([](const pointer& s) { return s->v; });


		luabind::RegisterObject<TestObj>()
			.method<&return_v>("return_v")
			.method<&TestObj::return_5>("return_5");

		using namespace luabind;
		LuaState s{ SOURCE_DIR"/object_tests.lua" };
		s.pcall();

		auto obj = std::make_shared<TestObj>();

		EXPECT(CallFunction<int>(s.state(), "call_member", obj, "return_v") == 123);
		EXPECT(CallFunction<int>(s.state(), "call_member", obj, "return_5") == 5);

		EXPECT(s.size() == 0);
	}

	CASE("inheritance_tests")
	{
		struct TestObj
		{
			int return_5()
			{
				return 5;
			}

			int v = 123;
		};
		struct TestObj2 : public TestObj
		{
			int return_7()
			{
				return 7;
			}
		};
		struct TestObj3 : public TestObj2
		{
			int return_8()
			{
				return 8;
			}
		};

		using pointer = std::shared_ptr<TestObj>;

		auto return_v = LB_MAKE_LUA_WRAPPER([](const pointer& s) { return s->v; });

		luabind::RegisterObject<TestObj>()
			.method("return_v", return_v)
			.method<&TestObj::return_5>("return_5");

		luabind::RegisterObject<TestObj2>()
			.parent<TestObj>()
			.method<&TestObj2::return_7>("return_7");

		luabind::RegisterObject<TestObj3>()
			.parent<TestObj2>()
			.method<&TestObj3::return_8>("return_8");

		using namespace luabind;
		LuaState s{ SOURCE_DIR"/object_tests.lua" };
		s.pcall();

		auto obj2 = std::make_shared<TestObj2>();
		EXPECT(CallFunction<int>(s.state(), "call_member", obj2, "return_v") == 123);
		EXPECT(CallFunction<int>(s.state(), "call_member", obj2, "return_5") == 5);


		auto obj3 = std::make_shared<TestObj3>();
		EXPECT(CallFunction<int>(s.state(), "call_member", obj3, "return_5") == 5);


		EXPECT(s.size() == 0);
	}

}
