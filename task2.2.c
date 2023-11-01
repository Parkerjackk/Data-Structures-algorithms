#include <stdio.h>
#include "t2.h"

int number_comparisons = 0;
int number_swaps = 0;

//swap function
void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
    number_swaps++; //every time swap is called, the counter is increased
}
//function that creates partitions within the array, in this case, the pivot is awlays going to the the highest index on the array passed to the function
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high; j++)
    {
        number_comparisons++; //number of comparisons increases for each iteration
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1); 
}

void selectionSort(int arr[], int size)
{
    int i, j, k;
    // One by one move boundary of unsorted subarray
    for (i = 0; i < size - 1; i++)
    {
        // Find the minimum element in unsorted array
        k = i;
        for (j = i + 1; j < size; j++)
        {
            number_comparisons++;
            if (arr[j] < arr[k])
                k = j;
        }

        if (k != i) //if the minumum element isnt i, we swap the positions of smallest with i.
        {
            swap(&arr[k], &arr[i]);
        }
    }
}

void insertionSort(int arr[], int size)
{
    int i, j, k;
    //iterates through array
    for (i = 1; i < size; i++)
    {
        number_comparisons++; //compares the two values that are side by side
        j = i - 1;
        k = arr[i];
        while (j > -1 && arr[j] > k)
        {
            arr[j + 1] = arr[j];
            number_swaps++;
            number_comparisons++;
            j--;
        }
        arr[j + 1] = k; 
    }
}
//Quicksort function that takes three parameters, the array, the lowest index and the highest index
void QuickSort(int arr[], int low, int high)
{
    if (low < high) //if the lowest index is less than the highest continue
    {
        int pi = partition(arr, low, high); //call on partition function and store value, this will be used when the Quicksort function is called upon recusrsivly
        QuickSort(arr, low, pi - 1);
        QuickSort(arr, pi + 1, high);
        // number_swaps++;
    }
}
void quickSort(int arr[], int size)
{
    int l = 0; //set the initial low to 0
    int h = size - 1; //set the initial high to the last index of the array
    QuickSort(arr, l, h);
}
