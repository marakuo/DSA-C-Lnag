// example 1
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/*
 * Create a Book struct containing title,
 * author, and price. Create one book, initialize its values,
 * and print all its information.Concepts: Struct declaration, initialization, member access
 */
typedef struct {
    char title[50];
    char auther[50];
    float prize;
}Book;
void printBook(Book book) {
    printf("Title: %s\n", book.title);
    printf("Author: %s\n", book.auther);
    printf("Prize: %.2f\n", book.prize);
}

int main(void) {
    Book book[]={
        {"happy life", "malak", 120},
        {"the art of not giving a F", "someone", 300},
        {"math book", "john deep", 600}
    };
    int n=sizeof(book)/sizeof(book[0]);
    for (int i=0; i <n;i++) {
        printBook(book[i]);
        printf("\n");
    }

    return 0;
}

///////////////////////////////////////////////////////////////
//example 2
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/*
* 2. Rectangle Calculator
Easy
Create a Rectangle struct with width and height.
Write functions that calculate and return its area and perimeter.
Test them with two rectangles.Concepts: Struct parameters, return values, calculations
 */
typedef struct {
    float height, width;

}Rectangle;
void Area(Rectangle *rectangle) {
    float area= rectangle->height * rectangle->width;
    printf("Area of rectangle is %.2f\n", area);
}
void Perimeter(Rectangle *rectangle) {
    float perimeter = (rectangle->height + rectangle->width)*2;
    printf("perimeter of rectangle is %.2f\n", perimeter);
}


int main(void) {
    float height, width;
    printf("Enter the height and width: \n");
    scanf("%f %f", &height, &width);
    Rectangle rectangle = {height, width};
    Area(&rectangle);
    printf("\n");
    Perimeter(&rectangle);


    return 0;
}
/////////////////////////////////////////////////////////////////////////////
// example 3
#include <stdio.h>

/*
* [ ]
3. Student Array
Easy–Medium


Create a Student struct with name, age, and GPA.
Store 5 students in an array,
then print the student with the highest GPA.Concepts: Arrays of structs, loops, comparisons
 */

typedef struct {
    char name[50];
    int age;
    float GPA;

}Student;
void Mx_GPA(Student *student, int n) {
    float mx=student[0].GPA;
    int pos =0;
    for (int i =1 ; i<n ;i++) {
        if (student[i].GPA > mx) {
            mx=student[i].GPA;
            pos =i;
        }

    }
    printf("Student %s has the highest GPA OF: %.1f", student[pos].name, student[pos].GPA);
}

int main(void) {
    Student student[5]={
        {"Kakuryo", 20, 3.5 },
        {"Hannah", 21, 4},
        {"Jack LA", 22, 3.2},
        {"John Jee", 20, 2.9},
        {"Oliver Luka", 21, 3.8}
    };
    int n=sizeof(student)/sizeof(student[0]);
    Mx_GPA(student, n);




    return 0;
}

///////////////////////////////////////////////////////////////////////////
//example 4
#include <stdio.h>

/*
4. Update a Student
Medium

Create a Student struct with name and GPA.
Write a function that receives a student and changes the GPA to 4.0.
Call it, then print the original student's GPA. Explain what happens.Concepts: Pass by value, copying structs
 */

typedef struct {
    char name[50];
    float GPA;

}Student;
void changeGPA(Student *student) {
    student->GPA =4.0;
    printf("Student GPA has been changed to %.2f", student->GPA);
}


int main(void) {
    Student student={"John Mk", 3.5};
    printf("Student GPA before the change is %.2f\n", student.GPA);
    changeGPA(&student);






    return 0;
}

//////////////////////////////////////////////////////////////////
//example 6
#include <stdio.h>

/*
* 5. Find a Product
Medium


Create a Product struct with ID, name, price, and quantity.
Store several products in an array.
Find and print the product with the lowest price.
Concepts: Struct arrays, searching, conditions
 */

typedef struct {
    char name[50];
    float price;
    int id, quantity;


}Product;
void LowestPrice(Product *product, int n) {
    float mn=product[0].price;
    int pos =0;
    for (int i = 0; i < n; i++) {
        if (product[i].price < mn) {mn =product[i].price; pos =i;}
    }
    printf("The product of the lowest price is %s and it costs %.2f", product[pos].name, product[pos].price);
}



int main(void) {
    Product product[]={
        {"Fridge", 200000, 1234, 4},
        {"Air fryer", 50000, 2232, 10},
        {"Smart Phone", 110000, 1009, 3},
        {"Tableta", 100000, 1401, 20}

    };
    int n = sizeof(product)/sizeof(product[0]);
    LowestPrice(product, n);






    return 0;
}

//////////////////////////////////////////////////

