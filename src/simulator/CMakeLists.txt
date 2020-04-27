find_package(Boost COMPONENTS program_options REQUIRED)
find_package(SimGrid REQUIRED)
find_package(Protobuf REQUIRED)
find_package(yaml-cpp REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${SimGrid_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src/protobufs/cpp ${CMAKE_CURRENT_SOURCE_DIR} ${Protobuf_INCLUDE_DIRS}  ${YAML_CPP_INCLUDE_DIR})

file(GLOB CPP_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB CPP_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB PROTO_CPP_HEADERS ${CMAKE_SOURCE_DIR}/src/protobufs/cpp/*.pb.h)
file(GLOB PROTO_CPP_SOURCE ${CMAKE_SOURCE_DIR}/src/protobufs/cpp/*.pb.cc)

message("${PROTO_CPP_HEADERS} ${PROTO_CPP_SOURCE}")
message("${CPP_SOURCE} ${CPP_HEADER}")

# add_executable(
#         simulator
#         ${CPP_SOURCE}
#         ${CPP_HEADER}
#         ${PROTO_CPP_SOURCE}
#         ${PROTO_CPP_HEADERS}
# )


# target_link_libraries(
#         simulator boost_program_options ${SimGrid_LIBRARY} ${Protobuf_LIBRARIES}
# )

add_executable(
        simple_simulator
        ${CMAKE_CURRENT_SOURCE_DIR}/simple.cpp
)
target_link_libraries(
        simple_simulator ${SimGrid_LIBRARY} boost_program_options ${YAML_CPP_LIBRARIES} stdc++fs
)
