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

    int n = 1;
    int mocnina = 2;
    while (mocnina <= strlen(bfunkcia)){
        if (mocnina == strlen(bfunkcia)){
            vysl->pocet_premennych = n;
            break;
        } else if(mocnina > strlen(bfunkcia)){
            return NULL;
        }
        mocnina = mocnina * 2;
        n++;
    }

    int pocet_uzlov = 0;

    NODE **arr = (NODE**)malloc(strlen(bfunkcia) * sizeof(NODE));

    char str[MAX] = "";
    NODE *tmp = NULL;

    for (int i = 0; i < strlen(bfunkcia); ++i) {
        str[0] = bfunkcia[i];
        arr[i] = create_node(str);
        pocet_uzlov++;
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
            pocet_uzlov++;
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
    vysl->pocet_uzlov = pocet_uzlov;

    return vysl;
}

char BDD_use(BDD *bdd, char *vstupy){

    NODE *tmp = bdd->head;

    if (strlen(vstupy) != bdd->pocet_premennych) {
        return 2;
    }
    for (int i = 0; i < strlen(vstupy); ++i) {
        if (vstupy[i] == '0'){
            tmp = tmp->left;
        } else if (vstupy[i] == '1'){
            tmp = tmp->right;
        } else return  2;
    }
    return (char) tmp->data[0];
}

int main()
{
    BDD* bdd;
    bdd = BDD_create("1001010011001011");

    printf("%s\n", bdd->head->data);
    printf("pocet uzlov: %d\n", bdd->pocet_uzlov);
    printf("pocet premennych: %d\n", bdd->pocet_premennych);

    printf("%c\n", BDD_use(bdd, "0000"));
    printf("%c\n", BDD_use(bdd, "0001"));
    printf("%c\n", BDD_use(bdd, "0010"));
    printf("%c\n", BDD_use(bdd, "0011"));
    printf("%c\n", BDD_use(bdd, "0100"));
    printf("%c\n", BDD_use(bdd, "0101"));
    printf("%c\n", BDD_use(bdd, "0110"));
    printf("%c\n", BDD_use(bdd, "0111"));
    printf("%c\n", BDD_use(bdd, "1000"));
    printf("%c\n", BDD_use(bdd, "1001"));
    printf("%c\n", BDD_use(bdd, "1010"));
    printf("%c\n", BDD_use(bdd, "1011"));
    printf("%c\n", BDD_use(bdd, "1100"));
    printf("%c\n", BDD_use(bdd, "1101"));
    printf("%c\n", BDD_use(bdd, "1110"));
    printf("%c\n", BDD_use(bdd, "1111"));


    return 0;
}