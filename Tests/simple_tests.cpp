#include "lest_helper.hpp"

namespace 
{

	CASE("simple_tests")
	{
		using namespace luabind;
		LuaState s{ SOURCE_DIR"/simple_tests.lua" };
		s.pcall();
		
		EXPECT( CallFunction<int>(s.state(), "return_5") == 5);

		CallFunction<void>(s.state(), "set_global_a", 2);
		EXPECT(CallFunction<int>(s.state(), "get_global_a") == 2);

		s.set_global("global_a", 55);
		EXPECT(CallFunction<int>(s.state(), "get_global_a") == 55);

		EXPECT(s.size() == 0);
	}

}
