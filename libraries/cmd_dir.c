
int stringEq(char* str, char* sqr){
    int n = strlen(sqr);
    for(int i = 0;i<n;i++){
        if(str[i] != sqr[i]) return 0;
    }
    return 1;
}

//* Executing dir command with $path, output is saved in tmp file
void dir(char* path){
    int n = strlen(path) + 10;
    char *command = calloc(n,sizeof(char));
    char start[] = "dir ";
    char end[] = " > tmp";
    strcat(command, start);
    if(strlen(path) != 0)
        strcat(command, path);
    strcat(command, end);
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