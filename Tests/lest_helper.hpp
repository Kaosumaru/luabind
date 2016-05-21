#include "lest.hpp"
#include "luabind/luabind.hpp"
#include "luabind/State.hpp"

#define CASE( name ) lest_CASE( specification(), name )
extern lest::tests & specification();
