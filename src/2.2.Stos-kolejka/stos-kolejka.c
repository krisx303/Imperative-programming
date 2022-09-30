#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../libraries/inout.c"

#define STACK_SIZE  10


 
double stack[STACK_SIZE];
int top = 0;   // jawne zerowanie nie jest konieczne

double stack_push(double x) {
    if(top == STACK_SIZE){
        return INFINITY;
    }
    stack[top++] = x;
    return 0;
}

double stack_pop(void) {
    if(top == 0){
        return NAN;
    }
    return stack[--top];
}

int stack_state(void) {
    return top == STACK_SIZE ? -1 : top;
}

//Kolejka z przesuwaniem w tablicy

#define QUEUE_SIZE  10

int queue[QUEUE_SIZE];
int in=0, curr_nr=0;  // 1. klient dostanie nr = 1

double queue_push(int in_nr) {
    int flag = 0;
    for(int i = 0; i<in_nr;i++){
        if(in >= QUEUE_SIZE){
            flag = 1;
        }else{
            queue[in] = curr_nr + 1;
            in++;
        }
        curr_nr++;
    }
    if(flag){
        return INFINITY;
    }
    return 0.0;
}

int queue_pop(int out_nr) {
    int flag = out_nr > in ? -1 : in - out_nr;
    for(int i = 0; i<out_nr;i++){
        for(int j = 0;j<in-1;j++){
            queue[j] = queue[j+1];
        }
        in--;
    }
    in = in < 0 ? 0 : in;
    return flag;
}

void queue_state(void) {
    for(int i = 0; i<in;i++){
        printf("%d ", queue[i]);
    }
    printf("\n");
}


//Kolejka w buforze cyklicznym

#define CBUFF_SIZE  10

int cbuff[CBUFF_SIZE];
int out=0, len=0;


double cbuff_push(int cli_nr) {
    if(len == CBUFF_SIZE){
        return INFINITY;
    }
    len++;
    cbuff[(out + len-1)%CBUFF_SIZE] = cli_nr;
    return 0.0;
}

int cbuff_pop(void) {
    if(len == 0){
        return -1;
    }
    len--;
    int s = out;
    out = (out + 1)%CBUFF_SIZE;
    return cbuff[s];
}

void cbuff_state(void) {
    printf("\n");
    if(len == 0){
        printf("%d", NAN);
    }
    for(int i = 0; i<len;i++){
        printf("%d ", cbuff[(out + i)%CBUFF_SIZE]);
    }
}

typedef void (*Test)();

void test_1(){
    printDebug("sekwencja operacji (w jednej linii, oddzielone spacjami):\n");
    double f_ans, x;
    do {
        x = readDouble();
        if(x > 0) {
            f_ans = stack_push(x);
            if(f_ans == INFINITY) printf("inf ");
        }
        else {
            if(x < 0) printf("%.2f ",stack_pop());
            else      printf("\n%d\n",stack_state());
        }
    } while(x != 0.0);
}

void test_2(){
    int n, ans;
    double f_ans;
    printDebug("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
    do {
        n = readInt();
        if(n > 0) {
            f_ans = queue_push(n);
            if(f_ans== INFINITY) printf("inf ");
        }
        else if(n < 0) {
            ans = queue_pop(-n);
            if(ans < 0) printf("nan ");
        }
        else  queue_state();
    } while(n != 0);
}

void test_3(){
    printDebug("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
    int nr_klienta = 0, code, ans;
    double f_ans;
    do {
        code = readInt();
        if(code > 0) {
            f_ans = cbuff_push(++nr_klienta);
            if(f_ans == INFINITY) printf("inf ");
        }
        else if(code < 0) {
                ans = cbuff_pop();
                if(ans < 0) printf("nan ");
                else        printf("%d ",ans);
            }
        else  cbuff_state();
    } while(code != 0);
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