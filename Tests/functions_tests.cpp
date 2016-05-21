#include "lest_helper.hpp"

namespace 
{

    CASE("functions_tests")
    {
        using namespace luabind;
        LuaState s{ SOURCE_DIR"/functions_tests.lua" };
        s.pcall();

        auto return_5 = LB_MAKE_LUA_WRAPPER([]() { return 5; });
        s.register_global_function("return_5", return_5);

        EXPECT(CallFunction<int>(s.state(), "expects_return_5") == 5);

    }

}
