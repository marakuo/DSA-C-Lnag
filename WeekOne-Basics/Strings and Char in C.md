# Strings and `char` in C

A practical guide to characters and strings in the C language, with runnable examples.

## Table of Contents

1. [The `char` type](#1-the-char-type)
2. [Character functions (`ctype.h`)](#2-character-functions-ctypeh)
3. [What is a string in C?](#3-what-is-a-string-in-c)
4. [Declaring and initializing strings](#4-declaring-and-initializing-strings)
5. [Reading and printing strings](#5-reading-and-printing-strings)
6. [Looping through a string](#6-looping-through-a-string)
7. [String functions (`string.h`)](#7-string-functions-stringh)
8. [Strings and pointers](#8-strings-and-pointers)
9. [Converting strings and numbers](#9-converting-strings-and-numbers)
10. [Common mistakes](#10-common-mistakes)
11. [Practice examples](#11-practice-examples)
12. [Quick reference](#12-quick-reference)

---

## 1. The `char` type

`char` is a data type that stores **one character**. Internally it is a small integer (usually 1 byte, so 256 possible values) that maps to a character through the **ASCII** table.

- Characters are written with **single quotes**: `'A'`
- `'A'` is stored as the number `65`, `'a'` as `97`, `'0'` as `48`

```c
#include <stdio.h>

int main(void) {
    char letter = 'A';

    printf("As a character: %c\n", letter);   // A
    printf("As a number:    %d\n", letter);   // 65

    return 0;
}
```

### Format specifiers

| Specifier | Prints             |
|-----------|--------------------|
| `%c`      | the character      |
| `%d`      | its ASCII number   |

### Arithmetic on characters

Because `char` is a number, you can do math with it.

```c
#include <stdio.h>

int main(void) {
    char c = 'a';

    printf("%c\n", c + 1);        // b
    printf("%c\n", c - 32);       // A  (lowercase -> uppercase)
    printf("%d\n", '7' - '0');    // 7  (digit character -> integer value)

    // Print the alphabet
    for (char ch = 'a'; ch <= 'z'; ch++) {
        printf("%c ", ch);
    }
    printf("\n");

    return 0;
}
```

### Escape sequences

Special characters are written with a backslash:

| Escape | Meaning         |
|--------|-----------------|
| `\n`   | new line        |
| `\t`   | tab             |
| `\\`   | backslash       |
| `\'`   | single quote    |
| `\"`   | double quote    |
| `\0`   | null character (value 0) |

---

## 2. Character functions (`ctype.h`)

Include `<ctype.h>` for helpful functions that test or convert a single character.

| Function       | Purpose                                   |
|----------------|-------------------------------------------|
| `isalpha(c)`   | is it a letter?                           |
| `isdigit(c)`   | is it a digit (`0`-`9`)?                  |
| `isalnum(c)`   | letter or digit?                          |
| `isspace(c)`   | space, tab, newline, ...?                 |
| `isupper(c)`   | uppercase letter?                         |
| `islower(c)`   | lowercase letter?                         |
| `ispunct(c)`   | punctuation?                              |
| `toupper(c)`   | convert to uppercase                      |
| `tolower(c)`   | convert to lowercase                      |

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char c = 'g';

    if (isalpha(c)) {
        printf("%c is a letter\n", c);
    }
    if (islower(c)) {
        printf("Uppercase version: %c\n", toupper(c));   // G
    }
    if (isdigit('5')) {
        printf("5 is a digit\n");
    }

    return 0;
}
```

---

## 3. What is a string in C?

C has **no built-in `string` type**. A string is simply:

> an **array of `char`** that ends with the null character `'\0'`.

The `'\0'` (called the **null terminator**) tells functions like `printf` and `strlen` where the string ends.

The string `"Hello"` is stored in memory like this:

```
Index:   0    1    2    3    4    5
Value:  'H'  'e'  'l'  'l'  'o'  '\0'
```

So `"Hello"` has **5 characters** but needs **6 bytes** of storage.

> `'A'` (single quotes) is a **char**. `"A"` (double quotes) is a **string** made of `'A'` and `'\0'`.

---

## 4. Declaring and initializing strings

```c
#include <stdio.h>

int main(void) {
    // 1. From a string literal (size is calculated for you: 6 bytes)
    char s1[] = "Hello";

    // 2. With an explicit size (extra space is filled with '\0')
    char s2[20] = "Hello";

    // 3. Character by character (you MUST add '\0' yourself)
    char s3[] = {'H', 'e', 'l', 'l', 'o', '\0'};

    // 4. Empty buffer to fill in later
    char s4[50] = "";

    printf("%s\n%s\n%s\n", s1, s2, s3);
    printf("sizeof(s1) = %zu\n", sizeof(s1));   // 6
    printf("sizeof(s2) = %zu\n", sizeof(s2));   // 20

    return 0;
}
```

### Assigning after declaration

You **cannot** assign a string to an array with `=` after it is declared:

```c
char name[20];
name = "Ali";            // ERROR: arrays are not assignable
```

Use `strcpy` instead:

```c
#include <string.h>

char name[20];
strcpy(name, "Ali");     // OK
```

### Modifying a single character

```c
char word[] = "cat";
word[0] = 'b';           // now "bat"
```

---

## 5. Reading and printing strings

### Printing

```c
char name[] = "Sara";

printf("Name: %s\n", name);   // %s prints a string
puts(name);                   // prints the string followed by a newline
putchar('X');                 // prints a single character
```

### Reading a single character

```c
char c;
scanf(" %c", &c);   // the space before %c skips leftover whitespace/newlines
c = getchar();      // alternative: reads one character
```

### Reading a word with `scanf`

`scanf("%s", ...)` reads **until the first whitespace**, so it cannot read a full sentence. Always limit the width to avoid overflowing the buffer.

```c
char word[20];
scanf("%19s", word);   // reads at most 19 characters (+1 for '\0'); no & needed for arrays
```

### Reading a full line with `fgets` (recommended)

`fgets` reads a whole line, including spaces, and is safe because you give it the buffer size.

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char line[100];

    printf("Enter your full name: ");
    fgets(line, sizeof(line), stdin);

    // fgets keeps the '\n' at the end - remove it
    line[strcspn(line, "\n")] = '\0';

    printf("Hello, %s!\n", line);
    return 0;
}
```

> Never use `gets()`. It has no size limit, is unsafe, and was removed from the C standard.

---

## 6. Looping through a string

Since a string ends with `'\0'`, you can loop until you reach it.

```c
#include <stdio.h>

int main(void) {
    char text[] = "Hello";

    for (int i = 0; text[i] != '\0'; i++) {
        printf("text[%d] = %c\n", i, text[i]);
    }

    return 0;
}
```

### Example: count letters, digits, and spaces

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char text[] = "C is fun 2024";
    int letters = 0, digits = 0, spaces = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha((unsigned char)text[i]))      letters++;
        else if (isdigit((unsigned char)text[i])) digits++;
        else if (isspace((unsigned char)text[i])) spaces++;
    }

    printf("Letters: %d, Digits: %d, Spaces: %d\n", letters, digits, spaces);
    // Letters: 4, Digits: 4, Spaces: 3
    return 0;
}
```

---

## 7. String functions (`string.h`)

Include `<string.h>` to use these.

| Function                    | What it does                                      |
|-----------------------------|---------------------------------------------------|
| `strlen(s)`                 | length of the string (without `'\0'`)             |
| `strcpy(dest, src)`         | copy `src` into `dest`                            |
| `strncpy(dest, src, n)`     | copy at most `n` characters                       |
| `strcat(dest, src)`         | append `src` to the end of `dest`                 |
| `strncat(dest, src, n)`     | append at most `n` characters                     |
| `strcmp(a, b)`              | compare two strings                               |
| `strncmp(a, b, n)`          | compare the first `n` characters                  |
| `strchr(s, c)`              | find first occurrence of a character              |
| `strrchr(s, c)`             | find last occurrence of a character               |
| `strstr(s, sub)`            | find first occurrence of a substring              |

### `strlen` vs `sizeof`

```c
char s[20] = "Hello";

printf("%zu\n", strlen(s));   // 5  -> characters before '\0'
printf("%zu\n", sizeof(s));   // 20 -> total size of the array
```

### `strcpy` and `strcat`

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char first[30] = "Hello";
    char copy[30];

    strcpy(copy, first);          // copy = "Hello"
    strcat(copy, ", World");      // copy = "Hello, World"
    strcat(copy, "!");            // copy = "Hello, World!"

    printf("%s\n", copy);
    return 0;
}
```

The destination array must be **big enough** for the result (including `'\0'`), otherwise you get a buffer overflow.

### `strcmp` - comparing strings

Do **not** compare strings with `==` (that compares addresses, not content). Use `strcmp`:

| Return value | Meaning              |
|--------------|----------------------|
| `0`          | strings are equal    |
| `< 0`        | first is "smaller"   |
| `> 0`        | first is "larger"    |

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[] = "apple";
    char b[] = "apple";
    char c[] = "banana";

    if (strcmp(a, b) == 0) printf("a and b are equal\n");
    if (strcmp(a, c) < 0)  printf("\"apple\" comes before \"banana\"\n");

    return 0;
}
```

### `strchr` and `strstr` - searching

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char text[] = "Learning C is fun";

    char *p = strchr(text, 'C');       // pointer to 'C', or NULL if not found
    if (p != NULL) {
        printf("Found 'C' at index %ld\n", p - text);   // 9
    }

    char *q = strstr(text, "fun");     // pointer to start of "fun", or NULL
    if (q != NULL) {
        printf("Found \"fun\" at index %ld\n", q - text);   // 14
    }

    return 0;
}
```

### Safer copying

`strncpy` does **not** always add `'\0'`, so terminate manually:

```c
char dest[10];
strncpy(dest, "A very long sentence", sizeof(dest) - 1);
dest[sizeof(dest) - 1] = '\0';
printf("%s\n", dest);   // "A very lo"
```

Or use `snprintf`, which always terminates:

```c
snprintf(dest, sizeof(dest), "%s", "A very long sentence");
```

---

## 8. Strings and pointers

An array name decays to a pointer to its first element, so strings are often handled with `char *`.

```c
#include <stdio.h>

int main(void) {
    char  arr[] = "Hello";   // array: a modifiable copy stored on the stack
    char *ptr   = "Hello";   // pointer to a string literal (read-only memory)

    arr[0] = 'J';            // OK  -> "Jello"
    // ptr[0] = 'J';         // UNDEFINED BEHAVIOR: literals must not be modified

    printf("%s %s\n", arr, ptr);
    return 0;
}
```

Use `const char *` for literals so the compiler protects you:

```c
const char *greeting = "Hello";
```

### Walking a string with a pointer

```c
const char *p = "Hello";
while (*p != '\0') {
    putchar(*p);
    p++;
}
putchar('\n');
```

### Passing strings to functions

```c
#include <stdio.h>

void print_twice(const char *s) {
    printf("%s\n%s\n", s, s);
}

int main(void) {
    print_twice("C is great");
    return 0;
}
```

### Array of strings

```c
#include <stdio.h>

int main(void) {
    const char *days[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    for (int i = 0; i < 5; i++) {
        printf("%d: %s\n", i, days[i]);
    }
    return 0;
}
```

Or as a 2D `char` array (each row has a fixed size):

```c
char names[3][20] = {"Ahmed", "Malak", "Hossam"};
```

---

## 9. Converting strings and numbers

### String to number

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int    n = atoi("123");        // 123
    double d = atof("3.14");       // 3.14
    long   l = strtol("ff", NULL, 16);   // 255 (base 16)

    printf("%d %.2f %ld\n", n, d, l);
    return 0;
}
```

`strtol` / `strtod` are preferred over `atoi` / `atof` because they can detect errors.

### Number to string

```c
#include <stdio.h>

int main(void) {
    char buffer[50];
    int age = 20;

    snprintf(buffer, sizeof(buffer), "Age: %d", age);
    printf("%s\n", buffer);   // Age: 20
    return 0;
}
```

### Parsing a string with `sscanf`

```c
int day, month, year;
sscanf("25/12/2026", "%d/%d/%d", &day, &month, &year);
```

---

## 10. Common mistakes

| Mistake | Problem | Fix |
|---------|---------|-----|
| Forgetting space for `'\0'` | `char s[5] = "Hello";` has no room for the terminator | `char s[6]` or `char s[] = "Hello";` |
| `if (a == b)` on strings | compares addresses, not content | `strcmp(a, b) == 0` |
| `name = "Ali";` on an array | arrays can't be assigned | `strcpy(name, "Ali");` |
| `scanf("%s", s)` with no limit | buffer overflow | `scanf("%19s", s)` or `fgets` |
| Using `gets()` | unsafe, removed from C | `fgets` |
| Modifying a string literal | undefined behavior (crash) | use a `char[]` array |
| `'A'` vs `"A"` | char vs string | single quotes for char, double for string |
| Not removing `\n` after `fgets` | extra newline in your data | `s[strcspn(s, "\n")] = '\0';` |
| Destination too small in `strcpy`/`strcat` | buffer overflow | check size, or use `snprintf` |
| Not terminating after `strncpy` | missing `'\0'` | set the last byte to `'\0'` |

---

## 11. Practice examples

### Reverse a string

```c
#include <stdio.h>
#include <string.h>

void reverse(char s[]) {
    int left = 0;
    int right = (int)strlen(s) - 1;

    while (left < right) {
        char tmp = s[left];
        s[left]  = s[right];
        s[right] = tmp;
        left++;
        right--;
    }
}

int main(void) {
    char text[] = "Hello";
    reverse(text);
    printf("%s\n", text);   // olleH
    return 0;
}
```

### Check for a palindrome

```c
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_palindrome(const char *s) {
    int left = 0;
    int right = (int)strlen(s) - 1;

    while (left < right) {
        if (s[left] != s[right]) return false;
        left++;
        right--;
    }
    return true;
}

int main(void) {
    printf("%d\n", is_palindrome("level"));   // 1
    printf("%d\n", is_palindrome("hello"));   // 0
    return 0;
}
```

### Count vowels

```c
#include <stdio.h>
#include <ctype.h>

int count_vowels(const char *s) {
    int count = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)s[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main(void) {
    printf("%d\n", count_vowels("Programming in C"));   // 4
    return 0;
}
```

### Convert a string to uppercase

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
    char text[] = "hello world";

    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = (char)toupper((unsigned char)text[i]);
    }

    printf("%s\n", text);   // HELLO WORLD
    return 0;
}
```

### Write your own `strlen`

```c
#include <stdio.h>

size_t my_strlen(const char *s) {
    size_t n = 0;
    while (s[n] != '\0') n++;
    return n;
}

int main(void) {
    printf("%zu\n", my_strlen("Hello"));   // 5
    return 0;
}
```

---

## 12. Quick reference

```text
char c = 'A';              one character (single quotes)
char s[] = "Hi";           string = char array ending with '\0'
char s[50];                buffer that can hold a 49-character string

strlen(s)                  length (without '\0')
strcpy(d, s)               copy
strcat(d, s)               append
strcmp(a, b)               compare (0 means equal)
strchr(s, c)               find a character
strstr(s, sub)             find a substring

fgets(s, sizeof s, stdin)  safe line input
snprintf(s, size, ...)     safe formatted output into a string

printf("%c", c)            print a char
printf("%s", s)            print a string
```

### Compile and run

```bash
gcc -Wall -Wextra -o program main.c
./program
```

---

## Key takeaways

- A `char` is a 1-byte integer that represents a character (ASCII).
- A string is a `char` array terminated by `'\0'`.
- Always reserve room for `'\0'`.
- Compare strings with `strcmp`, never `==`.
- Prefer `fgets` and `snprintf` over `gets`, `scanf("%s")`, and `strcpy` when buffer size matters.
- String literals are read-only; use an array if you need to change the text.
