cmake_minimum_required(VERSION 3.5)
project(colored_text)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        source/main.cpp
        Makefile
        source/string_utils.cpp
        source/string_utils.h source/vector_utils.cpp source/vector_utils.h source/console_debug.cpp source/console_debug.h)

add_executable(colored_text ${SOURCE_FILES})


set(CMAKE_SYSTEM_NAME Generic)

include(CMakeForceCompiler)
# Prefix detection only works with compiler id "GNU"
# CMake will look for prefixed g++, cpp, ld, etc. automatically
set(DEVKITPRO "$ENV{DEVKITPRO}")
set(DEVKITARM "$ENV{DEVKITARM}")

SET(CMAKE_FIND_ROOT_PATH /opt/devkitPro/devkitARM /opt/devkitPro/libctru )

CMAKE_FORCE_C_COMPILER("/opt/devkitPro/devkitARM/bin/arm-none-eabi-gcc" GNU)
CMAKE_FORCE_CXX_COMPILER("/opt/devkitPro/devkitARM/bin/arm-none-eabi-g++" GNU)

include_directories(/opt/devkitPro/devkitARM/include /opt/devkitPro/libctru/include)
link_directories(/opt/devkitPro/devkitARM/lib /opt/devkitPro/libctru/lib)

set(WITH_PORTLIBS ON CACHE BOOL "use portlibs ?")

if(WITH_PORTLIBS)
    include_directories(/opt/devkitPro/portlibs/3ds/include)
    link_directories(/opt/devkitPro/portlibs/3ds/lib)
    set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} /opt/devkitPro/portlibs)
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

SET(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs not available" )
SET(DCMAKE_EXE_LINKER_FLAGS "-static")

set(ARCH "-march=armv6k -mtune=mpcore -mfloat-abi=hard ")
set(CMAKE_C_FLAGS "-DARM11 -D_3DS -fomit-frame-pointer -ffast-math -mword-relocations ${ARCH}" CACHE STRING "C flags" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}  -fno-rtti -fno-exceptions -std=gnu++11" CACHE STRING "CXX flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-specs=3dsx.specs ${ARCH}" CACHE STRING "linker flags" FORCE)
