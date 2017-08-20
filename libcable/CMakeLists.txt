set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror \
    -Wno-unused-result -Wno-unused-parameter -Wno-unused-function \
    -Wno-missing-field-initializers -Wno-missing-braces")
set(CMAKE_C_STANDARD 11)

set(HEADERS
        include/cable/porting.h

        include/cable/core.h
        include/cable/core/object.h
        include/cable/core/error.h
        include/cable/core/allocator.h
        include/cable/core/data.h
        include/cable/core/string.h
        include/cable/core/array.h
        include/cable/core/set.h
        include/cable/core/map.h
        include/cable/core/scalar.h
        include/cable/core/stream.h
        include/cable/core/test.h
        include/cable/math.h
        include/cable/math/4f.h
        include/cable/math/simd/4f.h
        include/cable/math/scalar/4f.h
        include/cable/math/2d.h
        include/cable/math/simd/2d.h
        include/cable/math/scalar/2d.h
        include/cable/math/mat4f.h
        include/cable/math/mat2d.h
        )

set(SOURCES
        src/core/object.c
        src/core/error.c
        src/core/allocator.c
        src/core/data.c
        src/core/string.c
        src/core/array.c
        src/core/set.c
        src/core/map.c
        src/core/scalar.c
        src/core/stream.c
        src/core/test.c
        )

add_library(libcable SHARED ${HEADERS} ${SOURCES})
target_include_directories(libcable PUBLIC include)

set_target_properties(libcable PROPERTIES OUTPUT_NAME cable)

install(TARGETS libcable
        LIBRARY DESTINATION lib/cable)
install(DIRECTORY include/cable/ DESTINATION include/cable)

add_subdirectory(tst)
