find_package(Boost COMPONENTS program_options REQUIRED)
find_package(SimGrid REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${SimGrid_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src/protobufs/cpp ${CMAKE_CURRENT_SOURCE_DIR} ${Protobuf_INCLUDE_DIRS})

file(GLOB CPP_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB CPP_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB PROTO_CPP_HEADERS ${CMAKE_SOURCE_DIR}/src/protobufs/cpp/*.pb.h)
file(GLOB PROTO_CPP_SOURCE ${CMAKE_SOURCE_DIR}/src/protobufs/cpp/*.pb.cc)

message("${PROTO_CPP_HEADERS} ${PROTO_CPP_SOURCE}")
message("${CPP_SOURCE} ${CPP_HEADER}")

add_executable(
        simulator
        ${CPP_SOURCE}
        ${CPP_HEADER}
        ${PROTO_CPP_SOURCE}
        ${PROTO_CPP_HEADERS}
)

message(${Protobuf_LIBRARIES})

target_link_libraries(
        simulator boost_program_options ${SimGrid_LIBRARY} ${Protobuf_LIBRARIES}
)