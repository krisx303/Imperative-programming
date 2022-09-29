
char* strcut(char* str, int start, int end){
    int n = end - start + 1;
    char* s = (char*)malloc((n+1)*sizeof(char));
    for(int i = 0;i<n;i++){
        s[i] = str[start+i]; 
    }
    return s;
}

char* copyString(char* str, int len){
    char* nstr = (char*)malloc((len)*sizeof(char));
    for(int i = 0;i<len;i++){
        nstr[i] = str[i];
    }
    return nstr;
}
