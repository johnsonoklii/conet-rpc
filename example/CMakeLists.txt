set(BIN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)

link_libraries(mrpc protobuf)

add_subdirectory(callee)
add_subdirectory(caller)
add_subdirectory(zk)

# file(GLOB_RECURSE SRCS *.cpp)
# foreach(SRC_FILE ${SRCS})
#     get_filename_component(BASE_NAME ${SRC_FILE} NAME_WE)  
#     add_executable(${BASE_NAME} ${SRC_FILE})
#     set_target_properties(${BASE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
# endforeach()
