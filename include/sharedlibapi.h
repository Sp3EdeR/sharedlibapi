#if defined(_WIN32)
	// For compilers targeting Windows. i.e. MSVC, CygWin, MinGW, clang.cl...
	/// Marks a declaration to be imported from a shared library.
#	define LIBAPI_IMPORT __declspec(dllimport)
	/// Marks a declaration to be exported from this shared library.
#	define LIBAPI_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
	// For GNU interface compatible compilers. i.e. gcc, clang
#	if 4 <= __GNUC__
		/// Marks a declaration to be imported from a shared library.
#		define LIBAPI_IMPORT
		/// Marks a declaration to be exported from this shared library.
#		define LIBAPI_EXPORT __attribute__ ((visibility ("default")))
#	else
		// Old versions of GCC do not support setting the visibility by attributes
		/// Marks a declaration to be imported from a shared library.
#		define LIBAPI_IMPORT
		/// Marks a declaration to be exported from this shared library.
#		define LIBAPI_EXPORT
#	endif
#elif !DISABLE_LIBAPI_UNSUPPORTED_COMPILER_ERROR
#	error "Unsupported compiler for the LIBAPI macros. Ask the library's maintainers to add support for this compiler."
#endif

#if !DISABLE_LIBAPI
#	include "isempty.h"

#	define LIBAPI_INNER3_0() LIBAPI_IMPORT // Numeric helper for importing
#	define LIBAPI_INNER3_1() LIBAPI_EXPORT // Numeric helper for exporting

	// Concatenates macro arguments, single expansion
#	define LIBAPI_CAT(x, y) x ## y
	// x=0 -> dllimport, x=1 -> dllexport, uses LIBAPI_INNER3_*
#	define LIBAPI_INNER2(x) LIBAPI_CAT(LIBAPI_INNER3_, x) ()
	// Tests if `x` is defined -> exported function
#	define LIBAPI_INNER1(x) LIBAPI_INNER2(MARG_IS_EMPTY(x))

	/// Generates attributes to mark declarations as a shared library's API.
	/** Marks a declaration as part of a specific shared library's interface. When building the
	  * specified shared library, the symbol is exported on that library's API. When building a
	  * different application/library, the symbols is imported.
	  *
	  * Function declaration example:
	  * \code{.c}
	  * LIBAPI(MYLIB) void MyLibFunction();
	  * \endcode
	  *
	  * Then when compiling the "mylib" library, add the `-DSHARED_LIB_ID_MYLIB=` flag.
	  * \param x The shared library's unique identifier.
	  */
#	define LIBAPI(x) LIBAPI_INNER1(LIBAPI_CAT(SHARED_LIB_ID_, x))
#else
#	define LIBAPI(x)
#endif
