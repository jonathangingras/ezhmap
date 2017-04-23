project(ezhmap)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
include_directories(include)

include(CMakeShortcuts/ExternalProjectLocal.cmake)
include(CMakeShortcuts/CTestShortcuts.cmake)

# murmur3
ExternalProject_add_local(external-murmur3
  GIT_REPOSITORY "https://github.com/PeterScott/murmur3.git"
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/murmur3-dep.cmake" <SOURCE_DIR>/CMakeLists.txt
)
# sweetgreen
ExternalProject_add_local(external-sweetgreen
  GIT_REPOSITORY https://github.com/jonathangingras/sweetgreen.git
)

add_subdirectory(src)
add_test_subdirectory(test)
