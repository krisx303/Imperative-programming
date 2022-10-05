#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "libraries/string.c"
#include "libraries/inout.c"
#include "libraries/doublelinkedlist.c"
#include "libraries/cmd_dir.c"
#include <stdarg.h>

typedef struct Test{
    int ID;
    List* input;
    List* output;
    char* description;
} Test;

Test* initTest(){
    Test* node = (Test*)malloc(sizeof(Test));
    if(node == NULL){
        exit(MEMORY_ALLOCATION_ERROR);
    }
    node->input = initList();
    node->output = initList();
    node->input->freeElementData = freeString;
    node->output->freeElementData = freeString;
    if(node->input == NULL || node->output == NULL){
        exit(MEMORY_ALLOCATION_ERROR);
    }
    return node;
}

void freeTest(void* data){
    Test* test = (Test*)data;
    freeList(test->input);
    freeList(test->output);
    free(test->description);
}

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
    char* path = createString(3, "src\\", dir, "\\tests.test");
    FILE* tests = readFile(path);
    return tests;
}

#define WAIT 0
#define INPUT 1
#define OUTPUT 2
#define TESTNAME 3
#define ENDOF -1

List* getTests(FILE* file, char* filename){
    List* tests = initList();
    tests->freeElementData = freeTest;
    int len = 0;
    char line[LINE_SIZE];
    int id = 1;
    readFileLine(file, filename);
    int inputType = WAIT;
    int first = 1;
    Test* test;
    while(1){
        len = readFileLine(file, line);
        if(line[0] == '#'){
            if(stringEq(line, "#TEST")){
                if(first){
                    first = 0;
                    test = initTest();
                }
                else{
                    pushBack(tests, test);
                    test = initTest();
                }
                test->ID = id;
                id++;
                inputType = TESTNAME;
            }else if(stringEq(line, "#INPUT")){
                inputType = INPUT;
            }else if(stringEq(line, "#OUTPUT")){
                inputType = OUTPUT;
            }else if(stringEq(line, "#EOF")){
                break;
            }
        }else{
            if(inputType == TESTNAME){
                test->description = copyString(line, len);
            }else if(inputType == INPUT){
                pushBack(test->input, copyString(line, len));
            }else if(inputType == OUTPUT){
                pushBack(test->output, copyString(line, len));
            }
        }
    }
    pushBack(tests, test);
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
        printColored(console, color, "%s%s\n", c, "Run manual test");
        c = sel == 1 ? "->" : "--";
        color = sel == 1 ? 10 : 15;
        printColored(console, color, "%s%s\n", c, "Run all tests");
        i = 2;
        while(e !=NULL){
            c = sel == i ? "->" : "--";
            color = sel == i ? 10 : 15;
            printColored(console, color, "%sTest %d: %s\n", c, i-1, ((Test*)e->data)->description);
            e = e->next;
            i++;
        }

        while ((ch = _getch()) != 224){
            if(ch == 13) return sel;
            if(ch == 27) return -1;
        }
        ch = _getch();
        if(ch == 80 && sel < tests->size+1) sel++;
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

void printDiff(char* exp, char* given){
    int n = 0;
    int m = strlen(given);
    if(exp == NULL){
        n = m;
    }else{
        n = strlen(exp);
    }
    int printedChars = 0, overflow = 0;
    int overtype = -1; 
    if(n == m){
        printedChars = n;
        overflow = 0;
    }else if(n < m){
        printedChars = n;
        overflow = m - n;
        overtype = 1;
    }else{
        printedChars = m;
        overflow = n - m;
        overtype = 2;
    }

    if(exp == NULL){
        overflow = printedChars;
        printedChars = 0;
        overtype = 1;
        n = 0;
    }
    for(int i = 0;i<printedChars;i++){
        if(given[i] != exp[i]){
            if(given[i] == 32){
                given[i] = '_';
            }
            printColored(console, 4, "%c", given[i]);
        }
        else
            printf("%c", given[i]);
    }
    SetConsoleTextAttribute(console, 4);
    char c;
    if(overtype == 1){
        for(int i = 0;i<overflow;i++){
            c = given[n+i];
            if(c == 32)
                c = '_';
            printf("%c", c);
        }
    }
    else if(overtype == 2){
        for(int i = 0;i<overflow;i++){
            printf("-");
        }
    }
    SetConsoleTextAttribute(console, 15);
    printf("\n");
}

void printLegend(){
    SetConsoleTextAttribute(console, 5);
    printf("\nLegend:");
    printf("\n-> red chars means that program expected other char in this place");
    printf("\n-> '-' red char means that there is not enough chars in the output");
    printf("\n-> '_' red char means that there is space in the output but expected other char");
    printf("\n-> \"... is not recognized as an inter...\" - probably program was not compilated successfully and there is no .exe file ://");
    SetConsoleTextAttribute(console, 15);
}

int runSingleTest(Test* test, char* dir, char* filename, int printout){
    FILE* tmpin = writeFile("tmpin");
    ListElement* el = test->input->head;
    for(int i = 0;i<test->input->size; i++){
        fprintf(tmpin, "%s", (char*) el->data);
        if(i < test->input->size-1) fprintf(tmpin, "\n");
        el = el->next;
    }
    closeFile(tmpin);
    runCommand(5, "type tmpin | \"src\\", dir, "\\", filename, ".exe\" > tmpout");
    FILE* tmpout = readFile("tmpout");
    char line[BUFFER_SIZE];
    int passed = 1, len = 0, p = 0;
    el = test->output->head;
    if(printout) {
        printColored(console, 9, "Test Output:\n");
    }
    for(int i = 0;i<test->output->size;i++){
        len = readFileLine(tmpout, line);
        p = strcmp((char*)(el->data), line);
        if(p != 0){
            passed = 0;
            if(printout)
                printDiff(el->data, line);
            break;
        }else{
            if(printout)
                printf("%s\n", line);
        }
        el = el->next;
    }
    if(printout)
    while(1){
        len = readFileLine(tmpout, line);
        if(len == 1){
            break;
        }
        printDiff(NULL, line);
    }
    printColored(console, 9, "Test %d: ", test->ID);
    if(passed == 1){
        printColored(console, 10, "[PASSED]\n");
    }else{
        printColored(console, 4, "[FAILED]\n");
        if(printout)
            printLegend();
    }
    return passed;
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
        pass = runSingleTest(test, dir, fileName, 0);
        if(pass == 0) passed++;
        element = element->next;
    }
    printColored(console, 9, "Summary of tests:\nPassed: %d/%d\nFailed: %d/%d\n", passed, tests->size, tests->size-passed, tests->size);
}

