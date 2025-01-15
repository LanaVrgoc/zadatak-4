#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 20
#define ERROR_OPENING_FILE -1

typedef struct _polinom {
    int coeff;
    int exp;

    struct _polinom* next;
} Polinom;

typedef Polinom* Position;


int readPolynomialFromFile(const char* filename, Position head);
int insertSorted(Position head, int coefficient, int exponent);
int addPolynomials(Position p1, Position p2, Position result);
int multiplyPolynomials(Position p1, Position p2, Position result);
void printPolynomial(Position head);
int writePolynomialToFile(const char* filename, Position head);

int main() {
    Polinom p1 = { .coeff = 0, .exp = 0, .next = NULL };
    Polinom p2 = { .coeff = 0, .exp = 0, .next = NULL };
    Polinom sum = { .coeff = 0, .exp = 0, .next = NULL };
    Polinom product = { .coeff = 0, .exp = 0, .next = NULL };


    // Èitanje polinoma iz datoteka
    readPolynomialFromFile("polinomi1.txt", &p1);
    readPolynomialFromFile("polinomi2.txt", &p2);

    printf("Prvi polinom: ");
    printPolynomial(&p1);

    printf("Drugi polinom: ");
    printPolynomial(&p2);

    // Zbrajanje polinoma
    addPolynomials(p1.next, p2.next, &sum);
    printf("Zbroj polinoma: ");
    printPolynomial(&sum);
    writePolynomialToFile("polinomiPlus.txt", &sum);

    // Množenje polinoma
    multiplyPolynomials(p1.next, p2.next, &product);
    printf("Umnozak polinoma: ");
    printPolynomial(&product);
    writePolynomialToFile("polinomiPuta.txt", &product);

    return 0;
}


int readPolynomialFromFile(const char* filename, Position head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ne mogu otvoriti datoteku %s.\n", filename);
        return ERROR_OPENING_FILE;
    }

    int coefficient, exponent;
    while (fscanf(file, "%d %d", &coefficient, &exponent) == 2) {
        insertSorted(head, coefficient, exponent);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int insertSorted(Position head, int coefficient, int exponent) {
    Position newElement = (Position)malloc(sizeof(Polinom));
    if (!newElement) {
        printf("Greška pri alokaciji memorije!\n");
        return -1;
    }

    newElement->coeff = coefficient;
    newElement->exp = exponent;
    newElement->next = NULL;

    Position temp = head;
    while (temp->next && temp->next->exp > exponent) {
        temp = temp->next;
    }

    if (temp->next && temp->next->exp == exponent) {
        temp->next->coeff += coefficient;
        free(newElement);
        if (temp->next->coeff == 0) {
            Position toDelete = temp->next;
            temp->next = temp->next->next;
            free(toDelete);
        }
    }
    else {
        newElement->next = temp->next;
        temp->next = newElement;
    }

    return EXIT_SUCCESS;
}


int addPolynomials(Position p1, Position p2, Position result) {
    while (p1) {
        insertSorted(result, p1->coeff, p1->exp);
        p1 = p1->next;
    }
    while (p2) {
        insertSorted(result, p2->coeff, p2->exp);
        p2 = p2->next;
    }

    return EXIT_SUCCESS;
}


int multiplyPolynomials(Position p1, Position p2, Position result) {
    Position temp1 = p1;
    Position temp2 = p2;

    while (temp1) {
        temp2 = p2;
        while (temp2) {
            insertSorted(result, temp1->coeff * temp2->coeff, temp1->exp + temp2->exp);
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }

    return EXIT_SUCCESS;
}


void printPolynomial(Position head) {
    Position temp = head->next;
    while (temp) {
        printf("%d*x^%d ", temp->coeff, temp->exp);
        if (temp->next && temp->next->coeff > 0) {
            printf("+ ");
        }
        temp = temp->next;
    }
    printf("\n");
}

int writePolynomialToFile(const char* filename, Position head) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ne mogu otvoriti datoteku %s za pisanje.\n", filename);
        return ERROR_OPENING_FILE;
    }

    Position temp = head->next;
    while (temp) {
        fprintf(file, "%d %d\n", temp->coeff, temp->exp);
        temp = temp->next;
    }

    fclose(file);
    return EXIT_SUCCESS;
}
