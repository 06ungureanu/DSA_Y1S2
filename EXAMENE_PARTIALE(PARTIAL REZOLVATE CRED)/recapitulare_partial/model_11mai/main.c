#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bstNode {
    char *key;
    struct bstNode *left;
    struct bstNode *right;
} bstNode;

typedef struct listNode {
    char *key;
    struct listNode *prev;
    struct listNode *next;
} listNode;

typedef struct CircularList {
    listNode *head;
    listNode *tail;
} CircularList;

bstNode *new_bst_node(const char *key) {
    if(!key) {
        printf("E: Invalid key input\n");
        return NULL;
    }

    bstNode *temp = (bstNode*)malloc(sizeof(bstNode));
    if(!temp) return NULL;

    temp->key = strdup(key);
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}
listNode *new_list_node(const char *key) {
    if(!key) {
        printf("E : Invalid key input\n");
        return NULL;
    }

    listNode *temp = (listNode*)malloc(sizeof(listNode));
    if(!temp) return NULL;

    temp->key = strdup(key);
    temp->next = NULL;
    temp->prev = NULL;
    
    return temp;
}
CircularList *createEmptyCircularList() {
    CircularList *l = (CircularList*)malloc(sizeof(CircularList));
    if(!l) {
        printf("Error while alocating memory for a circular linked list\n");
        return NULL;
    }

    l->head = NULL;
    l->tail = NULL;

    return l;
}
bstNode *insert_bst_key(bstNode *root, const char *key) {
    if(!key) return NULL;

    if(root == NULL) return new_bst_node(key);

    if(strcmp(key,root->key) < 0)
        root->left = insert_bst_key(root->left, key);
    else if(strcmp(key,root->key) > 0)
        root->right = insert_bst_key(root->right, key);

    return root;
}
bstNode *construct_bst(char **arr, int low, int high) {
    if(low > high) return NULL;

    int mid = (low + high) / 2;
    bstNode *root = new_bst_node(arr[mid]);

    root->left = construct_bst(arr, low, mid - 1);
    root->right = construct_bst(arr, mid + 1, high );

    return root;
}
void insert_to_list(CircularList *l, const char *key) {
    if(!l || !key) return;

    listNode *temp = new_list_node(key);
    if(!temp) return;

    if(l->head == NULL)
    {
        
        l->head = temp;
        l->tail = temp;
        
        temp->prev = temp;
        temp->next = temp;
    }
    else 
    {
        
        temp->next = l->head;
        temp->prev = l->tail;

        l->tail->next = temp;
        l->head->prev = temp;

        l->tail = temp;
    }
}
void buildListFromTree(bstNode *root, CircularList *l) {
    if(!l || !root) return;

    buildListFromTree(root->left, l);
    insert_to_list(l,root->key);
    buildListFromTree(root->right, l);

}
void printList(CircularList *l) {
    if(!l || !l->head) return;
    
    printf("[ ");
    listNode *curr = l->head;
    do {
        printf("%s, ", curr->key);
        curr = curr->next;
    }while (curr->next != l->head);

    printf("]\n");
}
void printListInward(CircularList *l) {
    if(!l || !l->head) return;

    printf("[ ");
    listNode *curr = l->tail;
    do {
        printf("%s ",curr->key);
        curr = curr->prev;
    }
    while(curr != l->tail);
    printf("]\n");
}
int main() {
    char *arr[] = {"Ana","Bianca","Cristina","Daniela","Elena","Florina"};
    int arr_size = 7;

    bstNode *root = construct_bst(arr, 0, arr_size - 1);
    CircularList *list = createEmptyCircularList();
    buildListFromTree(root,list);

    printList(list);
    printListInward(list);

    return 0;
}
