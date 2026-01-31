#include <stdio.h>
#include <stdlib.h>

typedef struct Show {
    int start;
    int end;
} Show;

int CITY[4][4] = {
    {0, 10, 15, 20},
    {10, 0, 35, 25},
    {15, 35, 0, 30},
    {20, 25, 30, 0}
};
int PATH[4];
int USED[4];

int cmp(void *a, void *b) {
    Show *s1 = (Show*)a;
    Show *s2 = (Show*)b;

    return s1->end - s2->end;
}
int cmp_bils(void *a, void *b) {
    return *((int*)b) - *((int*)a);
}
int get_max_shows(Show *v, int n) {
    qsort(v,n,sizeof(Show),cmp); 

    int count = 1;
    int last_show = v[0].end;

    for(int i = 0; i < n; i++) {
        if(v[i].start >= last_show) {
            count++;
            last_show = v[i].end;
        }
    }

    return count;
}
void best_bills(int *bills, int b, int S) {
    qsort(bills,b,sizeof(int),cmp_bils); 

    printf("Suma %d va fi platita in urmatoarea ordine : ", S);

    for(int i = 0; i < b; i++) {
        int count = 0;
        while(S >= bills[i]) {
            S -= bills[i];
            count++;
        }

        printf("%d x %d\n",count,bills[i]);
    }
}
int TPS(int n) {
    int total_cost = 0;
    int current = 0;
    USED[current] = 1;
    PATH[0] = current;

    for(int i = 0; i < n; i++) {
        int next = -1;
        int min_cost = 999999;

        for(int j = 0; j < n; j++) {
            if(!USED[j] && CITY[current][j] < min_cost) {
                min_cost = CITY[current][j];
                next = j;
            }
        }

        USED[next] = 1;
        PATH[i] = next;
        total_cost += min_cost;
        current = next;
    }

    total_cost += CITY[current][0];
    PATH[n] = 0;

    for(int i = 0; i < n; i++) {
        printf ("%d -> ", PATH[i]);
    }
    printf("\nCost : %d\n", total_cost);
}
int main() {
    Show shows[] = {
        {1, 3},
        {2, 5},
        {4, 6},
        {6, 8},
        {5, 7},
        {8, 9}
    };
    int n = sizeof(shows) / sizeof(shows[0]);
    int good_shows = get_max_shows(shows,n);
    printf("Se pot realizaz %d spectacole fara a se suprapune\n", good_shows);

    int bills[] = {1, 5, 10, 50, 100};
    int b = sizeof(bills) / sizeof(bills[0]);
    int S = 289;

    best_bills(bills,b,S);
    TPS(4);
    return 0;
}