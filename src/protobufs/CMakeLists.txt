file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src/morpheus/proto)

file(GLOB PROTO_SOURCE src/*.proto)

message("command: protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/cpp       -I ${CMAKE_CURRENT_SOURCE_DIR}/src ${PROTO_SOURCE}")
message("command: protoc --python_out=${CMAKE_CURRENT_SOURCE_DIR}/python -I ${CMAKE_CURRENT_SOURCE_DIR}/src ${PROTO_SOURCE}")

execute_process(
        COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/cpp -I ${CMAKE_CURRENT_SOURCE_DIR}/src ${PROTO_SOURCE}
)

execute_process(
        COMMAND protoc --python_out=${CMAKE_CURRENT_SOURCE_DIR}/python -I ${CMAKE_CURRENT_SOURCE_DIR}/src ${PROTO_SOURCE}
)

file(GLOB PROTO_CPP_HEADERS cpp/*.h)
file(GLOB PROTO_CPP_SOURCE cpp/*.cc)
