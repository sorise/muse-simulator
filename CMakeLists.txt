cmake_minimum_required(VERSION 3.22)

project(muse-simulator
        VERSION         0.0.1
        DESCRIPTION     "分布式算法仿真软件"
        HOMEPAGE_URL    "https://github.com/sorise/muse-simulator"
        LANGUAGES       C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#解决 visual studio 编码问题,暂时不支持 windows

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (WIN32)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    endif ()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

else()
    MESSAGE(FATAL_ERROR "didnt support the ${CMAKE_SYSTEM_NAME}.")
endif()


add_executable(muse_simulator main.cpp)
