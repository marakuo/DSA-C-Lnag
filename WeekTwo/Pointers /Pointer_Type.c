#include <stdio.h>
/*
 * Pointers store memory addresses, so why are they strongly typed
 * instead of using a generic data type?
 *
 * Because pointers are not only used to store addresses but also
 * to access the objects at those addresses through dereferencing.
 * The pointed-to type tells the compiler how to interpret the memory
 * and how to perform pointer arithmetic.
 */


int main(void) {
    int a = 1025;
    // 00000000 00000000 00000010 00000001
    /*
     * 00000001 ==> int ptr covers 4 bytes ==> char ptr covers 1 byte
     * 00000010
     * 00000000
     * 00000000
     * *ptr = 1025 reads all 4 bytes
     * *ptrChar = 1 reads only 1 byte
     */
    int *ptr = &a;
    printf("the size of integer is %llu\n", sizeof(int));
    printf("the value of a is %d and the address is %d\n", *ptr, ptr);

    char *ptrChar = (char*)ptr; // typecasting
    printf("the size of char is %llu\n", sizeof(char));
    printf("the value is %d and the address is %d\n", *ptrChar, ptrChar); // 1 (1at byte) address the same 
    printf("checking the next value and address\n");
    printf("the value of ptr + 1 is %d and the address is  %d\n", *(ptr + 1), (ptr +1)); // skips 4 bytes after a so we get a garbage value and +4 address
    printf("the value of ptrChar + 1 is %d and the address is  %d\n", *(ptrChar + 1), (ptrChar +1)); // reads the 2nd byte 00000010 (4) and the +1 address 




    return 0;
}

