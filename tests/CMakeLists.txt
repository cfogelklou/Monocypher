cmake_minimum_required(VERSION 3.5)
project(test_monocypher)

set(CMAKE_CXX_STANDARD 11)
set(MC_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

include( ${MC_TOP_DIR}/LibraryFiles.cmake )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -fPIC -O3")

file(GLOB TWEETNACL_SRC
  ${MC_TOP_DIR}/tests/externals/tweetnacl/*.c
  ${MC_TOP_DIR}/tests/externals/tweetnacl/*.h
)

file(GLOB LOCAL_SRC

    #${CMAKE_CURRENT_SOURCE_DIR}/test.c
    #${CMAKE_CURRENT_SOURCE_DIR}/test-legacy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h

)

set(TEST_SRC
    ${EXTERNALS_SRC}
    ${LOCAL_SRC}
    ${MONOCYPHER_SRC}
    ${MC_TOP_DIR}/LibraryFiles.cmake
)


include_directories(
    ${MC_COMMON_SRC}
    ${MC_COMMON_SRC}/optional
    ${MC_TOP_DIR}/tests
    ${MC_TOP_DIR}/tests/externals/tweetnacl
)

include(${CMAKE_CURRENT_SOURCE_DIR}/FindSodium.cmake)


add_executable(test-test ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/test.c)
add_executable(test-legacy ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/test-legacy.c)
add_executable(test-tis-ci ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/tis-ci.c)
add_executable(test-ctgrind ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/ctgrind.c)
add_executable(test-speed ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed.c)

add_executable(
    test-tweetnacl 
    ${TEST_SRC}
    ${TWEETNACL_SRC} 
    ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed-tweetnacl.c
    )
target_link_libraries(test-tweetnacl sodium)
add_executable(test-sodium ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed-sodium.c)
target_link_libraries(test-sodium sodium)
#find_package(sodium REQUIRED)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
