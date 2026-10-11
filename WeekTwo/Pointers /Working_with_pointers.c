#include <stdio.h>



int main(void) {
    int value = 100;
    int *ptr = &value;
    printf("printf(value) = %d\n", value); // 100
    printf("printf(&value) = %p\n", &value); // address
    printf("printf(ptr) = %d\n", ptr); // address
    printf("printf(ptr+1) = %d\n", ptr + 1); // address
    printf("printf(*ptr) = %d\n", *ptr); // 100
    printf("printf(&ptr) = %p\n", &ptr); // pointer address
    *ptr = 200; // dereference of value
    printf("value = %d\n", value);
    printf("/////////\n");
    int b =10;
    printf("the address of value is %p\n", &value);
    printf("the address of b is %p\n", &b);
    *ptr =b;
    // the pointer to value just updates the value to b but doesn't change the address
    printf("b = %d\n", b);
    printf("value = %d\n", value);
    printf("printf(ptr) = %p\n", ptr);
    printf("printf(*ptr) = %d\n", *ptr);
    printf("printf(*(ptr +1)) = %d\n", *(ptr +1)); // garbage 
    return 0;
}

