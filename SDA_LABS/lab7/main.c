#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bstNode {
    int val;
    struct bstNode *parrent;
    struct bstNode *left;
    struct bstNode *right;
} bstNode;

typedef struct bstTree {
    bstNode *root;
    int height;
    int nodes;
} bstTree;

bstNode *createNode(int val) {
    bstNode *temp = (bstNode*)malloc(sizeof(bstNode));
    if(temp == NULL)
        return temp;

    temp->val =  val;
    temp->left = NULL;
    temp->right = NULL;
    
    return temp;
}
void insert(bstNode **root, int val) {
    if(*root == NULL) 
    {
        *root = createNode(val);
        return;
    }
    if((*root)->val == val) return; 

    if(val < (*root)->val)
        insert(&(*root)->left,val);
    else 
        insert(&(*root)->right,val);
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
    if(root == NULL) return;

    printPostOrder(root->left);
    printPostOrder(root->right);
    printf("%d ", root->val);
}
bstNode *bstSearchNode(bstNode *root, int key) {
    if(root == NULL || root->val == key) return root;

    if(root->val < key)
        return bstSearchNode(root->right,key);
    else 
        return bstSearchNode(root->left,key);
    
    return NULL;
}
int bstHeight(bstNode *root) {
    if(root == NULL) return -1;
    
    int HightLeft = bstHeight(root->left);
    int HighRight = bstHeight(root->right);

    return (HightLeft > HighRight ? HightLeft : HighRight) + 1;
}
int bstNodeCount(bstNode *root) {
    if(root == NULL)
        return 0;
    
    int leftNodes = bstNodeCount(root->left);
    int rightNodes = bstNodeCount(root->right);
    
    return 1 + leftNodes + rightNodes;
} 
int bstLeafCount(bstNode *root) {
    if(root == NULL) return 0;

    if(root->right == NULL || root->left == NULL) 
        return 1;
    return bstLeafCount(root->left) + bstLeafCount(root->right);
} 
void constructBST(bstNode **root, int *v, int low,int high) {
    if(low > high)
        return;

    int mid = (low + high) / 2;
    insert(root,v[mid]);

    constructBST(root, v, low, mid - 1);
    constructBST(root, v, mid + 1, high);
}
void bstFree(bstNode *root) {
    if(root == NULL) return;

    bstFree(root->left);
    bstFree(root->right);
    free(root);
}
void deteleNode(bstNode *root, int key) {
    if(root == NULL) return;

    bstNode *searched = bstSearchNode(root,key);
    if(searched == NULL) return;

    if(searched->left == NULL) {
        nodes *temp = root->right;
        free(setstate_r());
    }
}
int main() {

    bstNode *root = NULL;

    int v[] = {1,2,3,4,5,6,7};
    int arr_size = sizeof(v) / sizeof(v[0]);
    constructBST(&root,v,0,arr_size - 1);

    printf("Arborele afisat in order : ");
    printInOrder(root); printf("\n");
    printf("Arborele afisat in pre order : " );
    printPreOrder(root); printf("\n");
    printf("Arborele afisat post order : ");
    printPostOrder(root); printf("\n");
    
    bstNode *searched = bstSearchNode(root,5);

    if(searched) 
        printf("Nodul cu cheia %d a fost gasit in arbore\n", 5);
    else 
        printf("Nodul nu a fost gasit in lista\n");
   
    int height = bstHeight(root);
    printf("Inaltimea arborului este : %d\n", height);

    int nodes = bstNodeCount(root);
    printf("Arborele are %d noduri\n", nodes);
    
    int leafes = bstLeafCount(root);
    printf("Arborele are %d leaf uri\n", leafes);
    
    bstFree(root);
    return 0;
}
