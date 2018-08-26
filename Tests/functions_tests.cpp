#include "lest_helper.hpp"
#include "luabind/LuaStackStream_Functional.hpp"

namespace 
{

	CASE("functions_tests")
	{
		using namespace luabind;
		LuaState s{ SOURCE_DIR"/functions_tests.lua" };
		s.pcall();

		auto return_5 = LB_MAKE_LUA_WRAPPER([]() { return 5; });
		s.set_global("return_5", return_5);

		EXPECT(CallFunction<int>(s.state(), "expects_return_5") == 5);

		int x = 0;
		std::function<void(int)> func = [&](int y) { x = y; };
		s.set_global("set_x", func);
		CallFunction<void>(s.state(), "set_x", 666);
		EXPECT(x == 666);

		EXPECT(s.size() == 0);
	}

}
