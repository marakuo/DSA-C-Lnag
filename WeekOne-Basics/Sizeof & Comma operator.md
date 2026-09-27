# `sizeof` Operator & Comma Operator in C

This README explains two C operators that are easy to overlook but important to understand:

* `sizeof`
* Comma operator `,`

---

# 1. `sizeof` Operator

The `sizeof` operator is used to find the **size of a type or object in bytes**.

## Syntax

You can use `sizeof` in two ways:

```c
sizeof(type)
```

or:

```c
sizeof(expression)
```

### Example

```c
#include <stdio.h>

int main(void)
{
    int x = 10;

    printf("%zu\n", sizeof(x));
    printf("%zu\n", sizeof(int));

    return 0;
}
```

If an `int` occupies 4 bytes, the output is:

```text
4
4
```

---

# 2. Return Type of `sizeof`

The result of `sizeof` has type:

```c
size_t
```

`size_t` is an unsigned integer type used for representing sizes in memory.

Therefore, when printing the result of `sizeof`, use:

```c
%zu
```

Example:

```c
printf("%zu\n", sizeof(int));
```

### Important

`sizeof` does **not** return `unsigned long` specifically.

Its type is:

```text
size_t
```

The underlying type used for `size_t` depends on the implementation.

---

# 3. `sizeof` with Different Types

```c
#include <stdio.h>

int main(void)
{
    printf("char:      %zu\n", sizeof(char));
    printf("short:     %zu\n", sizeof(short));
    printf("int:       %zu\n", sizeof(int));
    printf("long:      %zu\n", sizeof(long));
    printf("long long: %zu\n", sizeof(long long));
    printf("float:     %zu\n", sizeof(float));
    printf("double:    %zu\n", sizeof(double));

    return 0;
}
```

A possible output on a typical 64-bit system is:

```text
char:      1
short:     2
int:       4
long:      8
long long: 8
float:     4
double:    8
```

The exact sizes can vary between systems.

---

# 4. `sizeof(char)`

One special rule in C:

```c
sizeof(char)
```

is **always `1`**.

This does not necessarily mean one byte is 8 bits. A byte in C is defined as the size of a `char`, and `CHAR_BIT` tells you how many bits are in a byte.

For example:

```c
printf("%zu\n", sizeof(char));
```

always produces:

```text
1
```

---

# 5. `sizeof` with Arrays

`sizeof` is very useful for finding the size of an array.

```c
#include <stdio.h>

int main(void)
{
    int numbers[] = {10, 20, 30, 40, 50};

    printf("%zu\n", sizeof(numbers));

    return 0;
}
```

If an `int` is 4 bytes:

```text
5 elements × 4 bytes = 20 bytes
```

So:

```text
20
```

---

## Finding the Number of Array Elements

You can divide the total array size by the size of one element:

```c
size_t count = sizeof(numbers) / sizeof(numbers[0]);
```

Example:

```c
#include <stdio.h>

int main(void)
{
    int numbers[] = {10, 20, 30, 40, 50};

    size_t count = sizeof(numbers) / sizeof(numbers[0]);

    printf("%zu\n", count);

    return 0;
}
```

Output:

```text
5
```

The idea is:

```text
                 total size
Number of elements = ─────────────
                     element size
```

In C:

```c
sizeof(array) / sizeof(array[0])
```

---

# 6. `sizeof` and Strings

Consider:

```c
char str[] = "Hello";
```

In memory, it contains:

```text
'H' 'e' 'l' 'l' 'o' '\0'
```

Notice the `'\0'` at the end.

Therefore:

```c
printf("%zu\n", sizeof(str));
```

prints:

```text
6
```

But:

```c
strlen(str)
```

returns:

```text
5
```

because `strlen()` does not count the null terminator.

So:

```text
sizeof(str) → size of the array in bytes
strlen(str) → number of characters before '\0'
```

---

# 7. `sizeof` Usually Does Not Evaluate Its Operand

Consider:

```c
#include <stdio.h>

int main(void)
{
    int x = 10;

    printf("%zu\n", sizeof(x++));
    printf("%d\n", x);

    return 0;
}
```

Output:

```text
4
10
```

You might expect `x++` to change `x` to `11`.

It doesn't.

In this case, `sizeof` determines the type's size without evaluating `x++`.

Therefore:

```c
sizeof(x++)
```

does not perform the increment.

---

# 8. Comma Operator `,`

The comma operator allows multiple expressions to be evaluated **from left to right**.

The value of the entire comma expression is the value of the **last expression**.

### Example

```c
int x;

x = (10, 20, 30);
```

The expressions are evaluated:

```text
10
↓
20
↓
30
```

The result of the entire expression is:

```text
30
```

Therefore:

```c
x = 30;
```

---

# 9. Another Example

```c
int x = 5;
int y;

y = (x++, x + 10);
```

Evaluate from left to right.

### First expression

```c
x++
```

`x` becomes:

```text
6
```

### Second expression

```c
x + 10
```

becomes:

```text
16
```

Therefore:

```c
y = 16;
```

