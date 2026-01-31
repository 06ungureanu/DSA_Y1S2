#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct Heap {
	int *heap;
	int capacity;
	int heapSize;
} Heap;

int getParent(int i) {
	return (i - 1) / 2;
}
int leftChild(int i) {
	return (i * 2) + 1;
}
int rightChild(int i) {
	return (i * 2) + 2;
}
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
Heap* createNewHeap(int capacity) {
	Heap *temp = (Heap*)malloc(sizeof(Heap));

	temp->heap = (int*)calloc(capacity, sizeof(int));
	temp->capacity = capacity;
	temp->heapSize = 0;

	return temp;
}

#pragma region MinHeap funcs
void bubbleUpMin(Heap* minHeap, int i) {
	if(i == 0)
		return;

	int parent = getParent(i);
	if (minHeap->heap[i] < minHeap->heap[parent]) {
		swap(&minHeap->heap[i], &minHeap->heap[parent]);
		bubbleUpMin(minHeap,parent);
	}
}
void sinkMin(Heap* minHeap, int i) {
	int left = leftChild(i);
	int right = rightChild(i);
	int smallest = i;

	if(left < minHeap->heapSize && minHeap->heap[left] < minHeap->heap[smallest])
		smallest = left;
	if(right < minHeap->heapSize && minHeap->heap[right] < minHeap->heap[smallest])
		smallest = right;
	if (i != smallest) {
		swap(&minHeap->heap[i], &minHeap->heap[smallest]);
		sinkMin(minHeap,smallest);
	}
}
void heapifyMin(Heap* minHeap) {
	if(!minHeap || !minHeap->heap)
		return;

	for(int i = (minHeap->heapSize - 1) / 2; i >= 0; i--)
		sinkMin(minHeap,i);
}
int extractMin(Heap* minHeap) {
	if(!minHeap || !minHeap->heap)
		return -1;
	int min = minHeap->heap[0];
	minHeap->heap[0] = minHeap->heap[minHeap->heapSize - 1];
	minHeap->heapSize--;
	sinkMin(minHeap,0);
	return min;
}
#pragma endregion

#pragma region MaxHeap funcs
void bubbleUpMax(Heap* maxHeap, int i) {
	if(i == 0)
		return;

	int parent = getParent(i);
	if (maxHeap->heap[i] > maxHeap->heap[parent]) {
		swap(&maxHeap->heap[i], &maxHeap->heap[parent]);
		bubbleUpMax(maxHeap,parent);
	}
}
void sinkMax(Heap *maxHeap,int i) {
	int left = leftChild(i);
	int right = rightChild(i);
	int largest = i;

	if(left < maxHeap->heapSize && maxHeap->heap[left] > maxHeap->heap[largest])
		largest = left;
	if(right < maxHeap->heapSize && maxHeap->heap[right] > maxHeap->heap[largest])
		largest = right;
	if (i != largest)
	{
		swap(&maxHeap->heap[i], &maxHeap->heap[largest]);
		sinkMax(maxHeap, largest);
	}
}
void heapifyMax(Heap* maxHeap) {
	if(!maxHeap || !maxHeap->heap)
		return;
	for(int i = (maxHeap->heapSize - 1) / 2; i>= 0; i--)
		sinkMax(maxHeap,i);
}
int extractMax(Heap* maxHeap) {
	if(!maxHeap || !maxHeap->heap)
		return -1;

	int max = maxHeap->heap[0];
	maxHeap->heap[0] = maxHeap->heap[maxHeap->heapSize - 1];
	maxHeap->heapSize -= 1;
	
	sinkMax(maxHeap,0);
	return max;
}
#pragma endregion
void printHeap(Heap* binaryHeap) {
	if(!binaryHeap || !binaryHeap->heap)
		return;
	for(int i = 0; i < binaryHeap->heapSize; i++)
		printf("%d ", binaryHeap->heap[i]);
	printf("\n");
}

int main() {
	int v[] = {30,25,8,15,12,10,20};
	int v_size = sizeof(v) / sizeof(v[0]);

	Heap* minHeap = createNewHeap(v_size);
	Heap* maxHeap = createNewHeap(v_size);

	minHeap->heapSize = v_size;
	maxHeap->heapSize = v_size;

	for (int i = 0; i < v_size; i++) {
		minHeap->heap[i] = v[i];
		maxHeap->heap[i] = v[i];
	}

	heapifyMin(minHeap);
	heapifyMax(maxHeap);

	printf("Min heap : ");
	printHeap(minHeap);

	printf("Max Heap : ");
	printHeap(maxHeap);
	printf("\n");

	int min1 = extractMin(minHeap);
	int min2 = extractMin(minHeap);

	int max1 = extractMax(maxHeap);
	int max2 = extractMax(maxHeap);

	printf("Min value 1 from heap %d\n", min1);
	printf("Min value 2 from heap %d\n\n", min2);

	printf("Max value 1 from heap %d\n", max1);
	printf("Max value 2 from heap %d\n", max2);

	return 0;
}