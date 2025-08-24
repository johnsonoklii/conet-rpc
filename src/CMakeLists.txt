file(GLOB_RECURSE SRC_SRCS ${PROJECT_SOURCE_DIR}/src/ *.cpp )
# proto
file(GLOB_RECURSE PROTO_SRCS ${PROJECT_SOURCE_DIR}/src/proto *.cc )

list(APPEND SRCS ${SRC_SRCS} ${PROTO_SRCS})

# ADD_LIBRARY(lemon STATIC ${SRCS})
ADD_LIBRARY(mrpc ${SRCS})
target_link_libraries(mrpc conet protobuf pthread zookeeper_mt)



