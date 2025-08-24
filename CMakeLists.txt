cmake_minimum_required(VERSION 3.0)

project(mrpc)

set(CXX_FLAGS
 -O2
 -g
#  -Wall
 -Wextra
#  -Werror
 -std=c++11
 -pthread
)

# FLAGS
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# OUTPUT
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# INCLUDE
include_directories(
  ${PROJECT_SOURCE_DIR}/src/proto
  ${PROJECT_SOURCE_DIR}/src/include
  /usr/local/include
  /usr/local/include/zookeeper
  /usr/include
)

# LIBRARIES
link_directories(
    /usr/local/lib
    /usr/lib
)

add_subdirectory(src)
add_subdirectory(example)
