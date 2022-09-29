#ifndef EOF
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#endif
#ifndef BUFFER_SIZE
#include "defines.c"
#endif


typedef void (*PrintElement)(void*);
typedef void (*FreeElementData)(void*);
typedef int  (*Compare)(void*, void*);

//* Pojedyńczy element Listy
typedef struct ListElement
{
    void* data;
    struct ListElement* next;
    struct ListElement* prev;
} ListElement;


//* Lista dwukierunkowa
typedef struct tagList
{
    ListElement *head;
    ListElement *tail;
    int size;
    PrintElement printElement;
    FreeElementData freeElementData;
    Compare compare;
} List;


//* Funkcje alokujące pamięć dla nowych obiektów
List* initList(){
    List* list = (List*)malloc(sizeof(List));
    if(list == NULL){
        exit(MEMORY_ALLOCATION_ERROR);
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

ListElement* initListElement(){
    ListElement* node = (ListElement*)malloc(sizeof(ListElement));
    if(node == NULL){
        exit(MEMORY_ALLOCATION_ERROR);
    }
    node->next = NULL;
    node->prev = NULL;
    return node;
}


//* Funkcja zwalniająca pamięć ze wskazanego elementu
void freeElement(List* list, ListElement* node){
    if(list->freeElementData == NULL) return;
    list->freeElementData(node->data);
    free(node);
}

//* Generyczna funkcja wypisująca dane z listy
void printList(List* list){
    if(list->printElement == NULL) return;
    printf("List: ");
    ListElement* node = list->head;
    for(int i = 0;i<list->size;i++){
        printf("-> ");
        list->printElement(node->data);
        node = node->next;
    }
    printf("\n");
}


//* Funkcja zwalniająca zajmowaną pamięć przez listę
void freeList(List* list){
    ListElement* node = list->head, *toDelete = list->head;
    for(int i = 0;i<list->size;i++){
        toDelete = node;
        node = node->next;
        freeElement(list, toDelete);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}


//* Klasa używana do znajdywania odpowiedniego ListElement'a, np do szukania pozycji która spełnia warunek
typedef struct iterator
{
    void* data;
    struct ListElement* element;
} iterator;


//* Funkcja dodająca nowy element na koniec listy z podanymi danymi
void pushBack(List *list, void* data)
{
    ListElement* node = initListElement();
    node->data = data;
    if(list->tail) list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
    if(!list->head) list->head = list->tail;
    list->size++;
}

//* Funkcja dodająca nowy element na początku listy z podanymi danymi
void pushFront(List *list, void *data){
    ListElement* node = initListElement();
    node->data = data;
    if(list->head) list->head->prev = node;
    node->next = list->head;
    list->head = node;
    if(!list->tail) list->tail = list->head;
    list->size++;
}


//* Funkcja dodająca nowy element przed danym elementem
void pushBefore(List *list, ListElement* el, void* data)
{
    if(list->head == el){
        pushFront(list, data);
        return;
    }
    ListElement* node = initListElement();
    node->data = data;
    node->prev = el->prev;
    node->next = el;
    if(el->prev != NULL) el->prev->next = node;
    el->prev = node;
    list->size++;
}

//* Funkcja dodająca nowy element przed danym elementem
void pushAfter(List *list, ListElement* el, void* data)
{
    if(list->tail == el){
        pushBack(list, data);
        return;
    }
    ListElement* node = initListElement();
    node->data = data;
    node->next = el->next;
    node->prev = el;
    if(el->next != NULL) el->next->prev = node;
    el->next = node;
    list->size++;
}


//* Funkcja usuwająca pierwszy element z listy
void popFront(List* list){
    if(!list->head) return;
    ListElement *toDelete = list->head;
    if(list->tail == toDelete) list->tail = NULL;
    else toDelete->next->prev = NULL;
    freeElement(list, toDelete);
    list->size--;
}

//* Funkcja usuwająca ostatni element z listy
void popBack(List* list){
    if(!list->tail) return;
    ListElement *toDelete = list->tail;
    if(list->head == toDelete) list->head = NULL;
    else toDelete->prev->next = NULL;
    freeElement(list, toDelete);
    list->size--;
}

//* Funkcja usuwająca element z listy pod wskazanym indexem
void popAtIndex(List* list, int index){
    ListElement *toDelete = list->head;
    for(int i = 0;i<index;i++){
        toDelete = toDelete->next;
    }
    if(list->head == toDelete) list->head = toDelete->next;
    if(list->tail == toDelete) list->tail = toDelete->prev;
    ListElement *before = toDelete->prev;
    ListElement *after = toDelete->next;
    if(after != NULL) after->prev = before;
    if(before != NULL) before->next = after;
    freeElement(list, toDelete);
    list->size--;
}

//* Funkcja usuwająca konkretny element z listy
void popListElement(List* list, ListElement* toDelete){
    if(list->head == toDelete) list->head = toDelete->next;
    if(list->tail == toDelete) list->tail = toDelete->prev;
    ListElement *before = toDelete->prev;
    ListElement *after = toDelete->next;
    if(after != NULL) after->prev = before;
    if(before != NULL) before->next = after;
    freeElement(list, toDelete);
    list->size--;
}


//* Funkcja szuka elementu który spełnia określony warunek
//* Jest również możliwość zapisywania danych z przetworzenia kolejnego elementu
void findNodeWhere(iterator* it, int (*check)(void* data, void* itData)){
    ListElement* element = it->element;
    void* data = it->data;
    while(element != NULL){
        if(check(element->data, data)){
            break;
        }
        element = element->next;
    }
    it->data = data;
    it->element = element;
}

//* Analogiczna funkcja do findNodeWhere ale szuka w przeciwnym kierunku
void findNodeWhereReversed(iterator* it, int (*check)(void* data, void* itData)){
    ListElement* element = it->element;
    void* data = it->data;
    while(element != NULL){
        if(check(element->data, data)){
            break;
        }
        element = element->prev;
    }
    it->data = data;
    it->element = element;
}