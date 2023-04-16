#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#define  N  6

void insert(bool(*f)(void* a, void* b), void* element, void** array, int count); // sorts and insert new number to the array
bool Int_Comp(void*, void*); // compare between two numbers
bool IsFull(int); // check if the array is full

int main()
{
    int Array[N], Value, i, CurrentCount = 0;
    void* PArray[N];                        /*pointers array*/
    printf("Enter up to %d numbers, -999 to stop\n", N);
    scanf("%d", &Value);
    while (Value != -999)
    {
        if (IsFull(CurrentCount) == false)
        {
            Array[CurrentCount] = Value;
            insert(Int_Comp, &Array[CurrentCount], PArray, CurrentCount);  /*call the general function*/
            scanf("%d", &Value);
            CurrentCount++;
        }
        else
        {
            printf("The array is full");
            break;
        }
    }
    printf("\n Your sorted array is:\n");             /*print the array in sorted order*/
    for (i = 0; i < CurrentCount; i++)
        printf(" %d ", *(int*)PArray[i]);
    return 0;
}

// General Function
void insert(bool(*f)(void* a, void* b), void* element, void** array, int count) 
{
    int i, arr_index = 0;
    if (!count) 
    {
        array[0] = element; // if it's the first element, just insert it in.
        return;
    }
    // find which index the new element is supposed to go.
    for (i = count; i > -1; i--) 
    {
        if (i == 0) // if we get to i = 0 then the number is the lowest in the array.
            break;
        if (f(element, array[i - 1])) // checks if inserted element is larger than array's numbers.
        { 
            arr_index = i;
            break;
        }
    }
    if (arr_index == count) 
    { // this is true when the element is the largest number in the array.
        array[arr_index] = element;
        return;
    }
    for (i = count; i > arr_index; i--) // shift array values forward to make space for the element.
        array[i] = array[i - 1];
    array[arr_index] = element; // insert the element in the correct index.
}

bool IsFull(int count)
{
    if (count == N)
        return true;
    return false;
}

bool Int_Comp(void* FirstElement, void* SecondElement)
{
    if (*(int*)FirstElement > *(int*)SecondElement)
        return true;
    return false;
}