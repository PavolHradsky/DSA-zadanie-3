/*********************************************
 * DSA Zadianie 3 - Binary decision diagram
 *
 * FIIT STU 2021
 * Pavol Hradsky
*********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 30

typedef struct node {
    struct node *left;
    struct node *right;
    struct node *parent;
    char data[MAX];
}NODE;

typedef struct bdd {
    int pocet_premennych;
    int pocet_uzlov;
    NODE *head;
}BDD;

NODE *create_node(char data[MAX]){
    NODE *result = NULL;
    result = (NODE*)malloc(sizeof(NODE));
    strcpy(result->data, data);
    result->left = NULL;
    result->right = NULL;
    result->parent = NULL;
    return result;
}


BDD *BDD_create(char *bfunkcia) {

    BDD *vysl = NULL;
    vysl = (BDD*)malloc(sizeof(BDD));
    vysl->pocet_premennych = 3;
    vysl->pocet_uzlov = 15;

    NODE **arr = (NODE**)malloc(strlen(bfunkcia) * sizeof(NODE));

    char str[MAX] = "";
    NODE *tmp = NULL;

    for (int i = 0; i < strlen(bfunkcia); ++i) {
        str[0] = bfunkcia[i];
        arr[i] = create_node(str);
    }
//    for (int i = 0; i < strlen(bfunkcia); ++i) {
//        printf("%s\n", arr[i]->data);
//    }

    int level = 2;

    do {
        NODE **arrNew = (NODE**)malloc(strlen(bfunkcia) / level * sizeof(NODE));


        for (int i = 0; i < strlen(bfunkcia) / level; ++i) {
            strcpy(str, arr[2 * i]->data);
            strcat(str, arr[2 * i + 1]->data);
            tmp = create_node(str);
            tmp->left = arr[2 * i];
            tmp->right = arr[2 * i+1];
            arr[2 * i]->parent = tmp;
            arr[2 * i+1]->parent = tmp;
            arrNew[i] = tmp;
        }
        free(arr);
        arr = (NODE**)malloc(strlen(bfunkcia) / level * sizeof(NODE));
        for (int i = 0; i < strlen(bfunkcia) / level; ++i) {
            arr[i] = arrNew[i];
        }
        free(arrNew);

//        printf("ARR:\n");
//        for (int i = 0; i < strlen(bfunkcia) / level; ++i) {
//            printf("%s\n", arr[i]->data);
//        }
        level *= 2;

    }while(strcmp(str, bfunkcia) != 0);
    free(arr);


    vysl->head = tmp;

    return vysl;
}


int main()
{
    BDD* bdd;
    bdd = BDD_create("10010011");

    printf("%s\n", bdd->head->data);


    return 0;
}