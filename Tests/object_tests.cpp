#include "lest_helper.hpp"
#include "luabind/LuaStackStream_Functional.hpp"

namespace
{


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
            .method("return_v", return_v)
            .method("return_5", LB_WRAP_MEMBER(&TestObj::return_5));

        using namespace luabind;
        LuaState s{ SOURCE_DIR"/object_tests.lua" };
        s.pcall();

        auto obj = std::make_shared<TestObj>();

        EXPECT(CallFunction<int>(s.state(), "call_member", obj, "return_v") == 123);
        EXPECT(CallFunction<int>(s.state(), "call_member", obj, "return_5") == 5);

        EXPECT(s.size() == 0);
    }

}
