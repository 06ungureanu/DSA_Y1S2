#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3

typedef struct PartialSolution {
	int originalVector[N];
	int permutation[N];
	int step;
	int choice;
} PartialSolution;

PartialSolution initPartialSolution() {
	PartialSolution PS;

	PS.choice = -1;
	PS.step = -1;

	for (int i = 0; i < N; i++) {
		PS.originalVector[i] = i * 2;
		PS.permutation[i] = -1;
	}

	return PS;
}
int canReject(PartialSolution PS) {
	for (int i = 0; i < PS.step; i++) {
		if(PS.permutation[i] == PS.permutation[PS.step])
			return 1;
	}
	return 0;
}
int isSolution(PartialSolution PS) {
	return !canReject(PS) && PS.step == N - 1;
}
void printSolution(PartialSolution PS) {
	for(int i = 0; i < N; i++)
		printf("%d ", PS.permutation[i]);
	printf("\n");
}
PartialSolution increaseStep(PartialSolution PS) {
	PS.choice = 0;
	PS.step++;
	return PS;
}
int hasChoiceAtStep(PartialSolution PS) {
	return PS.step < N && PS.choice < N;
}
PartialSolution getNextChoiceAtStep(PartialSolution PS) {
	PS.permutation[PS.step] = PS.originalVector[PS.choice];
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
		PS = getNextChoiceAtStep(PS);
		backtracking(PS);
	}
}
void main() {
	PartialSolution PS = initPartialSolution();
	backtracking(PS);
}