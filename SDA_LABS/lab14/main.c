#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    int                 key;
    struct Node         *next;
} Node;

typedef struct Person {
    int hi;
    int ki;
} Person;

#define INVALID_INPUT   "Invalid input\n"
#define SUCCES_CODE     0
#define FAILURE_CODE    -1     


static inline bool _check_int_array(int *v, int size) {
    
    return v && size > 0;
}
int cmp_pers(const void *a, const void *b) {
    Person *p1 = (Person*)a;
    Person *p2 = (Person*)b;
    return p2->hi - p1->hi;  
}
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
Node *create_node(int key) {
    Node *temp = (Node*)malloc(sizeof(Node));
    if(!temp) return NULL;

    temp->key = key;
    temp->next = NULL;

    return temp;
}
void insert_node_head(Node **head, int key) {
    if(!head) return;

    Node *temp = create_node(key);
    temp->next = *head;
    *head = temp;
}
void print_array(int *v, int size) {
    if(!_check_int_array(v,size)) {
        printf(INVALID_INPUT);
        return;
    }

    printf("[ ");
    for(int i = 0; i < size; i++) 
        printf("%d ", v[i]);   
    printf("]\n");
}
void print_list(Node *head) {
    if(!head) {
        printf(INVALID_INPUT);
        return;
    }

    printf("[ ");
    for(Node *p = head; p != NULL; p = p->next) 
        printf("%d " ,p->key);
    printf("]\n");
}


