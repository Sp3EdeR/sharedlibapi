# sharedlibapi

This C header allows its users to flexibly create shared library attributes for headers. The
library works with any mainstream C/C++ compiler that implements the standard preprocessor for any
platform.

Sometimes multiple libraries must be built, which depend on one another. The library interfaces
are defined in dedicated header files, where the symbols are marked as exported/imported. These
headers are used by both the library's code and the library's users. When compiling these headers
within the context of the library, the annotations imply the exporting of resources, but when
the headers are compiled within the context of the user, the annotations imply importing. A macro
is commonly introduced to control whether we're exporting or importing. For example:

```
#if defined(COMPILING_SHARED_LIB)
#   define EXPORTFUNC __attribute__ ((visibility ("default")))
#else
#   define EXPORTFUNC
#endif
```

This problem with this approach is that each library requires the creation of many macros in order
to create a single library that exports its own interface, and imports interfaces from other
libraries. This can become cumbersome to manage correctly. Some build systems, like CMake offer
automation features to support this use case, but then a rigid external tool dependency is
introduced. This library provides a standard C library that solves this problem.

# Usage example

In this example, we will implement the *IcecreamFarm* and *IcecreamTruck* libraries. Create the
following files in the following directory structure:

```bash
IcecreamExample
├───IcecreamFarm
│   ├───include
│   │       icecreamfarm.h
│   └───src
│           icecreamfarm.c
│
├───IcecreamTruck
│   ├───include
│   │       icecreamtruck.h
│   └───src
│           icecreamtruck.c
│
└───sharedlibapi
    │   LICENSE
    │   README.md
    └───include
            isempty.h
            sharedlibapi.h
```

Where `icecreamfarm.h` specifies the function `GetIcecream`:

```c
#include "../../sharedlibapi/include/sharedlibapi.h"

LIBAPI(ICECREAMFARM) void GetIcecream();
```

And `icecreamtruck.h` specifies the function `LoadIcecream`:

```c
#include "../../sharedlibapi/include/sharedlibapi.h"

LIBAPI(ICECREAMTRUCK) void LoadIcecream();
```

The `LoadIcecream` function's implementation uses the `GetIcecream` function from *IcecreamFarm*.

When building `icecreamfarm.c`, add the `-DSHARED_LIB_ID_ICECREAMFARM=` compiler command-line
argument. When building `icecreamtruck.c`, add the `-DSHARED_LIB_ID_ICECREAMTRUCK=` compiler
command-line argument, and of course import the *IcecreamFarm* library. (On the Microsoft Visual C
and Clang.cl compilers, replace the `-` with a `/`.) This results in two libraries:

* `libicecreamfarm.so` (or similar), which exports the `GetIcecream` function.
* `libicecreamtruck.so` that uses the `GetIcecream` function, and exports the `LoadIcecream`
  function.

## Additional notes

When using a GNU-compatible compiler, don't forget to add the `-fvisibility=hidden` command-line
option to hide all non-exported symbols.
