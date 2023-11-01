#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Max_String_Length 50 //define the max lenght of a string that cna be read in from csv file
#define Array_Size 20000 //defining the max size of the array that will be used to store the structs

typedef struct game //creating a struct of type game with its characteristics 
{
    char title[Max_String_Length], platform[Max_String_Length];
    int score, year;

} game;

game games[Array_Size]; //creating an array of strcuts
int count = 0; //setting a counter to 0, this will count the number of entries read in from the csv file

//func to go through csv file
int nextField( char *buf, FILE *f, int max ) {
	int i=0, end=0, quoted=0;
	
	while(!feof(f)) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}

void collectData(FILE *csv, game *p) //fucn to collect data, and disregard header line
{
    char buf[Max_String_Length]; //setting a buffer of size 50
    nextField(p->title, csv, Max_String_Length);
    nextField(p->platform, csv, Max_String_Length);
    nextField(buf, csv, Max_String_Length);
    p->score = atoi(buf);
    nextField(buf, csv, Max_String_Length);
    p->year = atoi(buf);

}

//func to insert each strcut in array
int insert(game *tmp)
{
    if (tmp == NULL) //if the value passed to the function is NULL it wont do anything (pointless to check as it is a given that a valid value will be passed).
    {
        return 0;
    }
    else {
        games[count] = *tmp;
        count++; //increase count ach time a strcut is added to array
    }
    return 0;
}

//func to load and read csv file
int load_file(char *fname)
{
    FILE *file = fopen(fname, "r");

    // boiler plate code to ensure we can open the file
    if (!file)
    {
        printf("Unable to open %s\n", fname);
        return 1;
    }
    collectData(file, games); //skips header
    // read until the end of the file
    while (!feof(file))
    {
        collectData(file, games); //calling on the collect data file and providing the csv file and an object.
        insert(games);  //inserting said object into an array of structs
    }
    fclose(file); //closes file once everthing is done.

    return 0;
}

//swap function
void swap(game *x, game * y){
    game tmp = *x; 
    *x = *y;
    *y = tmp;
}

//partition function
int partition(game arr[], int low, int high){
    int piv = arr[high].score;
    int i = (low - 1);

    for(int j = low; j <= high; j++){
        if (arr[j].score < piv)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

//sorting function, quicksort as deals well w large arrays of data
void quickSort(game arr[], int low, int high){
    if (low < high){
        int j = partition(arr, low , high);
        quickSort(arr, j + 1, high);
        quickSort(arr, low, j - 1);
    }
}

//func to print header, not needed, makes it look good
void printHeader(){
    int n = 25; // column width
    int n_first = 50; //first column width
    printf("%*s | %*s | %*s | %*s |\n", n_first, "Title", n, "Platform", n, "Rating", n, "Release year");
    printf ("------------------------------------------------------------------------------------------------------------------------------\n");
}

//func to print top 10 games with their characteristics
void printData(game arr[]){
    int n =25; // column width
    int n_first = 50; //first column width
    for (int i = 0; i < 10; i++){
    printf("%*s | %*s | %*d | %*d |\n",n_first, games[i].title, n, games[i].platform, n, games[i].score, n, games[i].year);

}
printf("\n");
}

//func to reverse the array sicne quicksort sorts it in ascending order, we want descending
void rvsArray(game arr[], int start, int end) 
{ 
    while (start < end) { 
        swap(&arr[start], &arr[end]);
        start++; 
        end--; 
    } 
} 


int main(int argc, char *argv[]){
    load_file("t4_ign.csv");
    int low = 0, high = count;
    quickSort(games, low, high);
    rvsArray(games, low, high);
    printHeader();
    printData(games);
    return 0;
}