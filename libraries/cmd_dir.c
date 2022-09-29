
int stringEq(char* str, char* sqr){
    int n = strlen(sqr);
    for(int i = 0;i<n;i++){
        if(str[i] != sqr[i]) return 0;
    }
    return 1;
}

char* createCommandFromListOfStrings(int n, List* listArgs){
    int len = 0;
    ListElement* el = listArgs->head;
    for(int i = 0; i < n; i++){
        len += strlen((char*) el->data);
        el = el->next;
    }

    char* command = (char*) calloc(len+1, sizeof(char));
    el = listArgs->head;
    for (int i = 0; i < n; i++){
        strcat(command, (char*)el->data);
        el = el->next;
    }
    return command;
}

// Runs system(command), command is created by n strings given to function
int runCommand(int n, ...){
    List* listArgs = initList();
    va_list ap;
    va_start (ap, n);

    for(int i = 0; i < n; i++){
        pushBack(listArgs, va_arg(ap, char*));
    }
    va_end (ap);

    char* command = createCommandFromListOfStrings(n, listArgs);

    return system(command);
}

// Creates string from given n strings and returns as single char* 
char* createString(int n, ...){
    List* listArgs = initList();
    va_list ap;
    va_start (ap, n);

    for(int i = 0; i < n; i++){
        pushBack(listArgs, va_arg(ap, char*));
    }
    va_end (ap);

    char* command = createCommandFromListOfStrings(n, listArgs);

    return command;
}

//* Executing dir command with $path, output is saved in tmp file
void dir(char* path){
    int n = strlen(path) + 20;
    char *command = calloc(n,sizeof(char));
    strcat(command, "cd src & dir ");
    if(strlen(path) != 0)
        strcat(command, path);
    strcat(command, " > ..\\tmp");
    system(command);
}

void freeString(void* data){
    free((char*)data);
}

//* Gets List of all dirs from tmp file
//* When hiddenDirs is true, returs also hidden dirs.
List* getAllDirs(bool hiddenDirs){
    List* list = initList();
    list->freeElementData = freeString;
    FILE *ls = readFile("tmp");
    char line[LINE_SIZE];
    int len = 0;
    for(int i = 0;i<7;i++){
        len = readFileLine(ls, line);
    }
    if(hiddenDirs == false){
        while(1){
            len = readFileLine(ls, line);
            if(line[36] == '.') continue;
            break;
        }
    }
    else{
        len = readFileLine(ls, line);
    }
    while(1){
        if(stringEq(&line[22], "DIR")){
            pushBack(list, strcut(line, 36, len));
        }
        else{
            break;
        }
        len = readFileLine(ls, line);
    }
    closeFile(ls);
    return list;
}