#include "lest_helper.hpp"
#include "luabind/LuaStackStream_Functional.hpp"

namespace 
{
    int pass_7(int a)
    {
        return 7;
    }

    CASE("callbacks_tests")
    {
        using namespace luabind;
        LuaState s{ SOURCE_DIR"/callbacks_tests.lua" };
        s.pcall();

        EXPECT(CallFunction<int>(s.state(), "call_with_5", LB_MAKE_LUA_WRAPPER(pass_7)) == 7);


        int return_value = 0;
        std::function<int(int)> callback = [&](int a) { return_value = a; return 7; };

        EXPECT(CallFunction<int>(s.state(), "call_with_5", callback) == 7);
        EXPECT(return_value == 5);


        EXPECT(s.size() == 0);
    }

    CASE("pass std::function")
    {
        using namespace luabind;
        LuaState s{ SOURCE_DIR"/callbacks_tests.lua" };
        s.pcall();

        using function = std::function<int(int)>;
        function f_a = [](int a) { return a * 2; };

        auto f_b = CallFunction<function>(s.state(), "pass_object", f_a);

        EXPECT(f_b(5) == 10);


        EXPECT(s.size() == 0);
    }

}
