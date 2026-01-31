#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BstNode {
    char            *key;
    struct BstNode  *left;
    struct BstNode  *right;
} BstNode;

typedef struct ListNode {
    char            *key;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

static inline int _cmp_key(BstNode *node, const char *key) {
    return strcmp(key,node->key);
}
BstNode *create_new_bst_node(const char *key) {
    if(!key) return NULL;

    BstNode *tmp = (BstNode*)malloc(sizeof(BstNode));
    if(!tmp) return NULL;

    tmp->key = strdup(key);
    tmp->left = NULL;
    tmp->right = NULL;
    
    return tmp;
}
ListNode *crete_new_list_node(const char *key) {
    if(!key) return NULL;

    ListNode *temp = (ListNode*)malloc(sizeof(ListNode));
    if(!temp) return NULL;

    temp->key = strdup(key);
    temp->prev = NULL;
    temp->next = NULL;

    return temp;
}
void insert_node_bst(BstNode **root, const char *key) {
    if(*root == NULL) {
        *root = create_new_bst_node(key);
        return;
    }

    if(_cmp_key(*root,key) < 0)
        insert_node_bst(&(*root)->left,key);
    else if(_cmp_key(*root,key) > 0)
        insert_node_bst(&(*root)->right,key);
    else return;
}
void insert_node_list(ListNode **head, const char *key) {
    ListNode *tmp = (ListNode*)malloc(sizeof(ListNode));
    
    if(*head == NULL) 
    {
        *head = tmp;
        
        tmp->prev = *head;
        tmp->next = *head;
        return;
    }
    else 
    {
        tmp->next = tmp;
        tmp->prev = tmp;

        (*head)->prev->next = tmp;
        (*head)->prev = tmp;

        *head = tmp;
    }
}
void construct_bst(BstNode **root, char **arr, int low, int high) {
    if(low > high) return;

    int mid = (low + high) / 2;
    insert_node_bst(root,arr[mid]);
    
    construct_bst(root,arr,low, mid - 1);
    construct_bst(root,arr,mid + 1, high);
}
void construct_list(BstNode *root, ListNode **l) {
    if(root == NULL) return;

    construct_list(root->left,l);
    insert_node_list(l,root->key);
    construct_list(root->right,l);
}
void print_list(ListNode *head) {
    if(head == NULL) return;

    printf("[ ");
    ListNode *curr = head;
    do
    {
        printf("%s ",curr->key);
        curr = curr->next;
    } while (curr != head);
    printf("]\n");
}
void print_list_inward(ListNode *head) {
    if(head == NULL) return;

    printf("[ ");
    ListNode *curr = head->prev;
    do
    {
        printf("%s ", curr->key);
        curr = curr->prev;
    } while (curr != head->prev);
    printf("]\n");
}
int main() {
    char *arr[] = {"Ana","Bianca","Camelia","Daniela","Elena","Felicia","Georgiana"};
    int arr_size = sizeof(arr) / sizeof(arr[0]); 

    BstNode *root = NULL;
    ListNode *l = NULL;

    construct_bst(&root,arr,0,arr_size - 1);
    construct_list(root,&l);

    print_list(l);
    print_list_inward(l);

    return 0;
}