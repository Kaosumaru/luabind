cmake_minimum_required (VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

macro (add_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND PROJECT_SRCS "${_relPath}/${_src}")
        else()
            list (APPEND PROJECT_SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate PROJECT_SRCS to parent directory
        set (PROJECT_SRCS ${PROJECT_SRCS} PARENT_SCOPE)
    endif()
endmacro()

macro (add_folder name)
  add_subdirectory(${name})
  string(REPLACE "/" "\\" filtername ${name})
  source_group(${filtername} FILES ${files})
endmacro()

project (luabind)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Modules CACHE TYPE STRING FORCE)

IF(NOT LUA_INCLUDE_DIR)
  MESSAGE(FATAL_ERROR "LUA_INCLUDE_DIR not set")
ENDIF(NOT LUA_INCLUDE_DIR)
IF(NOT LUA_LIBRARY)
  MESSAGE(FATAL_ERROR "LUA_LIBRARY not set")
ENDIF(NOT LUA_LIBRARY)

include_directories( ${LUA_INCLUDE_DIR} )
set(luabind_Includes ${PROJECT_SOURCE_DIR} ${LUA_INCLUDE_DIR} PARENT_SCOPE)
set(luabind_Deps ${LUA_LIBRARY} PARENT_SCOPE)
#set(LUA_DIR ${PROJECT_SOURCE_DIR}/../lua/ )
#include("${LUA_DIR}/CMakeLists.txt")
#include(FindLua)

if (MSVC)
add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4018 /wd4005 /wd4996 /nologo" )
endif(MSVC)

##folders
add_folder(luabind)

add_library(luabind ${PROJECT_SRCS})
target_link_libraries(luabind ${LUA_LIBRARY})
