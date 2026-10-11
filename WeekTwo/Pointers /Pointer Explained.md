# Pointers in C

A detailed guide to pointers in the C language: what they are, how they work with variables, arrays, strings, functions, and dynamic memory, with runnable examples.

## Table of Contents

1. [Memory and addresses](#1-memory-and-addresses)
2. [What is a pointer?](#2-what-is-a-pointer)
3. [Declaring and using pointers](#3-declaring-and-using-pointers)
4. [`NULL` and uninitialized pointers](#4-null-and-uninitialized-pointers)
5. [Pointers and function arguments](#5-pointers-and-function-arguments)
6. [Pointer arithmetic](#6-pointer-arithmetic)
7. [Pointers and arrays](#7-pointers-and-arrays)
8. [Pointers and strings](#8-pointers-and-strings)
9. [`const` and pointers](#9-const-and-pointers)
10. [Pointers to pointers](#10-pointers-to-pointers)
11. [Arrays of pointers and 2D arrays](#11-arrays-of-pointers-and-2d-arrays)
12. [`void *` (generic pointers)](#12-void--generic-pointers)
13. [Function pointers](#13-function-pointers)
14. [Pointers and structs](#14-pointers-and-structs)
15. [Dynamic memory allocation](#15-dynamic-memory-allocation)
16. [Dynamic 2D arrays](#16-dynamic-2d-arrays)
17. [Common bugs](#17-common-bugs)
18. [Practice examples](#18-practice-examples)
19. [Quick reference](#19-quick-reference)

---

## 1. Memory and addresses

A program's memory is like a long row of numbered boxes. Each box holds **1 byte** and has a unique number called its **address**.

```
Address   Content
0x1000    [ 42 ]   <- the variable `age` (an int uses 4 bytes: 0x1000..0x1003)
0x1001    [ .. ]
0x1002    [ .. ]
0x1003    [ .. ]
0x1004    [ .. ]
```

Every variable lives at some address. The **address-of operator** `&` gives it to you:

```c
#include <stdio.h>

int main(void) {
    int age = 42;

    printf("Value:   %d\n", age);
    printf("Address: %p\n", (void *)&age);   // cast to void * for %p

    return 0;
}
```

The address printed will differ on every machine and often on every run.

---

## 2. What is a pointer?

A **pointer is a variable that stores a memory address**.

```
        age                       ptr
   +-----------+            +-------------+
   |    42     | <--------- |   0x1000    |
   +-----------+            +-------------+
    at 0x1000                 holds the address of age
```

Two operators go with pointers:

| Operator | Name                  | Meaning                                   |
|----------|-----------------------|-------------------------------------------|
| `&x`     | address-of            | the address of `x`                        |
| `*p`     | dereference (indirection) | the value stored at the address in `p` |

---

## 3. Declaring and using pointers

```c
int *p;          // p can hold the address of an int
double *q;       // q can hold the address of a double
char *s;         // s can hold the address of a char
```

The `*` in a declaration means "pointer to". Put it next to the variable name to avoid confusion:

```c
int *a, b;       // a is a pointer to int, b is a plain int!
int *c, *d;      // both are pointers
```

### Full example

```c
#include <stdio.h>

int main(void) {
    int age = 42;
    int *p = &age;       // p stores the address of age

    printf("age      = %d\n", age);       // 42
    printf("&age     = %p\n", (void *)&age);
    printf("p        = %p\n", (void *)p); // same address as &age
    printf("*p       = %d\n", *p);        // 42 (value at that address)

    *p = 50;             // change age THROUGH the pointer
    printf("age now  = %d\n", age);       // 50

    return 0;
}
```

### Pointer type matters

The type tells the compiler how many bytes to read and how to interpret them when you dereference.

```c
int    i = 10;
double d = 3.14;

int    *pi = &i;
double *pd = &d;

// pi = &d;     // warning/error: incompatible pointer types
```

### Size of a pointer

All pointers on a given machine have the same size (typically 8 bytes on 64-bit systems), regardless of what they point to:

```c
printf("%zu %zu %zu\n", sizeof(int *), sizeof(double *), sizeof(char *));  // 8 8 8
```

---

## 4. `NULL` and uninitialized pointers

A pointer that doesn't point to anything valid should be set to `NULL`.

```c
int *p = NULL;

if (p == NULL) {
    printf("p points to nothing\n");
}

if (p) {                // same as p != NULL
    printf("%d\n", *p); // safe: only runs when p is valid
}
```

**Dereferencing `NULL` crashes the program** (segmentation fault).

An **uninitialized** pointer holds a random address, which is even worse:

```c
int *p;        // wild pointer: garbage address
*p = 5;        // undefined behavior - may crash or corrupt memory
```

Rules:

- Always initialize a pointer: to a valid address or to `NULL`.
- Check for `NULL` before dereferencing when it could be `NULL`.
- After `free(p)`, set `p = NULL`.

---

## 5. Pointers and function arguments

C passes arguments **by value**: the function receives a copy. To let a function modify the caller's variable, pass its **address**.

### The classic problem: swap

```c
#include <stdio.h>

void swap_wrong(int a, int b) {
    int tmp = a;
    a = b;
    b = tmp;           // changes only the copies
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;          // changes the originals
}

int main(void) {
    int x = 1, y = 2;

    swap_wrong(x, y);
    printf("%d %d\n", x, y);   // 1 2 (unchanged)

    swap(&x, &y);
    printf("%d %d\n", x, y);   // 2 1
    return 0;
}
```

### Returning multiple values ("output parameters")

```c
#include <stdio.h>

void min_max(const int arr[], int n, int *min, int *max) {
    *min = *max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

int main(void) {
    int data[] = {7, 2, 9, 4};
    int lo, hi;

    min_max(data, 4, &lo, &hi);
    printf("min=%d max=%d\n", lo, hi);   // min=2 max=9
    return 0;
}
```

### Never return a pointer to a local variable

```c
int *bad(void) {
    int x = 10;
    return &x;       // BUG: x is destroyed when the function returns
}
```

The pointer would point to memory that is no longer valid (a **dangling pointer**). Return a value, use `malloc`, or have the caller supply the storage.

---

## 6. Pointer arithmetic

Adding to a pointer moves it by **elements, not bytes**. `p + 1` moves by `sizeof(*p)` bytes.

```c
#include <stdio.h>

int main(void) {
    int arr[] = {10, 20, 30, 40};
    int *p = arr;                 // points to arr[0]

    printf("%d\n", *p);           // 10
    printf("%d\n", *(p + 1));     // 20
    printf("%d\n", *(p + 3));     // 40

    p++;                          // now points to arr[1]
    printf("%d\n", *p);           // 20

    return 0;
}
```

Visual (each `int` is 4 bytes):

```
arr:  | 10 | 20 | 30 | 40 |
addr: 1000 1004 1008 1012
       ^    ^
       p    p+1   (the address increases by 4, not 1)
```

### Allowed operations

| Operation       | Meaning                                         |
|-----------------|-------------------------------------------------|
| `p + n`, `p - n`| move forward/backward by `n` elements           |
| `p++`, `p--`    | move by one element                             |
| `p2 - p1`       | number of elements between two pointers into the same array (type `ptrdiff_t`) |
| `p1 < p2`, `==` | compare pointers into the same array            |

Not allowed: adding two pointers, multiplying, dividing.

### Distance between pointers

```c
int arr[5] = {1, 2, 3, 4, 5};
int *a = &arr[1];
int *b = &arr[4];

printf("%td\n", b - a);   // 3
```

> Only do arithmetic within the bounds of one array (or one past its end). Going outside is undefined behavior.

### Traversing an array with a pointer

```c
int arr[] = {1, 2, 3, 4, 5};
int n = sizeof(arr) / sizeof(arr[0]);

for (int *p = arr; p < arr + n; p++) {
    printf("%d ", *p);
}
printf("\n");
```

---

## 7. Pointers and arrays

In most expressions, an array name **decays** to a pointer to its first element.

```c
int arr[3] = {10, 20, 30};

arr        // same as &arr[0]
*arr       // same as arr[0]
*(arr + 2) // same as arr[2]
```

So `arr[i]` is defined as `*(arr + i)`. Both forms are equivalent:

```c
#include <stdio.h>

int main(void) {
    int arr[] = {10, 20, 30};
    int *p = arr;

    printf("%d %d %d\n", arr[1], *(arr + 1), p[1]);   // 20 20 20
    return 0;
}
```

### Arrays are NOT pointers

They behave similarly, but are different things:

```c
int arr[10];
int *p = arr;

printf("%zu\n", sizeof(arr));   // 40 (the whole array)
printf("%zu\n", sizeof(p));     // 8  (just a pointer)

// arr = p;     // ERROR: an array name is not assignable
p = arr;        // OK
```

### Passing arrays to functions

An array parameter is really a pointer, so the function **doesn't know the length**. Always pass it separately.

```c
#include <stdio.h>

int sum(const int *arr, int n) {      // same as: int arr[]
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;
}

void fill(int *arr, int n, int value) {
    for (int i = 0; i < n; i++) {
        arr[i] = value;               // modifies the caller's array
    }
}

int main(void) {
    int data[5] = {1, 2, 3, 4, 5};
    printf("%d\n", sum(data, 5));     // 15

    fill(data, 5, 0);
    printf("%d\n", sum(data, 5));     // 0
    return 0;
}
```

Inside `sum`, `sizeof(arr)` is the size of a pointer, not the array, so `sizeof(arr)/sizeof(arr[0])` would be wrong there.

---

## 8. Pointers and strings

A string is a `char` array ending in `'\0'`, so `char *` is the natural way to refer to one. (See the strings README for more detail.)

```c
#include <stdio.h>

int main(void) {
    char  arr[] = "Hello";     // modifiable array (6 bytes)
    char *ptr   = arr;

    while (*ptr != '\0') {
        printf("%c ", *ptr);
        ptr++;
    }
    printf("\n");
    return 0;
}
```

### Array vs pointer to literal

```c
char  a[] = "Hello";    // a copy on the stack: can be modified
char *p   = "Hello";    // points to read-only memory

a[0] = 'J';             // OK
// p[0] = 'J';          // undefined behavior
```

Use `const char *` for literals so the compiler warns you:

```c
const char *msg = "Hello";
```

### Writing your own string functions with pointers

```c
#include <stdio.h>

size_t my_strlen(const char *s) {
    const char *start = s;
    while (*s) s++;
    return (size_t)(s - start);
}

void my_strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++) != '\0') {
        // copies each character, including the final '\0'
    }
}

int main(void) {
    char buf[20];
    my_strcpy(buf, "Pointers");
    printf("%s (%zu)\n", buf, my_strlen(buf));   // Pointers (8)
    return 0;
}
```

---

## 9. `const` and pointers

`const` can apply to the **data**, the **pointer**, or both. Read declarations **right to left**.

| Declaration           | Meaning                                    | Can change `*p`? | Can change `p`? |
|-----------------------|--------------------------------------------|:---:|:---:|
| `int *p`              | pointer to int                             | yes | yes |
| `const int *p`        | pointer to **const int** (read-only data)  | no  | yes |
| `int *const p`        | **const pointer** to int                   | yes | no  |
| `const int *const p`  | const pointer to const int                 | no  | no  |

```c
int a = 1, b = 2;

const int *p1 = &a;
// *p1 = 5;        // ERROR: data is read-only through p1
p1 = &b;           // OK

int *const p2 = &a;
*p2 = 5;           // OK
// p2 = &b;        // ERROR: p2 can't point elsewhere
```

**Best practice:** mark pointer parameters `const` when the function only reads through them.

```c
size_t count_spaces(const char *s);
```

---

## 10. Pointers to pointers

A pointer can point to another pointer. Declare it with two stars.

```c
#include <stdio.h>

int main(void) {
    int    x  = 10;
    int   *p  = &x;      // pointer to int
    int  **pp = &p;      // pointer to pointer to int

    printf("%d\n", x);       // 10
    printf("%d\n", *p);      // 10
    printf("%d\n", **pp);    // 10

    **pp = 20;
    printf("%d\n", x);       // 20
    return 0;
}
```

```
  pp ---> p ---> x
 (&p)    (&x)   (10)
```

### Why is this useful? Modifying a pointer inside a function

To change **where a pointer points** in the caller, pass its address:

```c
#include <stdio.h>
#include <stdlib.h>

int allocate_array(int **out, int n) {
    *out = malloc(n * sizeof(int));   // changes the caller's pointer
    if (*out == NULL) return 0;
    for (int i = 0; i < n; i++) (*out)[i] = i * i;
    return 1;
}

int main(void) {
    int *arr = NULL;

    if (allocate_array(&arr, 5)) {
        for (int i = 0; i < 5; i++) printf("%d ", arr[i]);   // 0 1 4 9 16
        printf("\n");
        free(arr);
    }
    return 0;
}
```

---

## 11. Arrays of pointers and 2D arrays

### Array of pointers (e.g., list of strings)

```c
#include <stdio.h>

int main(void) {
    const char *names[] = {"Ali", "Sara", "Omar"};   // array of 3 char pointers

    for (int i = 0; i < 3; i++) {
        printf("%s\n", names[i]);
    }
    return 0;
}
```

`argv` in `main` has this form: `int main(int argc, char *argv[])`.

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
```

### 2D arrays and pointers

```c
int grid[2][3] = {
    {1, 2, 3},
    {4, 5, 6}
};

printf("%d\n", grid[1][2]);          // 6
printf("%d\n", *(*(grid + 1) + 2));  // 6 (same thing)
```

A 2D array is stored contiguously, row after row:

```
grid:  1 2 3 | 4 5 6
```

### Pointer to an array vs array of pointers

```c
int *a[3];       // array of 3 pointers to int
int (*b)[3];     // pointer to an array of 3 ints
```

Passing a 2D array to a function requires the column count:

```c
void print_grid(int rows, int (*g)[3]) {       // or: int g[][3]
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < 3; c++) {
            printf("%d ", g[r][c]);
        }
        printf("\n");
    }
}
```

---

## 12. `void *` (generic pointers)

A `void *` can hold the address of **any** type but can't be dereferenced or used in arithmetic directly. You must convert it to a typed pointer first.

```c
#include <stdio.h>

int main(void) {
    int    i = 5;
    double d = 2.5;

    void *vp = &i;
    printf("%d\n", *(int *)vp);       // cast, then dereference

    vp = &d;
    printf("%.1f\n", *(double *)vp);
    return 0;
}
```

In C, converting to and from `void *` is implicit, so you don't need to cast the result of `malloc`:

```c
int *p = malloc(10 * sizeof *p);      // no cast needed in C
```

### Generic functions: writing your own `swap`

```c
#include <stdio.h>
#include <string.h>

void generic_swap(void *a, void *b, size_t size) {
    unsigned char tmp[size];          // variable-length array (C99)
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

int main(void) {
    int    x = 1, y = 2;
    double p = 1.5, q = 2.5;

    generic_swap(&x, &y, sizeof(int));
    generic_swap(&p, &q, sizeof(double));

    printf("%d %d | %.1f %.1f\n", x, y, p, q);   // 2 1 | 2.5 1.5
    return 0;
}
```

Standard functions like `qsort`, `memcpy`, and `malloc` use `void *` this way.

---

## 13. Function pointers

Functions also have addresses. A function pointer stores one.

### Syntax

```c
int (*fp)(int, int);      // fp: pointer to a function taking (int, int) returning int
```

The parentheses around `*fp` are required. Without them, `int *fp(int, int)` declares a function returning `int *`.

### Example

```c
#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

int main(void) {
    int (*op)(int, int);

    op = add;                          // function name = its address
    printf("%d\n", op(5, 3));          // 8

    op = sub;
    printf("%d\n", op(5, 3));          // 2
    return 0;
}
```

### Callbacks: passing functions as arguments

```c
#include <stdio.h>

void apply(int *arr, int n, int (*f)(int)) {
    for (int i = 0; i < n; i++) {
        arr[i] = f(arr[i]);
    }
}

int square(int x) { return x * x; }
int negate(int x) { return -x; }

int main(void) {
    int data[] = {1, 2, 3, 4};

    apply(data, 4, square);
    for (int i = 0; i < 4; i++) printf("%d ", data[i]);   // 1 4 9 16
    printf("\n");

    apply(data, 4, negate);
    for (int i = 0; i < 4; i++) printf("%d ", data[i]);   // -1 -4 -9 -16
    printf("\n");
    return 0;
}
```

### Array of function pointers (jump table)

```c
#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int main(void) {
    int (*ops[])(int, int) = {add, sub, mul};
    const char *names[]    = {"add", "sub", "mul"};

    for (int i = 0; i < 3; i++) {
        printf("%s(6, 3) = %d\n", names[i], ops[i](6, 3));
    }
    return 0;
}
```

### A `typedef` makes it readable

```c
typedef int (*BinaryOp)(int, int);

BinaryOp op = add;
```

### `qsort` with a comparator

```c
#include <stdio.h>
#include <stdlib.h>

int compare_ints(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);          // avoids overflow from x - y
}

int main(void) {
    int arr[] = {5, 2, 9, 1, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    qsort(arr, n, sizeof(int), compare_ints);

    for (int i = 0; i < n; i++) printf("%d ", arr[i]);   // 1 2 5 7 9
    printf("\n");
    return 0;
}
```

---

## 14. Pointers and structs

Use `->` to access members through a struct pointer.

```c
#include <stdio.h>

typedef struct {
    char name[30];
    int  age;
} Person;

void birthday(Person *p) {
    p->age++;                 // same as (*p).age++
}

int main(void) {
    Person a = {"Ali", 20};
    Person *p = &a;

    printf("%s is %d\n", p->name, p->age);
    birthday(p);
    printf("%s is %d\n", a.name, a.age);   // 21
    return 0;
}
```

Self-referential structs (linked lists, trees) are built from pointers:

```c
typedef struct Node {
    int          value;
    struct Node *next;
} Node;
```

See the structs README for full linked list examples.

---

## 15. Dynamic memory allocation

Local variables live on the **stack** and disappear when the function returns. Memory from `malloc` lives on the **heap** until you free it.

Include `<stdlib.h>`.

| Function                   | Purpose                                              |
|----------------------------|------------------------------------------------------|
| `malloc(size)`             | allocate `size` bytes (uninitialized)                |
| `calloc(count, size)`      | allocate `count * size` bytes, zero-filled           |
| `realloc(ptr, new_size)`   | resize a block (may move it)                         |
| `free(ptr)`                | release a block                                      |

All allocation functions return `NULL` on failure, so **always check**.

### `malloc` and `free`

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 5;
    int *arr = malloc(n * sizeof *arr);
    if (arr == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    for (int i = 0; i < n; i++) arr[i] = i * 10;
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);   // 0 10 20 30 40
    printf("\n");

    free(arr);
    arr = NULL;
    return 0;
}
```

### `calloc`

```c
int *zeros = calloc(10, sizeof *zeros);   // 10 ints, all 0
```

### `realloc` (growing an array)

Always store the result in a **temporary** pointer, so you don't lose the original if it fails:

```c
int *tmp = realloc(arr, 10 * sizeof *arr);
if (tmp == NULL) {
    free(arr);          // original is still valid and must be freed
    return 1;
}
arr = tmp;
```

### A growable array (dynamic vector)

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int    *data;
    size_t  size;
    size_t  capacity;
} Vector;

void vec_init(Vector *v) {
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

int vec_push(Vector *v, int value) {
    if (v->size == v->capacity) {
        size_t new_cap = v->capacity ? v->capacity * 2 : 4;
        int *tmp = realloc(v->data, new_cap * sizeof *tmp);
        if (tmp == NULL) return 0;
        v->data = tmp;
        v->capacity = new_cap;
    }
    v->data[v->size++] = value;
    return 1;
}

void vec_free(Vector *v) {
    free(v->data);
    v->data = NULL;
    v->size = v->capacity = 0;
}

int main(void) {
    Vector v;
    vec_init(&v);

    for (int i = 1; i <= 10; i++) vec_push(&v, i * i);

    for (size_t i = 0; i < v.size; i++) printf("%d ", v.data[i]);
    printf("\n");

    vec_free(&v);
    return 0;
}
```

### Allocating a string

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *duplicate(const char *src) {
    char *copy = malloc(strlen(src) + 1);    // +1 for '\0'
    if (copy == NULL) return NULL;
    strcpy(copy, src);
    return copy;                             // caller must free
}

int main(void) {
    char *s = duplicate("Hello heap");
    if (s) {
        printf("%s\n", s);
        free(s);
    }
    return 0;
}
```

### Stack vs heap

| | Stack | Heap |
|---|---|---|
| Managed by | compiler (automatic) | you (`malloc`/`free`) |
| Lifetime | until the function returns | until you `free` it |
| Size | small, limited | large |
| Speed | very fast | slower |
| Typical bug | stack overflow, dangling pointers | leaks, double free |

---

## 16. Dynamic 2D arrays

### Method 1: array of row pointers

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int rows = 3, cols = 4;

    int **m = malloc(rows * sizeof *m);
    if (m == NULL) return 1;

    for (int r = 0; r < rows; r++) {
        m[r] = malloc(cols * sizeof **m);
        if (m[r] == NULL) {
            for (int k = 0; k < r; k++) free(m[k]);   // clean up earlier rows
            free(m);
            return 1;
        }
    }

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            m[r][c] = r * cols + c;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) printf("%2d ", m[r][c]);
        printf("\n");
    }

    for (int r = 0; r < rows; r++) free(m[r]);   // free every row
    free(m);                                      // then the row array
    return 0;
}
```

### Method 2: one contiguous block (simpler to allocate and free)

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int rows = 3, cols = 4;

    int *m = malloc(rows * cols * sizeof *m);
    if (m == NULL) return 1;

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            m[r * cols + c] = r * cols + c;       // index = row * cols + col

    printf("%d\n", m[2 * cols + 3]);              // 11

    free(m);                                       // one free is enough
    return 0;
}
```

---

## 17. Common bugs

| Bug | Example | Consequence | Fix |
|-----|---------|-------------|-----|
| **Uninitialized pointer** | `int *p; *p = 1;` | crash / corruption | initialize to a valid address or `NULL` |
| **NULL dereference** | `int *p = NULL; *p = 1;` | segmentation fault | check `p != NULL` |
| **Dangling pointer** | return `&local`, or use after `free` | undefined behavior | don't return locals; set pointer to `NULL` after `free` |
| **Memory leak** | `malloc` without `free` | memory usage grows | free everything you allocate |
| **Double free** | `free(p); free(p);` | crash / corruption | set `p = NULL` after freeing |
| **Buffer overflow** | writing past an array's end | corruption / security hole | track sizes, use bounds checks |
| **Off-by-one** | `for (i = 0; i <= n; i++)` | out-of-bounds access | use `i < n` |
| **Wrong `sizeof`** | `malloc(sizeof(p))` where `p` is a pointer | allocates 8 bytes, not the array | `malloc(n * sizeof *p)` |
| **`sizeof` on a pointer parameter** | `sizeof(arr)` inside a function | pointer size, not array size | pass the length |
| **Lost original pointer** | `p = realloc(p, n)` | leak if `realloc` fails | use a temporary pointer |
| **Mismatched pointer types** | `int *p = &a_double;` | misread memory | use matching types |
| **Modifying a string literal** | `char *s = "hi"; s[0] = 'H';` | crash | use `char s[] = "hi";` |
| **Pointer arithmetic out of range** | `arr + 100` on a 5-element array | undefined behavior | stay within the array |
| **Freeing non-heap memory** | `free(&x)` or `free(arr)` for a local array | crash | only free what `malloc` returned |

### Tools that help

```bash
# Compile with warnings and debug info
gcc -Wall -Wextra -g -o program main.c

# Detect memory errors and leaks (Linux/macOS)
valgrind --leak-check=full ./program

# Or use AddressSanitizer (GCC/Clang)
gcc -fsanitize=address -g -o program main.c
./program
```

---

## 18. Practice examples

### Reverse an array in place using two pointers

```c
#include <stdio.h>

void reverse(int *arr, int n) {
    int *left  = arr;
    int *right = arr + n - 1;

    while (left < right) {
        int tmp = *left;
        *left   = *right;
        *right  = tmp;
        left++;
        right--;
    }
}

int main(void) {
    int data[] = {1, 2, 3, 4, 5};
    reverse(data, 5);

    for (int i = 0; i < 5; i++) printf("%d ", data[i]);   // 5 4 3 2 1
    printf("\n");
    return 0;
}
```

### Find the largest element (return a pointer)

```c
#include <stdio.h>

const int *find_max(const int *arr, int n) {
    if (n <= 0) return NULL;
    const int *best = arr;
    for (int i = 1; i < n; i++) {
        if (arr[i] > *best) best = &arr[i];
    }
    return best;
}

int main(void) {
    int data[] = {4, 17, 8, 2};
    const int *m = find_max(data, 4);
    if (m) printf("Max = %d at index %td\n", *m, m - data);   // Max = 17 at index 1
    return 0;
}
```

### Split a sentence into words with pointers

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char text[] = "pointers are powerful";   // must be a modifiable array

    char *word = strtok(text, " ");
    while (word != NULL) {
        printf("[%s]\n", word);
        word = strtok(NULL, " ");
    }
    return 0;
}
```

### Linked list: reverse it

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int          value;
    struct Node *next;
} Node;

Node *push_front(Node *head, int v) {
    Node *n = malloc(sizeof *n);
    if (n == NULL) return head;
    n->value = v;
    n->next  = head;
    return n;
}

Node *reverse_list(Node *head) {
    Node *prev = NULL;
    while (head != NULL) {
        Node *next = head->next;   // remember the rest
        head->next = prev;         // flip the link
        prev = head;               // advance prev
        head = next;               // advance head
    }
    return prev;
}

void print_list(const Node *head) {
    for (; head != NULL; head = head->next) printf("%d ", head->value);
    printf("\n");
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void) {
    Node *head = NULL;
    for (int i = 1; i <= 5; i++) head = push_front(head, i);   // 5 4 3 2 1

    print_list(head);
    head = reverse_list(head);
    print_list(head);                                          // 1 2 3 4 5

    free_list(head);
    return 0;
}
```

### Binary search with pointers

```c
#include <stdio.h>

const int *binary_search(const int *arr, int n, int target) {
    const int *lo = arr;
    const int *hi = arr + n - 1;

    while (lo <= hi) {
        const int *mid = lo + (hi - lo) / 2;
        if (*mid == target)      return mid;
        else if (*mid < target)  lo = mid + 1;
        else                     hi = mid - 1;
    }
    return NULL;
}

int main(void) {
    int sorted[] = {1, 3, 5, 7, 9, 11};
    const int *r = binary_search(sorted, 6, 7);

    if (r) printf("Found at index %td\n", r - sorted);   // 3
    else   printf("Not found\n");
    return 0;
}
```

---

## 19. Quick reference

```text
int x = 5;
int *p = &x;           p holds the address of x
*p                     value at that address (dereference)
*p = 10;               change x through p
int *p = NULL;         pointer to nothing

p + n                  move n ELEMENTS forward (not bytes)
p2 - p1                number of elements between pointers
arr[i]  ==  *(arr+i)   array indexing is pointer arithmetic

const int *p           can't change the data through p
int *const p           can't change where p points
const int *const p     neither

int **pp = &p;         pointer to pointer
void *vp;              generic pointer (cast before dereferencing)
int (*fp)(int, int);   pointer to function
typedef int (*Op)(int, int);   readable alias

p->member              same as (*p).member

malloc(n * sizeof *p)  allocate (check for NULL!)
calloc(n, sizeof *p)   allocate and zero
realloc(p, new_size)   resize (use a temp pointer)
free(p); p = NULL;     release and clear
```

---

## Key takeaways

- A pointer stores a memory address. `&` gets an address, `*` follows one.
- C passes everything by value; pass a pointer when a function must modify the caller's data.
- `p + 1` moves by one element, and `arr[i]` is the same as `*(arr + i)`.
- An array decays to a pointer when passed to a function, so also pass the length.
- Initialize every pointer, check for `NULL`, and never dereference freed memory.
- Every `malloc` needs exactly one matching `free`.
- Use `const` on pointer parameters that only read data.
- Use tools like `valgrind` or AddressSanitizer to catch memory bugs early.
