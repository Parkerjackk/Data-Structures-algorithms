#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

//creates a new node in the tree in the apporpriate prosition regarding the value of the data for bst
void tree_insert(Tree_Node **root, char data)
{
    Tree_Node *newNode = NULL;
    if (*root == NULL) 
    {
        newNode = (Tree_Node *)malloc(sizeof(Tree_Node)); //allocating memory in the heap for a new object of type Tree_Node
        newNode->data = data;
        //setting both children nodes to NULL
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
        *root = newNode;
        return;
    }
    if (data == (*root)->data) //if the data value is equal to that of the root, the node is inserted on the left
    {
        tree_insert(&(*root)->leftChild, data);
    }

    if (data < (*root)->data) //if the data is less in value than that of the root, it is inserted on the left
    {
        tree_insert(&(*root)->leftChild, data);
    }
    else if (data > (*root)->data) //if the data is greater than that of the root, it is inserted to the right
    {
        tree_insert(&(*root)->rightChild, data);
    }
}

//function that iteratively calls on the insert function for each value of an array
Tree_Node *create_bst(char data[])
{
    Tree_Node *root = NULL;
    int i = 0;
    while (data[i] != '\0' ) //this will continue until the null character is found
    {
        tree_insert(&root, data[i]);
        i++;
    }
    return root;
}

//recursive function used to search for a value, returns a pointer to the node of that value if it exists
Tree_Node *tree_search(Tree_Node *root, char data)
{
    if (root == NULL){
        return NULL;
    }
    else if(root->data == data)
    {
       return root; //if the value is found at the root, retrun the root
    }
        else if (root->data > data)
        {
            tree_search(root->leftChild, data); //if the value is less than the roots value, search to the left of the root
        }
        else if (root->data < data)
        {
            tree_search(root->rightChild, data); //if the value is greater than that of the root, seach to the right of the root
        }
}

//fucntion that uses inorder traversal to print the elements of the tree is accending order 
void tree_print_sorted(Tree_Node *root)
{
    if (root == NULL)
    {
        return;
    }
    else{
        tree_print_sorted(root->leftChild); //uses recursion to go through entirety of tree from lowest value ot heightest
        printf("%c", root->data);
        tree_print_sorted(root->rightChild);
    }
}

//fucntion used to deleted tree and free all memory allocated in heap
void tree_delete(Tree_Node *root)
{
    if(root == NULL){
        return;
    }
    tree_delete(root->leftChild);
    tree_delete(root->rightChild);
    free(root);
}
