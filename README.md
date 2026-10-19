*This project has been created as part of the 42 curriculum by balsaray.*

# ft_printf

## Description

**ft_printf** is a re-implementation of the `printf()` function from the C standard library. The goal of the project is to learn how **variadic functions** work in C: functions that accept a variable number of arguments, read with the `<stdarg.h>` macros (`va_start`, `va_arg`, `va_end`).

The result is a static library, `libftprintf.a`, containing:

```c
int	ft_printf(const char *format, ...);
```

It writes formatted output to the standard output and returns the number of characters printed, like the original `printf()`.

Main constraints of the project:

- Written in C and compliant with the 42 Norm.
- Compiled with `cc -Wall -Wextra -Werror`.
- The buffer management of the original `printf()` is not implemented: output is written directly with `write()`.
- The library is built with `ar` and placed at the root of the repository.
- The header is named `ft_printf.h` and contains the prototype of `ft_printf()`.
- Libft is allowed and is included in the `libft` folder with its own Makefile.

## Instructions

### Compilation

```
make          # builds libft, then libftprintf.a
make clean    # removes the object files (ft_printf and libft)
make fclean   # removes the object files, libftprintf.a and libft.a
make re       # fclean, then all
```

The Makefile first builds libft with its own Makefile (`make -C libft`), copies `libft.a` to `libftprintf.a`, then adds the ft_printf object files to it with `ar`. A program only needs to link `libftprintf.a`. The Makefile does not relink when nothing has changed.

### Using the library

Include the header in your source file and link against `libftprintf.a`:

```c
#include "ft_printf.h"
```

```
cc -Wall -Wextra -Werror main.c libftprintf.a -o program
```

Example:

```c
int	main(void)
{
	int	len;

	len = ft_printf("Hello %s, you are %d years old\n", "world", 42);
	ft_printf("Printed %d characters\n", len);
	return (0);
}
```

### Testing

The easiest check is to call `printf` and `ft_printf` with the same arguments and compare both the output and the return value:

```c
int	a = printf("[%d %s %p]\n", 42, NULL, NULL);
int	b = ft_printf("[%d %s %p]\n", 42, NULL, NULL);
printf("printf: %d | ft_printf: %d\n", a, b);
```

Since `printf` is buffered and `ft_printf` is not, each test line should end with `\n` so the outputs are not printed out of order.

## Library description

### Supported conversions

| Conversion | Description |
|------------|-------------|
| `%c` | A single character. |
| `%s` | A string. Prints `(null)` for a `NULL` pointer. |
| `%p` | A `void *` pointer in hexadecimal, prefixed with `0x`. Prints `(nil)` for `NULL`. |
| `%d`, `%i` | A signed decimal integer, including `INT_MIN`. |
| `%u` | An unsigned decimal integer. |
| `%x`, `%X` | An unsigned hexadecimal number, in lowercase or uppercase. |
| `%%` | A percent sign. |

### Functions

| Function | Description |
|----------|-------------|
| `ft_printf` | Walks through the format string, writes normal characters, sends each conversion to `check_type`, and returns the total number of characters printed. |
| `check_type` | Reads the next argument with `va_arg` using the correct type and calls the matching print function. |
| `ft_putchar` | Writes one character. Returns `1`. |
| `ft_putstr` | Writes a string, or `(null)` if it is `NULL`. Returns its length. |
| `ft_putnbr_base` | Writes an `unsigned long` in any base, given as a string of digits. Returns the number of digits written. |
| `ft_putnbr_dec` | Writes a signed `int` in base 10, with a `-` sign when negative. |
| `ft_putptr` | Writes a pointer as `0x` followed by its address in lowercase hexadecimal, or `(nil)`. |
| `ft_putundef` | Writes `%` followed by an unknown specifier character. Returns `2`. |
## Algorithm and data structure

### Format parsing

`ft_printf` reads the format string from left to right with an index:

- A normal character is written and counted.
- A `%` means the next character is a conversion specifier. It is sent to `check_type`, and the index skips both the `%` and the specifier.
- If the format ends with a single `%`, there is no specifier to read, so `ft_printf` returns `-1`.

Each print function returns how many characters it wrote, and `ft_printf` adds these values to a total, which becomes its return value.

### Reading the arguments

The variable arguments are read with a `va_list`, which works like a bookmark: `va_start` places it after the `format` parameter, and each `va_arg` reads the next argument and moves forward.

`va_arg` must be given the type that was actually passed. Because of C's default argument promotions, a `char` argument arrives as an `int`:

| Conversion | Type read with `va_arg` |
|------------|-------------------------|
| `c`, `d`, `i` | `int` |
| `s` | `char *` |
| `p` | `void *` |
| `u`, `x`, `X` | `unsigned int` |
| `%` | none, no argument is read |

The `va_list` is passed to `check_type` **by pointer** (`va_list *`), so every `va_arg` moves the same bookmark. On some platforms, a `va_list` passed by value is a separate copy, and the position would not be updated in `ft_printf`.

### Number conversion

All numbers are printed by one recursive function, `ft_putnbr_base`, adapted from my libft `ft_putnbr_fd`:

- The **base string** contains the digits in order (`"0123456789"`, `"0123456789abcdef"`, `"0123456789ABCDEF"`). Its length is the base, and each digit's value is its index, so `base[n % length]` gives the character to print directly.
- `n % length` gives the last digit and `n / length` removes it. This produces the digits backwards, so the function **calls itself first** with `n / length` and prints its own digit after the recursive call returns. This puts the digits back in the correct order.
- Each call returns the count from the recursive call plus one for its own digit.
- The parameter is an `unsigned long` so the same function can print pointer addresses (8 bytes) as well as `unsigned int` values.

`%u`, `%x` and `%X` call it directly. `ft_putnbr_dec` handles the sign: it stores the `int` in a `long` before negating it, because `-INT_MIN` does not fit in an `int`. `ft_putptr` adds the `0x` prefix.

### Technical choices

- **One function for every base** instead of separate decimal and hexadecimal functions, so the digit-extraction logic exists only once.
- **One small function per conversion**, which keeps every function short for the Norm and makes adding a conversion a matter of adding one branch in `check_type`.

## Resources

### References

- Linux man pages (`man 3 printf`, `man 3 stdarg`)
- [GeeksforGeeks: Variadic functions in C](https://www.geeksforgeeks.org/c/variadic-functions-in-c/)
- [cppreference: Variadic functions](https://en.cppreference.com/w/c/variadic)
- [cppreference: printf format specification](https://en.cppreference.com/w/c/io/fprintf)

### Use of AI

AI (Claude) was used as a support tool for understanding. Tasks and parts of the project concerned:

- **Understanding concepts:** explanations of variadic functions and how `va_list` works, enums, the preprocessor and macros, include guards, how the real `printf` works, the compilation process.
- **Tooling:** help with compiler error messages and the embedded git repository warning for the `libft` folder.
- **Documentation:** help structuring this README.
