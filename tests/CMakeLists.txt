cmake_minimum_required(VERSION 3.5)
project(test_monocypher)

set(CMAKE_CXX_STANDARD 11)
set(MC_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

include( ${MC_TOP_DIR}/LibraryFiles.cmake )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -fPIC")

file(GLOB LOCAL_SRC

    #${CMAKE_CURRENT_SOURCE_DIR}/test.c
    #${CMAKE_CURRENT_SOURCE_DIR}/test-legacy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h

)

set(TEST_SRC
    ${LOCAL_SRC}
    ${MONOCYPHER_SRC}
    ${MC_TOP_DIR}/LibraryFiles.cmake
)


include_directories(
    ${MC_COMMON_SRC}
    ${MC_COMMON_SRC}/optional
)

add_executable(test-test ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/test.c)
add_executable(test-legacy ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/test-legacy.c)
add_executable(test-tis-ci ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/tis-ci.c)
add_executable(test-ctgrind ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/ctgrind.c)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
