# Macro

## Controled Macro(#define ...)
### ae2f_MAC_KEYWORD
[string]

> Keyword for macro startup.  
> Default is `"ae2f_MAC"`

### CMT_REQUIRED    
[bool]

> Is comment is required on output  
> If set to non-zero, additional comments will be emerged.

### SZPARAM         
[size_t]

> The static buffer size for parameters  
```c
ae2f_MAC() FUNCTION_NAME(/** HERE */)
```

### SZTPARAM        [size_t]
> The static buffer size for template parameters  
```c
ae2f_MAC((/** HERE */)) ...
```

## Main
- When [`ae2f_MAC`](#ae2f_MAC_KEYWORD) presents, named block will turn into a macro with `_` prefixed.

```c
/** before */

ae2f_MAC() foo { /** ... */ }
```

```c
/** after */

#define _foo( \
	/** tparam */ \
		\
 \
	/** param */ \
		/* */  \
){ /** ... */ }
```

- When something is in `ae2f_MAC( /** HERE */ )`, that will be `template parameters`.

```c
/** before */

ae2f_MAC((bar)) foo {}
```

```c
/** after */

#define _foo( \
	/** tparam */ \
		bar \
 \
	/** param */ \
		/* */  \
){}
```

- When named parameter has paramters, named block will considered as void returning function.

```c
/** before */

ae2f_MAC() foo(int bar) {}
ae2f_MAC() foo1(int* bar) {}
ae2f_MAC() foo2(int bar()) {}
ae2f_MAC() foo3(int (bar)) {}
ae2f_MAC() foo4(int (*bar)) {}
ae2f_MAC() foo5(int (*bar)(int b)) {}
ae2f_MAC() foo5(int& bar) {}
```

```c
/** after */

#define _foo( \
	/** tparam */ \
		\
 \
	/** param */ \
		/* , int */ bar \
) {}
#define _foo1( \
	/** tparam */ \
		\
 \
	/** param */ \
		/* ,int*  */ bar \
) {}
#define _foo2( \
	/** tparam */ \
		\
 \
	/** param */ \
		/* , int */ bar \
) {}
#define _foo3( \
	/** tparam */ \
		\
 \
	/** param */ \
		/* , int */ bar \
) {}
#define _foo4( \
	/** tparam */ \
		\
 \
	/** param */ \
		/* , int* */ bar \
) {}
#define _foo5( \
	/** tparam */ \
		\
 \
	/** param */ \
		/* , int* */ bar \
) {}
```

- `template parameters` and `function parameters` could be mixed, but `template parameters` must have `,` in the end.

```c
/** before */

ae2f_MAC(bar, ) foo(int bar1) {}
```

```c
/** after */
#define _foo( \
	/** tparam */ \
		bar, \
 \
	/** param */ \
		/* , int */ bar1 \
) {}
```

- one file must be ensured to be scoped with `__ae2f_MACRO_GENERATED`.

```c
/** before */

```

```c
/** after */

#undef	__ae2f_MACRO_GENERATED
#define	__ae2f_MACRO_GENERATED	1

#undef	__ae2f_MACRO_GENERATED

#define	__ae2f_MACRO_GENERATED	0
```

- to avoid confusion, under each inclusion has safe redefinition to ensure the scope.
```c
/** before */

#include <stdio.h>
#include <stddef.h>
#include <ctypes.h>
```
```c
/** after */

#undef	__ae2f_MACRO_GENERATED
#define	__ae2f_MACRO_GENERATED	1
#include <stdio.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1
#include <stddef.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1

#include <ctypes.h>
#undef __ae2f_MACRO_GENERATED
#define __ae2f_MACRO_GENERATED 1

#undef	__ae2f_MACRO_GENERATED

#define	__ae2f_MACRO_GENERATED	0
```
