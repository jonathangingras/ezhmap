
add_executable(hash_table_test hash_table_test.c)
target_link_libraries(hash_table_test ezhmap)
add_test(hash_table_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hash_table_test)
