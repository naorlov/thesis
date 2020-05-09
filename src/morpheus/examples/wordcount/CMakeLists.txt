

add_executable(wordcount_mapper mapper.cpp)
add_executable(wordcount_reducer reducer.cpp)

set_target_properties(wordcount_mapper wordcount_reducer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

