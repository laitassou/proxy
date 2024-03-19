cmake_minimum_required (VERSION 3.13)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set (CMAKE_BUILD_TYPE "Debug")
set (CMAKE_CXX_STANDARD
  "20"
  CACHE
  STRING
  "C++ standard to build with.")


set (Seastar_SCHEDULING_GROUPS_COUNT
  "16"
  CACHE
  STRING
  "A positive number to set Seastar's reactor number of allowed different scheduling groups.")

if (NOT Seastar_SCHEDULING_GROUPS_COUNT MATCHES "^[1-9][0-9]*")
  message(FATAL_ERROR "Seastar_SCHEDULING_GROUPS_COUNT must be a positive number (${Seastar_SCHEDULING_GROUPS_COUNT})")
endif()

project(proxyserver)
message("current src dir ${CMAKE_CURRENT_SOURCE_DIR}")


list (APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_BINARY_DIR} 
  "/home/laitasso/local/mount_build/usr/local/lib/cmake"
  "/home/laitasso/local/mount_build/lib/cmake")


message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

find_package(RapidJSON REQUIRED)


find_path(xxHash_INCLUDE_DIR NAME xxhash.h PATH_SUFFIXES include
HINTS "/home/laitasso/local/mount_build/usr/local/include")
find_library(xxhash_LIBRARY NAMES xxhash  PATH_SUFFIXES lib  
HINTS "/home/laitasso/local/mount_build/usr/local/lib")

find_library(seastar seastar 
HINTS "/home/laitasso/local/mount_build/lib")

find_package(xxHash 0.7 CONFIG REQUIRED PATHS /home/laitasso/local/mount_build/usr/local/lib/cmake/xxHash/)

IF (xxHash_FOUND)
    message("found: ${xxHash_INCLUDE_DIR}")
ENDIF()
# Logical target for all applications.
add_custom_target (apps)
macro (proxy_add_app name)
  set (args ${ARGN})

cmake_parse_arguments (
parsed_args
""
""
"SOURCES"
${args})

message("RAPIDJSON_INCLUDE_DIRS: ${RAPIDJSON_INCLUDE_DIRS}")

set(xxhash_LIBRARIES ${xxhash_LIBRARY})
set(xxhash_INCLUDE_DIRS ${xxHash_INCLUDE_DIR})

message("main xxhash_INCLUDE_DIR: ${xxHash_INCLUDE_DIR} ${xxhash_LIBRARY}")

message("name: ${name} ${target}")

  set (target app_${name})
  add_executable (${target} ${parsed_args_SOURCES})

  target_compile_definitions (${target}
  PUBLIC
    SEASTAR_SCHEDULING_GROUPS_COUNT=${Seastar_SCHEDULING_GROUPS_COUNT}
    )

target_include_directories (${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    /home/laitasso/local/mount_build/include/
    /home/laitasso/local/mount_build/usr/local/include/
    /home/laitasso/local/mount_build/lib/)

target_include_directories (${target}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}  ${RAPIDJSON_INCLUDE_DIRS})

link_directories(
    /home/laitasso/local/mount_build/lib/
    )

#message("target: ${target}")
#target_link_libraries (${target}
#    PUBLIC  seastar )

target_link_libraries (${target}
    PRIVATE  xxHash::xxhash)


target_link_libraries (${target} PRIVATE ${RapidJSON_LIBRARIES})

  set_target_properties (${target}
    PROPERTIES
      OUTPUT_NAME ${name})

add_dependencies (apps ${target})
endmacro ()



add_subdirectory (httpd)

