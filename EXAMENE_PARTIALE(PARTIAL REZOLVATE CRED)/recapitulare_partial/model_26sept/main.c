#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct ListNode {
    int key;
    struct ListNode *next;
} ListNode;

Node *createNode(int key) {
    Node *temp = (Node*)malloc(sizeof(Node));
    if(!temp) {
        printf("Error while alocating memory for a new node\n");
        return NULL;
    }

    temp->key = key;
    temp->right = NULL;
    temp->left = NULL;

    return temp;

}
ListNode *createListNode(int key) {
    ListNode *temp = (ListNode*)malloc(sizeof(ListNode));
    if(!temp) {
        printf("Error while alocating memory for a new node\n");
        return NULL;
    }

    temp->key = key;
    temp->next = NULL;

    return temp;
}
void insertKey(Node **root, int key) {
    if(root == NULL) return;

    if(*root == NULL) {
        *root = createNode(key);
        return;
    }
    
    
    if(key < (*root)->key) {
        insertKey(&(*root)->left, key);
    }
    else if(key > (*root)->key) {
        insertKey(&(*root)->right, key);
    } else return;
}
void constructBstTree(Node **root, int *arr, int low, int high) {
    if(low > high) return;

    int mid = (low + high) / 2;
    insertKey(root, arr[mid]);

    constructBstTree(root,arr, low, mid - 1);
    constructBstTree(root, arr, mid + 1, high);
}
void inOrderTraversal(Node *root) {
    if(root == NULL) return;

    inOrderTraversal(root->left);
    printf("%d ", root->key);
    inOrderTraversal(root->right);
}
int subTreeSum(Node *root, int *nr_elements) {
    if(root == NULL) return 0;

    int leftSum = subTreeSum(root->left, nr_elements);
    int rightSum = subTreeSum(root->right, nr_elements);
    
    (*nr_elements)++;
    return root->key + leftSum + rightSum;
}

int getAvr(Node *root) {
    if(root == NULL) return 0;

    int nodes = 0;
    int sum = subTreeSum(root, &nodes);

    return sum / nodes;
}
bool cycleCheck(ListNode *l) {
    if(l == NULL) return false;
    
    ListNode *slow = l; 
    ListNode *fast = l->next;     

    while(fast && fast->next) {
        if(slow == fast) 
            return true;
        slow = slow->next;
        fast = fast->next->next;
    }

    return false;
}
void push(ListNode **top, int key) {
    if(top == NULL) return;

    ListNode *temp = createListNode(key);
    if(!temp) return;

    temp->next = *top;
    *top = temp;
}
int pop(ListNode **top) {
    if(!top || !(*top)) return -1;

    ListNode *temp = *top;
    int key = temp->key;
    *top = (*top)->next;
    free(temp);

    return key;
}
void freeStack(ListNode *tos) {
    if(tos == NULL) return;

    ListNode *curr = tos;
    while(curr) {
        ListNode *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}
void printStackUpOrder(ListNode *TOS) {
    if(TOS == NULL) {
        return;
    }

    printStackUpOrder(TOS->next);
    printf("%d ", TOS->key);
}
void printStackOutOrder(ListNode *TOS) {
    if(TOS == NULL) {
        return;
    } 

    printf("%d ", TOS->key);
    printStackOutOrder(TOS->next);
} 
void copyStack(ListNode *src, ListNode **dest) {
    if(src == NULL) return;

    ListNode *temp = src;
    while(temp) {
        push(dest,temp->key);
        temp = temp->next;
    }
}
bool checkPalindrome(ListNode *tos) {
    if(tos == NULL) return false;
    
    ListNode *aux = NULL;
    copyStack(tos, &aux);

    while(tos && aux) {
        if(pop(&tos) != pop(&aux)) 
            return false;
    }
    return true;

}
void freeBst(Node *root) {
    if(root == NULL) return;

    freeBst(root->left);
    freeBst(root->right);
    free(root);
}
int main() {
    int arr[] = {1,2,3,4,5,6,7};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    Node *root = NULL;
    ListNode *l = NULL;
    ListNode *TOS = NULL;     

    constructBstTree(&root,arr, 0, arr_size - 1);
    
    push(&TOS, 1);
    push(&TOS, 2);
    push(&TOS, 3);
    
    freeBst(root);
    freeStack(TOS);
    return 0;
}
