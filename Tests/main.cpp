#include <iostream>
#include <stdexcept>
#include <lua.hpp>

#ifndef _MSC_VER
#define lest_FEATURE_COLOURISE 1
#endif

#include "lest.hpp"
#include "luabind/luabind.hpp"

using namespace std;


const lest::test specification[] =
{
	CASE("Simplest test")
	{
		EXPECT(1+1==2);

	},
};

void init_lua()
{
    lua_State *state = luaL_newstate();
    lua_close( state );
}

int main (int argc, char * argv[])
{
	return lest::run(specification, argc, argv);
}
