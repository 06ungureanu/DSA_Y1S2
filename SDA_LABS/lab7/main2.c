#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct bstNode {
    int val;
    struct bstNode *left;
    struct bstNode *right;
} bstNode;

typedef struct bstTree {
    bstNode *root;
    unsigned int height;
    unsigned int nodes;
} bstTree;

bstTree *createEmptyBstTree() {
    bstTree *temp = (bstTree*)malloc(sizeof(bstTree));
    if(temp == NULL) return temp;

    temp->height = 0;
    temp->nodes = 0;
    temp->root = NULL;

    return temp;
}
bstNode *createBstNode(int val) {
    bstNode *temp = (bstNode*)malloc(sizeof(bstNode));
    if(temp == NULL) return NULL;

    temp->val = val;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}   
int bstHeight(bstNode *root) {
    if(root == NULL) return -1;

    int leftHeight = bstHeight(root->left);
    int rightHeight = bstHeight(root->right);

    return 1 +  (leftHeight > rightHeight ? leftHeight : rightHeight);
}
int bstNodeCount(bstNode *root) {
    if(root == NULL) return 0;

    int leftNodes = bstNodeCount(root->left);
    int rightNodes = bstNodeCount(root->right);

    return 1 + leftNodes + rightNodes;
}
int bstLeafCout(bstNode *root) {
    if(root == NULL) return 0;

    if(root->left == NULL || root->right == NULL)
        return 1;

    return bstLeafCout(root->left) + bstLeafCout(root->right);
}
void insertNode(bstNode **root, int val) {
    if(*root == NULL)
    {
        *root = createBstNode(val);
        return;
    }

    if(val < (*root)->val) 
        return insertNode(&(*root)->left,val);
    else
        return insertNode(&(*root)->right,val);
}
void insert(bstTree *tree, int val) {
    if(tree == NULL) return;

    insertNode(&tree->root,val);
    tree->height = bstHeight(tree->root);
    tree->nodes = bstNodeCount(tree->root);
}
void constructBstTree(bstTree *tree, int *arr, int low, int high) {
    if(low > high) return;

    int mid = (low + high) / 2;
    insert(tree,arr[mid]);

    constructBstTree(tree,arr,low, mid - 1);
    constructBstTree(tree,arr,mid + 1, high);
}
void printInOrder(bstNode *root) {
    if(root == NULL) return;

    
    printInOrder(root->left);
    printf("%d ", root->val);
    printInOrder(root->right);
}
void printPreOrder(bstNode *root) {
    if(root == NULL) return;

    
    printf("%d ", root->val);
    printPreOrder(root->left);
    printPreOrder(root->right);
}
void printPostOrder(bstNode *root) {
    if(root == NULL) return ;

    

    printPostOrder(root->left);
    printPostOrder(root->right);
    printf("%d ", root->val);
}
bstNode *bstSearchNode(bstNode *root, int key) {
    if(root == NULL || root->val == key) return root;

    if(key < root->val) 
        return bstSearchNode(root->left,key);
    else
        return bstSearchNode(root->right,key);
}
void removeBstNode(bstNode *root, int key) {

}
void freeNodes(bstNode *root) {
    if(root == NULL) return;

    freeNodes(root->left);
    freeNodes(root->right);
    free(root);
}
void freeBst(bstTree *tree) {
    freeNodes(tree->root);
    free(tree);
}
void invertTree(bstNode **root) {
    if(*root == NULL) return;

    bstNode *aux = (*root)->left;
    (*root)->left = (*root)->right;
    (*root)->right = aux;

    invertTree(&(*root)->left);
    invertTree(&(*root)->right);
}

int isBinaryTree(bstNode *root,int min, int max) {
    if(root == NULL) return 1;

    if(root->val <= min || root->val >= max) return 0;

    return isBinaryTree(root->left,min,max) && isBinaryTree(root->right,min,max);
}
bstNode *getBSTLCA(bstNode *root, int key1, int key2) {
    if(root == NULL) return NULL;

    

    
    
    

    
    
 
    
    
    if(key1 < root->val && key2 < root->val) 
        return getBSTLCA(root->left,key1,key2);
    else if(key1 > root->val && key1 > root->val) 
        return getBSTLCA(root->right,key1,key2);

    return root;
}
int findDistanceFromRootToNode(bstNode *root, int key, int dist) {
    
    if(root == NULL) return -1;
    if(root->val == key) return dist;

    if(root->val < key) 
        return findDistanceFromRootToNode(root->right,key,dist + 1);
    else 
        return findDistanceFromRootToNode(root->left,key, dist + 1);
}
int findDistanceBetweenTwoNodes(bstNode *root, int key1,int key2) {
    bstNode *LCA = getBSTLCA(root,key1,key2);
    if(LCA == NULL) return -1; 

    int dist1 = findDistanceFromRootToNode(LCA,key1,0);
    int dist2 = findDistanceFromRootToNode(LCA,key2,0);

    return dist1 + dist2;
}
int main() {
    
    bstTree *tree = createEmptyBstTree();
    
    int arr[] = {1,2,3,4,5,6,7};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    constructBstTree(tree,arr,0,arr_size - 1);
    
    invertTree(&tree->root);
    invertTree(&tree->root);

    printf("Arborele afisat inOrder : ");
    printInOrder(tree->root);
    printf("\n");

    printf("Arborele afisat preOrder : ");
    printPreOrder(tree->root);
    printf("\n");

    printf("Arborele afisat postOrder : ");
    printPostOrder(tree->root);
    printf("\n");

    int key = 5;
    bstNode *searched = bstSearchNode(tree->root,key);
    if(searched) 
        printf("Nodul cu cheia %d a fost gasit in arbore\n", key);
    else
        printf("Nodul cu cheia %d nu a fost gasit in arbore\n", key);

    printf("Inaltimea arborului este : %d\n", tree->height);
    printf("Arborele are %d noduri\n",tree->nodes);
    
    int leafes = bstLeafCout(tree->root);
    printf("Arborele are %d frunze\n", leafes);
    
    
    if(isBinaryTree(tree->root,INT_MIN, INT_MAX))
        printf("Arborele este unul binar de cautare\n");
    else 
        printf("Arborele nu este unul binar de cautare\n");
    
    bstNode* LCA = getBSTLCA(tree->root,2,4);
    printf("Cel mai mic stramos pt 2 si 4 este : %d\n", LCA->val);
    
    int dist = findDistanceBetweenTwoNodes(tree->root,2,7);
    printf("Distanta dintre nodurile cu cheiile 7 si 2 este : %d\n", dist);
    
    freeBst(tree);
    return 0;
}
