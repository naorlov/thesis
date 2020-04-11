file(GLOB HEADERS include/*.h)
file(GLOB SOURCE src/*.cpp)

include_directories(include)

add_library(
        clconf
        STATIC
        ${HEADERS}
        ${SOURCE}
)

target_link_libraries(clconf PRIVATE nlohmann_json::nlohmann_json)