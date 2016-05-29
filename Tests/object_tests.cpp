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
            .method("return_5", LB_WRAP_MEMBER(&TestObj::return_5));

        luabind::RegisterObject<TestObj2>()
            .parent<TestObj>()
            .method("return_7", LB_WRAP_MEMBER(&TestObj2::return_7));

        luabind::RegisterObject<TestObj3>()
            .parent<TestObj2>()
            .method("return_8", LB_WRAP_MEMBER(&TestObj3::return_8));

        using namespace luabind;
        LuaState s{ SOURCE_DIR"/object_tests.lua" };
        s.pcall();

        auto obj2 = std::make_shared<TestObj2>();
        EXPECT(CallFunction<int>(s.state(), "call_member", obj2, "return_v") == 123);
        EXPECT(CallFunction<int>(s.state(), "call_member", obj2, "return_5") == 5);

#if 0
        //TODO
        auto obj3 = std::make_shared<TestObj3>();
        EXPECT(CallFunction<int>(s.state(), "call_member", obj3, "return_5") == 5);
#endif

        EXPECT(s.size() == 0);
    }

}
