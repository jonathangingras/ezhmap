project(murmur3)
cmake_minimum_required(VERSION 3.0)

add_library(murmur3 murmur3.h murmur3.c)

install(FILES murmur3.h DESTINATION include/ezhmap)
install(TARGETS murmur3
        ARCHIVE DESTINATION lib)

if(BUILD_TESTING)
  include(CTest)
  enable_testing()
  add_executable(tests test.c)
  target_link_libraries(tests murmur3)
  add_test(all tests)
endif()
