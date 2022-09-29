#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "libraries/string.c"
#include "libraries/inout.c"
#include "libraries/doublelinkedlist.c"
#include "libraries/cmd_dir.c"

typedef struct Test{
    int ID;
    char* input;
    char* output;
    char* description;
} Test;

Test* initTest(){
    Test* node = (Test*)malloc(sizeof(Test));
    if(node == NULL){
        exit(MEMORY_ALLOCATION_ERROR);
    }
    return node;
}

void freeTest(void* data){}

HANDLE console;

int selectDir(List* dirs){
    int sel = 0;
    int i, ch;
    bool loop = true;
    while(loop == true){
        system("cls");
        printColored(console, 9, "Choose module which one want to test: (ESC to exit)\n");
        ListElement* e = dirs->head;
        i = 0;
        while(e !=NULL){
            char* c = sel == i ? "->" : "--";
            int color = sel == i ? 10 : 15;
            printColored(console, color, "%s%s\n", c, (char*)e->data);
            e = e->next;
            i++;
        }

        while ((ch = _getch()) != 224){
            if(ch == 13) return sel;
            if(ch == 27) return -1;
        }
        ch = _getch();
        if(ch == 80 && sel < dirs->size - 1) sel++;
        if(ch == 72 && sel > 0) sel--;
    }
}

FILE* getTestsFile(char* dir){
    char *name = calloc(11 + strlen(dir),sizeof(char));
    char end[] = "\\tests.test";
    strcat(name, dir);
    strcat(name, end);
    FILE* tests = readFile(name);
    return tests;
}

List* getTests(FILE* file, char* filename){
    List* tests = initList();
    tests->freeElementData = freeTest;
    int len = 0;
    char line[LINE_SIZE];
    int id = 1;
    readFileLine(file, filename);
    while(1){
        len = readFileLine(file, line);
        if(len == 1) break;
        Test* test = initTest();
        test->ID = id;
        test->description = copyString(line, len);
        //* INPUT
        len = readFileLine(file, line);
        test->input = copyString(line, len);
        //* OUTPUT
        len = readFileLine(file, line);
        test->output = copyString(line, len);
        id++;
        pushBack(tests, test);
    }
    closeFile(file);
    return tests;
}

int selectTest(List* tests){
    int sel = 0;
    int i, ch, color;
    char* c;
    bool loop = true;
    while(loop == true){
        system("cls");
        printColored(console, 9, "Choose test which you want run (ESC to exit)\n");
        ListElement* e = tests->head;
        c = sel == 0 ? "->" : "--";
        color = sel == 0 ? 10 : 15;
        printColored(console, color, "%s%s\n", c, "All tests");
        i = 1;
        while(e !=NULL){
            c = sel == i ? "->" : "--";
            color = sel == i ? 10 : 15;
            printColored(console, color, "%sTest %d: %s\n", c, i, ((Test*)e->data)->description);
            e = e->next;
            i++;
        }

        while ((ch = _getch()) != 224){
            if(ch == 13) return sel;
            if(ch == 27) return -1;
        }
        ch = _getch();
        if(ch == 80 && sel < tests->size) sel++;
        if(ch == 72 && sel > 0) sel--;
    }
}

char* getTestCommand(char* dir, char* fileName){
    int n = strlen(dir) + strlen(fileName) + 25;
    char *command = calloc(n,sizeof(char));
    char start[] = "type tmpin | \"";
    char ch[] = "\\";
    char end[] = "\" > tmpout";
    strcat(command, start);
    strcat(command, dir);
    strcat(command, ch);
    strcat(command, fileName);
    strcat(command, end);
    return command;
}

int runSingleTest(Test* test, char* command, int printout){
    FILE* tmpin = writeFile("tmpin");
    fprintf(tmpin, "%s", test->input);
    closeFile(tmpin);
    system(command);
    FILE* tmpout = readFile("tmpout");
    char line[BUFFER_SIZE];
    readFileLine(tmpout, line);
    int a = strcmp(test->output, line);
    if(printout){
        printColored(console, 9, "Test Output:    %s\n", line);
    }
    printColored(console, 9, "Test %d: ", test->ID);
    if(a == 0){
        printColored(console, 10, "[PASSED]\n");
    }else{
        printColored(console, 4, "[FAILED]\n");
    }
    return a;
}

void runAllTests(List* tests, char* dir, char* fileName){
    printColored(console, 9, "Running %d tests:\n", tests->size);
    int passed = 0;
    ListElement* element = tests->head;
    char* command = getTestCommand(dir, fileName);
    int pass = 0;
    Test* test;
    while(element != NULL){
        test = (Test*)element->data;
        pass = runSingleTest(test, command, 0);
        if(pass == 0) passed++;
        element = element->next;
    }
    printColored(console, 9, "Summary of tests:\nPassed: %d/%d\nFailed: %d/%d\n", passed, tests->size, tests->size-passed, tests->size);
}

void testLoop(List* tests, char* dir, char* filename){
    int sel, ch;
    while(1){
    sel = selectTest(tests);
    if(sel == -1) return;
    system("cls");
    if(sel == 0){
        runAllTests(tests, dir, filename);
    }else{
        ListElement* el = tests->head;
        for(int i = 0;i<sel-1;i++) el = el->next;
        Test* test = el->data;
        printColored(console, 9, "Running single test with %d ID :\n", test->ID);
        printColored(console, 9, "Test description: %s\nTest Input: %s\nCorrect Output: %s\n", test->description, test->input, test->output);
        runSingleTest(test, getTestCommand(dir, filename), 1);
    }
    ch = getc(stdin);
    if(ch == 27) return;
    }
}

//TODO parsing path argument from command line prompt run
int main() {
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    dir("");
    List* dirs = getAllDirs(false);
    if(dirs->size == 0) exit(0);
    while(1){
        int sel = selectDir(dirs);
        if(sel == -1) exit(EXIT_PROGRAM);
        char* dir;
        ListElement* e = dirs->head;
        for(int i = 0;i<sel;i++) e = e->next;
        dir = (char*)e->data;
        FILE* testsFile = getTestsFile(dir);
        char filename[LINE_SIZE];
        List* tests = getTests(testsFile, filename);
        testLoop(tests, dir, filename);
        freeList(tests);
    }
    freeList(dirs);
    return 0;
}