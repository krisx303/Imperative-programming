#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../libraries/inout.c"
#define N  100


// Losuje jedna liczbe calkowita z przedzialu [a,b] stosujac funkcje rand() i operator %
// Jezeli a>b to zwraca INT_MIN
// Jezeli b-a > RAND_MAX to zwraca INT_MAX
// Jezeli a=b to zwraca a (bez wywolywania funkcji rand)
// Else zwraca liczbe z przedzialu [a,b]
int rand_from_interval(int a, int b) {
    if(a > b){
        return INT_MIN;
    }else if(b - a > RAND_MAX){
        return INT_MAX;
    }else if(a == b){
        return a;
    }
    return rand() % (b - a + 1) + a;
}

// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
// wg algorytmu przedstawionego w opisie zadania
// jest zapisywana w tablicy tab.
// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int tab[]) {
    if(n < 0 || n > 100)
        return;
    for(int i = 0; i<n;i++){
        tab[i] = i;
    }
    for(int i = 0;i<n;i++){
        int k = rand_from_interval(i, n-1);
        int buffor = tab[i];
        tab[i] = tab[k];
        tab[k] = buffor;
    }
}

// Metoda babelkowa sortowania n elementow tablicy tab w porzadku od wartosci najmniejszej do najwiekszej.
// Zwraca numer iteracji petli zewnetrznej (liczony od 1), po ktorej tablica byla uporzadkowana,
// np. dla { 0 1 2 3 7 4 5 6 } -> 1,
//     dla { 1 2 3 7 4 5 6 0 } -> 7,
//     dla { 0 1 2 3 4 5 6 7 } -> 0.
int bubble_sort(int n,int tab[]) {
    int flag;
    for(int i = 0; i<n-1;i++){
        flag = 0;
        for(int j = 0; j<n-i-1;j++){
            if(tab[j] > tab[j+1]){
                int buffor = tab[j];
                tab[j] = tab[j+1];
                tab[j+1] = buffor;
                flag = 1;
            }
        }
        if(!flag){
            return i;
        }
    }
    return n-1;
}

typedef void (*Test)();

void test_1(){
    printDebug("Enter seed: ");
    int seed = readInt();
    srand(seed);
    printDebug("\nEnter a and b: ");
    int a = readInt();
    int b = readInt();
    for(int i=0; i<3; ++i) 
    printf("%d ",rand_from_interval(a, b));
}

void test_2(){
    printDebug("Enter seed: ");
    int seed = readInt();
    srand(seed);
    printDebug("\nEnter n: ");
    int n = readInt();
    int tab[N];
    rand_permutation(n, tab);
    for(int i=0; i<n; ++i) printf("%d ",tab[i]);
}

void test_3(){
    printDebug("Enter seed: ");
    int seed = readInt();
    srand(seed);
    printDebug("\nEnter n: ");
    int n = readInt();
    int tab[N];
    rand_permutation(n,tab);
    printf("%d\n",bubble_sort(n,tab));
}


int main(int argc, char* args[]){
	if(argc > 1 && args[1][0] == '1')
		TEST = 1;
    printDebug("Wpisz numer testu [1, 3]: ");
    int test = readInt();
    Test tests[] = {test_1, test_2, test_3};
    if(test > 3 || test < 1){
        printDebug("Numer testu %d jest spoza zakresu [1, 3]\n", test);
    }else{
        tests[test-1]();
    }
    return 0;
}