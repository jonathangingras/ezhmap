project(murmur3)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -O3)

add_library(murmur3 murmur3.c)

add_executable(example example.c)
target_link_libraries(example murmur3)

install(TARGETS murmur3 ARCHIVE DESTINATION lib)
install(FILES murmur3.h DESTINATION include)

if(BUILD_TESTING)
  include(CTest)
  enable_testing()
  add_executable(tests test.c)
  target_link_libraries(tests murmur3)
  add_test(all tests)
endif()
