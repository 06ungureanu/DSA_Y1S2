#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;

#define BUFF_SIZE 256
#define MAX_CAPCITY 64

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

typedef struct Beach {
    char            *name;
    float           *grades;
    float           rating;
    int             num_grades;
    u8              flag;
    u8              visited;
} Beach;

typedef struct BeachNode {
    Beach            *key;
    struct BeachNode *next;
} BeachNode;

typedef struct Heap {
    Beach           **heap;
    int             size;
} Heap;

typedef struct HashTable {
    BeachNode        **HT;
    int             t_size;
} HashTable;


float get_rating(float *grades, int n) {
    if(grades == NULL || n <= 0) return 0;

    float rating = 0;
    for(int i = 0; i < n; i++)
        rating += grades[i];

    return rating / n;
}
int hash(const char *s) {
    if(s == NULL) return 0;

    int hash = 0;
    for(; *s; s++) {
        hash += (int)*s;
        hash %= MAX_CAPCITY; 
    }

    return hash;
}
void swap(Beach **a, Beach **b) {
    Beach *temp = *a;
    *a = *b;
    *b = temp;
}


Beach *create_new_beach(const char *name, float *grades, int num_grades, u8 flag) {
    if(name == NULL || grades == NULL) return NULL;

    Beach *temp = (Beach*)malloc(sizeof(Beach));
    
    temp->flag = flag;
    temp->num_grades = num_grades;
    temp->name = strdup(name);
    temp->rating = get_rating(grades,num_grades);
    temp->visited = 0;

    temp->grades = (float*)malloc(num_grades * sizeof(float));
    for(int i = 0; i < num_grades; i++)
        temp->grades[i] = grades[i];

    return temp;
}
BeachNode *create_new_beach_node(Beach *key) {
    if(!key) return NULL;

    BeachNode *temp = (BeachNode*)malloc(sizeof(BeachNode));
    
    temp->key = key;
    temp->next = NULL;
    return temp;
}
Heap *create_heap() {
    Heap *temp = (Heap*)malloc(sizeof(Heap));
    if(!temp) return NULL;

    temp->heap = (Beach**)malloc(MAX_CAPCITY * sizeof(Beach*));
    temp->size = 0;

    return temp;
}
HashTable *create_hash_table() {
    HashTable *temp = (HashTable*)malloc(sizeof(HashTable));
    if(!temp) return NULL;

    temp->HT = (BeachNode**)malloc(MAX_CAPCITY * sizeof(BeachNode*));
    temp->t_size = 0;

    return temp;
}


static inline int get_parent(int i) {
    return (i - 1) / 2;
}
static inline int get_left_child(int i) {
    return (i * 2) + 1;
}
static inline int get_right_child(int i) {
    return (i * 2) + 2;
}
void heapify_up(Heap *heap, int i ) {
    if(i == 0) return;

    int parent = get_parent(i);
    if(heap->heap[i]->rating > heap->heap[parent]->rating) {
        swap(&heap->heap[i], &heap->heap[parent]);
        heapify_up(heap,parent);
    }
}   
void heapify_down(Heap *heap, int i) {
    if (!heap || !heap->heap) return;

    int left = get_left_child(i);
    int right = get_right_child(i);
    int largest = i;

    if (left < heap->size && heap->heap[left]->rating > heap->heap[largest]->rating)
        largest = left;

    if (right < heap->size && heap->heap[right]->rating > heap->heap[largest]->rating)
        largest = right;

    if (largest != i) {
        swap(&heap->heap[i], &heap->heap[largest]);
        heapify_down(heap, largest);
    }
}
void insert_in_heap(Heap *heap, Beach *key) {
    if(!heap || !heap->heap) return;
    if(!key) return;

    heap->heap[heap->size++] = key;
    heapify_up(heap,heap->size - 1);
}
Beach *extract_max(Heap *heap) {
    if (!heap || !heap->heap || heap->size == 0)
        return NULL;

    Beach *max = heap->heap[0];

    heap->heap[0] = heap->heap[--heap->size];
    heapify_down(heap, 0);

    return max;
}
void heapify(Heap *heap) {
    for(int i = (heap->size / 2) - 1; i >= 0; i--)
        heapify_down(heap,i);
}

