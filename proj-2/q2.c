#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 5

// defining struct
typedef struct
{
	char code[11]; // taking into condiseration the '\0' char
	char* name; //max 20
	int fridaySales;
	int yearlySales;
}shop;

void PutValues(shop arr[N], shop* p_arr[N]); // sets the values from user
//finds the proper indexes
void find_index(shop arr[N], int* maxFridayIndex, int* minFridayIndex, int* maxYearlyIndex, int* minYearlyIndex);
void printing(shop* p_arr[N]); // printings final result

int main()
{
	int i, j, maxFridayIndex = 0, minFridayIndex = 0, maxYearlyIndex = 0, minYearlyIndex = 0; // initialize array.
	shop arr[N];
	shop* p_arr[N];
	PutValues(arr, p_arr);
	find_index(arr, &maxFridayIndex, &minFridayIndex, &maxYearlyIndex, &minYearlyIndex);
	p_arr[0] = &arr[maxFridayIndex];  // Set max friday sale into pointer array.
	p_arr[1] = &arr[minFridayIndex]; // Set min friday sale into pointer array.
	p_arr[N - 1] = &arr[maxYearlyIndex]; // Set max yearly sale into pointer array.
	p_arr[N - 2] = &arr[minYearlyIndex]; // Set min yearly sale into pointer array.
	printing(p_arr);
	for (i = 0; i < N; i++) // free the data we allocated before.
		free(arr[i].name);
	return 0;
}

void PutValues(shop arr[N], shop* p_arr[N])
{
	int i, j;
	char temp_name[21]; // taking into condiseration the '\0' char
	// get user input
	for (i = 0; i < N; i++)
	{
		p_arr[i] = &arr[i];
		printf("Department %d: \n", i + 1);
		printf("Enter shop code (max 10 chars): ");
		scanf("%s", arr[i].code);
		rewind(stdin); // make sure a rogue \n does not mess with input
		printf("Enter shop name (max 20 chars): ");
		scanf("%s", temp_name);
		arr[i].name = (char*)malloc(strlen(temp_name) * sizeof(char) + 1); // allocate memory for string 
		if (!arr[i].name) 
		{
			printf("Error allocating memory!");
			exit(1);
		}
		strcpy(arr[i].name, temp_name); // copy string from temp_name into struct 
		while (1) // make sure yearly sales is not less than black friday sales.
		{
			printf("Enter shop's Black Friday Sale: ");
			scanf("%d", &arr[i].fridaySales);
			printf("Enter shop's yearly sales: ");
			scanf("%d", &arr[i].yearlySales);
			if (arr[i].yearlySales > arr[i].fridaySales)
				break;
			printf("Yearly sales can not be lower than Black Friday sales!\n");
		}
		printf("\n");
	}
}

void find_index(shop arr[N], int* maxFridayIndex, int* minFridayIndex, int* maxYearlyIndex, int* minYearlyIndex)
{
	int i;
	// find the department with the max and min sales per year and in black friday.
	for (i = 1; i < N; i++) {
		if (arr[i].fridaySales > arr[*maxFridayIndex].fridaySales) // find max friday sale
			*maxFridayIndex = i;
		if (arr[i].fridaySales < arr[*minFridayIndex].fridaySales) // find min friday sale
			*minFridayIndex = i;
		if (arr[i].yearlySales > arr[*maxYearlyIndex].yearlySales) // find max friday sale
			*maxYearlyIndex = i;
		if (arr[i].yearlySales < arr[*minYearlyIndex].yearlySales) // find min friday sale
			*minYearlyIndex = i;
	}
}

void printing(shop* p_arr[N])
{
	printf("Best year sales department: \n"); // print results
	printf("Department code: %s\n", p_arr[N - 1]->code);
	printf("Department name: %s\n\n", p_arr[N - 1]->name);
	printf("Worst year sales department: \n");
	printf("Department code: %s\n", p_arr[N - 2]->code);
	printf("Department name: %s\n", p_arr[N - 2]->name);
}