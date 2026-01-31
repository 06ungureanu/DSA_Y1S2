#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

static inline int max(int a, int b) {
    return (a > b) ? a : b;
}
static inline int height(Node *n) {
    return (n == NULL) ? 0 : n->height;
}
static inline int getBalanceFactor(Node *n) {
    return (n == NULL) ? 0 : height(n->left) - height(n->right);
}
void updateHeight(Node *n) {
    if(n == NULL) return;

    n->height = 1 + max(height(n->left),height(n->right));
}
void leftRotate(Node **y) {
    if(y == NULL || *y == NULL) return;

}
void rightRotate(Node **y) {
    if(y == NULL || *y == NULL) return;
}
Node *createNode(int key) {
    Node *temp = (Node*)malloc(sizeof(Node));
    if(temp == NULL) return NULL;

    temp->key = key;
    temp->height = 1;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}
void insert(Node **root, int key) {
    if(*root == NULL) {
        *root = createNode(key);
        return;
    }

    if(key < (*root)->key)
        insert(&(*root)->left,key);
    else if(key > (*root)->key) 
        insert(&(*root)->right,key);
    else return; 

    updateHeight(*root);
    int balance = getBalanceFactor(*root);

    if(balance > 1 && key < (*root)->left->key)
        rightRotate(root);
}

int main() {

    Node *avlTree = NULL;
    return 0;
}