void insert_head_of_list(BeachNode **head, Beach *key) {
    if(!head || !key) return;

    BeachNode *temp = create_new_beach_node(key);
    temp->next = *head;
    *head = temp;
} 
Beach *search_node_in_list(BeachNode *head, const char *key) {
    if(!head || !key) return NULL;

    BeachNode *curr = head;
    while(curr) {
        if(strcmp(curr->key->name,key) == 0)
            return curr->key;
        
        curr = curr->next;
    }
    return NULL;
}
void insert_key_in_hash_table(HashTable *HT, Beach *key) {
    if(!HT || !HT->HT) return;
    if(!key) return;

    int index = hash(key->name);
    insert_head_of_list(&HT->HT[index],key);
    HT->t_size++;
}
Beach *search_beach_in_hash_table(HashTable *list_beach, const char *key) {
    if(!list_beach || !key) return NULL;

    int index = hash(key);
    Beach *searched = search_node_in_list(list_beach->HT[index],key);

    return searched;
}


Heap *construct_heap_from_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) {
        printf("Invalid input file\n");
        return NULL;
    }

    Heap *top_beaches = create_heap();
    if(!top_beaches) return NULL;

    char name[BUFF_SIZE];
    char bar_str[4];
    int num_grades;
    float grades[MAX_CAPCITY];

    while(fscanf(f,"%s %s %d", name, bar_str, &num_grades) == 3) {
        for(int i = 0; i < num_grades; i++) {
           if(fscanf(f,"%f",&grades[i]) != 1) {
            printf("Error while reading grades from file\n");
            break;
           }
        }
         
        int bar = (strcmp(bar_str,"yes") == 0);
        Beach *new_beach = create_new_beach(name,grades,num_grades,bar);
        insert_in_heap(top_beaches,new_beach);
    }

    fclose(f);
    return top_beaches;
}
HashTable *construct_hash_table_from_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) {
        printf("Invalid input file\n");
        return NULL;
    }

    HashTable *beach_list = create_hash_table();
    if(!beach_list) return NULL;

    char name[BUFF_SIZE];
    char bar_str[4];
    int num_grades;
    float grades[MAX_CAPCITY];

    while (fscanf(f, "%s %s %d", name, bar_str, &num_grades) == 3) {
        for (int i = 0; i < num_grades; i++) {
            if (fscanf(f, "%f", &grades[i]) != 1) {
                printf("Error while reading grades from file\n");
                break;
            }
        }
    
        int flag = (strcmp(bar_str, "yes") == 0);
        Beach *new_beach = create_new_beach(name, grades, num_grades, flag);
        insert_key_in_hash_table(beach_list, new_beach);
    }

    fclose(f);
    return beach_list;
}




void show_flag_menu(int *FLAG) {
    system(CLEAR);
    printf("========================= Bar Flag Options =========================\n");
    printf("= 0. Beaches without bars                                          =\n");
    printf("= 1. Beaches with bars                                             =\n");
    printf("= 2. All Beaches                                                   =\n");
    printf("====================================================================\n");
    printf("> ");
    scanf("%d", FLAG);
}
void show_menu(int FLAG) {
    system(CLEAR);
    printf("========================= Bar Flag : %d ===========================\n",FLAG);
    printf("= 1. Insert Beach                                                =\n");
    printf("= 2. Insert Grade Beach                                          =\n");
    printf("= 3. Get Next Beach                                              =\n");
    printf("= 4. Exit                                                        =\n");
    printf("==================================================================\n");
}

void show_beach_info(Beach *beach) {
    if(!beach) return;

    printf("Name : %s\n", beach->name);
    printf("Bar : %d\n", beach->flag);
    printf("Rating : %.1f\n",beach->rating);
    printf("Grades : ");
    for(int i = 0; i < beach->num_grades; i++)
        printf("%.1f ", beach->grades[i]);
    printf("\nAlready visted : No\n");
}
void show_next_beach(Heap *top_beaches, int FLAG) {
    if (!top_beaches || !top_beaches->heap) return;

    Beach *next_beach = extract_max(top_beaches);

    while (next_beach && (next_beach->visited || (FLAG != 2 && next_beach->flag != FLAG))) {
        next_beach = extract_max(top_beaches);
    }

    if (!next_beach) {
        printf("No matching beach found.\n");
        return;
    }

    show_beach_info(next_beach);
    next_beach->visited = 1;
}

