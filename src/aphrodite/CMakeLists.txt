file(GLOB HEADERS include/*.h)
file(GLOB SOURCE src/*.cpp)

include_directories(include)

add_library(
        aphrodite
        STATIC
        ${HEADERS}
        ${SOURCE}
)