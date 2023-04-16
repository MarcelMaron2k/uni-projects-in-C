#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX 99
#define LAB 5

typedef struct student
{
	char* name;
	long id;
	float grade;
	char lab[LAB + 1];
	char fin;
}student;

typedef struct university
{
	int num_std;
	student* arr;
}university;

float FinalGrade(student* std); // calculate the final grades
void GetData(FILE* in, university* uni); // gets the data from the file into sturcts
void PrintData(FILE* out, university* uni); // prints the data into file
void PrintFinalGrade(FILE* out, university* uni); // prints the final course grades into file
void Error_Msg(char* str); // send an error msg to the user
void InsertStudent(university* uni); // insert new student to university struct
void AddBonusGrade(FILE* out, university* uni, char* name, int bonus); // adds bonus to grade
void MyFree(university* uni); // free all allocated mem

int main()
{
	int user_input = 0, flag = 0, bonus;
	char tempname[MAX + 1];
	FILE* in, * out;
	university braude;
	out = fopen("output.txt", "w");
	if (!out)
		Error_Msg("couldn't open the file!!!");

	while (user_input != 6 || flag == 0){
		if (flag == 0) {
			printf("Please enter '1' first to read student data from input.txt\n");
		}
		else
		{
			printf("Choose options 2-6:\n");
			printf("2. Write student data to output.txt\n");
			printf("3. Calculate final grade and write data to output.txt\n");
			printf("4. Insert new student\n");
			printf("5. Give bonus grade for difficult question\n");
			printf("6. Run end of program sequence\n");
		}
		scanf("%d", &user_input);
		if (user_input != 1 && !flag) { // check if user chose option 1 firstly
			printf("You must choose 1 first!\n");
			continue;
		}

		switch (user_input) {

		case 1:
			if (flag) { // check if user already chose option 1
				printf("You already chose 1...\n");
				break;
			}
			flag = 1;
			in = fopen("input.txt", "r");
			if (!in)
				Error_Msg("couldn't open the file!!!");
			GetData(in, &braude);
			fclose(in);
			break;

		case 2:
			fprintf(out, "Option 2:\n");
			PrintData(out, &braude);
			break;

		case 3:
			fprintf(out, "Option 3:\n");
			PrintFinalGrade(out, &braude);
			break;

		case 4:
			fprintf(out, "Option 4:\n");
			InsertStudent(&braude);
			break;

		case 5:
			fprintf(out, "Option 5:\n");
			printf("Enter student name: ");
			scanf("%s", tempname);
			printf("Enter bonus: ");
			scanf("%d", &bonus);
			AddBonusGrade(out, &braude, tempname, bonus);
			break;
		case 6:
			fprintf(out, "Option 6:\nEnd Of Program");
			break;
		}
	}
	MyFree(&braude);
	fclose(out);
	return 0;
}

void AddBonusGrade(FILE* out, university* uni, char* name, int bonus) {
	int i, flag = 0;

	for (i = 0; i < uni->num_std; i++) {
		if (!strcmp(uni->arr[i].name, name)) {// seacthing for the wanted student and add bonus to its grade
			uni->arr[i].grade += bonus;
			uni->arr[i].grade = uni->arr[i].grade >= 100 ? 100 : uni->arr[i].grade;
			flag = 1;
			break;
		}
	}
	if (!flag)
		printf("Your wanted student was not found\n");
	else
		printf("Option finished successfully!\n");
	for (i = 0; i < uni->num_std; i++) { // prints the data to file after adding bonus and rounding the grades
		fprintf(out, "Student %d: %s %ld %d\n", (i + 1), uni->arr[i].name, uni->arr[i].id, (int)round(uni->arr[i].grade));
	}
}

void PrintFinalGrade(FILE* out, university* uni) {
	// prints the data before calculating the final grade
	int i;
	fprintf(out, "BEFORE:\n");
	PrintData(out, uni);

	// prints the data after calculating the final grade
	fprintf(out, "AFTER:\n");
	for (i = 0; i < uni->num_std; i++) {

		fprintf(out, "Student %d: %s %ld %.2f %d ", (i + 1), uni->arr[i].name, uni->arr[i].id, uni->arr[i].grade, uni->arr[i].fin);
		fprintf(out, "final: %.2f\n", FinalGrade(&uni->arr[i]));
	}
}