Beach *insert_new_beach_helper() {
    char name[BUFF_SIZE];
    int flag;
    int num_grades;
    float grades[MAX_CAPCITY];

    printf("Enter Beach Name : ");
    scanf("%s", name);
    printf("Bar flag : ");
    scanf("%d", &flag);
    printf("Number of grades : ");
    scanf("%d", &num_grades);

    for(int i = 0; i < num_grades; i++) {
        printf("Enter grade %d : ",i);
        scanf("%f", &grades[i]);
    }

    Beach *new_beach = create_new_beach(name,grades,num_grades,flag);

    return new_beach;

}
void insert_new_beach(Heap *top_beaches, HashTable *beach_list) {
    Beach *new_beach = insert_new_beach_helper();
    if(!new_beach) {
        printf("Insertion of a new beach failed\n");
        return;
    }

    insert_key_in_hash_table(beach_list,new_beach);
    insert_in_heap(top_beaches,new_beach);
}

void insert_grade_in_beach_helper(char *name, float *grade) {
    printf("Enter beach name : ");
    scanf("%s", name);
    printf("Enter beach grade : ");
    scanf("%f", grade);
}
void insert_grade_in_beach(HashTable *beach_list, Heap *top_beaches, const char *key, float grade) {
    if(!beach_list || !beach_list->HT) return;
    if(!key || grade < 0) return;

    Beach *searched = search_beach_in_hash_table(beach_list,key);
    if(!searched) {
        printf("Beach was not found in list\n");
        return;
    }

    if(searched->num_grades + 1 > MAX_CAPCITY) {
        printf("Can't insert a new grade in given beach\n");
        return;
    }

    searched->grades[searched->num_grades++] = grade; 
    searched->rating = get_rating(searched->grades,searched->num_grades);

    for (int i = 0; i < top_beaches->size; i++) {
        if (top_beaches->heap[i] == searched) {
            heapify_down(top_beaches, i); 
            break;
        }
    }
}


void merge(int *v, int low,int mid,int high) {
    int *temp = (int*)malloc(((high - low) + 1) * sizeof(int));

    int i = low;
    int j = mid + 1;
    int k = 0;
    
    while(i <= mid && j <= high) {
        if(v[i] <= v[j]) 
            temp[k++] = v[i++];
        else 
            temp[k++] = v[j++];
    }

    while(i <= mid) temp[k++] = v[i++];
    while(j <= high) temp[k++] = v[j++];

    for(int i = low; i <= high; i++)
        v[i] = temp[i - low];
    
    free(temp);
}
void merge_sort(int *v, int low, int high) {
    if(low < high) {
        int mid = low + (high - low) / 2;

        merge_sort(v,low,mid);
        merge_sort(v,mid + 1,high);
        merge(v,low,mid,high);
    }
}
int exist_triunghi(int *v, int n) {
    if(n < 3) return 0; 

    merge_sort(v,0,n - 1);

    for(int i = 0; i < n; i++) {
        if(v[i] + v[i + 1] > v[i + 2]) 
            return 1;
    }

    return 0;
}


int main() {
    const char *filename = "in.txt";

    Heap *top_beaches = construct_heap_from_file(filename);
    HashTable *beach_list = construct_hash_table_from_file(filename);

    int flag;
    int input;

    show_flag_menu(&flag);
    show_menu(flag);
    
    while(1) {
        printf("> ");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            insert_new_beach(top_beaches,beach_list);
            show_menu(flag);
            break;
        case 2: {
                char name[BUFF_SIZE];
                float grade;
                insert_grade_in_beach_helper(name, &grade);
                insert_grade_in_beach(beach_list,top_beaches, name, grade);
            
            
                show_menu(flag);
                break;
            }
        case 3:
            show_menu(flag);
            show_next_beach(top_beaches,flag);
            break;
        case 4:
            exit(EXIT_SUCCESS);
        default:
            printf("Invalid input\n");
            exit(EXIT_FAILURE);
            break;
        }
    }
    return 0;
}
