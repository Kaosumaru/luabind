#include <iostream>
#include <stdexcept>

#ifndef _MSC_VER
#define lest_FEATURE_COLOURISE 1
#endif

#include "lest.hpp"
#include "luabind/StackStreamCaller.hpp"

using namespace std;


const lest::test specification[] =
{
	CASE("Simplest test")
	{
		EXPECT(1+1==2);

	},
};

int main (int argc, char * argv[])
{
	return lest::run(specification, argc, argv);
}
