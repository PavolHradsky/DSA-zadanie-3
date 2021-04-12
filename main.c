/*********************************************
 * DSA Zadianie 3 - Binary decision diagram
 *
 * FIIT STU 2021
 * Pavol Hradsky
*********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    struct node *left;
    struct node *right;
    struct node *parent;
    char *data;
} NODE;

typedef struct bdd {
    struct node *head;
}BDD;

NODE *create_node(char *data){
    NODE *result = (NODE*)malloc(sizeof(NODE));
    strcpy(result->data, data);
    result->left = NULL;
    result->right = NULL;
    result->parent = NULL;

    return result;
}

int main()
{

    printf("Hello World");

    return 0;
}