#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET 0xcbf29ce48422325UL
#define ARRAY_SIZE 59                                                              // best to have a prime number as the size of the hashtable as this reduces the posibilities of collisions.
#define STRING_SIZE 20                                                             // defining the maximum size of a string that will be read in
#define NAME_PROMPT "Enter term to get frequency or type \"quit\" to escape\n>>> " // invitation to the user
#define NEW_LINE_PROMPT ">>> "                                                     // invitation to the user where the user should input the name

typedef struct Names
{
    char name[STRING_SIZE];
    int value;
} Names;

Names *hashTable[ARRAY_SIZE]; // establishing the size of the hashtable.
int collisions = 0;           // initialising the number of collisions to 0.
int num_terms = 0;

//FNV1a hashing algorithm. This algorithm was chosen due to its increased randomness and how well it deals with collisions. This function reduced the amount of collisions with respect to the initial function given by 15. 
unsigned long hash2(unsigned char *str)
{
    unsigned long hash = FNV_OFFSET;  //setting the hash to an initial value of FNV_OFFSET, a value proved to be the best for this hashing function.
    int length = sizeof(str); //obtaining the lenght of the string (name), being passed to the function.
    for (int i = 0; i < length; i++){ //iterate through the entire string letter by letter.
        hash ^= str[i]; //XOR the value of hash by the ASCI value of the  letter at each index of the string
        hash *= FNV_PRIME; //multply the new value of Hash by the FNV_PRIME value, the smallest prime number in headecimal representation.
    }
    return hash;
}

//THE REST OF THE CODE IS ESSENTIALLY THE SAME, ONLY CHANGING THE DATA TYPE WHEREVER THE HASHFUCNTION IS CALLED FROM IN TO UNSIGNED LONG AS IF LEFT AS AN INT, A SEG. FAILURE WOULD OCCURE AS THE VALUES RETURNED WOULD BE NEGATIVE.

// returns the element with name name or NULL if the element is not present
Names *search(char *name)
{
    unsigned long index = hash2(name);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        unsigned long atmpt = (i + index) % ARRAY_SIZE;
        if (hashTable[atmpt] != NULL && strncmp(hashTable[atmpt]->name, name, ARRAY_SIZE) == 0)
        {
           // printf("%s", name);
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
    unsigned long index = hash2(name->name);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        unsigned long atmpt = (i + index) % ARRAY_SIZE;
        if (hashTable[atmpt] == NULL)
        {
            hashTable[atmpt] = name;
            return 1;
        }
        else
            collisions++;
    }
    return 0;
}

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
        return this->value;
    }
    else
    {
        return 0;
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
    float load = ((float)num_terms/ARRAY_SIZE);
    printf(" Capacity: %i\n Num Terms: %i\n Collisions: %i\n Load: %f\n", ARRAY_SIZE, num_terms, collisions, load);

    fclose(file);

    return 1;
}

void userInput(){
    printf("%s", NAME_PROMPT );
    char answer[STRING_SIZE];
    scanf("%s", answer);
    char quit[] ={"quit"};
    while(strcmp(answer, quit) != 0){
        if(search(answer)!= NULL){
            int occur = printNumberOfOccurences(answer);
            printf("%s - %d \n", answer, occur);
        }
        else{printf("%s - 0 \n", answer);}

        printf("%s", NEW_LINE_PROMPT);
        scanf("%s", answer);
    }
}


int main(int argc, char *argv[])
{

    load_file (argv[1]);
    userInput();
    return 0;
}