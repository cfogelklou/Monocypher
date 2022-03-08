
set(MC_COMMON_SRC ${MC_TOP_DIR}/src)

#defines we need to build libsodium
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  #For MSVC less than or equal to 10.0, "inline" doesn't exist.
  if (MSVC_VERSION)
    if (MSVC_VERSION LESS 1601)
      add_definitions(-Dinline=__inline)
    endif ()
  endif ()
  add_definitions("/wd4146 /wd4244 /wd4996 -D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
endif ()

if (WIN32)
    #set(FREERTOS_PORT 1)
    add_definitions(-DWIN32)
elseif (APPLE)
    add_definitions(-DAPPLE=1 -D__APPLE__=1 -DTARGET_OS_OSX=1)
elseif (UNIX)
    add_definitions(-DNATIVE_LITTLE_ENDIAN)
elseif (EMSCRIPTEN)
    add_definitions(-DNATIVE_LITTLE_ENDIAN)    
endif ()

if (EMSCRIPTEN)
  add_definitions(-DEMSCRIPTEN -D__EMSCRIPTEN__)
endif(EMSCRIPTEN)




# USE recursive search to add all libsodium files.
file(GLOB_RECURSE MONOCYPHER_REC_SRC
  ${MC_COMMON_SRC}/*.c
)

set(MONOCYPHER_SRC
    ${MONOCYPHER_REC_SRC}

)

list(REMOVE_DUPLICATES MONOCYPHER_SRC)
