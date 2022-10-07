#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../libraries/inout.c"

// Quick sort
void quick_sort (int *A, int len)
{
    if(len < 2){
        return;
    }
    int PIWOt = *(A+len-1);
    int left = 0;
    int right = len-1;
    while(left <= right){
        while(*(A+left) < PIWOt){
            left++;
        }
        while(*(A+right) > PIWOt){
            right--;
        }
        if(left <= right){
            int buffor = *(A+left);
            *(A+left) = *(A+right);
            *(A+right) = buffor;
            left++;
            right--;
        }
    }
    quick_sort(A, right+1);
    quick_sort(A+left, len-left);
}

int cmpstr(const void* a, const void* b) {
    return strcmp(*(char* const*) a, *(char* const*) b);
}

// Sortowanie słów
void sort_words(char *words[], int count)
{
    qsort(words, count, sizeof(char*), cmpstr);
}

// Sortowanie leksykograficzne struktur

typedef struct structure
{
    char* name;
    char* nazwa;
} structure;

//...............

structure s[10];

int compare (const void * a, const void * b)
{
    structure *aa = (structure *)a;
    structure *bb = (structure *)b;

    return strcmp(aa->name, bb->name);
}

void struct_lexi_sort(int *len)
{
    qsort(s, 10, sizeof(structure), compare);
}

// Złożone wymagania dotyczące sortowania

typedef struct struktura
{
    char* sname;
    char* fname;
    int age;
} struktura;

struktura sa[11];

char getLastChar(struktura* a){
    int c = 0;
    while(a->sname[c] != '\0'){
        c++;
    }
    return a->sname[c-1];
}

int compare2 (const void * a, const void * b)
{
    struktura *aa = (struktura *)a;
    struktura *bb = (struktura *)b;

    char aend = getLastChar(aa);
    char bend = getLastChar(bb);

    if((aend != 'a' && bend == 'a') || (aend == 'a' && bend != 'a')){
        return aend - bend;
    }
    else{
        if(aend == 'a' && bend == 'a'){
            return aa->age - bb->age;
        }else if(aend != 'a' && bend != 'a'){
            return strcmp(aa->fname, bb->fname);
        }
    }
    printf("%c %c\n", aend, bend);

    return aend - bend;

    //return strcmp(aa->sname, bb->sname);
}

void struct_name_sort(int *len)
{
    qsort(sa, 11, sizeof(struktura), compare2);
}

///////////////////////////////////////////////////

typedef void (*Test)();

// Quick sort
void test_1(){
    int n = 10;
    int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    quick_sort(a, n);
    for(int i = 0; i<n; i++)
    {
        printf("%d ", a[i]);
    }
}

// Sortowanie słów
void test_2(){
    int n = 10;
    char *words[] = { "dog", "ant", "cat", "man", "bear", "frog", "leopard", "bat", "rabbit", "monkey"};
    sort_words(words,n);

    for (int i = 0; i < n; i++)
    {
        printf("%s ", words[i]);
    }
    printDebug("\n");
}

// Sortowanie leksykograficzne struktur
void test_3(){
    int n = 10;
    s[0].name = "dog";
    s[0].nazwa = "pies";
    s[1].name = "ant";
    s[1].nazwa = "mrowka";
    s[2].name = "cat";
    s[2].nazwa = "kot";
    s[3].name = "Man";
    s[3].nazwa = "czlowiek";
    s[4].name = "bear";
    s[4].nazwa = "niedzwiedz";
    s[5].name = "frog";
    s[5].nazwa = "zaba";
    s[6].name = "leopard";
    s[6].nazwa = "lampart";
    s[7].name = "BAT";
    s[7].nazwa = "nietoperz";
    s[8].name = "rabbit";
    s[8].nazwa = "krolik";
    s[9].name = "monkey";
    s[9].nazwa = "malpa";
    struct_lexi_sort(&n);
    for (int i = 0; i < n; i++)
    {
        printf("%s ",s[i].name);
        printDebug("znaczy ");
        printf("%s ", s[i].nazwa);
        printDebug("\n");
    }
}

// Złożone wymagania dotyczące sortowania
void test_4(){
    int n = 10;
    sa[0].sname = "Anastazja";
    sa[0].fname = "Kamiska";
    sa[0].age = 28;
    sa[1].sname = "Stefan";
    sa[1].fname = "Kowal";
    sa[1].age = 36;
    sa[2].sname = "Kamila";
    sa[2].fname = "Munk";
    sa[2].age = 25;
    sa[3].sname = "Ernest";
    sa[3].fname = "Goja";
    sa[3].age = 27;
    sa[4].sname = "Karol";
    sa[4].fname = "Wodecki";
    sa[4].age = 27;
    sa[5].sname = "Anna";
    sa[5].fname = "Stone";
    sa[5].age = 15;
    sa[6].sname = "Katarzyna";
    sa[6].fname = "Michalak";
    sa[6].age = 20;
    sa[7].sname = "Malgorzata";
    sa[7].fname = "Kowal";
    sa[7].age = 33;
    sa[8].sname = "Emil";
    sa[8].fname = "Kant";
    sa[8].age = 22;
    sa[9].sname = "Kamil";
    sa[9].fname = "Mist";
    sa[9].age = 29;
    sa[10].sname = "Tomasz";
    sa[10].fname = "Nod";
    sa[10].age = 18;
    struct_name_sort(&n);
    for (int i = 0; i < 11; i++)
    {
        printf("%s %s %d ",sa[i].sname, sa[i].fname, sa[i].age);
        printDebug("\n");
    }
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