#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000
#include "../../libraries/inout.c"

// Funkcje dla punktu 8.1.1 Macierze prostokatne

//void copy_mat(const int, int const, double*, const double*);
//
////  Oblicza S = A + B
//void sum_mat(int, int, const double*, double*, double*);

double get(const int, const int, const int, double*);

void set(const int, const int, const int, double*, const double);

void prod_mat(const int, const int, const int, const double*, const double*, double*);
//void prod_mat_v2(const int, const int, const int, ...   );
//// Nalezy uzupelnic liste parametrow - z parametrami typu tablicy 1D
//// W ciele funkcji nalezy zdefiniowaæ wskazniki do tablic 2D, do ktorych nalezy przepisac
//// (z odpowiednim rzutowaniem) parametry A, B i AB.
//// W algorytmie mnozenia macierzy bedzie mozna stosowac notacje indeksowa X[i][j]
//
//void prod_mat_v3(const int, const int, const int, ...   );
//// Nalezy uzupelnic liste parametrow - z parametrami typu tablicy 2D
//// W algorytmie mnozenia macierzy bedzie mozna stosowac notacje indeksowa X[i][j]
//
double get(const int row, const int col, const int cols, double* MAT){
    return *(MAT+(row*cols)+col);
}

void set(const int row, const int col, const int cols, double* MAT, const double val){
    *(MAT+(row*cols)+col) = val;
}

void prod_mat(const int rowsA, const int colsA, const int colsB, const double* A, const double* B, double* C){
    for(int y = 0; y<rowsA;y++){
        for(int x = 0;x<colsB;x++){
            double s = 0;
            for(int i = 0;i<colsA;i++){
                s = s + get(y, i, colsA, (double*) A) * get(i, x, colsB, (double*)B);
            }
            set(y, x, colsB, C, s);
        }
    }
}

void read_mat(const int rows, const int cols, double *t) {
    for(int i=0; i<rows*cols; ++i, ++t) scanf("%lf",t);
}

void print_mat(int rows, int cols, double *t) {
  for(int  r=0; r<rows; ++r) {
    for(int c=0; c<cols; ++c) printf("%.2f ",*t++);
    printf("\n");
  }
}

//// Funkcje do punktow 8.1.2, 8.1.3 i 8.1.4
//
//
//// 8.1.3.  Funkcje dla tablicy wierszy znakowych
//// Wskazane uzycie funkcji fgets(), memchr(), malloc()
int read_char_lines(char *tab[], int lines_count, FILE *stream){
    char buffor[100];
    int idc = 0;
    char c;
    fgetc(stream);
    while(1){
        c = fgetc(stream);
        if(c == EOF) break;
        if(c == '\n'){
            char *ccc = (char*)malloc((idc+1) * sizeof(char));
            tab[lines_count] = ccc;
            for(int j = 0; j<idc;j++){
                *(ccc+j) = buffor[j];
            }
            idc = 0;
            lines_count++;
        }else{
            buffor[idc] = c;
            idc++;
        }
    }
    return lines_count;
}
void write_char_line(char *tab[], int n) {
    char c;
    int i = 0;
    while(1){
        c = *(tab[n]+i);
        if(c == '\0') break;
        printf("%c", c);
        i++;
    }
}

//// Nalezy pamietac o zwolnieniu przydzialonej pamieci
void delete_lines(char *tab[], int line_count){

}
//
//
// 8.1.2.  Funkcje dla tablicy wierszy liczbowych w obszarze ciągłym
// Wskazane uzycie funkcji fgets(), strtod().

int read_dbl_lines_v1(double *ptr_tab[], int lines_counter, FILE *stream){
    char *ptr;
    char buffor[10];
    int idc = 0, item = 0;
    double ret;
    char c;
    double series_table[TAB_SIZE];
    int first = 1;

    while(1){
        c = fgetc(stream);
        if(c == EOF) break;
        if(c == '\n'){
            ret = strtod(buffor, &ptr);
            series_table[item] = ret;
            series_table[item+1] = NAN;
            item+=2;
            first = 1;
            for(int j = 0; j<10;j++) buffor[j] = ' ';
            idc = 0;
        }
        else if(c == ' '){
            ret = strtod(buffor, &ptr);
            series_table[item] = ret;
            if(first == 1){
                ptr_tab[lines_counter] = &series_table[item];
                lines_counter++;
            }
            first = 0;
            item++;
            for(int j = 0; j<10;j++) buffor[j] = ' ';
            idc = 0;
        }else{
            buffor[idc] = c;
            idc++;
        }
    }
    return lines_counter;
}

void write_dbl_line_v1(double *ptr_tab[], int n){
    int k = 0;
    double d = 0;
    while(!isnan(d)){
        d = *(ptr_tab[n-1]+k);
        if(isnan(d)){
            break;
        }
        printf("%.2lf ", d);
        k++;
    }
}
//
//
//// 8.1.4.  Funkcje dla tablicy wierszy liczbowych w odrebnych obszarach
//// Wskazane uzycie funkcji fgets(), strtod(), malloc().

struct line {
    double *values;
    int len;
    double average;
};

typedef struct line line_type;

