/*
 Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
èitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 20
#define ERROR_OPENING_FILE -1
#define EXIT_SUCCESS 0   
#define EXIT_FAILURE -1

struct _element;
typedef struct _element* position;

typedef struct _element {
    int coeff;
    int exp;

    position next;
}element;

int readFile(char* file, position head);
int insertSorted(position head, position newElement);
int createElement(position head, int coeff, int exp);
int deleteElement(position head, position temp);
position findPrev(position head, position elementToDelete);
int add(position prvi, position drugi, position head);
int multiply(position prvi, position drugi, position head);
int insertInFile(const char* file, position head);
void free_list(position head);


int main()
{
    position head1 = NULL;
    head1 = (position)malloc(sizeof(element));
    if (head1 == NULL)
    {
        printf("Pogreska prilikom alokacije memorije\n");
        return ERROR_OPENING_FILE;
    }
    head1->next = NULL;

	position head2 = NULL;
	head2 = (position)malloc(sizeof(element));
	if (head2 == NULL)
	{
		printf("Pogreska prilikom alokacije memorije\n");
		return ERROR_OPENING_FILE;
	}
	head2->next = NULL;

	position head3 = NULL;
	head3 = (position)malloc(sizeof(element));
	if (head3 == NULL)
	{
		printf("Pogreska prilikom alokacije memorije\n");
		return ERROR_OPENING_FILE;
	}
	head3->next = NULL;

	position head4 = NULL;
	head4 = (position)malloc(sizeof(element));
	if (head4 == NULL)
	{
		printf("Pogreska prilikom alokacije memorije\n");
		return ERROR_OPENING_FILE;
	}
	head4->next = NULL;

    const char* file1 = "polinomi1.txt";
	const char* file2 = "polinomi2.txt";
	const char* file3 = "polinomiPlus.txt";
	const char* file4 = "polinomiPuta.txt";

	readFile(file1, head1);
	printf("a");
	readFile(file2, head2);
	printf("b");
	
	add(head1, head2, head3);
	printf("c");
	insertInFile(file3, head3);
	printf("d");
	multiply(head1, head2, head4);
	printf("e");
	insertInFile(file4, head4);
	printf("f");

	free_list(head1);
	free_list(head2);
	free_list(head3);
	free_list(head4);
	
	return 0;
}


int insertSorted(position head, position newElement)
{
	position temp = head;

	//ako je dobro poredano
	while (temp->next && temp->next->exp < newElement->exp)
	{
		temp = temp->next;
	}

	//ako nije

	if (temp->next && temp->next->exp == newElement->exp)
	{
		temp->next->coeff += newElement->coeff;
		if (temp->next->coeff == 0) {
			deleteElement(head, temp->next);
		}
		free(newElement);
	}
	else
	{
		newElement->next = temp->next;
		temp->next = newElement;
	}
	return EXIT_SUCCESS;
}


int deleteElement(position head, position temp)
{
	position prev = findPrev(head, temp);
	if (prev == NULL || prev->next == NULL) {
		printf("Element nije pronaden.\n");
		return EXIT_FAILURE;
	}
	position elementToDelete = prev->next;

	prev->next = elementToDelete->next;
	free(elementToDelete);

	return EXIT_SUCCESS;
}

position findPrev(position head, position elementToDelete)
{
	position prev = head;
	position current = head->next;

	while ((current) && (current != elementToDelete))
	{
		prev = current;
		current = current->next;
	}
	if (current) return prev;
	return NULL;
}



int readFile(char* file, position head)
{
	FILE* datoteka = fopen(file, "r");
	if (!datoteka) {
		printf("Datoteka se ne moze otvoriti");
		return -1;
	}

	char buffer[1024];
	int coeff, exp;

	char* curr = buffer;

	while (fgets(buffer, sizeof(buffer), datoteka))
	{
		int numBytes = 2;

		int a = sscanf(buffer, " %d %d", &coeff, &exp);
		
		
			createElement(head, coeff, exp);
		
		curr = curr + numBytes;
	}
	return EXIT_SUCCESS;
}

int createElement(position head, int coeff, int exp)
{
	position newElement = NULL;
	newElement = (position)malloc(sizeof(element));
	if (newElement == NULL)
	{
		printf("pogreska prilikom alokacije memorije\n");
		return ERROR_OPENING_FILE;
	}
	newElement->next = head->next;
	head->next = newElement;

	newElement->coeff = coeff;
	newElement->exp = exp;
	newElement->next = NULL;

	insertSorted(head, newElement);

	return EXIT_SUCCESS;
}

int add(position prvi, position drugi, position head)
{
	position i = prvi;
	position j = drugi;

	int resultCoeff = 0;

	while (i != NULL)
	{
		while (j != NULL)
		{
			resultCoeff = i->coeff;
			if (i->exp == j->exp)
			{
				resultCoeff = resultCoeff + j->coeff;
				deleteElement(drugi, j);
			}
			j = j->next;
		}
		createElement(head, resultCoeff, i->exp);
		i = i->next;
		if (i == NULL)
		{
			while (j != NULL)
			{
				createElement(head, j->coeff, j->exp);
				j = j->next;
			}
		}
	}
	return EXIT_SUCCESS;
}

int multiply(position prvi, position drugi, position head)
{
	position i = prvi;
	position j = drugi;

	int resultCoeff;
	int resultExp;

	while (i != NULL)
	{
		while (j != NULL)
		{
			resultCoeff = i->coeff;
			resultExp = i->exp;
			resultCoeff = resultCoeff * j->coeff;
			resultExp = resultExp + j->exp;
			deleteElement(drugi, j);
			createElement(head, resultCoeff, resultExp);
			j = j->next;
		}
		i = i->next;
	}
	return EXIT_SUCCESS;
}
void free_list(position head)
{
	position x = NULL;
	while (head != NULL)
	{
		x = head;
		head = head->next;
		free(x);
	}

}


int insertInFile(const char* file, position head)
{
	FILE* datoteka = fopen(file, "w");
	if (!datoteka) {
		printf("Pogreška prilikom otvaranja datoteke za pisanje!\n");
		return EXIT_FAILURE;
	}

	position temp = head->next;
	if (!temp) {
		fprintf(datoteka, "Polinom je prazan.\n");
		fclose(datoteka);
		return EXIT_FAILURE;
	}
	while (temp)
	{
		fprintf(datoteka, "%d %d\n", temp->coeff, temp->exp); // Piše koeficijent i eksponent u svaki red
		temp = temp->next;
	}

	fprintf(datoteka, "\n");
	fclose(datoteka);
	return EXIT_SUCCESS;
}