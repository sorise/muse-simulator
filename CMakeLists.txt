cmake_minimum_required(VERSION 3.22)

project(muse-simulator
        VERSION         0.0.1
        DESCRIPTION     "分布式算法仿真软件"
        HOMEPAGE_URL    "https://github.com/sorise/muse-simulator"
        LANGUAGES       C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

#制定目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    #解决 visual studio 编码问题,暂时不支持 windows
    if (WIN32)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    endif ()
    if (CMAKE_GENERATOR MATCHES "Visual Studio")
        message(STATUS "Visual Studio compiler detected.")
    else()
        message(STATUS "Non-Visual Studio compiler detected.")
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

else()
    MESSAGE(FATAL_ERROR "didnt support the ${CMAKE_SYSTEM_NAME}.")
endif()

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
        muse_timer   #要获取的依赖项的名称
        GIT_REPOSITORY "git@github.com:sorise/muse-timer.git" # Git仓库地址
        GIT_TAG main  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        fmt   #要获取的依赖项的名称
        GIT_REPOSITORY "git@github.com:fmtlib/fmt.git" # Git仓库地址
        GIT_TAG 10.2.1  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(muse_timer fmt)

if (NOT ${muse_timer_POPULATED})
    MESSAGE(FATAL_ERROR "Fail to get muse-timer! ${muse_timer_POPULATED}")
endif ()

if (NOT ${fmt_POPULATED})
    MESSAGE(FATAL_ERROR "Fail to get muse-timer! ${fmt_POPULATED}")
endif ()

MESSAGE(STATUS "Download ${muse_timer_SOURCE_DIR}")
MESSAGE(STATUS "Download ${muse_timer_BINARY_DIR}")
MESSAGE(STATUS "Download ${fmt_SOURCE_DIR}")
MESSAGE(STATUS "Download ${fmt_BINARY_DIR}")


include_directories(${muse_timer_SOURCE_DIR}/include)
include_directories(${fmt_SOURCE_DIR}/include)

#include
include_directories(include)

add_library(simulator SHARED
    src/simulator/machines/computer.cpp
    src/simulator/machines/host.cpp
)

target_link_libraries(simulator PUBLIC fmt museTimer)

add_executable(runner main.cpp)

target_link_libraries(runner PUBLIC simulator)

