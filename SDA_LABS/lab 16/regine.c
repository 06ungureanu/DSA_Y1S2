#include <stdio.h>
#include <stdlib.h>

#define N 4

typedef struct PartialSolution {
    int queens[N];
    int step;
    int choice;
} PartialSolution;

PartialSolution init_partialSolution() {
    PartialSolution PS;
    PS.choice = -1;
    PS.step = -1;

    for(int i = 0; i < N; i++)
        PS.queens[i] = -1;  

    return PS;
}
int canReject(PartialSolution PS) {
    for(int i = 0; i < PS.step; i++) {
        if(PS.queens[i] == PS.queens[PS.step]) 
            return 1;
        if(abs(PS.queens[i] - PS.queens[PS.step]) == abs(i - PS.step))
            return 1;
    }
    return 0;
} 
int isSolution(PartialSolution PS) {
    return !canReject(PS) && PS.step == N - 1;
}
void printSolution(PartialSolution PS) {
    for(int i = 0; i < N; i++) 
        printf("%d ", PS.queens[i]);
    printf("\n");
}
PartialSolution increaseStep(PartialSolution PS) {
    PS.step++;
    PS.choice = 0;
    return PS;
}
int hasChoiceAtStep(PartialSolution PS) {
    return PS.choice < N && PS.step < N;
}
PartialSolution getNextStepAtChoice(PartialSolution PS) {
    PS.queens[PS.step] = PS.choice;
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
        PS = getNextStepAtChoice(PS);
        backtracking(PS);
    }
}
int main() {
    PartialSolution PS = init_partialSolution();
    backtracking(PS); 
    return 0;
}