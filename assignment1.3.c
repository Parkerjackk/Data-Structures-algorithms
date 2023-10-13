#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARRAY_SIZE 59                                                              // best to have a prime number as the size of the hashtable as this reduces the posibilities of collisions.
#define STRING_SIZE 20                                                             // defining the maximum size of a string that will be read in
#define NAME_PROMPT "Enter term to get frequency or type \"quit\" to escape\n>>> " // invitation to the user
#define NEW_LINE_PROMPT ">>> "                                                     // invitation to the user where the user should input the name

typedef struct Names //creating an struct of name, which will have a key, in this case a ,name and a value, being the number of times this key appears.
{
    char name[STRING_SIZE];
    int value;
} Names;

Names *hashTable[ARRAY_SIZE]; // creating a new object of type names, being the hashtable and setting its size to 59.
int collisions = 0;           // variable that will count the amount of collisions, initialising this to 0.
int num_terms = 0;            // variable that will be used to count the number of unique elements in the hashtable.

// hashing function that takes the ASCI values of the characters of a string and adds them, dividing them by the size of the array in order
// to obtain the cell value for each string in the hash table.
int hash1(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = (hash + *s - 'A') % ARRAY_SIZE;
        s++;
    }
    return hash;
}
// second hashfunction given
int hash3(char *s)
{
    int hash = 0;
    while (*s)
    {
        hash = 1 + (hash + *s - 'A') % (ARRAY_SIZE - 1);
        s++;
    }
    return hash;
}

//CODE WONT BE COMMENTED ON UNLESS CHANGES HAVE BEEN MADE IN COMPARISON TO TASK1 (ONLY SEARCH AND INSERT FUNC. HAVE BEEN MODIFIED!!!!).
// returns the element with name name or NULL if the element is not present
Names *search(char *name)
{
    int index = hash1(name); //Storing the value returned from the first hashfunction in index
    int index2 = hash3(name); //Storing the value returned from the second hashfunction in index2
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        int atmpt = (index +(i * index2)) % ARRAY_SIZE; //atmpt is updated in comp to task1. In this case, we take the hashvalue from the first H.func and add it to the product of the iteration i and the hashvalue from the second H.func and then the modulus of the hashtable size.
        if (hashTable[atmpt] != NULL && strncmp(hashTable[atmpt]->name, name, ARRAY_SIZE) == 0)
        {
            return hashTable[atmpt];
        }
    }
    return NULL;
}

// assuming that no element of key name is in the list (use search first!), add element at the correct place in the list
// NB: it would be more efficient for search to return the index where it should be stored directly, but aiming for simplicity here!
int insert(Names *name)
{
    if (name == NULL)
    {
        return 0;
    }
    int index = hash1(name->name); //Storing the value returned form the first hashfunction providing the key of the object.
    int index2 = hash3(name->name); //Storing the value returned from the second hashfuncton providing the key of the object.
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        int atmpt = (index + (i * index2)) % ARRAY_SIZE; //calc atmpt same as in search func.
        if (hashTable[atmpt] == NULL)
        {
            hashTable[atmpt] = name;
            return 1;
        }
        else
        {
            collisions++;
        }
    }
    return 0;
} //the rest of the function is the same as task 1. Doubling hashing implemented.

Names *createNewElement(char *name)
{
    Names *newName = (Names *)malloc(sizeof(Names));
    strcpy(newName->name, name);
    newName->value = 1;
    insert(newName);
    return newName;
}

// prints the number of occurences, or 0 if not in the file
int printNumberOfOccurences(char *name)
{
    Names *this = search(name);
    if (this != NULL)
    {
        // printf("%d", this->value);
        return this->value;
    }
    else
    {
        return 0;
        // printf("0");
    }
}

// searches the name in the array, if it is there increment its count, if not, add it
void addOrIncrement(char *name)
{
    Names *this = search(name);
    if (this != NULL)
    {
        this->value++;
        return;
    }
    else
    {
        createNewElement(name);
        num_terms++;
    }
}

// From assignment 0
// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
void next_token(char *buf, FILE *f, int string_max)
{
    // start by skipping any characters we're not interested in
    buf[0] = fgetc(f);
    while (!isalnum(buf[0]) && !feof(f))
    {
        buf[0] = fgetc(f);
    }
    // read string of alphanumeric characters
    int i = 1;
    for (;;)
    {
        buf[i] = fgetc(f); // get next character from file
        if (!isalnum(buf[i]))
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

//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file(char *fname)
{
    FILE *file = fopen(fname, "r");
    char buf[STRING_SIZE];

    // boiler plate code to ensure we can open the file
    if (!file)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }

    // read until the end of the file
    while (!feof(file))
    {
        next_token(buf, file, STRING_SIZE);
        addOrIncrement(buf);
    }

    printf("File %s loaded\n", fname);
    float load = ((float)num_terms / ARRAY_SIZE);
    printf(" Capacity: %i\n Num Terms: %i\n Collisions: %i\n Load: %f\n", ARRAY_SIZE, num_terms, collisions, load);

    fclose(file);

    return 1;
}

void userInput()
{
    printf("%s", NAME_PROMPT);
    char answer[STRING_SIZE];
    scanf("%s", answer);
    char quit[] = {"quit"};
    while (strcmp(answer, quit) != 0)
    {
        if (search(answer) != NULL)
        {
            int occur = printNumberOfOccurences(answer);
            printf("%s - %d \n", answer, occur);
        }
        else
        {
            printf("%s - 0 \n", answer);
        }

        printf("%s", NEW_LINE_PROMPT);
        scanf("%s", answer);
    }
}

int main(int argc, char *argv[])
{
    load_file(argv[1]);
    userInput();
    return 0;
}