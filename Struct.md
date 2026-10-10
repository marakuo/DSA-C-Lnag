# Structs in C

A detailed guide to `struct` in the C language: how to define, use, pass, store, and manage structured data, with runnable examples.

## Table of Contents

1. [What is a struct?](#1-what-is-a-struct)
2. [Defining a struct](#2-defining-a-struct)
3. [Creating and initializing variables](#3-creating-and-initializing-variables)
4. [Accessing members](#4-accessing-members)
5. [Strings inside structs](#5-strings-inside-structs)
6. [Copying and comparing structs](#6-copying-and-comparing-structs)
7. [Structs and functions](#7-structs-and-functions)
8. [Pointers to structs](#8-pointers-to-structs)
9. [Arrays of structs](#9-arrays-of-structs)
10. [Nested structs](#10-nested-structs)
11. [Dynamic memory with structs](#11-dynamic-memory-with-structs)
12. [Self-referential structs (linked list)](#12-self-referential-structs-linked-list)
13. [Memory layout, padding, and `sizeof`](#13-memory-layout-padding-and-sizeof)
14. [Unions and enums](#14-unions-and-enums)
15. [Bit fields](#15-bit-fields)
16. [Function pointers inside structs](#16-function-pointers-inside-structs)
17. [Common mistakes](#17-common-mistakes)
18. [Practice examples](#18-practice-examples)
19. [Quick reference](#19-quick-reference)

---

## 1. What is a struct?

An array groups values of the **same type**. A `struct` groups values of **different types** under one name.

Example: a student has a name (string), an ID (int), and a GPA (float). A struct lets you keep them together as one unit instead of three separate variables.

```
struct Student
+----------------------+
| name  : char[50]     |
| id    : int          |
| gpa   : float        |
+----------------------+
```

Each item inside a struct is called a **member** (or **field**).

---

## 2. Defining a struct

### Basic definition

```c
struct Student {
    char  name[50];
    int   id;
    float gpa;
};   // <-- don't forget the semicolon
```

This defines a **new type** called `struct Student`. It does not create a variable yet and uses no memory by itself.

### Using `typedef` to shorten the name

Without `typedef` you must write `struct Student` every time. `typedef` creates a shorter alias:

```c
typedef struct {
    char  name[50];
    int   id;
    float gpa;
} Student;

Student s1;   // no need to write "struct"
```

If the struct needs to refer to itself (see [linked lists](#12-self-referential-structs-linked-list)), give it a tag too:

```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;
```

---

## 3. Creating and initializing variables

```c
#include <stdio.h>
#include <string.h>

typedef struct {
    char  name[50];
    int   id;
    float gpa;
} Student;

int main(void) {
    // 1. Positional initialization (order must match the definition)
    Student a = {"Ali", 101, 3.5f};

    // 2. Designated initializers (C99) - any order, clearer
    Student b = {.id = 102, .name = "Sara", .gpa = 3.9f};

    // 3. Partial initialization - the rest is set to 0
    Student c = {.name = "Omar"};     // id = 0, gpa = 0.0

    // 4. Zero-initialize everything
    Student d = {0};

    // 5. Declare first, fill in later
    Student e;
    strcpy(e.name, "Nour");
    e.id  = 105;
    e.gpa = 3.2f;

    printf("%s %d %.1f\n", a.name, a.id, a.gpa);
    printf("%s %d %.1f\n", b.name, b.id, b.gpa);
    return 0;
}
```

> A local struct variable that is **not** initialized contains garbage values, just like any other local variable.

---

## 4. Accessing members

Use the **dot operator** `.` on a struct variable:

```c
Student s = {"Ali", 101, 3.5f};

printf("%s\n", s.name);
s.gpa = 3.8f;
s.id++;
```

Use the **arrow operator** `->` on a **pointer** to a struct (see section 8):

```c
Student *p = &s;
printf("%s\n", p->name);   // same as (*p).name
```

---

## 5. Strings inside structs

There are two ways to store text in a struct.

### Option A: `char` array (the struct owns the text)

```c
typedef struct {
    char name[50];
} Person;

Person p;
strcpy(p.name, "Ali");          // you cannot write p.name = "Ali";
```

- The text lives inside the struct.
- Copying the struct copies the text too.
- Text longer than the array overflows it, so use `snprintf` or `strncpy` for safety.

```c
snprintf(p.name, sizeof(p.name), "%s", "A very long name that might not fit");
```

### Option B: `char *` pointer (the struct points to text elsewhere)

```c
typedef struct {
    char *name;
} Person;

Person p;
p.name = "Ali";                 // OK: points to a string literal (read-only)
```

- It is lighter, but you must make sure the text outlives the struct.
- If you `malloc` the text, you must `free` it.
- Copying the struct copies only the **pointer**, so both structs share the same text (a *shallow copy*).

---

## 6. Copying and comparing structs

### Copying: assignment works

```c
Student a = {"Ali", 101, 3.5f};
Student b = a;        // copies every member
b.id = 999;           // does not affect a
```

This copies array members too (the `name[50]` bytes), but pointer members only copy the address.

### Comparing: `==` does NOT work

```c
if (a == b) { ... }    // ERROR: cannot compare structs with ==
```

Compare member by member:

```c
#include <string.h>

int same_student(const Student *x, const Student *y) {
    return strcmp(x->name, y->name) == 0
        && x->id  == y->id
        && x->gpa == y->gpa;
}
```

> `memcmp(&a, &b, sizeof a)` is not reliable, because padding bytes between members can contain different garbage even when the members are equal.

---

## 7. Structs and functions

### Pass by value (the function gets a copy)

```c
void print_student(Student s) {
    printf("%s (%d) GPA %.2f\n", s.name, s.id, s.gpa);
}

void try_to_change(Student s) {
    s.gpa = 4.0f;       // changes only the local copy
}
```

For big structs this copying is wasteful.

### Pass by pointer (the function works on the original)

```c
void set_gpa(Student *s, float gpa) {
    s->gpa = gpa;       // changes the caller's struct
}

void print_student_ptr(const Student *s) {   // const = read-only, no copy
    printf("%s (%d) GPA %.2f\n", s->name, s->id, s->gpa);
}
```

Use `const Student *` when the function only reads the struct.

### Returning a struct

```c
Student make_student(const char *name, int id, float gpa) {
    Student s;
    snprintf(s.name, sizeof(s.name), "%s", name);
    s.id  = id;
    s.gpa = gpa;
    return s;           // returned by value (a copy), which is safe
}

int main(void) {
    Student s = make_student("Sara", 102, 3.9f);
    print_student_ptr(&s);
    set_gpa(&s, 4.0f);
    print_student_ptr(&s);
    return 0;
}
```

---

## 8. Pointers to structs

```c
Student s = {"Ali", 101, 3.5f};
Student *p = &s;

printf("%s\n", p->name);       // arrow operator
printf("%s\n", (*p).name);     // equivalent, but clumsier
p->id = 200;                   // modifies s
```

`p->member` is just shorthand for `(*p).member`.

---

## 9. Arrays of structs

```c
#include <stdio.h>

typedef struct {
    char  name[30];
    int   id;
    float gpa;
} Student;

int main(void) {
    Student class_list[3] = {
        {"Ali",   101, 3.5f},
        {"Sara",  102, 3.9f},
        {"Omar",  103, 3.1f}
    };

    int n = sizeof(class_list) / sizeof(class_list[0]);

    float sum = 0;
    for (int i = 0; i < n; i++) {
        printf("%d. %s - %.1f\n", class_list[i].id, class_list[i].name, class_list[i].gpa);
        sum += class_list[i].gpa;
    }
    printf("Average GPA: %.2f\n", sum / n);

    return 0;
}
```

### Sorting an array of structs with `qsort`

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char  name[30];
    float gpa;
} Student;

// Comparator: highest GPA first
int by_gpa_desc(const void *a, const void *b) {
    const Student *x = a;
    const Student *y = b;
    if (x->gpa < y->gpa) return 1;
    if (x->gpa > y->gpa) return -1;
    return 0;
}

int main(void) {
    Student list[] = {{"Ali", 3.5f}, {"Sara", 3.9f}, {"Omar", 3.1f}};
    int n = sizeof(list) / sizeof(list[0]);

    qsort(list, n, sizeof(Student), by_gpa_desc);

    for (int i = 0; i < n; i++) {
        printf("%s %.1f\n", list[i].name, list[i].gpa);
    }
    return 0;
}
```

---

## 10. Nested structs

A struct can contain other structs.

```c
#include <stdio.h>

typedef struct {
    int day, month, year;
} Date;

typedef struct {
    char name[30];
    Date birth;          // struct inside a struct
} Person;

int main(void) {
    Person p = {"Ali", {15, 6, 2007}};

    printf("%s was born on %d/%d/%d\n",
           p.name, p.birth.day, p.birth.month, p.birth.year);

    p.birth.year = 2006;

    Person *ptr = &p;
    printf("Year: %d\n", ptr->birth.year);   // chain . and ->
    return 0;
}
```

A struct **cannot contain itself** directly (infinite size), but it **can contain a pointer to its own type**.

---

## 11. Dynamic memory with structs

Allocate a single struct on the heap:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[30];
    int  id;
} Student;

int main(void) {
    Student *s = malloc(sizeof(*s));
    if (s == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    strcpy(s->name, "Ali");
    s->id = 101;
    printf("%s %d\n", s->name, s->id);

    free(s);
    s = NULL;
    return 0;
}
```

> `sizeof(*s)` is safer than `sizeof(Student)`: it stays correct if you change the type of `s`.

### Dynamic array of structs

```c
int n = 5;
Student *list = calloc(n, sizeof(Student));   // zero-initialized
if (list == NULL) return 1;

for (int i = 0; i < n; i++) {
    list[i].id = i + 1;
}

// Grow the array
Student *bigger = realloc(list, 10 * sizeof(Student));
if (bigger == NULL) {
    free(list);        // realloc failed; the old block is still valid
    return 1;
}
list = bigger;

free(list);
```

### Struct that owns heap memory

When a struct contains a pointer to allocated memory, you need create and destroy functions.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;      // allocated on the heap
    int   id;
} Student;

Student *student_create(const char *name, int id) {
    Student *s = malloc(sizeof(*s));
    if (s == NULL) return NULL;

    s->name = malloc(strlen(name) + 1);
    if (s->name == NULL) {
        free(s);
        return NULL;
    }
    strcpy(s->name, name);
    s->id = id;
    return s;
}

void student_destroy(Student *s) {
    if (s == NULL) return;
    free(s->name);   // free the inner allocation first
    free(s);         // then the struct itself
}

int main(void) {
    Student *s = student_create("Ali", 101);
    if (s != NULL) {
        printf("%s %d\n", s->name, s->id);
        student_destroy(s);
    }
    return 0;
}
```

---

## 12. Self-referential structs (linked list)

A struct can hold a **pointer** to another struct of the same type. This is the basis of linked lists, trees, and graphs.

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int          value;
    struct Node *next;
} Node;

// Add a new node at the front; returns the new head
Node *push_front(Node *head, int value) {
    Node *n = malloc(sizeof(*n));
    if (n == NULL) return head;
    n->value = value;
    n->next  = head;
    return n;
}

void print_list(const Node *head) {
    for (const Node *cur = head; cur != NULL; cur = cur->next) {
        printf("%d -> ", cur->value);
    }
    printf("NULL\n");
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
    head = push_front(head, 3);
    head = push_front(head, 2);
    head = push_front(head, 1);

    print_list(head);    // 1 -> 2 -> 3 -> NULL
    free_list(head);
    return 0;
}
```

Visual:

```
head --> [1 | *]--> [2 | *]--> [3 | NULL]
```

---

## 13. Memory layout, padding, and `sizeof`

Members are stored in the order you declare them. The compiler may insert **padding** bytes so each member sits at an address suitable for its type (alignment).

```c
#include <stdio.h>

struct A {
    char a;     // 1 byte
    int  b;     // 4 bytes
    char c;     // 1 byte
};

struct B {
    int  b;     // 4 bytes
    char a;     // 1 byte
    char c;     // 1 byte
};

int main(void) {
    printf("sizeof(struct A) = %zu\n", sizeof(struct A));   // typically 12
    printf("sizeof(struct B) = %zu\n", sizeof(struct B));   // typically 8
    return 0;
}
```

Typical layout of `struct A` on a platform with 4-byte `int` alignment:

```
offset: 0   1 2 3   4 5 6 7   8   9 10 11
        [a] [pad ]  [  b   ]  [c] [pad  ]
```

Tips:

- Declare larger members first to reduce padding.
- Always use `sizeof(struct ...)` instead of adding up member sizes by hand.
- Exact sizes depend on the compiler and platform.
- Use `offsetof(type, member)` from `<stddef.h>` to see where a member sits:

```c
#include <stddef.h>
printf("%zu\n", offsetof(struct A, b));   // typically 4
```

---

## 14. Unions and enums

### Union: members share the same memory

A `union` is declared like a struct, but all members occupy the **same** memory. Only one is valid at a time. Its size is the size of its largest member.

```c
#include <stdio.h>

union Value {
    int   i;
    float f;
    char  c;
};

int main(void) {
    union Value v;

    v.i = 65;
    printf("%d\n", v.i);       // 65

    v.f = 3.14f;               // overwrites the same memory
    printf("%.2f\n", v.f);     // 3.14
    // v.i is now meaningless

    printf("sizeof = %zu\n", sizeof(union Value));   // 4
    return 0;
}
```

### Tagged union: struct + enum + union

Keep track of which union member is currently valid:

```c
#include <stdio.h>

typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_CHAR } Type;

typedef struct {
    Type type;
    union {
        int   i;
        float f;
        char  c;
    } data;
} Variant;

void print_variant(const Variant *v) {
    switch (v->type) {
        case TYPE_INT:   printf("int: %d\n",   v->data.i); break;
        case TYPE_FLOAT: printf("float: %.2f\n", v->data.f); break;
        case TYPE_CHAR:  printf("char: %c\n",  v->data.c); break;
    }
}

int main(void) {
    Variant a = {TYPE_INT,   .data.i = 42};
    Variant b = {TYPE_FLOAT, .data.f = 2.5f};
    print_variant(&a);
    print_variant(&b);
    return 0;
}
```

### Enum: named integer constants

```c
typedef enum { RED, GREEN, BLUE } Color;   // 0, 1, 2

Color c = GREEN;
if (c == GREEN) printf("green\n");
```

---

## 15. Bit fields

A bit field lets you store small values in a specific number of bits, useful for flags and hardware registers.

```c
#include <stdio.h>

typedef struct {
    unsigned int is_active : 1;   // 1 bit  (0 or 1)
    unsigned int mode      : 3;   // 3 bits (0..7)
    unsigned int level     : 4;   // 4 bits (0..15)
} Flags;

int main(void) {
    Flags f = {0};
    f.is_active = 1;
    f.mode      = 5;
    f.level     = 12;

    printf("%u %u %u\n", f.is_active, f.mode, f.level);
    printf("sizeof = %zu\n", sizeof(Flags));   // typically 4
    return 0;
}
```

Notes: you cannot take the address of a bit field, and the layout is implementation-defined, so avoid using them for portable file or network formats.

---

## 16. Function pointers inside structs

A struct can hold pointers to functions, which is a simple way to build "objects" with behavior in C.

```c
#include <stdio.h>

typedef struct {
    const char *name;
    int (*operation)(int, int);
} Calculator;

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

int main(void) {
    Calculator ops[] = {
        {"add", add},
        {"mul", mul}
    };

    for (int i = 0; i < 2; i++) {
        printf("%s(3, 4) = %d\n", ops[i].name, ops[i].operation(3, 4));
    }
    return 0;
}
```

---

## 17. Common mistakes

| Mistake | Problem | Fix |
|---------|---------|-----|
| Missing `;` after the closing `}` of a struct | confusing compile errors | `struct X { ... };` |
| `s.name = "Ali";` on a `char[]` member | arrays can't be assigned | `strcpy` / `snprintf` |
| `if (a == b)` on structs | `==` is not defined for structs | compare member by member |
| Using `.` on a pointer (or `->` on a value) | compile error | `.` for values, `->` for pointers |
| Forgetting to `free` inner allocations | memory leak | free members first, then the struct |
| Shallow copy of a struct with `char *` | two structs share one buffer, possible double free | allocate and copy the text (deep copy) |
| Returning a pointer to a local struct | dangling pointer | return by value or use `malloc` |
| Using uninitialized local structs | garbage values | `= {0}` or designated initializers |
| `malloc(sizeof(ptr))` instead of `sizeof(*ptr)` | allocates only pointer size | `malloc(sizeof(*ptr))` |
| Adding member sizes to guess `sizeof` | ignores padding | use `sizeof(struct ...)` |
| Using `memcmp` to compare structs | padding bytes differ | compare members |

---

## 18. Practice examples

### Student database (menu-free demo)

```c
#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    char  name[30];
    int   id;
    float gpa;
} Student;

typedef struct {
    Student items[MAX];
    int     count;
} Database;

int add_student(Database *db, const char *name, int id, float gpa) {
    if (db->count >= MAX) return 0;
    Student *s = &db->items[db->count++];
    snprintf(s->name, sizeof(s->name), "%s", name);
    s->id  = id;
    s->gpa = gpa;
    return 1;
}

const Student *find_by_id(const Database *db, int id) {
    for (int i = 0; i < db->count; i++) {
        if (db->items[i].id == id) return &db->items[i];
    }
    return NULL;
}

const Student *best_student(const Database *db) {
    if (db->count == 0) return NULL;
    const Student *best = &db->items[0];
    for (int i = 1; i < db->count; i++) {
        if (db->items[i].gpa > best->gpa) best = &db->items[i];
    }
    return best;
}

int main(void) {
    Database db = {0};

    add_student(&db, "Ali",  101, 3.5f);
    add_student(&db, "Sara", 102, 3.9f);
    add_student(&db, "Omar", 103, 3.1f);

    const Student *s = find_by_id(&db, 102);
    if (s) printf("Found: %s\n", s->name);

    const Student *top = best_student(&db);
    if (top) printf("Top student: %s (%.1f)\n", top->name, top->gpa);

    return 0;
}
```

### 2D point and distance

```c
#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y;
} Point;

double distance(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

int main(void) {
    Point p1 = {0, 0};
    Point p2 = {3, 4};
    printf("Distance: %.2f\n", distance(p1, p2));   // 5.00
    return 0;
}
```

Compile with `gcc main.c -o main -lm` (the `-lm` links the math library).

### Stack using a struct

```c
#include <stdio.h>
#include <stdbool.h>

#define CAP 10

typedef struct {
    int data[CAP];
    int top;
} Stack;

void stack_init(Stack *s)           { s->top = 0; }
bool stack_empty(const Stack *s)    { return s->top == 0; }
bool stack_full(const Stack *s)     { return s->top == CAP; }

bool push(Stack *s, int v) {
    if (stack_full(s)) return false;
    s->data[s->top++] = v;
    return true;
}

bool pop(Stack *s, int *out) {
    if (stack_empty(s)) return false;
    *out = s->data[--s->top];
    return true;
}

int main(void) {
    Stack s;
    stack_init(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    int v;
    while (pop(&s, &v)) {
        printf("%d ", v);   // 30 20 10
    }
    printf("\n");
    return 0;
}
```

---

## 19. Quick reference

```text
struct Name { type member; ... };         define a struct type
typedef struct { ... } Name;              define with a short alias

Name v = {a, b, c};                       positional init
Name v = {.b = 2, .a = 1};                designated init (C99)
Name v = {0};                             zero everything

v.member                                  access via variable
p->member                                 access via pointer  ( = (*p).member )

Name b = a;                               copy (shallow for pointer members)
a == b                                    NOT allowed

void f(Name s);                           pass by value (copy)
void f(Name *s);                          pass by pointer (can modify)
void f(const Name *s);                    pass by pointer (read-only)

Name *p = malloc(sizeof(*p));             heap allocation
free(p);                                  release it

sizeof(Name)                              includes padding
offsetof(Name, member)                    member offset (<stddef.h>)

union U { ... };                          members share memory
enum E { A, B, C };                       named integer constants
```

---

## Key takeaways

- A struct groups related data of different types into one type.
- Use `.` with variables and `->` with pointers.
- Structs are copied on assignment and when passed or returned by value; pass a pointer to avoid the copy or to modify the original.
- Arrays inside a struct are copied with it; pointers inside a struct are not (shallow copy).
- Compare structs member by member, never with `==`.
- Whatever you `malloc` inside a struct, you must `free`.
- Padding can make `sizeof` larger than the sum of the members, so order members from largest to smallest.
