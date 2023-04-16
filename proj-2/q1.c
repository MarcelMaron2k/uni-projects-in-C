#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	int age;
	int freq;
}age;

void AllocationError(); // check if allocation was successful
void most_freq_age(age* arr, int len); // check the most frequent age and its frequency
age* doubled_age(age* arr, age temp, int* len); // check if there was an age written by the user already

int main()
{
	int cnt = 1, len = 0, flag = 0;
	age temp, *arr = NULL;
	printf("Enter pairs of integers(0 0 to finish):\n");
	while (1)
	{
		printf("%d (age frequency): ", cnt);
		scanf("%d %d", &temp.age, &temp.freq); // gets the values from user
		while (temp.age == 0 || temp.freq == 0)
		{
			if (temp.age == 0 && temp.freq == 0) // the user want to finish the program
			{
				flag = 1;
				break;
			}
			printf("you entered 0 at the age or frequency, try again\n");
			printf("%d (age frequency): ", cnt);
			scanf("%d %d", &temp.age, &temp.freq);
		}
		if (flag == 1)
		{
			if (cnt == 1)
			{
				printf("No data inserted!");
				return 0;
			}
			break;
		}
		arr = doubled_age(arr, temp, &len);
		cnt++;
	}
	most_freq_age(arr, len);
	return 0;
}

void AllocationError()
{
	printf("Allocation failed!!!");
	exit(1);
}

age* doubled_age(age* arr, age temp, int* len)
{
	int i;
	age* p;
	if (len == 0) 
	{
		arr = (age*)malloc(sizeof(age));
		if (!arr)
			AllocationError();
		arr[0].age = temp.age;
		arr[0].freq = temp.freq;
	}
	for (i = 0; i < *len; i++) // check if the age was already written by user
	{
		if (temp.age == arr[i].age)
		{
			arr[i].freq += temp.freq;
			return arr;
		}
	}
	(*len)++;
	p = (age*)realloc(arr, sizeof(age) * (*len)); // allocation new memory with one more space 
	if (p != NULL)
	{
		arr = p;
		arr[i].age = temp.age;
		arr[i].freq = temp.freq;
	}
	else
	{
		free(arr);
		AllocationError();
	}
	return arr;
}

void most_freq_age(age* arr, int len)
{
	int max_freq = 0, i, j = 0;
	int* p, * arr_age = NULL;
	for (i = 0; i < len; i++) // gets the biggest frequency
	{
		if (arr[i].freq >= max_freq)
			max_freq = arr[i].freq;
	}
	arr_age = (int*)malloc(sizeof(int)); // new array for all the frequent ages
	if (arr_age == NULL)
		AllocationError();
	for (i = 0; i < len; i++)
	{
		if (arr[i].freq == max_freq)
		{
			p = (int*)realloc(arr_age, (j + 1) * sizeof(int)); // new space for the most frequent ages
			if (p != NULL)
			{
				arr_age = p;
				arr_age[j++] = arr[i].age;
			}
			else
			{
				free(arr_age);
				AllocationError();
			}
		}
	}
	for (i = 0; i < j; i++) // prints the final result
		printf("The most frequent age is %d which appears %d times\n", arr_age[i], max_freq);
	free(arr);
	free(arr_age);
}