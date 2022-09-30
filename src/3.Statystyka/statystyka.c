#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../libraries/inout.c"

// Zwraca wartość średniej arytmetycznej oraz wariancji elemntów tablicy o długości n.
// dla n = 0 zwracana jest pusta tablica
// dla n = 1 zwracana jest tablica jednoelementowa
void aver_varian(double v[], double *arith_aver, double *varian, int n)
{
    if(n == 0){
        *arith_aver = 0;
        *varian = 0;
        return;
    }
    double sum = 0.0;
    for(int i = 0; i< n;i++){
        sum += v[i];
    }
    *arith_aver = (sum)/n;
    sum = 0.0;
    for(int i = 0; i<n; i++){
        sum += (v[i] - *arith_aver)*(v[i] - *arith_aver);
    }
    *varian = (sum)/n;
}

// Wypełnianie tablicy wynikiem n prób Bernoulliego
// 1 - orzeł; 0 - reszka
// Losowanie realizowane na 100-elemntowym zbiorze liczb całkowitych
void bernoulli_gen(int v[], int bernoulli_prop, int n)
{
    for(int i = 0; i<n;i++){
        int k = rand() % (100);
        if(k >= bernoulli_prop){
            v[i] = 1;
        }else{
            v[i] = 0;
        }
    }
}

// Dyskretny rozkład prawdopodobieństwa reprezentujący procentowy udział poszczególnych wartości zmiennej losowej
// liczba elementów biorących udział w losowaniu to 11 bo na 2 kostkach możemy uzyskać liczby [2-12]
// trials_num - liczba losowań

void probab_distrib(double v[], int trials_num)
{
    for(int i = 0; i<12;i++){
        v[i] = 0;
    }
    for(int i = 0; i<trials_num; i++){
        int a = rand() % 6;
        int b = rand() % 6;
        v[a+b]++;
    }
    for(int i = 0; i<12;i++){
        v[i] = (v[i]/trials_num)*100;
    }
}

// Oblicza i zwraca Dystrybuantę (Cumulative Distribution Function)
// len - liczba wylosowanych elementów z przediału
void var_dystryb(double dystr[], int len)
{
    for(int i = 0; i<12;i++){
        dystr[i] = 0.0;
    }
    for(int i = 0; i<len; i++){
        int a = rand() % 6;
        int b = rand() % 6;
        dystr[a+b]+=1.0;
    }
    double ll = 0, lll = len;
    for(int i = 11; i>-1;i--){
        ll = lll - dystr[i];
        dystr[i] = (lll/len)*100;
        lll = ll;
    }
}

// Symuluje problem Monty'ego-Hall'a dla przypadku 3-bramokowego
//Wejście: n - liczba prób
//Wyjście: switch_wins - liczba wygranych przez zamianę, switch_probabil - prawdopodobieństwo wygranej przez zamianę
//Wyjście: non_switch_wins - liczba wygranych bez zamiany, non_switch_probabil - prawdopodobieństwo wygranej bez zamiany
void monty_hall(int *switch_wins, int *nonswitch_wins, double *switch_probabil, double *nonswitch_probabil, int n)
{
    int choice, door[3];
    for(int i=0; i<n; i++)
    {
        for(int j = 0; j<3; j++){
            door[j] = 0;
        }
        door[0] = (!(rand()%2)) ? 1: 0;
        if(!door[0]){
            int r = (!(rand()%2)) ? 1: 0;
            door[1] = r;
            door[2] = !r;
        }
        //dlaczego po prostu nie zrobić door[rand() % 3] = 1 ??
        choice = rand() % 3;
        if(door[choice]){
            (*nonswitch_wins)++;
        }else{
            (*switch_wins)++;
        }
    }
    *switch_probabil = ((double)(*switch_wins)/n)*100;
    *nonswitch_probabil = ((double)(*nonswitch_wins)/n)*100;
}

// read double vector of size n
void read_vector(double v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        scanf("%lf", v + i);
    }
}

