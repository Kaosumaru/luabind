#include <iostream>
#include <stdexcept>
#include <lua.hpp>
#include "lest.hpp"
#include "luabind/luabind.hpp"

using namespace std;


#define CASE( name ) lest_CASE( specification(), name )

lest::tests& specification()
{
    static lest::tests tests;
    return tests;
}

/*
const lest::test specification[] =
{
	CASE("Simplest test")
	{
		EXPECT(1+1==2);

	},
};
*/

void init_lua()
{
    lua_State *state = luaL_newstate();
    lua_close( state );
}

int main (int argc, char * argv[])
{
	return lest::run(specification(), argc, argv);
}
