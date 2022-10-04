/*
Identyfikatory:

typy:
	//aktualnie rozpatrywany znak jest wewnątz
	NIC - ani nie w komentarzu ani nie w napisie
	KOMENTARZ_POJEDYŃCZY - wewnątrz // \n
	KOMENTARZ_WIELOLINIJKOWY - wewnątrz /-* *-/  
	NAPIS - wewnątrz " "
	ID - wewnątrz potenjalnego ID
	
typ = NIC

wykonuj do końca pliku:
	az = wczytaj_znak()
	
	jeśli typ != NIC (aktulanie jest wewnątrz czegokolwiek to):
		jeśli az to znak kończący wnętrze (", \n, *-/) to:
			typ = NIC
		jeśli wewnątz ID to:
			jeśli znak alphanum to:
				dopisz go do ID
			w przeciwnym wypadku:
				koniec ID
				typ = NIC
		w przeciwnym wypadku:
			ignoruj wszystko
	w przeciwnym wypadku (jeśli nie jest wewnątrz niczego):
		jeśli az to znak rozpoczynający wnętrze (", //, /*) to:
			typ = (odpowiadające znakowi wnętrze)
		w przeciwnym wypadku jeśli to znak alpha:
			typ = ID

*/



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../../libraries/inout.c"
#include "../../libraries/string.c"

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8
#define NOTHING 0

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int checkStartSection(char last, char actual){
	if(actual == '\"'){
		return IN_STRING;
	}else if(last == '/'){
		if(actual == '/'){
			return IN_LINE_COMMENT;
		}else if(actual == '*'){
			return IN_BLOCK_COMMENT;
		}
	}
	return NOTHING;
}

int checkEndSection(char last, char actual){
	if(actual == '\"'){
		return IN_STRING;
	}else if(actual == 10){
		return IN_LINE_COMMENT;
	}else if(last == '*' && actual == '/'){
		return IN_BLOCK_COMMENT;
	}
	return NOTHING;
}

char* createIDString(char ID[MAX_ID_LEN], int len){
	char* id = (char*)malloc(len*sizeof(char));
	for(int i = 0;i<len;i++){
		id[i] = ID[i];
	}
	return id;
}



char** ids;
int count = 0;



int isIDkeyword(char* id, int len){
	for(int i=0;i<32;i++){
		if(len == strlen(keywords[i])){
			if(strcmp(id, keywords[i]) == 0){
				return 1;
			}
		}
	}
	return 0;
}

int isIDUnique(char* id, int len){
	for(int i = 0;i<count;i++){
		if(!strcmp(ids[i], id)) return 0;
	}
	return 1;
}

int find_idents(FILE *stream){
	int uniqueIDs = 0;
	ids = (char**)malloc(1*sizeof(char*));
	char actual_c, prev_c, back;
	int type = NOTHING, id_len = 0;
	char ID[MAX_ID_LEN];
	while(1){
		actual_c = fgetc(stream);
		back = actual_c;
		if(type != NOTHING){
			int check = checkEndSection(prev_c, actual_c);
			if(check == type){
				type = NOTHING;
				actual_c = 0;
			}else if(type == IN_ID){
				if(isalnum(actual_c)){
					ID[id_len] = actual_c;
					id_len++;
				}else{
					//printf("ID: ");
					char* id = createIDString(ID, id_len);
					// for(int i = 0;i<id_len;i++){
					// 	printf("%c", ID[i]);
					// }
					//printf(" ");
					if(!isIDkeyword(id, id_len)){
						//printf(" is not keyword");
						if(isIDUnique(id, id_len)){
							//printf(" is unique");
							count++;
							ids = realloc(ids, count*sizeof(char*));
							ids[count-1] = copyString(id, id_len);
							uniqueIDs++;
						}
					}
					//printf("\n");
					id_len = 0;
					type = NOTHING;
				}
			}
		}else{
			int check = checkStartSection(prev_c, actual_c);
			if(check){
				type = check;
				actual_c = 0;
			}else{
				if(isalpha(actual_c)){
					ID[0] = actual_c;
					id_len = 1;
					type = IN_ID;
				}
			}
		}
		prev_c = actual_c;
		if(actual_c == EOF) break;
	}
	return uniqueIDs;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(int argc, char* args[]) {
    if(argc > 1 && args[1][0] == '1')
		TEST = 1;
	TEST = 0;
	FILE* stream = getStreamFromInputOrStdByTest();
    if(stream == NULL){
        printf("fopen failed\n");
        return 0;
    }
	printf("%d", find_idents(stream));
	return 0;
}

