#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct Tree_Node					  //creating a struct of type Tree_node
{
	unsigned int ID;						  // unique identifier for the document
	char *name;								  // file name of the document
	int word_count;							  // number of words in the document
	char *author;							  // author of the book
	struct Tree_Node *leftChild, *rightChild; // pointer to the next node in the list
} Tree_Node;

Tree_Node *root;							  //declaring a root
int count = 0;								  //declaring a count and initializing it to 0

//declaring and initializing an array of 17 2^n values.
const int pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072};

int bstdb_init(void) //function that runs once when program is ran 
{
	root = NULL;	 //initializing root to NULL.
	return 1;		 //returns 1, meaning that it was correctly executed
}

//function that creates a new object of type tree_node and stores each piece of data in its corresponding location.
Tree_Node *newBook(char *name, int word_count, char *author, unsigned int ID)
{
	Tree_Node *newNode = (Tree_Node *)malloc(sizeof(Tree_Node));
	newNode->ID = ID;
	newNode->name = name;
	newNode->word_count = word_count;
	newNode->author = author;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;
	count++; //increase book counter
	return newNode;
}


unsigned int IdVal() //fucntion that generates unique IDs
{
	static int count = 1, k = 0, ph = 0, n = 16;
	int h = log2(count++); //h is assigned the value of log2(count), resulting in the height of the tree, and then count is increased for every new book entered.
	k = (ph != h) ? 0 : k; //if p!=h is true, k is assigned 0, otherwise, k remains the same
	int val = (2 * k++ + 1) * (pow2[n - h]);
	ph = h;
	return val; //returns the value for the ID calculated.
}

//function from task1 modified to handle new struct
void tree_insert(Tree_Node **root, char *name, int word_count, char *author, unsigned int ID)
{
	if (*root == NULL) //if the root is null, the element is the first to be inserted into the tree, making it the root
	{
		*root = newBook(name, word_count, author, ID);
		return;
	}
	if (ID < (*root)->ID) //if the ID of the element that is being inserted is less than that of the root, it will be placed to the left of the root.
	{
		tree_insert(&(*root)->leftChild, name, word_count, author, ID);
	}
	else if (ID > (*root)->ID)//if the ID of the element that is being inserted is less than that of the root, it will be placed to the right of the root.
	{
		tree_insert(&(*root)->rightChild, name, word_count, author, ID);
	}
}

//adds a new book to the BST. Stores name, word_count, and author.
int bstdb_add(char *name, int word_count, char *author)
{
	unsigned int ID = IdVal(); //ID for book is generated
	tree_insert(&root, name, word_count, author, ID); //with this ID, the object is then stored in the BST
	if (root != NULL)
	{
		return ID; //returns ID of the object
	}
	else
	{
		return -1; //if this occurs, the root is NULL meaning that the BST is non-existent or an ERROR has occured
	}
}

//function from task1 modified to search for an int ID
Tree_Node *search(Tree_Node *root, unsigned int ID)
{
	if (ID == root->ID)
	{
		return root;
	}
	else if (ID < root->ID)
	{
		search(root->leftChild, ID);
	}
	else if (ID > root->ID)
	{
		search(root->rightChild, ID);
	}
	if (root == NULL)
	{
		return NULL;
	}
}

//function that uses the search function above to obtain the word_count of a book using the ID to retrieve said info
int bstdb_get_word_count(int ID)
{
	Tree_Node *newNode = search(root, ID);
	if (newNode != NULL)
	{
		return newNode->word_count;
	}
	else
	{
		return -1;
	}
}

//function that uses the search function above to obtain the name of a book using the ID to retrieve said info
char *bstdb_get_name(int ID)
{
	Tree_Node *newNode = search(root, ID);
	if (newNode != NULL)
	{
		return newNode->name;
	}
	else
	{
		return 0;
	}
}

//function that returns the max value between two integers
int maxVal(int x, int y)
{
	return (x >= y) ? x : y;
}

//function to obtain the height of a tree given the root
int height(Tree_Node *root)
{
	if (root == NULL)
	{
		return 0;
	}
	return 1 + maxVal(height(root->leftChild), height(root->rightChild)); //adds 1 to the value of maxVal as we are counting the height from the roots immediate children
}

//function to check if a bst is balanced or not
int isBalanced(Tree_Node *root)
{
	int leftHeight, rightHeight;

	if (root == NULL)
	{
		return 1; //if the root doesnt exist, the tree does exist so its obv not balanced
	}

	leftHeight = height(root->leftChild); //gets the height of the left-subtree
	rightHeight = height(root->rightChild); //gets the height of the right-subtree

	if (abs(leftHeight - rightHeight) <= 1 && isBalanced(root->leftChild) && isBalanced(root->rightChild))
	{
		return 1; //if the tree is balanced, the fucntion returns true.
	}
	else
	{
		return 0; //otherwise, it will return false
	}
}

void bstdb_stat(void)
{
	int balanced = isBalanced(root); //storing the value returned fro mthe isBalanced function for the BST given the root.

	if (balanced == 1) //if the value is 1, its true
	{
		printf("Tree is balanced :) \n");
		printf("Height of Tree = %d \n", height(root));
		//prints out the height of both sides of the tree to prove that it is indeed balanced.
		printf("Left side of tree has a height of = %d \n", height(root->leftChild)); 
		printf("Right side of tree has a height of = %d \n", height(root->rightChild));
		printf("NUMBER OF BOOKS: %d \n", count);
	}
	else //otherwise, its false
	{
		printf("Tree is not balanced :( \n");
		printf("Height of Tree = %d \n", height(root));
		printf("Left side of tree has a height of = %d \n", height(root->leftChild));
		printf("Right side of tree has a height of = %d \n", height(root->rightChild));
		printf("NUMBER OF BOOKS: %d \n", count);
	}
	//  + Check if your tree is balanced and print the result
}

//recursive function to delete every node of the tree given the root. Same as task1
void delete(Tree_Node *root)
{
	if (root != NULL)
	{
		delete (root->leftChild);
		delete (root->rightChild);
		free(root);
	}
}

//if the quit function is called, the tree will be deleted and the memory will be freed from the heap
void bstdb_quit(void)
{
	delete (root);
}
