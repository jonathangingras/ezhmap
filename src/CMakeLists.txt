add_library(ezhmap hash_table.c)

target_link_libraries(ezhmap murmur3)

install(TARGETS murmur3 ezhmap DESTINATION lib)
