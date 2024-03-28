cmake_minimum_required (VERSION 3.13)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set (CMAKE_BUILD_TYPE "Debug")
set (CMAKE_CXX_STANDARD
  "20"
  CACHE
  STRING
  "C++ standard to build with.")


link_directories(
    $ENV{HOME}/local/mount_build/lib/
)

find_package(fmt REQUIRED PATHS $ENV{HOME}/local/mount_build/lib/cmake/fmt)


add_library(backend SHARED connector.cc repository.cc fdb_connector.cc)

message("home $ENV{HOME}")
target_include_directories (backend
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  $ENV{HOME}/.local/usr/include
  $ENV{HOME}/local/mount_build/include/
  $ENV{HOME}/local/mount_build/lib/
  $ENV{HOME}/local/mount_build/usr/local/include/)

