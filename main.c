/*********************************************
 * DSA Zadianie 3 - Binary decision diagram
 *
 * FIIT STU 2021
 * Pavol Hradsky
*********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX 10000

char* int2binary(long long size, long long vstup){  //pomocna funkcia, prekonvertuje cislo v 10 sustave na cislo
                                                    // v 2 sustave na size bitov a vrati retazec ako str
    long long vysl = 0;
    long long t = 0;
    char* str = malloc((size + 1) * sizeof(char));

    for (long long i = size-1; i >= 0; i--) {
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

long long size_from_count(long long size){  //pomocna funkcia, zisti pocet bitov binarnej funkcie pre size premennych
                                            //(2^n)
    long long n = size;
    size = 1;

    for (long long i = 0; i < n; ++i) {
        size *= 2;
    }

    return size;
}

typedef struct node {   //struktura uzlu v strome
    struct node *left;  //lave dieta
    struct node *right; //prave dieta
    struct node *levelRight;    //pravy sused
    char data[MAX]; //data typu string
}NODE;

typedef struct bdd {    //struktura hlavicky BDD
    long long pocet_premennych;
    long long pocet_uzlov;
    long long height;
    NODE *head; //ukazovatel na root stromu
}BDD;

NODE *create_node(char data[MAX]){  //funkcia dostane data (str) a vytvori uzol stromu
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
    vysl = (BDD*)malloc(sizeof(BDD));   //alokuje sa hlavicka bdd

    long long n = 1;    //pocet premennych
    long long mocnina = 2;  //konstanta na mocninu
    while (mocnina <= strlen(bfunkcia)){    //test, ci je spravny vstup
        if (mocnina == strlen(bfunkcia)){
            vysl->pocet_premennych = n;
            break;
        }
        mocnina = mocnina * 2;
        n++;
    }
    if(mocnina > strlen(bfunkcia)){ //ak vstup nie je spravny, vrati NULL
        printf("Vstup nie je vektor BF\n");
        return NULL;
    }

    long long height = 0;
    long long pocet_uzlov = 0;

    NODE **arr = (NODE**)malloc(strlen(bfunkcia) * sizeof(NODE));   //alokuje sa pole uzlov pre najspodnejsiu uroven

    char str[MAX] = "";
    NODE *tmp = NULL;   //vytvori sa tmp uzol

    for (long long i = 0; i < strlen(bfunkcia); ++i) {  //do pola arr sa vytvoria uzly s jednotlivymi znakmi bfunkcie
        str[0] = bfunkcia[i];
        arr[i] = create_node(str);
        if(i != 0){
            arr[i-1]->levelRight = arr[i];  //nastavia sa ukazovatele na praveho suseda uzlov
        }
        pocet_uzlov++;
    }
    height++;
//    for (int i = 0; i < strlen(bfunkcia); ++i) {
//        printf("%s\n", arr[i]->data);
//    }

    long long level = 2;    //pocita sa level
    do {    //postupne idem od spodu stromu ku korenu
        NODE **arrNew = (NODE**)malloc(strlen(bfunkcia) / level * sizeof(NODE));    //alokuje sa pole arrNew (uroven nad arr)

        for (long long i = 0; i < strlen(bfunkcia) / level; ++i) {  //pre tuto uroven sa vytvoria uzly tak,
                                                                // ze sa spoja vzdy 2 susediace uzly z urovne pod nou
            strcpy(str, arr[2 * i]->data);
            strcat(str, arr[2 * i + 1]->data);
            tmp = create_node(str);
            tmp->left = arr[2 * i]; //nastavia sa ukazovatele left a right na 2 susedne uzlu pod tmp
            tmp->right = arr[2 * i+1];
//            arr[2 * i]->parent = tmp;
//            arr[2 * i+1]->parent = tmp;
            arrNew[i] = tmp;    //tmp sa skopiruje do pola arrNew na dany index
            if(i != 0){
                arrNew[i-1]->levelRight = arrNew[i];    //nastavi sa ukazovatel na praveho suseda na tejto urovni
            }
            pocet_uzlov++;  //pocita pocet uzlov
        }
        height++;   //pocita vysku
        free(arr);  //pole arr sa uvolni
        arr = (NODE**)malloc(strlen(bfunkcia) / level * sizeof(NODE));  //pole arr sa alokuje vo velkosti ako arrNew
        for (long long i = 0; i < strlen(bfunkcia) / level; ++i) {  //arrNew sa skopiruje do arr
            arr[i] = arrNew[i];
        }
        free(arrNew);   //uvolni sa arr

//        printf("ARR:\n");
//        for (int i = 0; i < strlen(bfunkcia) / level; ++i) {
//            printf("%s\n", arr[i]->data);
//        }
        level *= 2; //premenna level sa umocni 2

    }while(strcmp(str, bfunkcia) != 0);     //tento cyklus sa opakuje, az kym sa premenna str nerovna bfunkcii (koren)
    free(arr);  //uvolni sa arr

    vysl->height = height;  //ulozi sa vyska, pocet uzlov a hlavicka
    vysl->head = tmp;
    vysl->pocet_uzlov = pocet_uzlov;

    return vysl;    //vrati sa bdd
}

int BDD_reduce(BDD *bdd){   //funkcia redukuje bdd

    NODE *tmp = bdd->head;  //tmp sa nastavi na hlavicku bdd
    NODE *tmp_in_line;  //tmp_in_line bude prehladavat vzdy jednu uroven stromu po ukazovateloch levelRight
    long long n = 1;
    long long current_size_of_arr_above = n;

    NODE** arr = (NODE**)malloc(n * sizeof(NODE*)); //alokuje sa arr a arr_above
    NODE** arr_above = (NODE**)malloc(n * sizeof(NODE*));


    while (tmp != NULL){    //strom sa prechadza po lavych uzloch od korena smerom nadol
        tmp_in_line = tmp;  //tmp je teda vzdy lavy uzol na danej urovni v strome
        long long i = 0;
        while (tmp_in_line != NULL){    //nastavi sa pole arr ako jedna uroven (uzly vedla seba) stromu
            arr[i] = tmp_in_line;
            tmp_in_line = tmp_in_line->levelRight;
            i++;
        }
        for(long long first = i-1; first >= 0; first--){    //od konca porovnavam kazdy s kazdym uzolm na danej urovni
            for (long long second = i-1; second >= 0; second--) {
                if(first > second && strcmp(arr[first]->data, arr[second]->data) == 0){ //ak je first > second a zaroven
                    //sa data na danych uzloch rovnaju, odstranim ten viac v pravo (first)

                    for (long long j = 0; j < current_size_of_arr_above; ++j) { //v urovni nad nou, pozeram ci nejaky
                        // uzol ukazuje left alebo right na vymazany uzol, ak ano, prepisem ho na druhy uzol ktory ostal
                        if(arr_above[j]->left == arr[first]){
                            arr_above[j]->left = arr[second];
                        } if(arr_above[j]->right == arr[first]){
                            arr_above[j]->right = arr[second];
                        }
                    }

                    arr[first-1]->levelRight = arr[first]->levelRight;  //v spajanom zozname obidem vymazany uzol

                    free(arr[first]);   //tento uzol uvolnim
                    bdd->pocet_uzlov--; //decrementujem pocet uzlov v strome
                    break;  //kedze uzol first som vymazal, nema zmysel ho dalej porovnavat, tak sa posuniem dalej
                }
            }
        }

        free(arr_above);    //uvolnim pole arr_above
        current_size_of_arr_above = 0;
        tmp_in_line = tmp;
        while(tmp_in_line != NULL){ //postupne prejdem danu uroven a zistim, kolko je tam uzlov
            current_size_of_arr_above++;
            tmp_in_line = tmp_in_line->levelRight;
        }
        arr_above = (NODE**)malloc(current_size_of_arr_above * sizeof(NODE*));  //podla toho alokujem nove pole arr_above
        tmp_in_line = tmp;
        i = 0;
        while (tmp_in_line != NULL){    //v tomto alokovanom poli nastavim hodnoty na uzly na danej urovni
            arr_above[i] = tmp_in_line;
            tmp_in_line = tmp_in_line->levelRight;
            i++;
        }
        n *= 2; //n vynasobim 2, cim dostanem pocet uzlov na dalsej urovni
        free(arr);  //uvolnim arr
        arr = (NODE**)malloc(n * sizeof(NODE*));    //alokujem arr na pocet n
        tmp = tmp->left;    //posuniem sa o uroven nizsie
    }

    return 0;
}


char BDD_use(BDD *bdd, char *vstupy){

    NODE *tmp = bdd->head;  //vytvorenie premennej tmo, s ktorou sa budem hybat po strome


    if (strlen(vstupy) != bdd->pocet_premennych) {  //zistim ci som dostal dobry vstup
        return 2;
    }
    for (int i = 0; i < strlen(vstupy); ++i) {  //prehladavam strom, ak mam na danom indexe vo vektore 0,
                                                // posuniem sa dolava, ak 1 tak doprava
        if (vstupy[i] == '0'){
            tmp = tmp->left;
        } else if (vstupy[i] == '1'){
            tmp = tmp->right;
        } else return  2;
    }
    return (char) tmp->data[0]; //vratim hodnotu posledneho uzlu (0 alebo 1)
}

void print_tree(BDD *bdd){  //pomocna funkcia, ktora mi vypise strom
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

int test_to_6(BDD *bdd){    //test, kde manualne vlozim vektor 6 premennych a potom ho prehladavam
    //    bdd = BDD_create("1011001011010001010010111011001011010010101100101110110100010100");
    bdd = BDD_create("0000000000000000000000000000000000000000000000000000000000000001");
    if (bdd == NULL) {
        return 0;
    }
    BDD_reduce(bdd);
    print_tree(bdd);

    printf("pocet uzlov: %d\n", bdd->pocet_uzlov);
    printf("pocet premennych: %d\n", bdd->pocet_premennych);
    printf("%s\n", bdd->head->data);

    printf("%c", BDD_use(bdd, "000000"));
    printf("%c", BDD_use(bdd, "000001"));
    printf("%c", BDD_use(bdd, "000010"));
    printf("%c", BDD_use(bdd, "000011"));
    printf("%c", BDD_use(bdd, "000100"));
    printf("%c", BDD_use(bdd, "000101"));
    printf("%c", BDD_use(bdd, "000110"));
    printf("%c", BDD_use(bdd, "000111"));
    printf("%c", BDD_use(bdd, "001000"));
    printf("%c", BDD_use(bdd, "001001"));
    printf("%c", BDD_use(bdd, "001010"));
    printf("%c", BDD_use(bdd, "001011"));
    printf("%c", BDD_use(bdd, "001100"));
    printf("%c", BDD_use(bdd, "001101"));
    printf("%c", BDD_use(bdd, "001110"));
    printf("%c", BDD_use(bdd, "001111"));
    printf("%c", BDD_use(bdd, "010000"));
    printf("%c", BDD_use(bdd, "010001"));
    printf("%c", BDD_use(bdd, "010010"));
    printf("%c", BDD_use(bdd, "010011"));
    printf("%c", BDD_use(bdd, "010100"));
    printf("%c", BDD_use(bdd, "010101"));
    printf("%c", BDD_use(bdd, "010110"));
    printf("%c", BDD_use(bdd, "010111"));
    printf("%c", BDD_use(bdd, "011000"));
    printf("%c", BDD_use(bdd, "011001"));
    printf("%c", BDD_use(bdd, "011010"));
    printf("%c", BDD_use(bdd, "011011"));
    printf("%c", BDD_use(bdd, "011100"));
    printf("%c", BDD_use(bdd, "011101"));
    printf("%c", BDD_use(bdd, "011110"));
    printf("%c", BDD_use(bdd, "011111"));

    printf("%c", BDD_use(bdd, "100000"));
    printf("%c", BDD_use(bdd, "100001"));
    printf("%c", BDD_use(bdd, "100010"));
    printf("%c", BDD_use(bdd, "100011"));
    printf("%c", BDD_use(bdd, "100100"));
    printf("%c", BDD_use(bdd, "100101"));
    printf("%c", BDD_use(bdd, "100110"));
    printf("%c", BDD_use(bdd, "100111"));
    printf("%c", BDD_use(bdd, "101000"));
    printf("%c", BDD_use(bdd, "101001"));
    printf("%c", BDD_use(bdd, "101010"));
    printf("%c", BDD_use(bdd, "101011"));
    printf("%c", BDD_use(bdd, "101100"));
    printf("%c", BDD_use(bdd, "101101"));
    printf("%c", BDD_use(bdd, "101110"));
    printf("%c", BDD_use(bdd, "101111"));
    printf("%c", BDD_use(bdd, "110000"));
    printf("%c", BDD_use(bdd, "110001"));
    printf("%c", BDD_use(bdd, "110010"));
    printf("%c", BDD_use(bdd, "110011"));
    printf("%c", BDD_use(bdd, "110100"));
    printf("%c", BDD_use(bdd, "110101"));
    printf("%c", BDD_use(bdd, "110110"));
    printf("%c", BDD_use(bdd, "110111"));
    printf("%c", BDD_use(bdd, "111000"));
    printf("%c", BDD_use(bdd, "111001"));
    printf("%c", BDD_use(bdd, "111010"));
    printf("%c", BDD_use(bdd, "111011"));
    printf("%c", BDD_use(bdd, "111100"));
    printf("%c", BDD_use(bdd, "111101"));
    printf("%c", BDD_use(bdd, "111110"));
    printf("%c", BDD_use(bdd, "111111"));

    return 0;
}

int test_for_x(BDD *bdd, int x, int cislo){ //test, kde zadam pocet vstupov a cislo,
                                            // funkcia mi vytvori a zredukuje strom a potom ho prehladava
    int pocet_vstupov = x;

    char *str = "";
    str = int2binary(size_from_count(pocet_vstupov), cislo);
    bdd = BDD_create(str);
    if (bdd == NULL) {
        return 0;
    }
    BDD_reduce(bdd);

    //print_tree(bdd);

    //printf("pocet uzlov: %d\n", bdd->pocet_uzlov);
    //printf("pocet premennych: %d\n", bdd->pocet_premennych);
    //printf("%s\n", bdd->head->data);

    for (int i = 0; i < strlen(str); ++i) {
        BDD_use(bdd, int2binary(pocet_vstupov, i));
        //printf("%c", );
    }
    //printf("\n\n");
    return 0;
}

int main()
{
    BDD *bdd = NULL;
//    test_to_6(bdd);
//    test_for_x(bdd, 13, 436578);

    long long n;
    time_t t;
    srand((unsigned) time(&t)); //random hodnoty

//    for (int i = 0; i < 16; ++i) {
//        test_for_x(bdd, 2, i);
//    }

//    for (int i = 0; i < 256; ++i) {
//        test_for_x(bdd, 3, i);
//    }

    clock_t start, end;
    double cpu_time_used;

    start = clock();    //zapne casovac

    for(int j = 2; j < 14; j++){    //postupne vytvori strom s 2 - 13 premennymi a skusa pouzit 10 nahodnych vektorov
        for (int i = 0; i < 10; ++i) {
            n = pow(2, size_from_count(j));
            test_for_x(bdd, j, rand() % n);
        }
    }
    printf("ok\n");

    end = clock();      //skonci casovac
    cpu_time_used = ((double) (end - start));
    printf("Time: %lfms\n", cpu_time_used);


    return 0;
}