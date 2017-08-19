set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror \
    -Wno-unused-result -Wno-unused-parameter -Wno-unused-function \
    -Wno-missing-field-initializers -Wno-missing-braces")
set(CMAKE_C_STANDARD 11)

set(TESTS)

list(APPEND TESTS
        core/allocator.c
        )

foreach(file ${TESTS})
    get_filename_component(test ${file} NAME_WE)
    # We can prefex or suffix test targets
    string(CONCAT test ${test} "_test")
    add_executable(${test} ${file})
    target_link_libraries(${test} libcable)
endforeach(file)
