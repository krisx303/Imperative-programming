#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000
#define ROOT_PRECISION 10
typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair*, int);
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair*, int);
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair*, int);
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair*, int);
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*, int);
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair*, int);

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);
// A total order relation is a partial order relation that is connected
int is_total_order(pair*, int);
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair*, int);
int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);
int get_domain(pair*, int, int*);

// Case 3:
int composition (pair*, int, pair*, int, pair*);

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair*);

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int read_relation(pair* pairs){
    int size;
    scanf("%d", &size);
    int a, b;
    for(int i = 0; i<size;i++){
        scanf("%d %d", &a, &b);
        pairs[i].first = a;
        pairs[i].second = b;
    }
}

int get_domain(pair* pairs, int n, int domain[]){
    int dsize = 0;
    for(int i = 0;i<n;i++){
        int af = pairs[i].first;
        int as = pairs[i].second;
        int b = 0;
        for(int j = 0; j<dsize;j++){
            if(domain[j] == af){
                b = 1;
                break;
            }
        }
        if(b == 0){
            domain[dsize] = af;
            dsize++;
        }
        b = 0;
        for(int j = 0; j<dsize;j++){
            if(domain[j] == as){
                b = 1;
                break;
            }
        }
        if(b == 0){
            domain[dsize] = as;
            dsize++;
        }
    }
    return dsize;
}

int is_reflexive(pair* pairs , int n){
    int domain[n];
    int dsize = get_domain(pairs, n, domain);
    int counter = 0;
    for(int i = 0; i<n;i++){
        if(pairs[i].first == pairs[i].second){
            counter++;
        }
    }
    return counter == dsize ? 1 : 0;
}

int is_irreflexive(pair* pairs , int n){
    for(int i = 0;i<n;i++){
        if(pairs[i].first == pairs[i].second){
            return 0;
        }
    }
    return 1;
}

int equals_reversed(pair p1, pair p2){
    return p1.first == p2.second && p2.first == p1.second;
}

int is_symmetric(pair* pairs, int n){
    pair pp[n];
    int tocheck = 1;
    pp[0] = pairs[0];
    for(int i = 1;i<n;i++){
        int flag = 0;
        for(int j = 0;j<tocheck;j++){
            if(equals_reversed(pp[j], pairs[i])){
                tocheck--;
                pp[j] = pp[tocheck];
                flag = 1;
                break;
            }
        }
        if(flag) continue;
        pp[tocheck] = pairs[i];
        tocheck++;
    }
    return tocheck == 0;
}

int is_antisymmetric(pair* pairs, int n){
    pair pp[n];
    int tocheck = 0;
    for(int i = 0; i<n;i++){
        for(int j = 0;j<tocheck;j++){
            if(equals_reversed(pp[j], pairs[i]) && pp[j].first != pp[j].second){
                return 0;
            }
        }
        pp[tocheck] = pairs[i];
        tocheck++;
    }
    return 1;
}

int is_asymmetric(pair* pairs, int n){
//    int domain[n];
//    int dsize = get_domain(pairs, n, domain);
//    for(int i = 0;i<dsize;i++){
//        for(int j = i;j<dsize;j++){
//            int flag = 0;
//            for(int k = 0;k<n;k++){
//                if((pairs[k].first == domain[i] && pairs[k].second == domain[j])) flag +=1;
//                else if(pairs[k].first == domain[j] && pairs[k].second == domain[i]) flag +=10;
//                if(pairs[k].first == domain[i] && pairs[k].second == domain[j] && i == j) return 0;
//            }
//            if(flag == 0) return 0;
//        }
//    }
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            if(equals_reversed(pairs[j], pairs[i])){
                return 0;
            }
        }
    }
    return 1;
}

int is_transitive(pair* pairs, int n){
    pair pp[10][n];
    int size[10];
    for(int i = 0;i<10;i++){
        size[i] = 0;
    }
    for(int i = 0; i<n;i++){
        int f = pairs[i].first%10;
        pp[f][size[f]] = pairs[i];
        size[f]++;
    }
    for(int i = 0;i<10;i++){
        for(int x = 0;x<size[i];x++){
            pair xx = pp[i][x];
            int xi = xx.second%10;
            int b = 0;
            for(int y = 0;y<size[xi];y++){
                pair yy = pp[xi][y];
                for(int j = 0;j<size[i];j++){
//                    printf("\n%d %d - %d %d - %d %d", xx.first, xx.second, yy.first, yy.second, pp[i][j].first, pp[i][j].second);
                    if(pp[i][j].second == yy.second){
                        b = 1;
                        break;
                    }
                }
                if(b) break;
            }
            if(b == 0 && size[xi] > 0) return 0;
        }
    }
    return 1;
}

int is_partial_order(pair* pairs, int n){
    return is_reflexive(pairs, n) && is_transitive(pairs, n) && is_antisymmetric(pairs, n);
}

int is_total_order(pair* pairs, int n){
    if(!is_partial_order(pairs, n)) return 0;
    int domain[n];
    int dsize = get_domain(pairs, n, domain);
    for(int i = 0;i<dsize;i++){
        for(int j = i;j<dsize;j++){
            int flag = 0;
            for(int k = 0;k<n;k++){
                if((pairs[k].first == domain[i] && pairs[k].second == domain[j])) flag +=1;
                else if(pairs[k].first == domain[j] && pairs[k].second == domain[i]) flag +=10;
            }
            if(flag == 0) return 0;
        }
    }
    return 1;
}

int find_max_elements(pair* pairs, int n, int* tab){
    int domain[n];
    int dsize = get_domain(pairs, n, domain);
    int counter = 0;
    for(int i = 0;i<dsize;i++){
        int flag = 0;
        for(int j = 0;j<n;j++){
            if(pairs[j].first == domain[i] && pairs[j].first != pairs[j].second){
                flag = 1;
            }
        }
        if(!flag){
            tab[counter] = domain[i];
            counter++;
        }
    }
    return counter;
}
int find_min_elements(pair* pairs, int n, int* tab){
    int domain[n];
    int dsize = get_domain(pairs, n, domain);
    int counter = 0;
    for(int i = 0;i<dsize;i++){
        int flag = 0;
        for(int j = 0;j<n;j++){
            if(pairs[j].second == domain[i] && pairs[j].first != pairs[j].second){
                flag = 1;
            }
        }
        if(!flag){
            tab[counter] = domain[i];
            counter++;
        }
    }
    return counter;
}


int composition (pair* pairs1, int n1, pair* pairs2, int n2, pair* pairs3){
    int counter = 0;
    for(int i = 0;i<n1;i++){
        for(int j = 0;j<n2;j++){
            if(pairs1[i].second == pairs2[j].first){
                int flag = 0;
                pair p;
                p.first = pairs1[i].first;
                p.second = pairs2[j].second;
                for(int k = 0;k<counter;k++){
                    if(pairs3[k].first == p.first && pairs3[k].second == p.second){
                        flag = 1;
                    }
                }
                if(!flag){
                        pairs3[counter] = p;
                        counter++;
                }
            }
        }
    }
    return counter;
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];


	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			qsort(domain, n_domain, sizeof(int), cmpfunc);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
