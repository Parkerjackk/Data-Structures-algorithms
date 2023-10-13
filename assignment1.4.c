#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARRAY_SIZE 99991                                                           // best to have a prime number as the size of the hashtable as this reduces the posibilities of collisions.
#define STRING_SIZE 20                                                             // defining the maximum size of a string that will be read in
#define NAME_PROMPT "Enter term to get frequency or type \"quit\" to escape\n>>> " // invitation to the user
#define NEW_LINE_PROMPT ">>> "                                                     // invitation to the user where the user should input the name
#define INIT_LINE_PROMPT "Person ID Deposition ID         Surname        Forename Age Person Type  Gender Nationality Religion Occupation"

typedef struct person //defining a struct person with the following characteristics->
{
    char ID[STRING_SIZE], depID[STRING_SIZE], age[STRING_SIZE];
    char surname[STRING_SIZE], name[STRING_SIZE], type[STRING_SIZE], gender[STRING_SIZE], nat[STRING_SIZE], rel[STRING_SIZE], occ[STRING_SIZE];
    struct person *next; //pointer to the next object, used in the case that a linked list is implemented. (two of the same names are inserted).
} person;

person *hashTable[ARRAY_SIZE]; // establishing the size of the hashtable.
int collisions = 0;            // initialising the number of collisions to 0.
int num_terms = 0;             // initialising the number of terms to 0.

unsigned long hash1(char *s) //first of two hashfunctions (hashfunction 1 provided by professor).
{
    unsigned long hash = 0;
    while (*s)
    {
        hash = (hash + *s - 'A') % ARRAY_SIZE;
        s++;
    }
    return hash;
}

unsigned long hash2(char *s) //second hashfunction (hashfunction 3 provided by professor).
{
    unsigned long hash = 0;
    while (*s)
    {
        hash = 1 + (hash + *s - 'A') % (ARRAY_SIZE - 1);
        s++;
    }
    return hash;
}

// Search function that returns the element with name surname or NULL if the element is not present
person *search(char *name)
{
    unsigned long index1 = hash1(name); //storing the value returned from the hash1 fucntion in index1.
    unsigned long index2 = hash2(name); //storing the value returned from the hash2 function in index2.
    for (int i = 0; i < ARRAY_SIZE; i++) //iterating through hashtable.
    {
        int atmpt = (index1 + (i * index2)) % ARRAY_SIZE; //obtaining atmpt value through a mathematical eqn.
        if (hashTable[atmpt] != NULL) //if the cell in the hashtable at index (atmpt) is not empty continue.
        {
            if (strncmp(hashTable[atmpt]->surname, name, ARRAY_SIZE) == 0) //if the value sought is equal to the value stored in the hashtable at index(atmpt) continue.
            {
                person *tmp = hashTable[atmpt]; //new oject of type person stores value found in said index of the table.
                return tmp; //return object tmp.
            }
        }
    }
    return NULL; //if the value isnt found, return NULL
}
//function that inserts a name & its corresponding struct into the hashtable. Takes an object of type person as its parameter.
int insert(person *tmp)
{
    if (tmp == NULL) //if the value passed to the function is NULL it wont do anything (pointless to check as it is a given that a valid value will be passed).
    {
        return 0;
    }
    unsigned long index1 = hash1(tmp->surname); //storing the value returned from the hash1 fucntion in index1.
    unsigned long index2 = hash2(tmp->surname); //storing the value returned from the hash2 fucntion in index2.
    for (int i = 0; i < ARRAY_SIZE; i++) //iterates through hashtable.
    {
        unsigned long atmpt = (index1 + (i * index2)) % ARRAY_SIZE; //obtaining atmpt value through a mathematical eqn. that varies depending on the iteration and values of index1 & index2
        if (hashTable[atmpt] == NULL) //if the cell at index (atmpt) isnt full continue...
        {
            hashTable[atmpt] = tmp; //store the struct tmp at index (atmpt).
            return 1;
        }
        else if (strcmp(tmp->surname, hashTable[atmpt]->surname) == 0) //if the cell at index(atmpt) is full, and the data input by the user is equal to the data found at said cell... 
        {
            tmp->next = hashTable[atmpt]; //add a new node (linked list), pushing the previous head to be the new heads next.
            hashTable[atmpt] = tmp; //new head is the most recent value entered that follows requierements.
        }
        else if (hashTable[atmpt] != NULL && (strcmp(tmp->surname, hashTable[atmpt]->surname) != 0)) //if the cell is full and the words dont coincide
        {
            collisions++; //we increase the number of collisions.
            //printf("%s %s\n", hashTable[atmpt]->surname, tmp->name); //show how the collisions is  correct as two names magically have the same hashvlaue despite the double hashing.
        }
    }
    return 0;
}

// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1 (FUNCTION HASNT BEEN MODIFIED)
void next_token(char *buf, FILE *f, int string_max)
{
    // start by skipping any characters we're not interested in
    buf[0] = fgetc(f);
    while (!isalnum(buf[0]) && !feof(f))
    {
        buf[0] = fgetc(f);
    }
    if (feof(f))
    {
        return;
    }
    // read string of alphanumeric characters
    int i = 1;
    for (;;)
    {
        buf[i] = fgetc(f); // get next character from file
        if (!isalnum(buf[i])) //checking if the character is not alphanumeric
        {
            if (buf[i] != ' ')
            {          // we still want to keep spaces
                break; // only load letters and numbers
            }
        } // only load letters and numbers
        if (feof(f))
        {
            break;
        } // file ended?
        if (i < (string_max - 1))
        {
            ++i;
        } // truncate strings that are too long
    }
    buf[i] = '\0'; // NULL terminate the string
}

int skipped = 0; //declaring a skipped integer and initiialising to 0, shall be used in nex function.
//Function that collects data from the csv file and stores each token in its designated variable within an "object" of type person.
void collectData(FILE *csv, person *p)
{
    char buf[STRING_SIZE]; //setting a buffer of size 20
    if (skipped == 0) //checking if skipped i equal to zero, this is used to skip the initial header of the file.
    {
        for (int i = 0; i < 10; i++) //iterate through all tokens of first line. (header line)
        {
            next_token(buf, csv, STRING_SIZE);//not storing any of the values as it is the header.
        }
        skipped = 1; //setting skipped to 1 as we have already passed the header.
    }
    //storing each of the values in its designated variable within the struct p of type person.
    next_token(p->ID, csv, STRING_SIZE);
    next_token(p->depID, csv, STRING_SIZE);
    next_token(p->surname, csv, STRING_SIZE);
    next_token(p->name, csv, STRING_SIZE);
    next_token(p->age, csv, STRING_SIZE);
    next_token(p->type, csv, STRING_SIZE);
    next_token(p->gender, csv, STRING_SIZE);
    next_token(p->nat, csv, STRING_SIZE);
    next_token(p->rel, csv, STRING_SIZE);
    next_token(p->occ, csv, STRING_SIZE);

    //if uncommented, it prints out the enitre table with all corresponding data.
    //printf("%s    %s   %s  %s   %s %s    %s   %s   %s  %s \n", p->ID, p->depID, p->surname, p->name, p->age, p->type, p->gender, p->nat, p->rel, p->occ);
}


//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file(char *fname)
{
    FILE *file = fopen(fname, "r");
    //char buf[STRING_SIZE];

    // boiler plate code to ensure we can open the file
    if (!file)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }

    // read until the end of the file
    while (!feof(file))
    {
        person *tmp = (person *)malloc(sizeof(person)); //creating a new object of type person and allocating it memory, this is repeated until the file reaches the end.
        collectData(file, tmp); //calling on the collect data file and providing the csv file and an object.
        insert(tmp);  //inserting said object into the hashtable
        num_terms++; //increase the number of terms. THIS HAS AN ERROR AS IT IS READING IN AN EXTRA LINE THAT IS RETURNING "Replacement Character"'s.
    }
    //prints out stats of hashtable
    printf("File %s loaded\n", fname);
    float load = ((float)num_terms / ARRAY_SIZE);
    printf(" Capacity: %i\n Num Terms: %i\n Collisions: %i\n Load: %f\n", ARRAY_SIZE, num_terms, collisions, load);

    fclose(file); //closes file once everthing is done.

    return 1;
}

//function used to interact with the user an process the users input.
void userInput()
{
    printf("%s", NAME_PROMPT);
    char answer[STRING_SIZE];   //declaring an array of chars named answer with a max lenght of 20chars.
    scanf("%s", answer);    //taking the input from the user and storing it in answer.
    char quit[] = {"quit"}; //declaring an array of chars named quit that stores the word "quit".
    while (strcmp(answer, quit) != 0) //while the answer input isnt equal to the word "quit" continue...
    {
        person *tmp = search(answer); //storing the value returned from the function search (of the users input) in a person object.
        if (tmp != NULL) //if it is found...
        {
            printf("%s\n", INIT_LINE_PROMPT); //print out the header
            while (tmp != NULL) //while the object isnt NULL print out all the values stored in it.
            {
                printf("    %s    %s            %s         %s   %s      %s    %s     %s  %s  %s\n", tmp->ID, tmp->depID, tmp->surname, tmp->name, tmp->age, tmp->type, tmp->gender, tmp->nat, tmp->rel, tmp->occ);
                tmp = tmp->next; //in the case that the cell where the answer was found has a linked list, it will print all of the nodes in this list.
            }
        }
        else //otherwise, if the asnwer isnt in the table...
        {
            printf("%s not in table\n", answer); //print out that it isnt in table
        }

        printf("%s", NEW_LINE_PROMPT);
        scanf("%s", answer); //take in a new answer (we must include this as we dont want to create an infinit loop)
    }
}

//main function takes files as its parameters and simply calls functions that have been explained previously 
int main(int argc, char *argv[])
{
    load_file("truncated.csv");
    userInput();
    return 0;
}