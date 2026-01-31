#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define N 4	

typedef struct PartialSolution {
	int originalVector[N];
	int selection[N];
	int step;
	int choice;
} PartialSolution;

PartialSolution initPS() {
	PartialSolution PS;

	PS.choice = -1;
	PS.step = -1;

	for (int i = 0; i < N; i++) {
		PS.originalVector[i] = i * 2;
		PS.selection[i] = -1;
	}

	return PS;
}
int canReject(PartialSolution PS) {
	return 0; 
}
int isSolution(PartialSolution PS) {
	return PS.step == N - 1; 
}
void printSolution(PartialSolution PS) {
	for (int i = 0; i < N; i++) {
		if (PS.selection[i] == 1)
			printf("%d ", PS.originalVector[i]);
	}
	printf("\n");
}
PartialSolution increaseStep(PartialSolution PS) {
	PS.step++;
	PS.choice = 0;
	return PS;
}
int hasChoiceAtStep(PartialSolution PS) {
	return PS.choice < 2;
}
PartialSolution getChoiceAtStep(PartialSolution PS) {
	PS.selection[PS.step] = PS.choice;
	PS.choice++;
	return PS;
}
void backtracking(PartialSolution PS) {
	if(canReject(PS))
		return;
	if(isSolution(PS))
		printSolution(PS);
	PS = increaseStep(PS);
	while (hasChoiceAtStep(PS)) {
		PS = getChoiceAtStep(PS);
		backtracking(PS);
	}
}
int main() {
	PartialSolution PS = initPS();
	backtracking(PS);
	return 0;
}