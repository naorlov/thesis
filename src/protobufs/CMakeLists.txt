file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src/morpheus/proto)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpp )
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python)
file(GLOB PROTO_SOURCE src/*.proto)

foreach(protofile ${PROTO_SOURCE})
        message("Compiling protobuf ${protofile}")
        execute_process(
                COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/cpp -I ${CMAKE_CURRENT_SOURCE_DIR}/src ${protofile}
        )

        execute_process(
                COMMAND protoc --python_out=${CMAKE_CURRENT_SOURCE_DIR}/python -I ${CMAKE_CURRENT_SOURCE_DIR}/src ${protofile}
        )

endforeach()
