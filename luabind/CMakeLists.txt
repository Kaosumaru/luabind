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
set(luabind_Includes ${PROJECT_SOURCE_DIR} PARENT_SCOPE)

if (MSVC)
add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4018 /wd4005 /wd4996 /nologo" )
endif(MSVC)

##folders
add_folder(luabind)


add_library(luabind ${PROJECT_SRCS})
