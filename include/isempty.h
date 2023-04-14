/*
Use the `MARG_IS_EMPTY(MAYBE_MACRO)` call to test whether `MAYBE_MACRO` evaluates to an empty
expression. This expression evaluates to `1` when the argument is empty, `0` otherwise.

Uses the example from: https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments
Credits to: Jens Gustedt, June 8, 2010
*/

// Parameter counter helper; maximum 16
#define MARG_COUNTER_16(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, size, ...) size
#if defined(_MSC_VER) && !defined( __clang__)
	// On MSVC, we must add expansion to evaluate MPARAM_HAS_COMMA_16
#   define MSVC_EXPAND_MACRO_I(x) x
#   define MSVC_EXPAND_MACRO(x) MSVC_EXPAND_MACRO_I(x)
    // Returns 1 if the argument list has a comma, 0 otherwise (max. 16 args).
#   define MPARAM_HAS_COMMA_16(...) MSVC_EXPAND_MACRO(MARG_COUNTER_16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))
#else
    // Returns 1 if the argument list has a comma, 0 otherwise (max. 16 args).
#   define MPARAM_HAS_COMMA_16(...) MARG_COUNTER_16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#endif

// Called only if the argument(s) are empty or list that's parenthesized
// Some versions of clang count `,` as one macro arg only, `0` ensures two
#define MARG_TRIGGER_PARENTHESIS(...) 0,
// Called only if the argument was empty
// Some versions of clang count `,` as one macro arg only, `0` ensures two
#define MARG_EMPTY_TEST_CASE_0001 0,
// Pastes 5 arguments together
#define MARG_PASTE_5(p0, p1, p2, p3, p4) p0 ## p1 ## p2 ## p3 ## p4
// Combined evaluation for tests, we're seeking case 0001
#define MARG_IS_EMPTY_INNER(p0, p1, p2, p3) MPARAM_HAS_COMMA_16(MARG_PASTE_5(MARG_EMPTY_TEST_CASE_, p0, p1, p2, p3))

/// Tests if no arguments were given to the macro when called.
/** \return Returns 1 when no arguments were given, 0 otherwise.
  */ 
#define MARG_IS_EMPTY(...)                                              \
    MARG_IS_EMPTY_INNER(                                                \
        /* check for multiarg case (2 or more) */                       \
        MPARAM_HAS_COMMA_16(__VA_ARGS__),                               \
        /* check if args form a macro call (parenthesized list) */      \
        MPARAM_HAS_COMMA_16(MARG_TRIGGER_PARENTHESIS __VA_ARGS__),      \
        /* check if arg is a callable macro's name itself */            \
        MPARAM_HAS_COMMA_16(__VA_ARGS__ ()),                            \
        /* trigger macro is only called when args are empty */          \
        MPARAM_HAS_COMMA_16(MARG_TRIGGER_PARENTHESIS __VA_ARGS__ ())    \
    )
