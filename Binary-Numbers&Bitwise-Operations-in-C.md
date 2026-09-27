# Binary Numbers & Bitwise Operations in C

A beginner-friendly guide to understanding how **positive and negative numbers are represented in C**, how **1's and 2's complements** work, and how to use **bitwise operations and bit masking**.

---

## Table of Contents

* [1. Binary Representation](#1-binary-representation)
* [2. Positive Numbers](#2-positive-numbers)
* [3. Negative Numbers](#3-negative-numbers)
* [4. 1's Complement](#4-1s-complement)
* [5. 2's Complement](#5-2s-complement)
* [6. Why Do We Use 2's Complement?](#6-why-do-we-use-2s-complement)
* [7. Bitwise Operators in C](#7-bitwise-operators-in-c)
* [8. Bitwise AND](#8-bitwise-and)
* [9. Bitwise OR](#9-bitwise-or)
* [10. Bitwise XOR](#10-bitwise-xor)
* [11. Bitwise NOT](#11-bitwise-not)
* [12. Left Shift](#12-left-shift)
* [13. Right Shift](#13-right-shift)
* [14. Bit Masking](#14-bit-masking)
* [15. Checking a Bit](#15-checking-a-bit)
* [16. Setting a Bit](#16-setting-a-bit)
* [17. Clearing a Bit](#17-clearing-a-bit)
* [18. Toggling a Bit](#18-toggling-a-bit)
* [19. Useful Bit Tricks](#19-useful-bit-tricks)

---

# 1. Binary Representation

Computers store integers as **bits**.

A bit can have only two values:

```text
0
1
```

For example, the decimal number `13` can be represented in binary as:

```text
13 = 1101₂
```

Because:

```text
1 × 2³ + 1 × 2² + 0 × 2¹ + 1 × 2⁰

= 8 + 4 + 0 + 1

= 13
```

---

# 2. Positive Numbers

Positive integers are represented using ordinary binary notation.

For example, using 8 bits:

```text
5 = 00000101
```

```text
25 = 00011001
```

The leading zeros don't change the value.

For example:

```text
00000101
     ↑
```

is still `5`.

---

# 3. Negative Numbers

Negative integers need a way to be represented using bits.

Modern computers commonly use **2's complement** to represent signed integers.

For example, using 8 bits:

```text
+5 = 00000101
```

The 2's complement representation of `+5` gives:

```text
-5 = 11111011
```

So, with 8 bits:

```text
+5 → 00000101
-5 → 11111011
```

The most significant bit (MSB) is used as the **sign bit** in signed representations:

```text
0 → positive
1 → negative
```

---

# 4. 1's Complement

The **1's complement** is obtained by flipping every bit.

```text
0 → 1
1 → 0
```

Example:

```text
10101110
```

Flip every bit:

```text
01010001
```

Therefore:

```text
1's complement of 10101110
= 01010001
```

### C Example

The bitwise NOT operator `~` flips every bit:

```c
#include <stdio.h>

int main(void)
{
    unsigned char x = 0b10101110;

    unsigned char result = ~x;

    printf("%u\n", result);

    return 0;
}
```

> Note: `~` operates on the integer type after C's integer promotions, so when discussing an exact 8-bit pattern, be careful about the type and the number of bits you're displaying.

---

# 5. 2's Complement

The **2's complement** can be calculated in two common ways.

## Method 1: 1's Complement + 1

Example:

```text
10101110
```

### Step 1 — Find the 1's complement

```text
10101110
↓
01010001
```

### Step 2 — Add 1

```text
01010001
       +1
---------
01010010
```

Therefore:

```text
2's complement = 01010010
```

---

## Method 2: Start From the Right

You don't actually need to explicitly calculate the 1's complement.

Starting from the **rightmost bit**:

1. Copy bits until and including the first `1`.
2. Flip all bits to the left.

Example:

```text
10101110001000
```

Starting from the right:

```text
10101110001000
             ↑
```

Copy the rightmost `1` and everything after it.

Then flip the remaining bits:

```text
10101110001000
↓
01010001111000
```

Therefore:

```text
2's complement = 01010001111000
```

This method is often much faster when solving by hand.

---

# 6. Why Do We Use 2's Complement?

2's complement gives computers a convenient way to represent both positive and negative integers using the same binary arithmetic.

For example, using 4 bits:

```text
+5 = 0101
```

To represent `-5`:

```text
0101
↓ flip
1010
↓ +1
1011
```

Therefore:

```text
-5 = 1011
```

Now look at what happens when we add them:

```text
  0101   (+5)
+ 1011   (-5)
------
 10000
```

With only 4 bits, the extra carry is discarded:

```text
0000
```

So:

```text
5 + (-5) = 0
```

This is one of the main reasons 2's complement is useful.

---

# 7. Bitwise Operators in C

C provides several operators that work directly on bits.

| Operator | Name        | Purpose                   |
| -------- | ----------- | ------------------------- |
| `&`      | AND         | Compare bits              |
| `\|`     | OR          | Set bits                  |
| `^`      | XOR         | Toggle/detect differences |
| `~`      | NOT         | Flip bits                 |
| `<<`     | Left Shift  | Shift bits left           |
| `>>`     | Right Shift | Shift bits right          |

Example:

```c
unsigned int a = 12;
unsigned int b = 10;

printf("%u\n", a & b);
printf("%u\n", a | b);
printf("%u\n", a ^ b);
```

---

# 8. Bitwise AND

The AND operator `&` compares two bits.

The result is `1` **only when both bits are 1**.

```text
A B | A & B
----+------
0 0 |   0
0 1 |   0
1 0 |   0
1 1 |   1
```

Example:

```text
  1100
& 1010
------
  1000
```

Therefore:

```text
12 & 10 = 8
```

### C

```c
unsigned int a = 12;
unsigned int b = 10;

unsigned int result = a & b;

printf("%u\n", result);
```

---

# 9. Bitwise OR

The OR operator `|` produces `1` when **at least one bit is 1**.

```text
A B | A | B
----+------
0 0 |   0
0 1 |   1
1 0 |   1
1 1 |   1
```

Example:

```text
  1100
| 1010
------
  1110
```

Therefore:

```text
12 | 10 = 14
```

### C

```c
unsigned int result = 12 | 10;

printf("%u\n", result);
```

---

# 10. Bitwise XOR

XOR means **exclusive OR**.

The result is `1` when the two bits are different.

```text
A B | A ^ B
----+------
0 0 |   0
0 1 |   1
1 0 |   1
1 1 |   0
```

Example:

```text
  1100
^ 1010
------
  0110
```

Therefore:

```text
12 ^ 10 = 6
```

XOR is particularly useful for **toggling bits**.

---

# 11. Bitwise NOT

The NOT operator `~` flips every bit.

```text
0 → 1
1 → 0
```

Example:

```text
  00001100
~
  --------
  11110011
```

In C:

```c
unsigned int x = 12;

unsigned int result = ~x;
```

### Important

`~` works on the entire integer type.

For example, if `x` is a 32-bit unsigned integer:

```text
00000000 00000000 00000000 00001100
```

becomes:

```text
11111111 11111111 11111111 11110011
```

So the number of bits matters when interpreting the result.

---

# 12. Left Shift

The left shift operator is:

```c
<<
```

Example:

```c
unsigned int x = 5;

unsigned int result = x << 1;
```

Binary:

```text
00000101
   ↓ shift left
00001010
```

Therefore:

```text
5 << 1 = 10
```

Another example:

```text
5 << 2
```

```text
00000101
↓
00010100
```

```text
5 << 2 = 20
```

For unsigned integers, shifting left by one position is equivalent to multiplying by 2 when no significant bit is lost.

```text
x << 1 ≈ x × 2
x << 2 ≈ x × 4
x << 3 ≈ x × 8
```

---

# 13. Right Shift

The right shift operator is:

```c
>>
```

Example:

```c
unsigned int x = 20;

unsigned int result = x >> 2;
```

Binary:

```text
00010100
↓
00000101
```

Therefore:

```text
20 >> 2 = 5
```

For unsigned integers:

```text
x >> 1 ≈ x / 2
x >> 2 ≈ x / 4
x >> 3 ≈ x / 8
```

---

# 14. Bit Masking

A **bit mask** is a value used with bitwise operations to access or modify specific bits.

For example:

```text
x = 10110110
```

Suppose we only care about the last 4 bits.

Create a mask:

```text
mask = 00001111
```

Then:

```text
  10110110
& 00001111
----------
  00000110
```

The unwanted bits were cleared.

In C:

```c
unsigned int x = 0b10110110;
unsigned int mask = 0b00001111;

unsigned int result = x & mask;
```

Result:

```text
00000110
```

---

# 15. Checking a Bit

Suppose we want to check whether **bit 3** is `1`.

Remember that bit positions usually start from `0` on the right:

```text
Bit:   7 6 5 4 3 2 1 0
       ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
Value: 1 0 1 1 0 1 1 0
```

Create a mask with a `1` at bit 3:

```text
00001000
```

This can be generated in C using:

```c
1 << 3
```

Then:

```c
if (x & (1 << 3))
{
    printf("Bit 3 is ON\n");
}
else
{
    printf("Bit 3 is OFF\n");
}
```

The important idea is:

```c
x & (1 << position)
```

checks whether a particular bit is set.

---

# 16. Setting a Bit

**Setting a bit** means changing it to `1`.

Suppose:

```text
x = 10100100
```

We want to set bit 1.

Create a mask:

```text
00000010
```

Use OR:

```text
  10100100
| 00000010
----------
  10100110
```

### C

```c
x = x | (1 << 1);
```

or simply:

```c
x |= (1 << 1);
```

Now bit 1 is guaranteed to be `1`.

---

# 17. Clearing a Bit

**Clearing a bit** means changing it to `0`.

Suppose:

```text
x = 10100110
```

We want to clear bit 1.

Create the mask:

```text
00000010
```

Invert it:

```text
11111101
```

Then use AND:

```text
  10100110
& 11111101
----------
  10100100
```

### C

```c
x &= ~(1 << 1);
```

This clears bit 1.

---

# 18. Toggling a Bit

**Toggling** means:

```text
0 → 1
1 → 0
```

XOR is perfect for this.

Suppose:

```text
x = 10100100
```

We want to toggle bit 1:

```text
  10100100
^ 00000010
----------
  10100110
```

### C

```c
x ^= (1 << 1);
```

If bit 1 was `0`, it becomes `1`.

If bit 1 was `1`, it becomes `0`.

---

# 19. Useful Bit Tricks

## Check if a number is odd

The least significant bit tells us whether an integer is odd or even.

```text
Even → last bit = 0
Odd  → last bit = 1
```

Therefore:

```c
if (x & 1)
    printf("Odd\n");
else
    printf("Even\n");
```

---

## Multiply by powers of 2

```c
x << 1   // x × 2
x << 2   // x × 4
x << 3   // x × 8
```

Example:

```c
int x = 7;

printf("%d\n", x << 2);
```

Output:

```text
28
```

---

## Divide unsigned integers by powers of 2

```c
x >> 1   // x / 2
x >> 2   // x / 4
x >> 3   // x / 8
```

Example:

```c
unsigned int x = 32;

printf("%u\n", x >> 3);
```

Output:

```text
4
```

---

## Turn a bit ON

```c
x |= (1 << n);
```

## Turn a bit OFF

```c
x &= ~(1 << n);
```

## Toggle a bit

```c
x ^= (1 << n);
```

## Check a bit

```c
x & (1 << n);
```

These four patterns are worth remembering:

```text
SET      → OR
CLEAR    → AND + NOT
TOGGLE   → XOR
CHECK    → AND
```

---

# Quick Summary

### 1's Complement

Flip every bit:

```text
0 → 1
1 → 0
```

### 2's Complement

Two equivalent methods:

```text
1's complement + 1
```

or:

```text
Copy from the right through the first 1,
then flip everything to its left.
```

### Bitwise Operators

```text
&   AND
|   OR
^   XOR
~   NOT
<<  Left Shift
>>  Right Shift
```

### Bit Manipulation

```c
// Check bit n
x & (1 << n)

// Set bit n
x |= (1 << n)

// Clear bit n
x &= ~(1 << n)

// Toggle bit n
x ^= (1 << n)
```

---

## Final Example

Here's a small program combining several concepts:

```c
#include <stdio.h>

int main(void)
{
    unsigned int x = 0b10100100;

    // Check bit 2
    if (x & (1 << 2))
        printf("Bit 2 is ON\n");
    else
        printf("Bit 2 is OFF\n");

    // Set bit 1
    x |= (1 << 1);

    // Clear bit 7
    x &= ~(1 << 7);

    // Toggle bit 3
    x ^= (1 << 3);

    return 0;
}
```

The important thing isn't memorizing the code immediately.

Understand the pattern:

```text
             What do I want?
                    │
       ┌────────────┼────────────┐
       ↓            ↓            ↓
     Check         Set         Clear
       │            │            │
       AND          OR        AND + NOT
       │            │            │
       └────────────┼────────────┘
                    ↓
              Bit Masking
```

---

## Further Topics to Explore

Once these concepts are comfortable, useful next topics include:

* Signed vs. unsigned integers
* Integer overflow
* Integer promotions
* Arithmetic vs. logical right shift
* Operator precedence with bitwise operators
* Extracting multiple bits
* Packing multiple values into one integer
* Flags and bit fields
* `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`
* Endianness
* Binary representation of floating-point numbers
* Two's complement overflow
* Bit manipulation in embedded systems

---

### Practice

Try solving these without a calculator:

1. Find the 1's complement of:

```text
10110010
```

2. Find the 2's complement of:

```text
10110010
```

3. Calculate:

```text
1101 & 1011
```

4. Calculate:

```text
1101 | 1011
```

5. Calculate:

```text
1101 ^ 1011
```

6. What is:

```c
5 << 2
```

7. What is:

```c
20 >> 2
```

8. Write C code to set bit `4`.

9. Write C code to clear bit `6`.

10. Write C code to check bit `2`.

11. Write C code to toggle bit `7`.

---

> **Core idea:** Bit manipulation is simply using binary operations to control individual bits or groups of bits inside an integer.
