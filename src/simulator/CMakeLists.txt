include_directories(../aphrodite/lib/include)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(SimGrid REQUIRED) # This template requires SimGrid v3.22
include_directories(${SimGrid_INCLUDE_DIR})

add_executable(
        simulator
        main.cpp
        ${PROTO_CPP_SOURCE}
)

target_link_libraries(
        simulator aphrodite boost_program_options ${SimGrid_LIBRARY}
)