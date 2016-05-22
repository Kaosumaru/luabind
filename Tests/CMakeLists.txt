cmake_minimum_required (VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

project (Tests)

if (MSVC)
	add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4018 /wd4005 /wd4996 /nologo" )
else (MSVC)

endif(MSVC)

add_definitions( "/Dlest_FEATURE_COLOURISE" )
add_definitions( "/Dlest_FEATURE_AUTO_REGISTER" )
add_definitions( "/DSOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"" )

add_subdirectory(../luabind "${CMAKE_CURRENT_BINARY_DIR}/luabind")
include_directories ("${luabind_Includes}")

add_executable (Tests main.cpp simple_tests.cpp callbacks_tests.cpp functions_tests.cpp shared_ptr_tests.cpp object_tests.cpp lest_helper.hpp)
target_link_libraries(Tests ${luabind_Deps})
#target_link_libraries (Tests RPN)
