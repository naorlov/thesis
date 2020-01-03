file(GLOB HEADERS include/*.h)
file(GLOB SOURCE src/*.cpp)

add_library(
        aphrodite
        STATIC
        ${HEADERS}
        ${SOURCE}
)