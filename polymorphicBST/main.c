#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#define EPSILON 1e-9

enum FLAGS {
    I_FLAG =            0x1,
    F_FLAG =            0x2,
    S_FLAG =            0x4
};

typedef union Val {
    int                 iVal;
    double              fVal;
    char                *sVal;
} Val;

typedef struct bstNode {
    Val key;
    struct bstNode      *left;
    struct bstNode      *right;
} bstNode;

typedef struct bstTree {
    bstNode             *root;
    int                 flag;
    int                 height;
    int                 nodes;
} bstTree;


static inline int *convertArrToInt(void *arr) {
    return (int*)arr;
}
static inline double *convertArrToFloat(void *arr) {
    return (double*)arr;
}
static inline char **convertArrToStrings(void *arr)
{
    return (char**)arr;
}
static inline int flagBSTCheck(int FLAG) {
    return FLAG != I_FLAG && FLAG != F_FLAG && FLAG != S_FLAG;
}

bstTree *createEmptyBstTree(int FLAG) {
    if(FLAG != I_FLAG && FLAG != F_FLAG && FLAG != S_FLAG)
    {
        printf("Error while creating a new bst : Unsuported data type for binar search tree\n");
        return NULL;
    }
    bstTree *temp = (bstTree*)malloc(sizeof(bstTree));
    if(temp == NULL) return NULL;

    temp->root = NULL;
    temp->height = 0;
    temp->nodes = 0;
    temp->flag = FLAG;

    return temp;
}
bstNode *createNode(int FLAG, va_list args) {
    bstNode *temp = (bstNode*)malloc(sizeof(bstNode));
    if(temp == NULL) return NULL;

    temp->left = NULL;
    temp->right = NULL;

    switch (FLAG) {
        case I_FLAG: {
            temp->key.iVal = va_arg(args,int);
            break;
        }
        case F_FLAG: {
            temp->key.fVal = va_arg(args, double);
            break;
        }
        case S_FLAG: {
            char *str = va_arg(args, char*);
            if(str != NULL)
                temp->key.sVal = strdup(str);
            else
                temp->key.sVal = NULL;
            break;
        }
        default: {
            printf("Error while creating a new node : Unsuported data type for binary search tree\n");
            return NULL;
        }
    }
    return temp;
}
void insertNode(bstNode **root, int FLAG, va_list args) {
    va_list temp_args;
    va_copy(temp_args,args);

    if(*root == NULL) {
        *root = createNode(FLAG,args);
        return;
    }

    switch (FLAG) {
        case I_FLAG: {
            int iKey = va_arg(args,int);
            if(iKey < (*root)->key.iVal)
                return insertNode(&(*root)->left,FLAG,temp_args);
            else 
                return insertNode(&(*root)->right,FLAG, temp_args);
        }
        case F_FLAG: {
            double fVal = va_arg(args,double);
            if(fVal < (*root)->key.fVal)
                return insertNode(&(*root)->left,FLAG,temp_args);
            else 
                return insertNode(&(*root)->right,FLAG, temp_args);
        }
        case S_FLAG: {
            char* sVal = va_arg(args,char*);
            if(strcmp(sVal,(*root)->key.sVal) < 0)
                return insertNode(&(*root)->left,FLAG,temp_args);
            else 
                return insertNode(&(*root)->right,FLAG, temp_args);
            break;
        }
        default: {
            printf("Error while inserting a new node : Unsuported data type for binary search tree\n");
            return;
        }
    }
}
int getBstHeight(bstNode *root) {
    if(root == NULL) return -1;

    int LeftHeight = getBstHeight(root->left);
    int RightHeight = getBstHeight(root->right);

    return 1 + (LeftHeight > RightHeight ? LeftHeight : RightHeight);
}
int countBstLeafs(bstNode *root) {
    if(root == NULL) return 0;

    if(root->right == NULL || root->left == NULL)
        return 1;

    return countBstLeafs(root->left) + countBstLeafs(root->right);
}
int checkBstTree(bstNode *root,int FLAG, int min, int max) {
    if(root == NULL) return 1;

    switch (FLAG) {
        case I_FLAG: {
            if(root->key.iVal < min || root->key.iVal > max) return 0;
            break;
        }
        case F_FLAG: {
            if(root->key.fVal < min || root->key.fVal > max) return 0;
            break;
        }
        case S_FLAG: {
            if (root->left && strcmp(root->key.sVal, root->left->key.sVal) <= 0) return 0;
            if (root->right && strcmp(root->key.sVal, root->right->key.sVal) >= 0) return 0;
            break;
        }
        default:
            return 0; 
    }
    
    return checkBstTree(root->left,FLAG,min,max) && checkBstTree(root->right,FLAG,min,max);
}
void insertNodeInBST(bstTree* tree, int FLAG, ...) {
    if(tree == NULL) return;
    
    va_list args;
    va_start(args,FLAG);

    insertNode(&tree->root,FLAG,args);
    tree->nodes++;
    tree->height = getBstHeight(tree->root);

    va_end(args);
}
void constructBstTree(bstTree *tree, int FLAG,int low, int high, void *arr) {
    if(tree == NULL || tree->flag != FLAG ||low > high) return;
         
    int mid = (low + high) / 2;
   
    switch(FLAG)
    {
        case I_FLAG: {
            int *intArr = convertArrToInt(arr);
            insertNodeInBST(tree,I_FLAG,intArr[mid]);
            constructBstTree(tree,I_FLAG,low, mid - 1, intArr);
            constructBstTree(tree,I_FLAG,mid + 1, high, intArr);
            break;
        }
        case F_FLAG: {
            double *floatArr = convertArrToFloat(arr);
            insertNodeInBST(tree,F_FLAG,floatArr[mid]);
            constructBstTree(tree,F_FLAG,low, mid - 1, floatArr);
            constructBstTree(tree,F_FLAG,mid + 1, high, floatArr);
            break;
        }
        case S_FLAG: {
            char **strArr = convertArrToStrings(arr);
            insertNodeInBST(tree,S_FLAG,strArr[mid]);
            constructBstTree(tree,S_FLAG,low, mid - 1, strArr);
            constructBstTree(tree,S_FLAG,mid + 1, high, strArr);
            break;
        }
        default: {
            printf("Error while construncting binary search tree : invalid data type\n");
            return;
        }
    }
}
void printInOrder(bstNode *root, int FLAG) {
    if(root == NULL) return;

    printInOrder(root->left,FLAG);
    switch (FLAG) {
        case I_FLAG: {
            printf("%d ", root->key.iVal);
            break;
        }
        case F_FLAG: {
            printf("%.2f ", root->key.fVal);
            break;
        }
        case S_FLAG: {
            printf("%s ", root->key.sVal);
            break;
        }
        default: {
            printf("Unsuported data type for binary search tree\n");
            return;
        }
    }
    printInOrder(root->right,FLAG);
}
void printInPreOrder(bstNode *root, int FLAG) {
    if(root == NULL) return;

    switch (FLAG) {
        case I_FLAG: {
            printf("%d ", root->key.iVal);
            break;
        }
        case F_FLAG: {
            printf("%.2f ", root->key.fVal);
            break;
        }
        case S_FLAG: {
            printf("%s ", root->key.sVal);
            break;
        }
        default: {
            printf("Unsuported data type for binary search tree\n");
            return;
        }
    }
    printInPreOrder(root->left,FLAG);
    printInPreOrder(root->right,FLAG);
}
void printInPostOrder(bstNode *root, int FLAG) {
    if(root == NULL) return;

    printInPostOrder(root->right,FLAG);
    printInPostOrder(root->left,FLAG);
    switch (FLAG) {
        case I_FLAG: {
            printf("%d ", root->key.iVal);
            break;
        }
        case F_FLAG: {
            printf("%.2f ", root->key.fVal);
            break;
        }
        case S_FLAG: {
            printf("%s ", root->key.sVal);
            break;
        }
        default: {
            printf("Unsuported data type for binary search tree\n");
            return;
        }
    }
}
void DisplayAllKindOfPrints(bstTree *tree) {
    if(tree == NULL) {
        printf("Error while printing bst: Tree is empty\n");
        return;
    }
    int TREE_FLAG = tree->flag;
    printf("Arborele cu flagul 0x%x afisat in cele 3 tipuri (0x1 - INT, 0x2 - FLOAT, 0x3 - STR): \n",TREE_FLAG);
    
    printf("In Order: ");
    printInOrder(tree->root,TREE_FLAG);
    printf("\n");

    printf("Pre Order: ");
    printInPreOrder(tree->root,TREE_FLAG);
    printf("\n");

    printf("Post Order: ");
    printInPostOrder(tree->root,TREE_FLAG);
    printf("\n\n");
}
void DisplayBstInfo(bstTree *tree) {
    if(tree == NULL) {
        printf("Error while showing bst info : Tree is empty\n");
        return;
    }

    int FLAG = tree->flag;
    int isBst = 0;
    int leafes = countBstLeafs(tree->root);

    switch(FLAG) {
        case I_FLAG: {
            printf("Data type : INT\n");
            if(checkBstTree(tree->root,I_FLAG,INT_MIN,INT_MAX)) isBst |= 1;
            break;
        }
        case F_FLAG: {
            printf("Data type : FlOAT\n");
            if(checkBstTree(tree->root,F_FLAG,FLT_MIN,FLT_MAX)) isBst |= 1; 
            break;
        }
        case S_FLAG: {
            printf("Data type : STRING\n");
            if(checkBstTree(tree->root, S_FLAG,'\0','\0')) isBst |= 1;
            break;
        }
        default: 
            break;
    }
    printf("Height : %d\n", tree->height);
    printf("Nodes : %d\n", tree->nodes);
    printf("Leafes : %d\n", leafes);
    printf("Binary search tree : ");
    isBst ? printf("Yes\n") : printf("No\n");
    printf("\n");
}
bstNode *lookForNodes(bstNode *root, int FLAG, va_list args) {
    if(root == NULL) return NULL;

    va_list temp_args;
    va_copy(temp_args,args);

    switch (FLAG) {
        case I_FLAG: {
            int iVal = va_arg(args,int);
            if(root->key.iVal == iVal) 
                return root;
            if(root->key.iVal < iVal)
                return lookForNodes(root->right,FLAG,temp_args);
            else 
                return lookForNodes(root->left,FLAG,temp_args);
        }
        case F_FLAG: {
            double fVal = va_arg(args,double);
            if(fabs(root->key.fVal - fVal) < EPSILON) 
                return root;
            if(root->key.fVal <fVal)
                return lookForNodes(root->right,FLAG,temp_args);
            else 
                return lookForNodes(root->left,FLAG,temp_args);
        }
        case S_FLAG: {
            char* sVal = va_arg(args,char*);
            if(strcmp(root->key.sVal,sVal) == 0)  
                return root;
            if(strcmp(root->key.sVal,sVal) < 0)
                return lookForNodes(root->right,FLAG,temp_args);
            else 
                return lookForNodes(root->left,FLAG,temp_args);
        }
        default: {
            return NULL;
        }
    }

    va_end(temp_args);
}
bstNode *searchNodeInBst(bstTree *tree, int FLAG, ...) {
   
    if(flagBSTCheck(FLAG)) {
        printf("Error while searching node in binary search tree : Unsuported data type\n");
        return NULL;
    }
 
    if(tree == NULL || tree->root == NULL) return NULL;

    va_list args;
    va_start(args,FLAG);
    
    bstNode *searchcedNode = lookForNodes(tree->root,FLAG,args);

    va_end(args);
    return searchcedNode;
}
void freeBSTNode(bstNode *root, int FLAG){
    if (root == NULL) return;

    freeBSTNode(root->right,FLAG);
    freeBSTNode(root->left, FLAG);
    
    if((FLAG & S_FLAG) && root->key.sVal != NULL) {
        free(root->key.sVal);
        root->key.sVal = NULL;
    } 
    free(root);
}
void bstFree(bstTree *tree) {
    if(tree == NULL) return;
    freeBSTNode(tree->root,tree->flag);
    free(tree);
}
static inline void swapNodes(bstNode **node_1, bstNode **node_2)
{
    if(*node_1 == NULL || *node_2 == NULL) return;

    bstNode *aux = *node_1;
    *node_1 = *node_2;
    *node_2 = aux;
}
void invertBST(bstNode **root) {
    if(*root == NULL) return;

    swapNodes(&(*root)->left, &(*root)->right);
    
    invertBST(&(*root)->left);
    invertBST(&(*root)->right);
}

