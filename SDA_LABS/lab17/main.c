#include <stdio.h>
#include <stdlib.h>

typedef struct avlNode {
    int     key;
    struct avlNode *left;
    struct avlNode *right;
} avlNode;

typedef struct Student {
    char *name;
    int grade;
} Student;


void swap_studs(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

int partition_studs(Student studs[], int low, int high) {
    int pivot = studs[high].grade;
    int i = low;

    for(int j = low; j < high; j++) {
        if(studs[j].grade >= pivot) {
            
            
            swap_studs(&studs[i], &studs[j]);
            i++;
        }
    }
    swap_studs(&studs[i], &studs[high]);

    return i;
}
Student quickselect_stud(Student studs[],int low,int high, int k) {
    if(low == high)
        return studs[high];
    
    int pivot_index = partition_studs(studs,low,high);
    if(k == pivot_index)
        return studs[k];    
    if(k < pivot_index)
        return quickselect_stud(studs,low,pivot_index - 1,k);
    else 
        return quickselect_stud(studs, pivot_index + 1, high, k);
}


avlNode *create_new_avl_node(int key) {
    avlNode *temp = (avlNode*)malloc(sizeof(avlNode));

    temp->key = key;
    temp->right = NULL;
    temp->left = NULL;

    return temp;
}
void insert_node_avl(avlNode **root, int key) {
    if(*root == NULL) {
        *root = create_new_avl_node(key);
        return;
    }

    if(key < (*root)->key)
        insert_node_avl(&(*root)->left,key);
    if(key > (*root)->key)
        insert_node_avl(&(*root)->right,key);
    else return;
}
void inOrder_traversal(avlNode *root) {
    if(root == NULL) return;

    inOrder_traversal(root->left);
    printf("%d ", root->key);
    inOrder_traversal(root->right);
}
void construct_bst_from_arr(avlNode **root, int *arr, int low, int high) {
    if(low > high) return;

    int mid = (low + high) / 2;
    insert_node_avl(root,arr[mid]);


    construct_bst_from_arr(root, arr,low, mid - 1);
    construct_bst_from_arr(root, arr, mid + 1, high);
}


int good_index(int *v, int low, int high) {
    if(low > high) 
        return 0;
    int mid = (low + high) / 2;
    if(v[mid] == mid)
        return 1;
    if(v[mid] > mid)
        return good_index(v, low, mid - 1);
    else 
        return good_index(v, mid + 1, high);
}


int boyer_moore(int v[], int n) {
    int count = 0, candidate = -1;

    for (int i = 0; i < n; i++) {
        if (count == 0) {
            candidate = v[i];
            count = 1;
        } else if (v[i] == candidate) {
            count++;
        } else {
            count--;
        }
    }

    
    count = 0;
    for (int i = 0; i < n; i++) {
        if (v[i] == candidate)
            count++;
    }

    return (count > n / 2) ? candidate : -1;
}
int main() {
    int arr[] = {1,2,3,4,5,6,7};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    Student studs[] = {{"Alex",9}, {"Mirela", 7}, {"Ioana",5}, {"Liliana", 2}};
    int s_size = sizeof(studs) / sizeof(studs[0]);

    int v[] = {2,1,7,6,4,3,5};
    int v_size = sizeof(v) / sizeof(v[0]);

    int majoritar[] = {3, 3, 4, 2, 3, 3, 3};
    int m_size = sizeof(majoritar) / sizeof(majoritar[0]);

    int k = 1; 

    avlNode *root = NULL;
    construct_bst_from_arr(&root,arr,0, arr_size - 1);
    inOrder_traversal(root);
    printf("\n");

    Student kth = quickselect_stud(studs,0, s_size - 1, k);
    printf("Al %d student ca nota este %s\n", k + 1, kth.name);

    if(good_index(v,0,v_size - 1))
        printf("Vectorul este frumos indexat\n");
    else   
        printf("Vectorul nu este frumos indexat\n");

    int maj = boyer_moore(majoritar,m_size);
    printf("Elementul majoritar este %d\n", maj);
    return 0;
}