#include <iostream>
#include <stdexcept>
#include <lua.hpp>
#include "lest.hpp"
#include "luabind/luabind.hpp"

using namespace std;

lest::tests& specification()
{
	static lest::tests tests;
	return tests;
}

int main (int argc, char * argv[])
{
	return lest::run(specification(), argc, argv);
}
