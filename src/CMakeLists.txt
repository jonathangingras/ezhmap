add_library(ezhmap
  ../include/ezhmap/hash_table.h
  hash_table.c)

add_dependencies(ezhmap murmur3-target)

target_link_libraries(ezhmap murmur3)

install(TARGETS ezhmap DESTINATION lib)
