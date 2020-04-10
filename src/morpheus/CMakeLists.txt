add_subdirectory(examples)

add_custom_target(
        morpheus
        COMMAND echo "building morpheus"
    DEPENDS protobufs
)
