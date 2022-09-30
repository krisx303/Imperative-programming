#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../libraries/inout.c"

#define CARDS_SIZE 52
#define COLORS 4

const char CHARS[4] = "JQKA";
int conflicts = 0;

typedef struct Player{
    int cards[CARDS_SIZE];
    int last;
    int len;
} Player;

Player* initPlayer(){
    Player* player = (Player*)malloc(sizeof(Player));
    player->last = 0;
    player->len = 0;
    return player;
}

int rand_from_interval(int a, int b) {
    if(a > b){
        return INT_MIN;
    }else if(b - a > RAND_MAX){
        return INT_MAX;
    }else if(a == b){
        return a;
    }
    return rand() % (b - a + 1) + a;
}

void rand_permutation(int n, int tab[]) {
    if(n < 0)
        return;
    for(int i = 0; i<n;i++){
        tab[i] = i;
    }
    for(int i = 0;i<n;i++){
        int k = rand_from_interval(i, n-1);
        int buffor = tab[i];
        tab[i] = tab[k];
        tab[k] = buffor;
    }
}

void give_away_cards(Player* player1, Player* player2){
    int cards[CARDS_SIZE];
    rand_permutation(CARDS_SIZE, cards);
    for(int i = 0;i<CARDS_SIZE/2;i++){
        player1->cards[i] = cards[i];
        player2->cards[i] = cards[CARDS_SIZE/2 + i];
    }
    player1->last = 0;
    player2->last = 0;
    player1->len = CARDS_SIZE/2;
    player2->len = CARDS_SIZE/2;
}

void print_card(int card){
    int color = card%4;
    int number = card/4;
    if(number < 8)
        printf("%c ", 50 + number);
    else if(number == 8)
        printf("10 ");
    else
        printf("%c ", CHARS[number - 9]);
}

void print_cards(Player* player){
    if(TEST == 0) return;
    for(int i = 0;i<player->len;i++){
        print_card(player->cards[(player->last+i)%CARDS_SIZE]);
    }
    printf("\n");
}

void print_cards_force(Player* player){
    for(int i = 0;i<player->len;i++){
        print_card(player->cards[(player->last+i)%CARDS_SIZE]);
    }
    printf("\n");
}

void push_card(Player* player, int card){
    // printf("%d %d\n", player->len, player->last);
    // print_card(player->cards[player->last + player->len - 1]);
    // printf("\n");
    player->cards[(player->last + player->len) % CARDS_SIZE] = card;
    player->len ++;
    // print_cards(player);
    // printf("%d %d\n", player->len, player->last);
    // print_card(player->cards[(player->last + player->len - 1)%CARDS_SIZE]);
    // printf("\n");
}

int pop_card(Player* player){
    if(player->len == 0)
        return -1;
    int tmp = player->cards[player->last];
    player->last = (player->last+1)%CARDS_SIZE;
    player->len--;
    return tmp;
}

int play(Player* player1, Player* player2){
    int a = pop_card(player1);
    int b = pop_card(player2);
    int aa = a/4;
    int bb = b/4;
    if(a == -1){
        printDebug("Player A do not have any cards");
        return -1;
    }else if(b == -1){
        printDebug("Player B do not have any cards");
        return -2;
    }else{
        conflicts++;
        printDebug("->%d %d\n", aa, bb);
        if(aa > bb){
            if(TEST){
                printDebug("Player A wins: ");
                print_card(a);
                print_card(b);
                printDebug("\n");
            }
            push_card(player1, a);
            push_card(player1, b);
        }else if(bb > aa){
            if(TEST){
                printDebug("Player B wins: ");
                print_card(a);
                print_card(b);
                printDebug("\n");
            }
            push_card(player2, b);
            push_card(player2, a);
        }else{
            if(TEST){
                printDebug("Draw: ");
                print_card(a);
                print_card(b);
                printDebug("\n");
            }
            //draw wersja uproszczona
            push_card(player1, a);
            push_card(player2, b);
        }
    }
    print_cards(player1);
    print_cards(player2);
}

int main(int argc, char* args[]){
    if(argc > 1 && args[1][0] == '1')
		TEST = 1;
    int seed = readIntHint("Enter seed: ");
    int type = readIntHint("Enter type (0/1): ");
    int max_conf = readIntHint("Enter max number of conflicts: ");
    srand(seed);
    Player* player1 = initPlayer();
    Player* player2 = initPlayer();
    give_away_cards(player1, player2);
    print_cards(player1);
    print_cards(player2);
    while(1){
        int result = play(player1, player2);
        if(result < 0){
            printf("%d\n", result + 4);
            if(result == -2) print_cards_force(player1);
            if(result == -1) print_cards_force(player2);
            break;
        }else if(conflicts > max_conf){
            printf("0\n%d\n%d", player1->len, player2->len);
            break;
        }

    }
    return 0;
}
