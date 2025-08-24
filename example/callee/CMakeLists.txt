add_executable(provider provider.cpp userservice.cpp ../user.pb.cc)
set_target_properties(provider PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
target_include_directories(provider PRIVATE ../)