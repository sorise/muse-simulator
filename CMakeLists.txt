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
    link_directories("/usr/local/lib")
    message(STATUS "Build in Linux OS on ${CURRENT_DATETIME} !")
    SET(OpenSSL_Include_Directory "/usr/include/openssl")
    SET(OpenSSL_Library_Directory "/usr/lib/x86_64-linux-gnu")
    set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++17 -pthread -lcrypto -lssl")
    set(OpenSSL_LIB_NAME_SSL "ssl")
    set(OpenSSL_LIB_NAME_CRYPTO "crypto")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fPIC")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fPIC")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Build in Windows OS on ${CURRENT_DATETIME} !")
    SET(OpenSSL_Include_Directory "C:\\Program Files\\OpenSSL-Win64\\include")
    SET(OpenSSL_Library_Directory "C:\\Program Files\\OpenSSL-Win64\\lib")
    SET(OpenSSL_Link_Directory "C:\\Program Files\\OpenSSL-Win64\\bin")
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
    set(OpenSSL_LIB_NAME_SSL "libssl")
    set(OpenSSL_LIB_NAME_CRYPTO "libcrypto")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fPIC")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fPIC")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
    endif()

else()
    MESSAGE(FATAL_ERROR "didnt support the ${CMAKE_SYSTEM_NAME}.")
endif()

add_subdirectory(dependency/bls)
include_directories(dependency/bls/include)
include_directories(dependency/bls/mcl/include)

#add_subdirectory(dependency/bls-signatures)
#include_directories(dependency/bls-signatures/src)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
        fmt   #要获取的依赖项的名称
        GIT_REPOSITORY "git@github.com:fmtlib/fmt.git" # Git仓库地址
        GIT_TAG 10.2.1  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        secp256k1   #要获取的依赖项的名称
        GIT_REPOSITORY "git@github.com:BlockstreamResearch/secp256k1-zkp.git" # Git仓库地址
        GIT_TAG master  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(fmt secp256k1)

###########--------------------打印下载的库信息------------------------############
MESSAGE(STATUS "Download ${fmt_SOURCE_DIR}")
MESSAGE(STATUS "Download ${fmt_BINARY_DIR}")
MESSAGE(STATUS "Download ${secp256k1_SOURCE_DIR}")
MESSAGE(STATUS "Download ${secp256k1_BINARY_DIR}")
###########--------------------打印下载的库信息------------------------############

# << 外部头文件
include_directories(${fmt_SOURCE_DIR}/include       )
include_directories(${secp256k1_SOURCE_DIR}/include )
include_directories(${OpenSSL_Include_Directory}    )

# include
include_directories(include)

MESSAGE(STATUS "${OpenSSL_Library_Directory}")
MESSAGE(STATUS "${OpenSSL_Link_Directory}")

# 库文件目录 >>
link_directories("${OpenSSL_Link_Directory}"     )
link_directories("${OpenSSL_Library_Directory}"  )
link_directories("${LevelDB_Link_Directory}"     )

find_library( openssl_Find_Result  ${OpenSSL_LIB_NAME_SSL}     REQUIRED )
find_library( crypto_Find_Result   ${OpenSSL_LIB_NAME_CRYPTO}  REQUIRED )

aux_source_directory(src/simulator/encryption simulator_encryption_list)
aux_source_directory(src/simulator/machines simulator_machines_list)
aux_source_directory(src/simulator/pool threads_pool_cpp_list)
aux_source_directory(src/simulator/serializer serializer_cpp_list)
aux_source_directory(src/simulator simulator_list)
aux_source_directory(src/simulator/utils utils_list)

add_library(simulator SHARED
    ${simulator_encryption_list}
    ${simulator_machines_list}
    ${threads_pool_cpp_list}
    ${serializer_cpp_list}
    ${simulator_list}
    ${utils_list}
)

target_link_libraries(simulator
    PUBLIC
        fmt::fmt
        bls::bls384_256
        ${OpenSSL_LIB_NAME_SSL}
        ${OpenSSL_LIB_NAME_CRYPTO}
        secp256k1
)

set(INSTALL_PROGRAM_NAME "muse-simulator")

# 指定安装目录
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    SET(CMAKE_INSTALL_PREFIX "C:/Program Files/${INSTALL_PROGRAM_NAME}")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    SET(CMAKE_INSTALL_PREFIX "/usr/local")
else(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    SET(CMAKE_INSTALL_PREFIX "/usr/local")
endif ()

# 安装头文件
install(DIRECTORY include/ DESTINATION include)
#安装目标
install(TARGETS simulator DESTINATION lib)

add_executable(runner
        main.cpp
)

target_link_libraries(runner PUBLIC simulator)