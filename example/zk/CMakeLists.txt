add_executable(zk_test zk_test.cpp)
set_target_properties(zk_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
# target_include_directories(zk_test PRIVATE  ../)

add_executable(zk_test2 zk_test2.cpp)
set_target_properties(zk_test2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})