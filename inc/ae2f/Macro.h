#ifndef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 0
#endif

#undef	ae2f_WhenMac
#undef	ae2f_WhenNMac
#undef	ae2f_MAC

#if !__ae2f_MACRO_GENERATED

/** Available when is macro-generated */
#define	ae2f_WhenMac(...)

/** Available when is not-macro-generated. */
#define	ae2f_WhenNMac(...) __VA_ARGS__

#else

/** Available when is macro-generated .*/
#define ae2f_WhenMac(...) __VA_ARGS__

/** Available when is not-macro-generated. */
#define ae2f_WhenNMac(...)

#endif

/**
 * @def ae2f_MAC
 * @brief
 * A hint for creating a macro from void-returning function.
 *
 * @details
 * Its parameters will be additional template parameters you could customise.
 * */
#define ae2f_MAC(...) ae2f_WhenNMac(inline static void)

#if !(defined(OFF) && defined(ON))
#define OFF	0
#define ON 	1
#endif

#define ae2f_MAC_BUILD OFF
