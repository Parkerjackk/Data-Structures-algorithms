#include <stdio.h>
#include <stdlib.h>
#include "t1.h"
#include <time.h>


//Fills the array with ascending, consecutive numbers, starting from 0.
void fill_ascending(int *array, int size)
{
    for(int i = 0; i < size; i++){
        array[i] = i; //fills the array from 0 to size - 1 in ascending order
    }
    
}
//Fills the array with descending numbers, starting from size-1
void fill_descending(int *array, int size)
{
    int i = size - 1;
    int j = 0;
    while( i > -1){ //decreasing i until it reaches 0 placing it in each index of the array
        array[j] = i;
        i--; //decrease i, the value 
        j++; //increase j, the index
    }
}

//Fills the array with uniform numbers.
void fill_uniform(int *array, int size)
{
    int value = rand() % size; //randomly generates a value within the range
    for(int i = 0; i < size; i++){
        array[i] = value; //inserts said random value at each index
    }
}

//Fills the array with random numbers within 0 and size-1. Duplicates are allowed.
void fill_with_duplicates(int *array, int size)
{
    for(int i = 0; i < size; i++){
        int number = rand() % size; //randomly generate a values within the range for each iteration
        array[i] = number;
    }
}


//Fills the array with unique numbers between 0 and size-1 in a shuffled order. Duplicates are not allowed.
void fill_without_duplicates(int *array, int size)
{
    for(int i = 0; i < size; i++){
        array[i] = i; //fills array in ascending order
    }
    for(int j = 0; j < size; j++) { //reorganises values in different indexes of the array, therefore creating a rnadomly sorted array

        int tmp = array[j];
        int rndInx = rand()%(size-1);
        array[j] = array[rndInx];
        array[rndInx] = tmp;

    }
}

void printArray(int* arr, int size){
  int i;
  for(i=0; i<size;i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}
/*
//Functions created to test functions
void printAllFunc(int *arr, int size){
    printf("First example: ascending array \n");
    fill_ascending(arr, size);
    printArray(arr, size);

    printf("Second example: descending array \n");
    fill_descending(arr, size);
    printArray(arr, size);

    printf("Third example: fill with same value \n");
    fill_uniform(arr, size);
    printArray(arr, size);

    printf("Fourth example: fill randomly with duplicates \n");
    fill_with_duplicates(arr, size);
    printArray(arr, size);

    printf("Fifth example: fill randomly without duplicates \n");
    fill_without_duplicates(arr, size);
    printArray(arr, size);
    fill_without_duplicates(arr, size);
    printArray(arr, size);

}

int main(){
    int array[6];
    int length = sizeof(array)/sizeof(array[0]);
    printAllFunc(array, length);
    return 0;
}
*/