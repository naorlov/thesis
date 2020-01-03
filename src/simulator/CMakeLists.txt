include_directories(../aphrodite/lib/include)

find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(
        simulator
        main.cpp
)

target_link_libraries(
        simulator aphrodite boost_program_options
)