int bubble_sort(int *v, int size) {
    
    if(!_check_int_array(v,size)) {
        printf(INVALID_INPUT);
        return FAILURE_CODE;
    }

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size - 1; j++) {
            if(v[j] > v[j + 1]) {
                v[j] ^= v[j + 1];
                v[j + 1] ^= v[j];
                v[j] ^= v[j + 1];
            }
        }
    }

    return SUCCES_CODE;
}
int selection_sort(int *v, int size) {
    
    if(!_check_int_array(v,size)) {
        printf(INVALID_INPUT);
        return FAILURE_CODE;
    }

    for(int i = 0; i < size; i++) {
        int min = v[i];
        int index = i;
        
        for(int j = i + 1; j < size; j++) {
            if(v[j] < min) {
                min = v[j];
                index = j;
            }
        }

        swap(&v[i],&v[index]);
    }

    return SUCCES_CODE;
}
int insertion_sort(int *v, int size) {
    if(!_check_int_array(v,size)) {
        printf(INVALID_INPUT);
        return FAILURE_CODE;
    }

    int i,j;
    for(i = 1; i < size; i++) {
        for(j = i - 1; j >= 0; j--) { 
            if(v[j] <= v[i]) break;
        }
        int aux = v[i]; 
        
        for(int k = i; k > j + 1; k--) v[k] = v[k - 1];
        v[j + 1] = aux;
    }
}
int binary_search(char **arr, char *key, int low, int high) {
    if(low > high) 
        return -1;
    
    int mid = (low + high) / 2;
    int cmp = strcmp(key,arr[mid]);
    
    if(cmp == 0)
        return mid;
    else if(cmp > 0)
        return binary_search(arr,key,mid + 1, high);
    else
        return binary_search(arr,key,low,mid - 1);
}
void insertion_sort_list(Node **head) {
    if(!head || !*head || !(*head)->next) return;

    Node *sorted = NULL;    
    Node *curr = *head;

    while(curr) { 
        Node *next = curr->next;

        
        if(!sorted || curr->next < sorted->key) {
            curr->next = sorted;
            sorted = curr;
        }
        else 
        {
            
            Node *p = sorted;
            
            
            while(p->next && p->next->key < curr->key)
                p = p->next;

            
            curr->next = p->next;
            p->next = curr;
        }
        
        curr = next;
    }

    *head = sorted;
}
void sort_colors(int *v, int size) {
    if(!v || size < 1) {
        printf(INVALID_INPUT);
        return;
    }

    int low = 0;
    int mid = 0;
    int high = size - 1;

    while(mid <= high) {
        if(v[mid] == 0) {
            swap(&v[mid],&v[low]);
            low++;
            mid++;
        }
        else if(v[mid] == 2) {
            swap(&v[mid], &v[high]);
            high--;

        }
        else if(v[mid] == 1) mid++;
    }
}
void counting_sort(char *v, int size) {
    int freq[128] = {0};

    for(int i = 0; i < size; i++) freq[v[i]]++;

    int index = 0;
    for(int i = 0; i < 128; i++) {
        while (freq[i] > 0)
        {
            v[index++] = i;
            freq[i]--;
        }
    }
    v[index] = '\0';
}
void counting_sort_radix(int *v, int size, int exp) {
    int *output = (int*)calloc(size, sizeof(int));
    int count[10] = {0};

    
    for(int i = 0; i < size; i++) {
        int digit = (v[i] / exp) % 10;
        count[digit]++;
    }
    
    
    for(int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    
    for(int i = size - 1; i >= 0; i--) {
        int digit = (v[i] / exp) % 10;
        output[count[digit] - 1] = v[i];
        count[digit]--;
    }

    for(int i = 0; i < size; i++)
        v[i] = output[i];
    
    
    free(output);
}
void radix_sort(int *v, int size) {
    int max = v[0];
    for(int i = 0; i < size; i++)
        if(v[i] > max)
            max = v[i];
        
    for(int exp = 1; max / exp > 0; max *= 10)
        counting_sort_radix(v,size,exp);
}
void insert_sorted_queue(Person *q, int *completed, Person key,int pos) {
    
    for(int i = *completed; i > pos; i--)
        q[i] = q[i - 1];
    
    q[pos] = key;
    (*completed)++;
}
void reconstruct_queue() {
    Person initial_queue[] = {{7,1},{4,4},{7,0},{5,0},{6,1},{5,2}};
    int q_size = sizeof(initial_queue) / sizeof(initial_queue[0]);

    Person *new_queue = (Person*)malloc((q_size + 1) * sizeof(Person));
    if(!new_queue) {
        printf("Error while alocating memory for a new queue\n");
        return;
    }

    
    qsort(initial_queue,q_size,sizeof(Person),cmp_pers);
    
    for(int i = 0 ; i < q_size; i++)
        printf("[%d,%d] ", initial_queue[i].hi, initial_queue[i].ki);

    printf("\n");

    
    int completed = 0;
    for(int i = 0; i < q_size; i++)
        insert_sorted_queue(new_queue,&completed,initial_queue[i],initial_queue[i].ki);

    for(int i = 0; i < q_size; i++)
        printf("[%d,%d], ", new_queue[i].hi,new_queue[i].ki);
    

    free(new_queue);
}
int main() {
    int v[] = {0,7,3,1,2,5,8,6,9};
    int v_size = sizeof(v) / sizeof(v[0]);

    int colors[] = {2,0,2,1,1,0};
    int c_size = sizeof(colors) / sizeof(colors[0]);

    char *names[] = {"Ana", "Bianca","Cristina","Dana","Elena","Florentina"};
    int n_size = sizeof(names) / sizeof(names[0]);

    
    Person peoples[] = {{6,0},{5,0},{4,0},{3,2},{2,2},{1,4}};
    int p_size = sizeof(peoples) / sizeof(peoples[0]);

    char word[] = "tree";
    int w_size = strlen(word);

    char *key = "Cristina";
    int pos = binary_search(names,key,0,n_size - 1);
    printf("%s se afla la pozitia %d\n",key,pos);

    counting_sort(word,w_size);
    printf("%s\n\n", word);

    insertion_sort(v,v_size);
    print_array(v,v_size);

    sort_colors(colors,c_size);
    print_array(colors,c_size);

    reconstruct_queue();

    
    return 0;
}