int read_dbl_lines(line_type lines_tab[], int line_count, FILE *stream){
    char *ptr;
    char buffor[10];
    int idc = 0, item = 0;
    double ret;
    char c;
    double series_table[TAB_SIZE];
    while(1){
        c = fgetc(stream);
        if(c == EOF) break;
        if(c == '\n'){
            ret = strtod(buffor, &ptr);
            series_table[item] = ret;
            item++;
            for(int j = 0; j<10;j++) buffor[j] = ' ';
            double *d = (double*)malloc((item) * sizeof(double));
            double s = 0.0;
            for(int o = 0; o<item; o++){
                *(d+o) = series_table[o];
                s += series_table[o];
            }
            lines_tab[line_count].len = item;
            lines_tab[line_count].values = d;
            lines_tab[line_count].average = s/item;
            idc = 0;
            item = 0;
            line_count++;
        }
        else if(c == ' '){
            ret = strtod(buffor, &ptr);
            series_table[item] = ret;
            item++;
            for(int j = 0; j<10;j++) buffor[j] = ' ';
            idc = 0;
        }else{
            buffor[idc] = c;
            idc++;
        }
    }
    return line_count;
}

void write_dbl_line(line_type lines_tab[], int n){
    for(int i = 0; i<lines_tab[n].len; i++){
        printf("%.2lf ", lines_tab[n].values[i]);
    }
    printf("%.2lf", lines_tab[n].average);
}

int cmp (const void *a, const void *b){
    int l = ((struct line *)a)->average;
    int r = ((struct line *)b)->average;
    return (l - r);
}

// Nalezy zastosowac funkcje qsort() z wywolaniem funkcji cmp().
void sort_by_average(line_type lines_tab[], int line_count){
    qsort((void*)lines_tab, line_count, sizeof(lines_tab[0]), cmp);
}

// Nalezy pamietac o zwolnieniu przydzialonej pamieci
void delete_dbl_lines(line_type *tab[], int line_count){}


typedef void (*Test)();

/////////////////////////////////////////////////////////////////////////

void test_1(){
    int rowsA, colsA, rowsB, colsB;
    double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
    printDebug("Wpisz liczbe wierszy i kolumn oraz elementy 1. macierzy ");
    scanf("%d %d",&rowsA,&colsA);
    read_mat(rowsA, colsA, A);
    printDebug("Wpisz liczbe wierszy i kolumn oraz elementy 2. macierzy ");
    scanf("%d %d",&rowsB,&colsB);
    read_mat(rowsB, colsB, B);
    prod_mat(rowsA,colsA,colsB,A,B,C);
    print_mat(rowsA,colsB,C);
}

void test_2(){
    double series_table[TAB_SIZE];
    double *ptr_table[TAB_SIZE];
    // Ciagi liczbowe w ciaglym obszarze, wiersze roznej dlugosci:
    // ptr_table[lines_counter] points out 1. byte of free memory
    ptr_table[0] = series_table;
    int n = readIntHint("Wpisz numer wiersza (liczony od 1) ");
    printDebug("Wpisz kolejne linie zawierajace liczby, zakoncz znakiem EOF\n");
    FILE* stream = getStreamFromInputOrStdByTest();
    if(stream == NULL) { printf("fopen failed\n");  return; }
    int lines_counter = read_dbl_lines_v1(ptr_table,0,stream);
    write_dbl_line_v1(ptr_table,n);
}

void test_3(){
    // Linie znakow w odrebnych obszarach pamieci:
    char *char_lines_table[TAB_SIZE];
    int n = readIntHint("Wpisz numer wiersza (liczony od 1) ");
    printDebug("Wpisz kolejne linie zawierajace znaki ASCII, zakoncz znakiem EOF\n");
    FILE* stream = getStreamFromInputOrStdByTest();
    int lines_counter = read_char_lines(char_lines_table, 0, stream);
    write_char_line(char_lines_table,n-1);
    delete_lines(char_lines_table,lines_counter);
}

void test_4(){
    // Ciagi liczbowe w odrebnych obszarach pamieci + sortowanie:
    line_type dbl_lines_table[TAB_SIZE];
    int n = readIntHint("Wpisz numer wiersza (liczony od 1) w kolejnosci rosnacej sredniej ");
    printDebug("Wpisz kolejne linie zawierajace liczby, zakoncz znakiem EOF\n");
    FILE* stream = getStreamFromInputOrStdByTest();
    int lines_counter = read_dbl_lines(dbl_lines_table, 0, stream);
    sort_by_average(dbl_lines_table, lines_counter);
    write_dbl_line(dbl_lines_table, n-1);
    //delete_dbl_lines(dbl_lines_table, lines_counter);
}

int main(int argc, char* args[]){
	if(argc > 1 && args[1][0] == '1')
		TEST = 1;
    printDebug("Wpisz numer testu [1, 4]: ");
    int test = readInt();
    Test tests[] = {test_1, test_2, test_3, test_4};
    if(test > 4 || test < 1){
        printDebug("Numer testu %d jest spoza zakresu [1, 4]\n", test);
    }else{
        tests[test-1]();
    }
    return 0;
}