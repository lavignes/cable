# libcable (c-able)

This is my personal extended c standard library.

## <cable/core.h>

The foundational runtime library and data structures.

* **<cable/core/object.h>**
Routines for doing thread-safe reference counting memory management and limited OOP.

* **<cable/core/error.h>**
Error handling.

* **<cable/core/allocator.h>**
Create and work with custom memory allocators.
Provides a wrapper for malloc/free that plays nice with reference counting.
**Includes run-time memory leak detector!**

* **<cable/core/data.h>**
Perform operations on blocks of raw data.

* **<cable/core/array.h>**
Manage homogeneous collections.

* **<cable/core/set.h>**
Manage collections without duplicates.

* **<cable/core/map.h>**
Associative arrays.

* **<cable/core/string.h>**
Unicode strings.

* **<cable/core/scalar.h>**
Box scalar primitives.

* **<cable/test.h>**
A bare-bones unit-testing library.

## <cable/math.h>
Scalar and vector math library.

* **<cable/math/4f.h>**
4d single-precision floating point vector math.

* **<cable/math/2d.h>**
2d double-precision floating point vector math.

* **<cable/math/mat4f.h>**
4d single-precision floating point matrix math.

