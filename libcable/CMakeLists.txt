set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror \
    -Wno-unused-result -Wno-unused-parameter -Wno-unused-function \
    -Wno-missing-field-initializers -Wno-missing-braces")
set(CMAKE_C_STANDARD 11)

set(HEADERS
        include/cable/porting.h
        include/cable/test.h

        include/cable/core.h
        include/cable/core/object.h
        include/cable/core/allocator.h
        include/cable/core/data.h
        include/cable/core/string.h
        include/cable/core/array.h
        include/cable/core/set.h
        include/cable/core/map.h
        include/cable/core/scalar.h

        include/cable/math.h
        include/cable/math/4f.h
        include/cable/math/simd/4f.h
        include/cable/math/scalar/4f.h
        include/cable/math/2d.h
        include/cable/math/simd/2d.h
        include/cable/math/scalar/2d.h
        include/cable/math/mat4f.h
        )

set(SOURCES
        src/test.c

        src/core/object.c
        src/core/allocator.c
        src/core/data.c
        src/core/string.c
        src/core/array.c
        src/core/set.c
        src/core/map.c
        src/core/scalar.c
        )

add_library(libcable ${HEADERS} ${SOURCES})
set_target_properties(libcable PROPERTIES OUTPUT_NAME cable)
target_include_directories(libcable PUBLIC include)

target_link_libraries(libcable backtrace)

add_subdirectory(tst)
