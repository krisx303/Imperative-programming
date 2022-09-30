#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../libraries/inout.c"
#include <conio.h>
// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

char readCharFromStream(FILE* stream){
	char c, last = 1;
	if(stream == stdin){
		while(1){
			c = _getch();
			if(!last){
				last = 1;
				continue;
			}
			printf("%c", c);
			if(c == 13) printf("\n");
			if(c == 27)
				return EOF;
			else if((c > 33 && c <= 126) || c == 13 || c == 9 || c == 32){
				return c;
			}
			else{
				last = 0;
			}
		}
	}else{
		return fgetc(stream);
	}
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stream
void wc(int *nl, int *nw, int *nc, FILE *stream){
    (*nl) = 1; (*nw) = 0; (*nc) = 0;
    char c;
	int isWord = 0;
    if(stream == stdin){
        (*nc)--;
    }
	while(1){
        c = readCharFromStream(stream);
		//printf("[%d]", c);
        if(c == EOF){
            break;
        }
        if(c == ' ' || c == '\t'){
			isWord = 0;
		}
		if (c == '\n'){
            (*nc)--;
			*nl +=1;
			isWord=0;
		}
		if((c != ' ' && c != '\t' && c != '\n') && isWord == 0){
            isWord =1;
			*nw +=1;
		}
        *nc +=1;
	}
}

typedef struct CharStat {
	char c;
	int count;
} CharStat;

CharStat* initCharStat(char c){
	CharStat* stats = (CharStat*)malloc(sizeof(CharStat));
	stats->c = c;
	stats->count = 0;
	return stats;
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stream. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt, FILE *stream){
	char chars[MAX_CHARS];
	for(int i = 0;i<MAX_CHARS;i++){
		count[i] = 0;
		chars[i] = FIRST_CHAR + i;
	}
	int c;
	while (1) {
        c = readCharFromStream(stream);
        if (c == EOF) break;
		if(c >= FIRST_CHAR && c < LAST_CHAR)
			count[c - FIRST_CHAR]++;
    };

	int ids[MAX_CHARS];
	for(int i =0; i < MAX_CHARS; i++){
		ids[i] = i;
	}
	qsort(ids, MAX_CHARS, sizeof(int), cmp);

	int outc = ids[char_no -1];
    *cnt = count[outc];
	*n_char = outc + FIRST_CHAR;
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stream. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[], FILE *stream){
	int actual;
	int previous = -1;
	while (1) {
        actual = fgetc(stream);
        if (actual == EOF) break;
		if(previous != -1 && actual >= FIRST_CHAR && actual < LAST_CHAR && previous >= FIRST_CHAR && previous < LAST_CHAR){
			int index = (previous - FIRST_CHAR)*MAX_CHARS + (actual - FIRST_CHAR);
			count[index]++;
		}
		previous = actual;
    }

	int ids[MAX_DIGRAMS];
	for(int i =0; i < MAX_DIGRAMS; i++){
		ids[i] = i;
	}
	qsort(ids, MAX_DIGRAMS, sizeof(int), cmp_di);
	int outc = ids[digram_no -1];
	digram[0] = outc / MAX_CHARS + FIRST_CHAR;
	digram[1] = outc % MAX_CHARS + FIRST_CHAR;
    digram[2] = count[outc];
}

// Count block and line comments in the text read from stream. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter, FILE *stream){
	*line_comment_counter = 0;
    *block_comment_counter = 0;
    int status = -1;
    int actual;
    int previous = -1;
    while(1){
        actual = fgetc(stream);
        if (actual == EOF) break;
        if(previous != -1){
            if(previous == '/' && actual == '/' && status == -1){
                status = 0;
                (*line_comment_counter)++;
            }
            if(previous == '/' && actual == '*' && status == -1){
                status = 1;
                (*block_comment_counter)++;
                actual = -1;
            }
            if(previous == '*' && actual == '/' && status == 1){
                status = -1;
            }
        }
        if(actual == '\n' && status == 0){
            status = -1;
        }
        previous = actual;
    }
}

#define MAX_LINE 128

int read_line() {
	char line[MAX_LINE];
	int n;

	fgets (line, MAX_LINE, stdin); // to get the whole line
	sscanf (line, "%d", &n);
	return n;
}

typedef void (*Test)();

void test_1(){
    FILE* stream = getStreamFromInputOrStdByTest();
    if(stream == NULL){
        printf("fopen failed\n");
        return;
    } read_line();
    int nl, nw, nc;
    wc(&nl, &nw, &nc, stream);
	printf("%d %d %d", nl, nw, nc);
}

void test_2(){
	FILE* stream = getStreamFromInputOrStdByTest();
    if(stream == NULL){
        printf("fopen failed\n");
        return;
    } read_line();
	int n_char, cnt;
	int char_no = readIntHint("Wpisz numer znaku ");
	char_count(char_no, &n_char, &cnt, stream);
	printf("%c %d", n_char, cnt);
}

void test_3(){
	FILE* stream = getStreamFromInputOrStdByTest();
    if(stream == NULL){
        printf("fopen failed\n");
        return;
    } read_line();
	int digram[3];
	int char_no = readIntHint("Wpisz numer diagramu ");
	digram_count(char_no, digram, stream);
	printf("%c%c %d", digram[0], digram[1], digram[2]);
}

void test_4(){
	FILE* stream = getStreamFromInputOrStdByTest();
    if(stream == NULL){
        printf("fopen failed\n");
        return;
    } read_line();
	int line_comment_counter, block_comment_counter;
	find_comments(&line_comment_counter, &block_comment_counter, stream);
 	printf("%d %d", block_comment_counter, line_comment_counter);
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