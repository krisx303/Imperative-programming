#include <stdio.h>
#include <math.h>
#include "../../libraries/inout.c"
#define N 100

typedef void (*Test)();

// Returns n evenly spaced samples, calculated over the interval [start, stop].
// 0 <= n <= N
// for n = 0 return empty array
// for n = 1 return one-element array, with array[0] = start
void linspace(double array[], double start, double stop, int n) {
    if(n == 0){
        return;
    }else if(n == 1){
        array[0] = start;
    }else{
        double r = (stop - start)/(n-1);
        for(int i = 0; i<n+1; i++){
            array[i] = start + (i*r);
        }
    }
}

// Multiply each element of v by the value of scalar
void multiply_by_scalar(double v[], int n, double scalar) {
    for(int i = 0; i<n;i++){
        v[i] = v[i] * scalar;
    }
}

// Add to each element v1[i] value of v2[i]
void add(double v1[], const double v2[], int n) {
    for(int i = 0; i<n; i++){
        v1[i] = v1[i] + v2[i];
    }
}

// Calculate and return the dot product of v1 and v2
double dot_product(const double v1[], const double v2[], int n) {
    double s = 0;
    for(int i = 0; i<n; i++){
        s = s + (v1[i] * v2[i]);
    }
    return s;
}

// Generates the sequence of n samples by incrementing the start value
// using the step size (|step| > 1.e-5).
// 0 <= n <= N
// for n = 0 return empty array
// for n = 1 return one-element array, with array[0] = start
void range(double array[], double start, double step, int n) {
    if(n == 0){
        return;
    }else if(n == 1){
        array[0] = start;
    }else{
        for(int i = 0; i<n; i++){
            array[i] = start + (i*step);
        }
    }
}

// read double vector of size n
void read_vector(double v[], int n) {
	for (int i = 0; i < n; ++i) {
		scanf("%lf", v + i);
	}
}

// print double vector of size n (with 2 figures after the decimal point)
void print_vector(const double v[], int n) {
	for (int i = 0; i < n; ++i) {
		printf("%.2f ", v[i]);
	}
}

void test_1(){
	int len, start, stop;
	double vector[N];
	printDebug("LINSPACE: Wpisz liczby: n, start, stop ");
	len = readInt();
	start = readDouble();
	stop = readDouble();
	if(len < 0 || len > N) { printDebug("n<0 lub n > 100\n");  return;}
	linspace(vector, start, stop, len);
	print_vector(vector, len);
}

void test_2(){
	int len;
	double vector_1[N], vector_2[N]; 
	printDebug("ADD: Wpisz dlugosc wektorow: ");
	len = readInt();
	if(len < 0 || len > N) { printDebug("n<0 lub n > 100\n");  return; }
	printDebug("Wpisz elementy pierwszego wektora: ");
	read_vector(vector_1, len);
	printDebug("Wpisz elementy drugiego wektora: ");
	read_vector(vector_2, len);
	add(vector_1, vector_2, len);
	print_vector(vector_1, len);
}

void test_3(){
	double vector_1[N], vector_2[N];
	printDebug("DOT PRODUCT: Wpisz dlugosc wektorow: ");
	int len = readInt();
	if(len < 0 || len > N) { printDebug("n<0 lub n > 100\n");  return; }
	printDebug("Wpisz elementy pierwszego wektora: ");
	read_vector(vector_1, len);
	printDebug("Wpisz elementy drugiego wektora: ");
	read_vector(vector_2, len);
	printf("%.2f\n", dot_product(vector_1, vector_2, len));
}

void test_4(){
	double vector[N];
	printDebug("MULTIPLY BY SCALAR: Wpisz dlugosc wektora i wartosc skalara: ");
	int len = readInt();
	double scalar = readDouble();
	if(len < 0 || len > N) { printDebug("n<0 lub n > 100\n");  return; }
	printDebug("Wpisz elementy wektora: ");
	read_vector(vector, len);
	multiply_by_scalar(vector, len, scalar);
	print_vector(vector, len);
}

void test_5(){
	double vector[N];
	printDebug("RANGE: Wpisz liczby: n, start, step: ");
	int len = readInt();
	double start = readDouble();
	double step = readDouble();
	if(len < 0 || len > N) { printDebug("n<0 lub n > 100\n");  return; }
	range(vector, start, step, len);
	print_vector(vector, len);
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