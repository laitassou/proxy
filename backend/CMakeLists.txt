cmake_minimum_required (VERSION 3.13)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set (CMAKE_BUILD_TYPE "Debug")
set (CMAKE_CXX_STANDARD
  "20"
  CACHE
  STRING
  "C++ standard to build with.")

  add_library(backend SHARED connector.cc repository.cc )

