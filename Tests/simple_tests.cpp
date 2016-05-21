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
    }

}
