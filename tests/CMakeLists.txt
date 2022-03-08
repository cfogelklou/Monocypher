cmake_minimum_required(VERSION 3.5)
project(test_monocypher)

set(CMAKE_CXX_STANDARD 11)
set(MC_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

include( ${MC_TOP_DIR}/LibraryFiles.cmake )

if (WIN32)
elseif (APPLE)
    set(CMAKE_BUILD_TYPE Debug)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")    
elseif (UNIX)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99")
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -fPIC -c")

file(GLOB LOCAL_SRC

    ${CMAKE_CURRENT_SOURCE_DIR}/test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
    #${CMAKE_CURRENT_SOURCE_DIR}/test-legacy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h

)

set(TEST_SRC

    ${LOCAL_SRC}

    ${MONOCYPHER_SRC}
    
    ${MC_TOP_DIR}/LibraryFiles.cmake

)

list(REMOVE_DUPLICATES TEST_SRC)

include_directories(
    ${MC_COMMON_SRC}
    ${MC_COMMON_SRC}/optional
)

add_executable(${PROJECT_NAME} ${TEST_SRC})


enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
