#include "lest_helper.hpp"

namespace 
{

    CASE("A passing test" "[pass]")
    {
         EXPECT(42 == 42);
    }

}
