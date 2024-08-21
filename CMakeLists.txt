cmake_minimum_required(VERSION 3.28)
project(TOTPManager LANGUAGES C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/global-flags.cmake")

find_package(OpenSSL CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(TOTPManager ${SOURCES})

target_link_libraries(TOTPManager PRIVATE OpenSSL::SSL)
target_link_libraries(TOTPManager PRIVATE OpenSSL::Crypto)
