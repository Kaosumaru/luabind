#include "lest_helper.hpp"
#include "luabind/LuaStackStream_Functional.hpp"

namespace 
{

	CASE("shared_ptr_tests")
	{
		using namespace luabind;
		LuaState s{ SOURCE_DIR"/shared_ptr_tests.lua" };
		s.pcall();

		using pointer = std::shared_ptr<int>;
		auto int_a = std::make_shared<int>(5);
		auto int_b = CallFunction<pointer>(s.state(), "pass_object", int_a);

		EXPECT(int_a == int_b);

		std::weak_ptr<int> weak_b = int_b;
		int_a = nullptr;
		int_b = nullptr;
		s.force_gc();
		EXPECT(weak_b.expired() == true);

		EXPECT(s.size() == 0);
	}

}