void runManualTest(char* dir, char* filename){
    printColored(console, 9, "Now is running: %s\\%s.exe\n", dir, filename);
    printColored(console, 9, "You can write input below:\n");
    runCommand(5, "\"src\\", dir, "\\", filename, ".exe\" 1");
}

void testLoop(List* tests, char* dir, char* filename){
    int sel, ch;
    while(1){
        sel = selectTest(tests);
        if(sel == -1) return;
        system("cls");
        if(sel == 0){
            runManualTest(dir, filename);
        }else if(sel == 1){
            runAllTests(tests, dir, filename);
            //RUN test manually
        }else{
            ListElement* el = tests->head;
            for(int i = 0;i<sel-2;i++) el = el->next;
            Test* test = el->data;
            printColored(console, 9, "Running single test with %d ID :\n", test->ID);
            printColored(console, 9, "Test description: ");
            printf("%s\n", test->description);
            printColored(console, 9, "Test Input:\n");
            ListElement* line = test->input->head;
            for(int i = 0;i<test->input->size;i++){
                printf("%s\n", line->data);
                line = line->next;
            }
            printColored(console, 9, "Expected Output:\n");
            line = test->output->head;
            for(int i = 0;i<test->output->size;i++){
                printf("%s\n", line->data);
                line = line->next;
            }
            runSingleTest(test, dir, filename, 1);
        }
        ch = getc(stdin);
        if(ch == 27) return;
    }
}

int compileProgram(char* dir, char* filename){
    runCommand(7, "cd src\\", dir, " & gcc ", filename, ".c -o ", filename, " 2> logs");
    FILE* logs = readFile(createString(3, "src\\", dir, "\\logs"));
    char line[LINE_SIZE];
    int len = readFileLine(logs, line);
    closeFile(logs);
    if(len > 5){
        return -1; 
    }
    else return 1;
}


// //TODO parsing path argument from command line prompt run
int main() {
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cd src & dir > ..\\tmp");
    List* dirs = getAllDirs(false);
    if(dirs->size == 0) exit(0);
    int sel = 0;
    while(1){
        sel = selectDir(dirs);
        if(sel == -1) exit(EXIT_PROGRAM);
        char* dir;
        ListElement* e = dirs->head;
        for(int i = 0;i<sel;i++) e = e->next;
        dir = (char*)e->data;
        FILE* testsFile = getTestsFile(dir);
        char filename[LINE_SIZE];
        List* tests = getTests(testsFile, filename);
        if(compileProgram(dir, filename) != 1){
            system("cls");
            printColored(console, 4, "Compilation returned that logs!!\n");
            FILE* logs = readFile(createString(3, "src\\", dir, "\\logs"));
            char line[LINE_SIZE];
            int len = readFileLine(logs, line);
            while(len > 5){
                printf("%s\n", line);
                len = readFileLine(logs, line);
            }
            closeFile(logs);
            printColored(console, 4, "!! If in logs is error information program won't be working properly or will run PREVIOUS compilated version !!\n");
            printf("If you want, that was saved in logs file in %s directory :)\nRemember, best c program has no errors and warnings :P\n", dir);
            printf("Enter any key for skip that information:");
            getch();
        }
        testLoop(tests, dir, filename);
        freeList(tests);
    }
    freeList(dirs);
    return 0;
}