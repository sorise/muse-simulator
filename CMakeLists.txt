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

FetchContent_Declare(
        secp256k1   #要获取的依赖项的名称
        GIT_REPOSITORY "git@github.com:BlockstreamResearch/secp256k1-zkp.git" # Git仓库地址
        GIT_TAG master  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(muse_timer fmt secp256k1)

if (NOT ${muse_timer_POPULATED})
    MESSAGE(FATAL_ERROR "Fail to get muse-timer! ${muse_timer_POPULATED}")
endif ()

if (NOT ${fmt_POPULATED})
    MESSAGE(FATAL_ERROR "Fail to get fmt! ${fmt_POPULATED}")
endif ()

if (NOT ${secp256k1_POPULATED})
    MESSAGE(FATAL_ERROR "Fail to get libsecp256k1! ${secp256k1_POPULATED}")
endif ()

MESSAGE(STATUS "Download ${muse_timer_SOURCE_DIR}")
MESSAGE(STATUS "Download ${muse_timer_BINARY_DIR}")

MESSAGE(STATUS "Download ${fmt_SOURCE_DIR}")
MESSAGE(STATUS "Download ${fmt_BINARY_DIR}")

MESSAGE(STATUS "Download ${secp256k1_SOURCE_DIR}")
MESSAGE(STATUS "Download ${secp256k1_BINARY_DIR}")

# << 外部头文件
include_directories(${muse_timer_SOURCE_DIR}/include)
include_directories(${fmt_SOURCE_DIR}/include)
include_directories(${secp256k1_SOURCE_DIR}/include)
include_directories(${OpenSSL_Include_Directory})

# include
include_directories(include)

MESSAGE(STATUS "${OpenSSL_Library_Directory}")

find_library(openssl_Find_Result ${OpenSSL_LIB_NAME_SSL}  VERSION 3 REQUIRED)
find_library(crypto_Find_Result ${OpenSSL_LIB_NAME_CRYPTO} VERSION 3 REQUIRED)

#openssl 不在就G
if (openssl_Find_Result)
    MESSAGE("-- find openssl-ssl in ${openssl_Find_Result} ----- OK!")
else ()
    MESSAGE(FATAL_ERROR "-- didn't find openssl-ssl ----- ERROR!")
endif ()

#crypto 不在就G
if (crypto_Find_Result)
    MESSAGE("-- find openssl-crypto in ${crypto_Find_Result} ----- OK!")
else ()
    MESSAGE(FATAL_ERROR "-- didn't find openssl-crypto ----- ERROR!")
endif ()

# 库文件 >>
link_directories("${OpenSSL_Link_Directory}")
link_directories("${OpenSSL_Library_Directory}")

add_library(simulator SHARED
        src/simulator/machines/computer.cpp
        src/simulator/machines/host.cpp
        src/simulator/db/block.cpp
        src/simulator/encryption/bitcoin_secp256k1.cpp
        src/simulator/encryption/ecc_secp256k1.cpp
        src/simulator/encryption/ecc_sm2_handler.cpp
        src/simulator/encryption/encryption.cpp
        src/simulator/encryption/hash_handler.cpp
        src/simulator/encryption/rsa_handler.cpp
        src/simulator/simulator.cpp
        src/simulator/network_dispatcher.cpp
        src/simulator/consensus/stack_calculator.cpp
        src/simulator/machines/response_data.cpp
        src/simulator/machines/rpc_response_header.cpp
        src/simulator/machines/transmitter_event.cpp
        src/simulator/machines/synchronous_registry.cpp
        src/simulator/machines/simulator_timer.cpp
        src/simulator/machines/network_card.cpp
        src/simulator/machines/message.cpp
        src/utils/singleton.cpp
        include/serializer/binaryDeserializeView.cpp
        include/serializer/binarySerializer.cpp
        include/serializer/util.cpp

)

target_link_libraries(simulator PUBLIC fmt museTimer libssl libcrypto secp256k1)

add_executable(runner main.cpp)

target_link_libraries(runner PUBLIC simulator)