Final values:

```text
x = 6
y = 16
```

---

# 10. The Important Case: `x = 1, 2, 3`

This is where the **comma operator + operator precedence** becomes important.

Consider:

```c
int x;

x = 1, 2, 3;
```

It might look like:

```c
x = (1, 2, 3);
```

But it is **not**.

Because the assignment operator `=` has higher precedence than the comma operator `,`, C interprets it as:

```c
(x = 1), 2, 3;
```

Therefore:

```text
x = 1
```

The expressions `2` and `3` are evaluated, but their values are discarded.

So:

```c
printf("%d\n", x);
```

prints:

```text
1
```

---

# 11. Compare It With Parentheses

Now look at:

```c
int x;

x = (1, 2, 3);
```

The parentheses force the comma expression to be evaluated first.

```text
1 → 2 → 3
          ↓
        result
```

The value of:

```c
(1, 2, 3)
```

is `3`.

Therefore:

```c
x = 3;
```

and:

```c
printf("%d\n", x);
```

prints:

```text
3
```

---

# 12. Side-by-Side Comparison

### Without parentheses

```c
x = 1, 2, 3;
```

is interpreted as:

```c
(x = 1), 2, 3;
```

Result:

```text
x = 1
```

### With parentheses

```c
x = (1, 2, 3);
```

is interpreted as:

```c
x = 3;
```

Result:

```text
x = 3
```

### Remember

```text
= has higher precedence than ,
```

So:

```c
x = 1, 2, 3;
```

does **not** mean:

```c
x = (1, 2, 3);
```

---

# 13. Comma Operator in a `for` Loop

One practical use of the comma operator is in `for` loops.

```c
#include <stdio.h>

int main(void)
{
    int i, j;

    for (i = 0, j = 10; i < 5; i++, j--)
    {
        printf("i = %d, j = %d\n", i, j);
    }

    return 0;
}
```

Output:

```text
i = 0, j = 10
i = 1, j = 9
i = 2, j = 8
i = 3, j = 7
i = 4, j = 6
```

Here:

```c
i = 0, j = 10
```

initializes both variables.

And:

```c
i++, j--
```

updates both variables after each iteration.

---

# 14. Comma Operator vs Comma Separator

Not every comma you see in C is the **comma operator**.

### Function arguments

```c
printf("%d %d", x, y);
```

The commas separate function arguments.

### Variable declarations

```c
int x = 10, y = 20;
```

The comma separates declarations.

### Comma operator

```c
x = (10, 20);
```

Here the comma is an actual **comma operator**.

---

# 15. Quick Summary

## `sizeof`

```c
sizeof(x)
```

* Returns the size in **bytes**
* Result type is `size_t`
* Use `%zu` with `printf`
* Very useful with arrays
* Usually does not evaluate its operand

Example:

```c
int arr[10];

size_t n = sizeof(arr) / sizeof(arr[0]);
```

---

## Comma Operator

```c
(expression1, expression2, expression3)
```

* Evaluates expressions from **left to right**
* The value of the entire expression is the **last expression**

Example:

```c
x = (1, 2, 3);
```

Result:

```text
x = 3
```

But:

```c
x = 1, 2, 3;
```

is interpreted as:

```c
(x = 1), 2, 3;
```

Result:

```text
x = 1
```

---

# Quick Reference

```text
┌─────────────────────────────────────────────┐
│                  sizeof                     │
├─────────────────────────────────────────────┤
│ Gives size in bytes                         │
│ Return type: size_t                         │
│ printf: %zu                                 │
│                                             │
│ sizeof(int)                                 │
│ sizeof(array) / sizeof(array[0])            │
└─────────────────────────────────────────────┘

┌─────────────────────────────────────────────┐
│                Comma Operator               │
├─────────────────────────────────────────────┤
│ Evaluates left → right                     │
│ Result = value of the last expression       │
│                                             │
│ (1, 2, 3) → 3                              │
│                                             │
│ x = 1, 2, 3                                │
│ → (x = 1), 2, 3                            │
│ → x = 1                                    │
└─────────────────────────────────────────────┘
```

---

# Practice

Try to predict the output before running each program.

### 1.

```c
int x = 10;

printf("%zu\n", sizeof(x));
```

---

### 2.

```c
int arr[10];

printf("%zu\n", sizeof(arr) / sizeof(arr[0]));
```

---

### 3.

```c
int x;

x = (10, 20, 30);

printf("%d\n", x);
```

---

### 4.

```c
int x;

x = 10, 20, 30;

printf("%d\n", x);
```

---

### 5.

```c
int x = 5;
int y;

y = (x++, x * 2);

printf("x = %d, y = %d\n", x, y);
```

---

## Core Ideas

```text
sizeof
   ↓
size in bytes
   ↓
type = size_t
   ↓
use %zu
```

```text
Comma operator
   ↓
evaluate left → right
   ↓
last expression gives the result
```

And always pay attention to parentheses:

```c
x = (1, 2, 3);   // x = 3

x = 1, 2, 3;     // x = 1
```
