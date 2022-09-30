#include "defines.c"

void print_int(int x) { printf("%d", x); }
void print_dbl(double x) { printf("%g", x); }
void print_s(char* s) { printf("%s", s); }
void print_default() { puts("unknown argument"); }

#define print(X) _Generic((X), \
        int: print_int, \
        double: print_dbl, \
        char*: print_s, \
        default: print_default)(X)

__mingw_ovr
__attribute__((__format__ (gnu_printf, 1, 2))) __MINGW_ATTRIB_NONNULL(1)
//* Funkcja wyświetlająca zadany tekst tylko jeśli TEST == 1
void printDebug(const char *__format, ...)
{
    if(!TEST) return;
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfprintf( stdout, __format, __local_argv );
  __builtin_va_end( __local_argv );
}

#ifdef _WINDOWS_
__mingw_ovr
__attribute__((__format__ (gnu_printf, 3, 4))) __MINGW_ATTRIB_NONNULL(3)
void printColored(const HANDLE console, const int col, const char *__format, ...)
{
  SetConsoleTextAttribute(console, col);
  int __retval;
  __builtin_va_list __local_argv; __builtin_va_start( __local_argv, __format );
  __retval = __mingw_vfprintf( stdout, __format, __local_argv );
  __builtin_va_end( __local_argv );
  SetConsoleTextAttribute(console, 15);
}
#endif

//* Funkcja pobierająca z terminala liczbę zmiennoprzecinkową
double readDouble(){
  double n = 0;
  scanf("%lf", &n);
  return n;
}

//* Funkcja pobierająca z terminala zmienną liczbową
int readInt(){
  int n = 0;
  scanf("%d", &n);
  return n;
}

//* Funkcja pobierająca z terminala pojedyńczy znak
char readChar(){
  char c = ' ';
  scanf("%c", &c);
  return c;
}

//* Funkcja pobierająca z terminala n liczb i zapisująca je do tablicy tab 
//* Nie jest powiązana z IntArray ponieważ wczytuje dane do int tab[] a nie IntArray
void readIntArray(int tab[], int n){
  for(int i = 0;i<n;i++){
    tab[i] = readInt();
  }
}

//* Funkcja pobierająca z terminala n liczb i zapisująca je do tablicy tab 
void readDoubleArray(double tab[], int n){
  for(int i = 0;i<n;i++){
    tab[i] = readDouble();
  }
}

//* Funkcja pobierająca z terminala zmienną liczbową wyświetlając podaną podpowiedź
int readIntHint(char* hint){
  printDebug(hint);
  int n = 0;
  scanf("%d", &n);
  return n;
}

//* Funkcja pobierająca z terminala liczbę zmiennoprzecinkową wyświetlając podaną podpowiedź
double readDoubleHint(char* hint){
  printDebug(hint);
  double n = 0;
  scanf("%lf", &n);
  return n;
}

//* Funkcja pobierająca z terminala pojedyńczy znak wyświetlając podaną podpowiedź
char readCharHint(char* hint){
  printDebug(hint);
  char c = ' ';
  scanf("%c", &c);
  return c;
}

//* Funkcja czytająca plik o wskazanej nazwie.
//! Pamiętaj o fclose() po zakończeniu pracy ze streamem!
FILE* readFile(char* name){
  FILE* stream = fopen(name, "r");
  if(stream == NULL) return NULL;
  return stream;
}

//* Funkcja zwracająca plik do zapisu o zadanej nazwie.
//! Pamiętaj o fclose() po zakończeniu pracy ze streamem!
FILE* writeFile(char* name){
  FILE* stream = fopen(name, "w");
  if(stream == NULL) return NULL;
  return stream;
}

void closeFile(FILE* file){
  fclose(file);
}

//* Funkcja czytająca pojedyńczą linię z pliku do zmiennej line. Zwraca długość przeczytanej linii.
int readFileLine(FILE* src, char line[LINE_SIZE]){
  int i = 0;
  char c = 0;
  while(c != EOF && c != '\n'){
    c = fgetc(src);
    line[i] = c;
    i++;
  }
  if(i > 0) line[i-1] = '\0';
  line[i] = '\0';
  return i;
}

//* Funckja zwracająca stream stdin lub stream z pliku o wskazanej nazwie
//! Pamiętaj o closeStreamByTest() po zakończeniu pracy ze streamem!
FILE* getStreamByTest(char* name){
  if(TEST){
    return stdin;
  }else{
    return readFile(name);
  }
}

//* Funkcja zamykająca otwarty wcześniej stream danych
void closeStreamByTest(FILE* stream){
  if(TEST){
    rewind(stream);
  }else{
    fclose(stream);
  }
}

#ifdef _INC_STRING

typedef void (*ForEveryWord)(void*, char*);

//* Funkcja wykonująca dla każdego napotkanego słowa w streamie funkcję ForEveryWord function
void forEachWordInStreamDo(FILE *stream, void *data, ForEveryWord function){
    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-";
    while(fgets(buf,BUFFER_SIZE,stream))
    {
        p = strtok(buf, delimits);
        while(p)
        {
            function(data, p);
            p = strtok(NULL, delimits);
        }
    }
}

#endif