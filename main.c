/*********************************************
 * DSA Zadianie 3 - Binary decision diagram
 *
 * FIIT STU 2021
 * Pavol Hradsky
*********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100

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

//int BDD_reduce(BDD *bdd){
//
//    NODE *tmp = bdd->head;
//    NODE *tmp_in_line;
//    int level = 0;
//    int height = bdd->height;
//    int n = 1;
//    int current_size_of_arr_above = n;
//
//    NODE** arr = (NODE**)malloc(n * sizeof(NODE*));
//    NODE** arr_above = (NODE**)malloc(n * sizeof(NODE*));
//
//
//    while (tmp != NULL){
//        tmp_in_line = tmp;
//        NODE* tmpToRemove = NULL;
//        NODE* tmpLast = NULL;
//        int i = 0;
//        int isRemoved = 0;
//        int count_of_removed = 0;
//        while (tmp_in_line != NULL){
//            arr[i] = tmp_in_line;
//            tmp_in_line = tmp_in_line->levelRight;
//            i++;
//        }
//        for(int first = 0; first < i; first++){
//            for (int second = 0; second < i; ++second) {
//                if(first < second && strcmp(arr[first]->data, arr[second]->data) == 0){
//                    if(first == 0){
//                        isRemoved = 1;
//                    } else{
////                        NODE *tmp1 = tmp;
////                        while (tmp1 != arr[first] && tmp1 != NULL){
////                            tmp1 = tmp1->levelRight;
////                        }
//                        if(tmp == arr[first]){
//                            if(arr[second] == arr[second-1]){
//
//                            }else{
//                                tmp = arr[first]->levelRight;
//                            }
//                        }else if(arr[first] != NULL){
//                            tmpLast = arr[first]->levelRight;
////                            arr[j]->levelRight = arr[first]->levelRight;
//                        }
//                    }
//                    if (isRemoved == 1){
//                        tmp = arr[first]->levelRight;
//                        isRemoved = 0;
//                    }
//
//
//                    for (int j = 0; j < current_size_of_arr_above; ++j) {
//                        if(arr_above[j]->left == arr[first]){
//                            arr_above[j]->left = arr[second];
//
//                            if(j*2 < second && arr_above[j]->left != arr_above[j]->right){
//                                arr[second-1]->levelRight = arr[second]->levelRight;
//                                if(tmpLast != NULL){
//                                    arr[second]->levelRight = tmpLast;
//                                }else{
//                                    tmp = arr[second];
//                                    if(arr[first] == arr[second-1]){
//                                        arr[second]->levelRight = NULL;
//                                    }else{
//                                        arr[second]->levelRight = arr[second-1];
//                                    }
//                                }
//                            }
//
//
//                        } if(arr_above[j]->right == arr[first]){
//                            arr_above[j]->right = arr[second];
//                        }
//                    }
//
//                    //count_of_removed++;
//                    if(first == 0){
//                        tmpToRemove = arr[first];
//                        strcpy(arr[first]->data, "2");
//                    } else{
//                        arr[first]->levelRight = NULL;
//                        free(arr[first]);
//                        arr[first] = NULL;
//                    }
//                    bdd->pocet_uzlov--;
//                    break;
//                }
//            }
//        }
//
//        if(tmpToRemove != NULL){
//            free(tmpToRemove);
//        }
//
//        free(arr_above);
//        current_size_of_arr_above = 0;
//        tmp_in_line = tmp;
//        while(tmp_in_line != NULL){
//            current_size_of_arr_above++;
//            tmp_in_line = tmp_in_line->levelRight;
//        }
//        arr_above = (NODE**)malloc(current_size_of_arr_above * sizeof(NODE*));
//        tmp_in_line = tmp;
//        i = 0;
//        while (tmp_in_line != NULL){
//            arr_above[i] = tmp_in_line;
//            tmp_in_line = tmp_in_line->levelRight;
//            i++;
//        }
//        n *= 2;
//        free(arr);
//        arr = (NODE**)malloc(n * sizeof(NODE*));
//        tmp = tmp->left;
//    }
//
//    return 0;
//}

int BDD_reduce(BDD *bdd){

    NODE *tmp = bdd->head;
    NODE *tmp_in_line;
    int n = 1;
    int current_size_of_arr_above = n;

    NODE** arr = (NODE**)malloc(n * sizeof(NODE*));
    NODE** arr_above = (NODE**)malloc(n * sizeof(NODE*));


    while (tmp != NULL){
        tmp_in_line = tmp;
        int i = 0;
        while (tmp_in_line != NULL){
            arr[i] = tmp_in_line;
            tmp_in_line = tmp_in_line->levelRight;
            i++;
        }
        for(int first = i-1; first >= 0; first--){
            for (int second = i-1; second >= 0; second--) {
                if(first > second && strcmp(arr[first]->data, arr[second]->data) == 0){


                    for (int j = 0; j < current_size_of_arr_above; ++j) {
                        if(arr_above[j]->left == arr[first]){
                            arr_above[j]->left = arr[second];
                        } if(arr_above[j]->right == arr[first]){
                            arr_above[j]->right = arr[second];
                        }
                    }

                    arr[first-1]->levelRight = arr[first]->levelRight;

                    free(arr[first]);
                    bdd->pocet_uzlov--;
                    break;
                }
            }
        }

        free(arr_above);
        current_size_of_arr_above = 0;
        tmp_in_line = tmp;
        while(tmp_in_line != NULL){
            current_size_of_arr_above++;
            tmp_in_line = tmp_in_line->levelRight;
        }
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
    bdd = BDD_create("00000000");
//    bdd = BDD_create("0000000000000000000000000000000000000000000000000000000000000001");
    if (bdd == NULL) {
        return 0;
    }
    BDD_reduce(bdd);
    print_tree(bdd);

    printf("pocet uzlov: %d\n", bdd->pocet_uzlov);
    printf("pocet premennych: %d\n", bdd->pocet_premennych);
    printf("%s\n", bdd->head->data);


    printf("%c", BDD_use(bdd, "000"));
    printf("%c", BDD_use(bdd, "001"));
    printf("%c", BDD_use(bdd, "010"));
    printf("%c", BDD_use(bdd, "011"));
    printf("%c", BDD_use(bdd, "100"));
    printf("%c", BDD_use(bdd, "101"));
    printf("%c", BDD_use(bdd, "110"));
    printf("%c", BDD_use(bdd, "111"));



//    printf("%c", BDD_use(bdd, "000000"));
//    printf("%c", BDD_use(bdd, "000001"));
//    printf("%c", BDD_use(bdd, "000010"));
//    printf("%c", BDD_use(bdd, "000011"));
//    printf("%c", BDD_use(bdd, "000100"));
//    printf("%c", BDD_use(bdd, "000101"));
//    printf("%c", BDD_use(bdd, "000110"));
//    printf("%c", BDD_use(bdd, "000111"));
//    printf("%c", BDD_use(bdd, "001000"));
//    printf("%c", BDD_use(bdd, "001001"));
//    printf("%c", BDD_use(bdd, "001010"));
//    printf("%c", BDD_use(bdd, "001011"));
//    printf("%c", BDD_use(bdd, "001100"));
//    printf("%c", BDD_use(bdd, "001101"));
//    printf("%c", BDD_use(bdd, "001110"));
//    printf("%c", BDD_use(bdd, "001111"));
//    printf("%c", BDD_use(bdd, "010000"));
//    printf("%c", BDD_use(bdd, "010001"));
//    printf("%c", BDD_use(bdd, "010010"));
//    printf("%c", BDD_use(bdd, "010011"));
//    printf("%c", BDD_use(bdd, "010100"));
//    printf("%c", BDD_use(bdd, "010101"));
//    printf("%c", BDD_use(bdd, "010110"));
//    printf("%c", BDD_use(bdd, "010111"));
//    printf("%c", BDD_use(bdd, "011000"));
//    printf("%c", BDD_use(bdd, "011001"));
//    printf("%c", BDD_use(bdd, "011010"));
//    printf("%c", BDD_use(bdd, "011011"));
//    printf("%c", BDD_use(bdd, "011100"));
//    printf("%c", BDD_use(bdd, "011101"));
//    printf("%c", BDD_use(bdd, "011110"));
//    printf("%c", BDD_use(bdd, "011111"));
//
//    printf("%c", BDD_use(bdd, "100000"));
//    printf("%c", BDD_use(bdd, "100001"));
//    printf("%c", BDD_use(bdd, "100010"));
//    printf("%c", BDD_use(bdd, "100011"));
//    printf("%c", BDD_use(bdd, "100100"));
//    printf("%c", BDD_use(bdd, "100101"));
//    printf("%c", BDD_use(bdd, "100110"));
//    printf("%c", BDD_use(bdd, "100111"));
//    printf("%c", BDD_use(bdd, "101000"));
//    printf("%c", BDD_use(bdd, "101001"));
//    printf("%c", BDD_use(bdd, "101010"));
//    printf("%c", BDD_use(bdd, "101011"));
//    printf("%c", BDD_use(bdd, "101100"));
//    printf("%c", BDD_use(bdd, "101101"));
//    printf("%c", BDD_use(bdd, "101110"));
//    printf("%c", BDD_use(bdd, "101111"));
//    printf("%c", BDD_use(bdd, "110000"));
//    printf("%c", BDD_use(bdd, "110001"));
//    printf("%c", BDD_use(bdd, "110010"));
//    printf("%c", BDD_use(bdd, "110011"));
//    printf("%c", BDD_use(bdd, "110100"));
//    printf("%c", BDD_use(bdd, "110101"));
//    printf("%c", BDD_use(bdd, "110110"));
//    printf("%c", BDD_use(bdd, "110111"));
//    printf("%c", BDD_use(bdd, "111000"));
//    printf("%c", BDD_use(bdd, "111001"));
//    printf("%c", BDD_use(bdd, "111010"));
//    printf("%c", BDD_use(bdd, "111011"));
//    printf("%c", BDD_use(bdd, "111100"));
//    printf("%c", BDD_use(bdd, "111101"));
//    printf("%c", BDD_use(bdd, "111110"));
//    printf("%c", BDD_use(bdd, "111111"));


//    int pocet_vstupov = 4;
//
//    char *str = "";
//    str = int2binary(size_from_count(pocet_vstupov), 4);
//    bdd = BDD_create(str);
//    if (bdd == NULL) {
//        return 0;
//    }
//    BDD_reduce(bdd);
//
//    print_tree(bdd);
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