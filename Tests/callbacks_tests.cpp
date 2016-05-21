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

#if 0
        int return_value = 0;
        std::function<int(int)> callback = [&](int a) { return_value = a; return 7; };

        EXPECT(CallFunction<int>(s.state(), "call_with_5", LB_MAKE_LUA_WRAPPERcallback) == 7);
        EXPECT(return_value == 5);
#endif
    }

}
