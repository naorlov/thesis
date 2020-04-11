find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(
    datagen
    datagen.cpp
    string_formatter.h
    string_formatter.cpp
)

target_link_libraries(datagen boost_program_options)