// read int vector of size n
void read_int_vector(int v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", v + i);
    }
}

// print double vector of size n (with 2 figures after the decimal point)
void print_vector(const double v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%.2f ", v[i]);
    }
    printf("\n");
}

// print integer vector
void print_bern_vector(int bern_v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", bern_v[i]);
    }
    printf("\n");
}

typedef void (*Test)();

void test_1(){
    double arith_aver = 0.0, varian = 0.0, dtab[100];
    int len = readIntHint("aver_varian: Wpisz dlugosc wektora: ");
     if(len < 0 || len > 100){
        printf("n < 0 lub n > 100\n");
        return;
    }
    printDebug("Wpisz elementy wektora: ");
    readDoubleArray(dtab, len);
    aver_varian(dtab, &arith_aver, &varian, len);
    printDebug("średnia arith_aver = ");
    printf("%.2f ", arith_aver);
    printDebug(" wariancja = ");
    printf("%.2f\n ", varian);
}

void test_2(){
    int seed = readIntHint("Enter seed: ");
    srand(seed);
    int len = readIntHint("BERNOULLI_GEN: Wpisz dlugość wektora: ");
    if(len < 0 || len > 100){
        printf("n < 0 lub n > 100\n");
        return;
    }
    int itab[100];
    int propabl = readIntHint("Wpisz prawdopodobieństwo reszki w %%: ");
    bernoulli_gen(itab, propabl, len);
    print_bern_vector(itab, len);
}

void test_3(){
    double dtab[100];
    int seed = readIntHint("Enter seed: ");
    srand(seed);
    int len = readIntHint("PROBAB_DISTRIB: Wpisz liczbę prób: ");
    if(len < 0 || len > 1000000)
    {
        printf("n < 0 lub n > 1000000\n");
        return;
    }
    probab_distrib(dtab, len);
    print_vector(dtab, 11); //
}

void test_4(){
    double dtab[100];
    int seed = readIntHint("Enter seed: ");
    srand(seed);
    int len = readIntHint("VAR_DYSTRYB: Wpisz liczbę rzutów dwiema kostkami: ");
    if(len < 0 || len > 1000000){
        printf("n<0 lub n > 1000000\n");
        return;
    }
    printDebug("Wpisz elementy wektora: ");
    var_dystryb(dtab, len);
    print_vector(dtab, 11); //11 ponieważ w przypadku rzutu 2 kostkami istnieje 11 wartości zmiennej losowej [2-12]
}

void test_5(){
    int m_h_wins, non_m_h_wins;
    double m_h_probabil, non_m_h_probabil;
    int seed = readIntHint("Enter seed: ");
    srand(seed);
    int len = readIntHint("MONTY_HALL: Wpisz liczbę prób: ");
    m_h_wins = non_m_h_wins=0;
    m_h_probabil = non_m_h_probabil = 0.0;
    if(len < 0 || len > 1000000){
        printf("n<0 lub n > 1000000\n");
        return;
    }
    monty_hall(&m_h_wins, &non_m_h_wins, &m_h_probabil, &non_m_h_probabil, len);
    printDebug("Po %d grach, wygrałem ", len);
    printf("%d ", m_h_wins);
    printDebug("razy przez zmianę wyboru. To jest [%%] ");
    printf("%.2f ", m_h_probabil);
    printDebug("Natomiast, nie zmieniając wyboru wygrałem ");
    printf("%d ", non_m_h_wins);
    printDebug("razy. To jest [%%] ");
    printf("%.2f\n", non_m_h_probabil);
}

int main(int argc, char* args[]){
	if(argc > 1 && args[1][0] == '1')
		TEST = 1;
    printDebug("Wpisz numer testu [1, 5]: ");
    int test = readInt();
    Test tests[] = {test_1, test_2, test_3, test_4, test_5};
    if(test > 5 || test < 1){
        printDebug("Numer testu %d jest spoza zakresu [1, 5]\n", test);
    }else{
        tests[test-1]();
    }
    return 0;
}