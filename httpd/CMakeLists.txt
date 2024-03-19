#
# This file is open source software, licensed to you under the terms
# of the Apache License, Version 2.0 (the "License").  See the NOTICE file
# distributed with this work for additional information regarding copyright
# ownership.  You may not use this file except in compliance with the License.
#
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

#
# Copyright (C) 2018 Scylladb, Ltd.
#



proxy_add_app (httpd
  SOURCES

    utils/bytes.cc
    utils/base64.cc
    utils/rjson.cc
    api_handler.cc
    proxyserver.cc
    main.cc)


find_library(XXHASH_LIB xxhash)

target_include_directories(app_httpd
PUBLIC )

message("RAPIDJSON_INCLUDE_DIRS : ${RAPIDJSON_INCLUDE_DIRS}")

target_include_directories (app_httpd
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}  ${RAPIDJSON_INCLUDE_DIRS})

target_include_directories (app_httpd
PRIVATE xxHash)

link_directories(
/home/laitasso/local/mount_build/lib/
)

message("CMAKE_MODULE_PATH : ${CMAKE_MODULE_PATH}")

find_package(fmt REQUIRED)
find_package (Valgrind REQUIRED)
find_package(Seastar REQUIRED PATHS /home/laitasso/local/mount_build/lib/cmake/Seastar)

if (Seastar_FOUND)
    message("seastar")
ENDIF()

find_library(seastar NAMES seastar  PATH_SUFFIXES lib  
HINTS "/home/laitasso/local/mount_build/lib")

target_include_directories (${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    /home/laitasso/local/mount_build/include/
    /home/laitasso/local/mount_build/lib/
    /home/laitasso/local/mount_build/usr/local/include/)


message("new target: ${target}")
target_link_libraries (${target}
    LINK_PUBLIC Seastar::seastar)

#target_link_libraries(app_httpd xxHash::xxhash)


target_link_libraries (${target} ${RapidJSON_LIBRARIES})
#target_link_libraries(app_httpd xxHash::xxhash)

#add_dependencies (app_httpd app_httpd_swagger)
