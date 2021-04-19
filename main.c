/*********************************************
 * DSA Zadianie 3 - Binary decision diagram
 *
 * FIIT STU 2021
 * Pavol Hradsky
*********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 20

char* int2binary(int size, int vstup){

    int vysl = 0;
    int t = 0;
    char* str = malloc((size + 1) * sizeof(char));

    for (int i = size-1; i >= 0; i--) {
        vysl = vstup >> i;
        if(vysl & 1)
            *(str + t) = 1 + '0';
        else
            *(str + t) = 0 + '0';

        t++;
    }
    *(str + t) = '\0';

    return str;
}

int size_from_count(int size){
    int n = size;
    size = 1;

    for (int i = 0; i < n; ++i) {
        size *= 2;
    }

    return size;
}

typedef struct node {
    struct node *left;
    struct node *right;
    struct node *levelRight;
    char data[MAX];
}NODE;

typedef struct bdd {
    int pocet_premennych;
    int pocet_uzlov;
    int height;
    NODE *head;
}BDD;

NODE *create_node(char data[MAX]){
    NODE *result = NULL;
    result = (NODE*)malloc(sizeof(NODE));
    strcpy(result->data, data);
    result->left = NULL;
    result->right = NULL;
    result->levelRight = NULL;
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
        }
        mocnina = mocnina * 2;
        n++;
    }
    if(mocnina > strlen(bfunkcia)){
        printf("Vstup nie je vektor BF\n");
        return NULL;
    }

    int height = 0;
    int pocet_uzlov = 0;

    NODE **arr = (NODE**)malloc(strlen(bfunkcia) * sizeof(NODE));

    char str[MAX] = "";
    NODE *tmp = NULL;

    for (int i = 0; i < strlen(bfunkcia); ++i) {
        str[0] = bfunkcia[i];
        arr[i] = create_node(str);
        if(i != 0){
            arr[i-1]->levelRight = arr[i];
        }
        pocet_uzlov++;
    }
    height++;
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
//            arr[2 * i]->parent = tmp;
//            arr[2 * i+1]->parent = tmp;
            arrNew[i] = tmp;
            if(i != 0){
                arrNew[i-1]->levelRight = arrNew[i];
            }
            pocet_uzlov++;
        }
        height++;
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

    vysl->height = height;
    vysl->head = tmp;
    vysl->pocet_uzlov = pocet_uzlov;

    return vysl;
}

int BDD_reduce(BDD *bdd){

    NODE *tmp = bdd->head;
    NODE *tmp_in_line;
    int level = 0;
    int height = bdd->height;
    int n = 1;
    int current_size_of_arr_above = n;

    NODE** arr = (NODE**)malloc(n * sizeof(NODE*));
    NODE** arr_above = (NODE**)malloc(n * sizeof(NODE*));


    while (tmp != NULL){
        tmp_in_line = tmp;
        NODE* tmpToRemove = NULL;
        int i = 0;
        int isRemoved = 0;
        int count_of_removed = 0;
        while (tmp_in_line != NULL){
            arr[i] = tmp_in_line;
            tmp_in_line = tmp_in_line->levelRight;
            i++;
        }
        for(int first = 0; first < i; first++){
            for (int second = 0; second < i; ++second) {
                if(first < second && strcmp(arr[first]->data, arr[second]->data) == 0){
                    if(first == 0){
                        isRemoved = 1;
                    } else{
                        int j = first-1;
                        while(arr[j] == NULL && j >= 0){
                            j--;
                        }
                        if(j == 0){
                            tmp = arr[first]->levelRight;
                        }else if(arr[j] != NULL){
                            arr[j]->levelRight = arr[first]->levelRight;
                        }
                    }
                    if (isRemoved == 1){
                        tmp = arr[first]->levelRight;
                        isRemoved = 0;
                    }


                    for (int j = 0; j < current_size_of_arr_above; ++j) {
                        if(arr_above[j]->left == arr[first]){
                            arr_above[j]->left = arr[second];
                        } else if(arr_above[j]->right == arr[first]){
                            arr_above[j]->right = arr[second];
                        }
                    }

                    count_of_removed++;
                    if(first == 0){
                        tmpToRemove = arr[first];
                    } else{
                        arr[first]->levelRight = NULL;
                        free(arr[first]);
                        arr[first] = NULL;
                    }
                    break;
                }
            }
        }

        if(tmpToRemove != NULL){
            free(tmpToRemove);
        }

        free(arr_above);
        current_size_of_arr_above = n - count_of_removed;
        arr_above = (NODE**)malloc(current_size_of_arr_above * sizeof(NODE*));
        tmp_in_line = tmp;
        i = 0;
        while (tmp_in_line != NULL){
            arr_above[i] = tmp_in_line;
            tmp_in_line = tmp_in_line->levelRight;
            i++;
        }
        n *= 2;
        free(arr);
        arr = (NODE**)malloc(n * sizeof(NODE*));
        tmp = tmp->left;
    }

    return 0;
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

void print_tree(BDD *bdd){
    NODE *tmp = bdd->head;
    NODE *tmp_in_line = tmp;

    while (tmp != NULL){
        tmp_in_line = tmp;

        while (tmp_in_line != NULL){
            printf("%s   ", tmp_in_line->data);
            tmp_in_line = tmp_in_line->levelRight;
        }
        printf("\n");


        tmp = tmp->left;
    }
}

int main()
{
    BDD* bdd;
    bdd = BDD_create("10110101");
    if (bdd == NULL) {
        return 0;
    }

    printf("%s\n", bdd->head->data);
    printf("pocet uzlov: %d\n", bdd->pocet_uzlov);
    printf("pocet premennych: %d\n", bdd->pocet_premennych);

    //print_tree(bdd);

    BDD_reduce(bdd);

    print_tree(bdd);

    printf("%c\n", BDD_use(bdd, "000"));
    printf("%c\n", BDD_use(bdd, "001"));
    printf("%c\n", BDD_use(bdd, "010"));
    printf("%c\n", BDD_use(bdd, "011"));
    printf("%c\n", BDD_use(bdd, "100"));
    printf("%c\n", BDD_use(bdd, "101"));
    printf("%c\n", BDD_use(bdd, "110"));
    printf("%c\n", BDD_use(bdd, "111"));
//    printf("%c\n", BDD_use(bdd, "1000"));
//    printf("%c\n", BDD_use(bdd, "1001"));
//    printf("%c\n", BDD_use(bdd, "1010"));
//    printf("%c\n", BDD_use(bdd, "1011"));
//    printf("%c\n", BDD_use(bdd, "1100"));
//    printf("%c\n", BDD_use(bdd, "1101"));
//    printf("%c\n", BDD_use(bdd, "1110"));
//    printf("%c\n", BDD_use(bdd, "1111"));


//    int pocet_vstupov = 6;
//
//    char *str = "";
//    str = int2binary(size_from_count(pocet_vstupov), 123);
//    bdd = BDD_create(str);
//    if (bdd == NULL) {
//        return 0;
//    }
//
//    printf("%s\n", bdd->head->data);
//    printf("pocet uzlov: %d\n", bdd->pocet_uzlov);
//    printf("pocet premennych: %d\n", bdd->pocet_premennych);
//
//    for (int i = 0; i < strlen(str); ++i) {
//        printf("%c", BDD_use(bdd, int2binary(pocet_vstupov, i)));
//    }

//    char *str;
//    str = int2binary(4, 255);
//    printf("%s\n", str);


    return 0;
}