void PrintData(FILE* out, university* uni) {

	int i, j, count = 0;
	for (i = 0; i < uni->num_std; i++) {
		for (j = 0; j < LAB; j++) { //count laboratory submissions
			count += uni->arr[i].lab[j] - '0';
		}
		uni->arr[i].fin = count >= 3; // can be 1 or 0
		// prints the data 
		fprintf(out,"Student %d: %s %ld %.2f %d\n", (i + 1), uni->arr[i].name, uni->arr[i].id, uni->arr[i].grade, uni->arr[i].fin);
		count = 0;
	}
	printf("Option finished successfully!\n");
}

void GetData(FILE* in, university* uni)
{
	student* temp;
	char str[MAX + 1];
	uni->num_std = 0;
	while (fscanf(in, "%s", str) != EOF) // check if we reached end of file
	{
		if (uni->num_std == 0) // allocating mem for the first student
		{
			uni->arr = (student*)malloc(sizeof(student));
			if (uni->arr == NULL)
				Error_Msg("Error in allocation!!!");
		}
		else // allocating mem for the rest of the students
		{
			temp = (student*)realloc(uni->arr, sizeof(student) * (uni->num_std + 1));
			if (temp == NULL)
			{
				MyFree(uni);
				Error_Msg("Error in allocation!!!");
			}
			else
				uni->arr = temp;
		}
		uni->arr[uni->num_std].name = (char*)malloc(strlen(str) + 1); // allocating mem for name
		if (uni->arr[uni->num_std].name == NULL)
		{
			MyFree(uni);
			Error_Msg("Error in allocation!!!");
		}
		strcpy(uni->arr[uni->num_std].name, str); // copying to the name field
		fscanf(in, "%ld", &uni->arr[uni->num_std].id);
		fscanf(in, "%f", &uni->arr[uni->num_std].grade);
		fscanf(in, "%s", uni->arr[uni->num_std].lab);
		uni->num_std++;
	}
	printf("Option finished successfully!\n");
}

float FinalGrade(student* std)
{
	if (std->grade < 55) // means its his final grade
		return std->grade;
	return (0.85 * std->grade) + (15 * std->fin); // calculate final grade
}

void InsertStudent(university* uni)
{
	student* temp;
	char str[MAX + 1];
	// allocating new room in array for the new student
	temp = (student*)realloc(uni->arr, sizeof(student) * (uni->num_std + 1));
	if (temp == NULL)
	{
		MyFree(uni);
		Error_Msg("Error in allocation!!!");
	}
	else
		uni->arr = temp;
	printf("Enter student's name: ");
	scanf("%s", str);
	uni->arr[uni->num_std].name = (char*)malloc(strlen(str) + 1); // allocate mem for student's name
	if (uni->arr[uni->num_std].name == NULL) // check if allocation was successful
	{
		MyFree(uni);
		Error_Msg("Error in allocation!!!");
	}
	strcpy(uni->arr[uni->num_std].name, str); // copying to the name field
	// gets the rest of the data from user
	printf("Enter student's ID: ");
	scanf("%ld", &uni->arr[uni->num_std].id);
	printf("Enter student's grade: ");
	scanf("%f", &uni->arr[uni->num_std].grade);
	printf("Enter student's laboratory submissions: ");
	scanf("%s", uni->arr[uni->num_std].lab);
	uni->num_std++; // increament the number of students in array
	printf("Option finished successfully!\n");
}

void MyFree(university* uni)
{
	int i;
	for (i = 0; i < uni->num_std; i++)
		free(uni->arr[i].name); // free the allocated names
	free(uni->arr); // free the allocated cells in array
}

void Error_Msg(char* str)
{
	printf("\n%s", str);
	exit(1);
}