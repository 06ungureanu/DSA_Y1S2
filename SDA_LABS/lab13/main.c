#include <stdio.h>
#include <stdlib.h>

#define HEAP_CAPACITY 20

static inline int get_parrent(int i) {
    return (i - 1) / 2;     
}
static inline int get_left_child(int i) {
    return (2*i) + 1;
}
static inline int get_right_child(int i) {
    return (2*i) + 2;
}
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void heapify_up(int *heap, int i) {
    if(i == 0) return;

    int parent = get_parrent(i);
    if(heap[i] < heap[parent]) {
        swap(&heap[i],&heap[parent]);
        heapify_up(heap,parent);
    }
}
void heapify_down(int *heap, int i, int heap_size) {
    int left = get_left_child(i);
    int right = get_right_child(i);
    int smallest = i;

    if(left < heap_size && heap[left] < heap[smallest]) 
        smallest = left;
    if(right < heap_size && heap[right] < heap[smallest])
        smallest = right;
    if(smallest != i)
    {
        swap(&heap[i],&heap[smallest]);
        heapify_down(heap,smallest,heap_size);
    }
}
void insert(int *heap, int *heap_size, int key) {
    heap[(*heap_size)++] = key;
    heapify_up(heap,*heap_size - 1);
}
void heapify(int *heap, int size) {
    if(!heap || size < 1) return;

    for(int i = (size / 2) - 1; i >= 0; i--)
        heapify_down(heap,i,size);
}
int extract_min(int *heap, int *size) {
    if(*size <= 0) {
        printf("Heap is empty\n");
        return -1;
    }

    int min = heap[0];
    
    heap[0] = heap[*size - 1];      
    (*size)--;

    heapify_down(heap,0,*size);

    return min;
}
void print_heap_sorted(int *heap, int *size) {
    while(*size > 0) {
        int min = extract_min(heap,size);
        printf("%d ", min);
    }
    printf("\n");
}
void print_heap(int *heap, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ", heap[i]);
    }
    
}
int main() {
    int heap[HEAP_CAPACITY];
    int heap_size = 0;

    int heapified[] = {30,25,8,15,12,10,20};
    int size = sizeof(heapified) / sizeof(heapified[0]);
    heapify(heapified,size);

    insert(heap,&heap_size,30);
    insert(heap,&heap_size,25);
    insert(heap,&heap_size,8);
    insert(heap,&heap_size,15);
    insert(heap,&heap_size,12);
    insert(heap,&heap_size,10);
    insert(heap,&heap_size,20);

    print_heap_sorted(heap, &heap_size);
    print_heap(heapified,size);
    return 0;
}