bstNode *searchBstLCA(bstNode *root, int FLAG, va_list args) {
    if (root == NULL) {
        return NULL;
    }

    switch(FLAG) {
        case I_FLAG: {
            int key_1 = va_arg(args, int);
            int key_2 = va_arg(args, int);
            
            if ((key_1 < root->key.iVal && key_2 < root->key.iVal)) {
                return searchBstLCA(root->left, FLAG, args);
            }
            if (key_1 > root->key.iVal && key_2 > root->key.iVal) {
                return searchBstLCA(root->right, FLAG, args);
            }
            return root;
        }
        case F_FLAG: {
            double key_1 = va_arg(args, double);
            double key_2 = va_arg(args, double);
            
            if (key_1 < root->key.fVal && key_2 < root->key.fVal) {
                return searchBstLCA(root->left, FLAG, args);
            }
            if (key_1 > root->key.fVal && key_2 > root->key.fVal) {
                return searchBstLCA(root->right, FLAG, args);
            }
            return root;
        }
        case S_FLAG: {
            char *key_1 = va_arg(args, char*);
            char *key_2 = va_arg(args, char*);
            
            if (strcmp(key_1, root->key.sVal) < 0 && strcmp(key_2, root->key.sVal) < 0) {
                return searchBstLCA(root->left, FLAG, args);  
            } 
            if (strcmp(key_1, root->key.sVal) > 0 && strcmp(key_2, root->key.sVal) > 0) {
                return searchBstLCA(root->right, FLAG, args);  
            }
            return root;
        }
        default: {
            va_end(args);
            return NULL;
        }
    }
}
bstNode *getBstLCA(bstTree *tree, int FLAG, ...) {
    if(tree == NULL || flagBSTCheck(FLAG)) return NULL;

    va_list args;
    va_start(args,FLAG);

    bstNode *LCA = searchBstLCA(tree->root,FLAG,args);
    va_end(args);

    return LCA;
}
int getDistanceFromRootToNode(bstNode *root, int FLAG,int dist, va_list args) {
    va_list temp_args;
    va_copy(temp_args,args);

    switch (FLAG) {
        case I_FLAG: {
            int iVal = va_arg(args,int);
            if(iVal == root->key.iVal) return dist;
            if(iVal < root->key.iVal)
                return getDistanceFromRootToNode(root->left,FLAG,dist + 1,temp_args);
            else 
                return getDistanceFromRootToNode(root->right,FLAG,dist + 1,temp_args);
        }
        case F_FLAG: {
            double fVal = va_arg(args,double);
            if(fVal == root->key.fVal) return dist;
            if(fVal < root->key.fVal)
                return getDistanceFromRootToNode(root->left,FLAG,dist + 1, temp_args);
            else
                return getDistanceFromRootToNode(root->right,FLAG,dist + 1, temp_args);
        }
        case S_FLAG: {
            char *sVal = va_arg(args,char*);
            if(strcmp(sVal,root->key.sVal) == 0) return dist;
            if(strcmp(sVal,root->key.sVal) < 0)
               return getDistanceFromRootToNode(root->left,FLAG,dist + 1, temp_args);
            else
                return getDistanceFromRootToNode(root->right,FLAG,dist + 1, temp_args);
        }
        default: {
            va_end(temp_args);
            return -1;
        }
    }
    return -1;
}
int getDistanceBetweenTwoNodesBst(bstTree *tree, int FLAG, ...)
{
    if(tree == NULL || tree->root == NULL) return 0;
    
    va_list args;
    va_start(args,FLAG);

    int dist_key_1 = getDistanceFromRootToNode(tree->root,tree->flag,0,args);
    int dist_key_2 = getDistanceFromRootToNode(tree->root,tree->flag,0,args);

    va_end(args);
    return dist_key_1 + dist_key_2;
}
int main() {
    bstTree *intTree = createEmptyBstTree(I_FLAG);
    bstTree *floatTree = createEmptyBstTree(F_FLAG);
    bstTree *strTree = createEmptyBstTree(S_FLAG);

    int arr[] = {1,2,3,4,5,6,7};
    double arrF[] = {1.1,2.2,3.3,4.4,5.5,6.6,7.7};
    char *arrS[] = { "Ana","Alexia","Cristina","Marilena","Mihaela","Mirela","Miruna"};

    int int_arr_size = sizeof(arr) / sizeof(arr[0]);
    int float_arr_size = sizeof(arrF) / sizeof(arrF[0]);
    int str_arr_size = sizeof(arrS) / sizeof(arrS[0]);

    constructBstTree(intTree,I_FLAG,0,int_arr_size - 1, (void*)arr);
    constructBstTree(floatTree,F_FLAG,0,float_arr_size - 1,(void*)arrF);
    constructBstTree(strTree,S_FLAG,0,str_arr_size - 1, (void*)arrS);

    DisplayAllKindOfPrints(intTree);
    DisplayAllKindOfPrints(floatTree);
    DisplayAllKindOfPrints(strTree);

    DisplayBstInfo(intTree);
    DisplayBstInfo(floatTree);
    DisplayBstInfo(strTree);

    int iKey = 5;
    double fKey = 2.2;
    char *sKey = "Miruna";

    bstNode *searchedInt = searchNodeInBst(intTree,I_FLAG,iKey);
    bstNode *searchedFloat = searchNodeInBst(floatTree,F_FLAG,fKey);
    bstNode *seachedStr = searchNodeInBst(strTree,S_FLAG,sKey);

    searchedInt ? printf("Node with key %d exist in BST\n",iKey) : printf("Node with key %d don't exist in BST\n",iKey); 
    searchedFloat ? printf("Node with key %.2f exist in BST\n",fKey) : printf("Node with key %.2f don't exist in BST\n",fKey); 
    seachedStr ? printf("Node with key %s exist in BST\n",sKey) : printf("Node with key %s don't exist in BST\n",sKey);

    int key_1 = 1;
    int key_2 = 5;

    double fKey_1 = 1.1;
    double fKey_2 = 5.5;

    const char sKey_1[] = "Miruna";
    const char sKey_2[] = "Alexia";

    bstNode *LCA = getBstLCA(intTree,I_FLAG,key_1,key_2);
    bstNode *fLCA = getBstLCA(floatTree,F_FLAG,fKey_1,fKey_2);
    bstNode *sLCA = getBstLCA(strTree,S_FLAG,sKey_1,sKey_2);
    
    printf("\nLCA between %d and %d : %d\n",key_1,key_2,LCA->key.iVal);
    printf("LCA between %.1f and %.1f : %.1f\n",fKey_1,fKey_2,fLCA->key.fVal);
    printf("LCA between %s and %s : %s\n",sKey_1,sKey_2,sLCA->key.sVal);

    int iDist = getDistanceBetweenTwoNodesBst(intTree,intTree->flag,key_1,key_2);
    int fDist = getDistanceBetweenTwoNodesBst(floatTree,floatTree->flag,fKey_1,fKey_2);
    int sDist = getDistanceBetweenTwoNodesBst(strTree,S_FLAG,sKey_1,sKey_2);

    printf("\nDistance between %d and %d : %d\n",key_1,key_2,iDist);
    printf("Distance between %.1f and %.1f : %d\n", fKey_1,fKey_2,fDist);
    printf("Distance between %s and %s : %d\n", sKey_1,sKey_2,sDist);

    bstFree(intTree);
    bstFree(floatTree);
    bstFree(strTree);

    return 0;
}
