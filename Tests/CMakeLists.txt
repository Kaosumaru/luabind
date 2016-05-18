cmake_minimum_required (VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

project (Tests)

if (MSVC)
add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4018 /wd4005 /wd4996 /nologo" )
endif(MSVC)

add_subdirectory(../luabind "${CMAKE_CURRENT_BINARY_DIR}/luabind")
include_directories ("${luabind_Includes}")

add_executable (Tests main.cpp)
target_link_libraries(Tests ${luabind_Deps})
#target_link_libraries (Tests RPN)
