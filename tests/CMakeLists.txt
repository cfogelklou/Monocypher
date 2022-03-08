cmake_minimum_required(VERSION 3.5)
project(test_monocypher)

set(CMAKE_CXX_STANDARD 11)
set(MC_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(MC_COMMON_SRC ${MC_TOP_DIR}/src)



# USE recursive search to add all libsodium files.
file(GLOB_RECURSE MONOCYPHER_REC_SRC
  ${MC_COMMON_SRC}/*.c
)

set(MONOCYPHER_SRC
    ${MONOCYPHER_REC_SRC}
)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -fPIC -O3")

file(GLOB TWEETNACL_SRC
  ${MC_TOP_DIR}/tests/externals/tweetnacl/*.c
  ${MC_TOP_DIR}/tests/externals/tweetnacl/*.h
)

file(GLOB C25519_SRC
  ${MC_TOP_DIR}/tests/externals/c25519/*.c
  ${MC_TOP_DIR}/tests/externals/c25519/*.h
)

file(GLOB ED25519_DONNA_SRC
  ${MC_TOP_DIR}/tests/externals/ed25519-donna/*.c
  ${MC_TOP_DIR}/tests/externals/ed25519-donna/*.h
)

file(GLOB LOCAL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

set(TEST_SRC
    ${EXTERNALS_SRC}
    ${LOCAL_SRC}
    ${MONOCYPHER_SRC}
)

include_directories(
    ${MC_COMMON_SRC}
    ${MC_COMMON_SRC}/optional
    ${MC_TOP_DIR}/tests
    ${MC_TOP_DIR}/tests/externals/tweetnacl
)

include(${CMAKE_CURRENT_SOURCE_DIR}/FindSodium.cmake)

# Standard Monocypher Test
add_executable(test-test ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/test.c)
add_test(test-test test-test)

# Legacy Monocypher Test
add_executable(test-legacy ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/test-legacy.c)
add_test(test-legacy test-legacy)

# TIS CI Test
add_executable(test-tis-ci ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/tis-ci.c)
add_test(test-tis-ci test-tis-ci)

# CT Grind Test
add_executable(test-ctgrind ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/ctgrind.c)
add_test(test-ctgrind test-ctgrind)

# Tweet NACL
add_executable(
    test-tweetnacl 
    ${TEST_SRC}
    ${TWEETNACL_SRC} 
    ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed-tweetnacl.c
    )
target_link_libraries(test-tweetnacl sodium)
add_test(test-tweetnacl test-tweetnacl)

# Sodium
add_executable(test-sodium ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed-sodium.c)
target_link_libraries(test-sodium sodium)
add_test(test-sodium test-sodium)

# C25519
add_executable(test-c25519 
    ${TEST_SRC} 
    ${C25519_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed-c25519.c)
target_include_directories(test-c25519 PUBLIC ${MC_TOP_DIR}/tests/externals/c25519)
add_test(test-c25519 test-c25519)


if (LINUX)
else (LINUX)
# Speed Test
add_executable(test-speed ${TEST_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed.c)
add_test(test-speed test-speed)
endif (LINUX)

if (LINUX)
else (LINUX)
# ED25519 Donna
add_definitions("-DUSE_MONOCYPHER
    -DED25519_CUSTOMHASH
    -DED25519_TEST
    -DED25519_NO_INLINE_ASM
    -DED25519_FORCE_32BIT")
add_executable(test-donna 
    ${TEST_SRC} 
    ${ED25519_DONNA_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/speed/speed-donna.c)
target_include_directories(test-donna PUBLIC
    ${MC_TOP_DIR}/tests/externals/ed25519-donna
)
add_test(test-donna test-donna)
endif (LINUX)


enable_testing()

