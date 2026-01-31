#include <stdio.h>
#include <stdlib.h>

#define N 4

typedef struct PartialSolution {
    int path[N];
    int used[N];
    int step;
    int choice;
} PartialSolution;

int D[N][N] = {  
    { 0, 10, 15, 20 },
    { 10, 0, 35, 25 },
    { 15, 35, 0, 30 },
    { 20, 25, 30, 0 }
};

PartialSolution init_partialSolution() {
    PartialSolution PS;
    PS.choice = 0;
    PS.step = -1;

    for(int i = 0; i < N; i++) {
        PS.path[i] = -1;
        PS.used[i] = 0;
    }

    PS.path[0] = 0;
    PS.used[0] = 1;
    
    return PS;
}

int canReject(PartialSolution PS) {
    if(PS.used[PS.choice] == 1)
        return 1;
    return 0;
}
int isSolution(PartialSolution PS) {
    return PS.step == N - 1;
}
void printSolution(PartialSolution PS) {
    int cost = 0;
    for(int i = 0; i < N; i++)
        printf("%d ", PS.path[i]);
    
    for(int i = 0; i < N - 1; i++)
        cost += D[PS.path[i]][PS.path[i + 1]];
    cost += D[PS.path[N - 1]][PS.path[0]];
    printf(" | Cost: %d\n", cost);
}
PartialSolution increaseStep(PartialSolution PS) {
    PS.step++;
    PS.choice = 0;
    return PS;
}
int hasChoiceAtStep(PartialSolution PS) {
    return PS.step < N;
}
PartialSolution getNextChoiceAtStep(PartialSolution PS) {
    PS.path[PS.step] = PS.choice;
    PS.used[PS.choice] = 1;
    PS.choice++;
    
    return PS;
}
void backtracking(PartialSolution PS) {
    if(canReject(PS))
        return;
    if(isSolution(PS))
        printSolution(PS);
    PS = increaseStep(PS);
    while(hasChoiceAtStep(PS)) {
        PS = getNextChoiceAtStep(PS);
        backtracking(PS);
    }
}
int main() {
    PartialSolution PS = init_partialSolution();
    backtracking(PS);
    return 0;
}