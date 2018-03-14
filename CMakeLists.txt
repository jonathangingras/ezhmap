project(ezhmap)
cmake_minimum_required(VERSION 3.0)

include(CMakeShortcuts/ExternalProjectLocal.cmake)
include(CMakeShortcuts/CTestShortcuts.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

include_directories(include)

# murmur3
ExternalProject_add_global(murmur3-target
  GIT_REPOSITORY "https://github.com/PeterScott/murmur3.git"
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/murmur3-dep.cmake" <SOURCE_DIR>/CMakeLists.txt
  CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release"
)

# sweetgreen
ExternalProject_add_local(external-sweetgreen
  GIT_REPOSITORY https://github.com/jonathangingras/sweetgreen.git
)

add_subdirectory(include)
add_subdirectory(src)
add_test_subdirectory(test)
