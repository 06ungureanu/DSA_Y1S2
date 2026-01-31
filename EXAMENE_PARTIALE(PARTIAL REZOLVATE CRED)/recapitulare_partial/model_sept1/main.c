#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bstNode {
    char *key;
    struct bstNode *left;
    struct bstNode *right;
} bstNode;

typedef struct Node {
    char *key;
    struct Node *next;
} Node;

int countlines(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    int lines = 0;
    char ch;
    while((ch = fgetc(f)) != EOF) {
        if(ch == '\n') lines++;
    }

    fclose(f);
    return lines;
}
Node *createNode(const char *key) {
    Node *tmp = (Node*)malloc(sizeof(Node));
    if(!tmp) return NULL;

    tmp->key = strdup(key);
    tmp->next = NULL;

    return tmp;
}
bstNode *createBstNode(const char *key) {
    if(key == NULL) return NULL;

    bstNode *tmp = (bstNode*)malloc(sizeof(bstNode));
    if(!tmp) return NULL;

    tmp->key = strdup(key);
    tmp->left = NULL;
    tmp->right = NULL;
   
    return tmp;
}
void insertBstNode(bstNode **root, const char *key) {
    if(root == NULL || key == NULL) return;

    if(*root == NULL) {
        *root = createBstNode(key);
        return;
    }

    if(strcmp(key, (*root)->key) < 0) 
        insertBstNode(&(*root)->left, key);
    else if(strcmp(key,(*root)->key) > 0) 
        insertBstNode(&(*root)->right,key); 
    else return;
}
void constructBst(bstNode **root, char **arr, int low, int high) {
    if(low > high) return;

    int mid = (low + high) / 2;
    insertBstNode(root, arr[mid]);

    constructBst(root,arr,low, mid - 1);
    constructBst(root,arr,mid + 1, high);
}
char **retiveFileData(const char *filename, int *arr_size) {
    FILE *f = fopen(filename, "r");
    if(!f) return NULL;

    int lines = countlines(filename);
    char **words = (char**)malloc(lines * sizeof(char*));
    if(!words) return NULL;

    char buffer[256];
    int i = 0;
    while(fgets(buffer,256,f) && i < lines) {
        strtok(buffer,"\n");
        words[i] = strdup(buffer);
        i++;
    }

    *arr_size = lines;
    fclose(f);
    return words;
}
void inOrderTraversal(bstNode *root) {
    if(root == NULL) return;

    inOrderTraversal(root->left);
    printf("%s ", root->key);
    inOrderTraversal(root->right);
}

bstNode *getLowestCommonAncestor(bstNode *root,const char *key_1, const char *key_2) {
    if(!root || !key_1 || !key_2 ) return NULL;

    if(strcmp(key_1, root->key) < 0 && strcmp(key_2, root->key) < 0) 
        return getLowestCommonAncestor(root->left,key_1,key_2);
    else if(strcmp(key_1, root->key) > 0 && strcmp(key_2,root->key) > 0)
        return getLowestCommonAncestor(root->right, key_1, key_2);
    else return root;
}
void push(Node **top, const char *key) {
    if(!top || !key) return;

    Node *tmp = createNode(key);
    if(!tmp) return;

    tmp->next = *top;
    *top = tmp;
}
void getAvragePath(bstNode *root, int depth, int *sum, int *nodes) {
    if(root == NULL) return;

    *sum += depth;
    (*nodes)++;

    getAvragePath(root->left,depth + 1, sum,nodes);
    getAvragePath(root->right,depth + 1, sum, nodes);
}
double getAvragePathLenght(bstNode *root) {
    int nodes = 0;
    int sum = 0;

    getAvragePath(root,0,&sum, &nodes);

    return (double)sum / nodes;
}
char *pop(Node **top) {
    if(!top || !(*top)) return NULL;

    Node *tmp = *top;
    *top = (*top)->next;
    
    char *key = strdup(tmp->key);

    free(tmp->key);
    free(tmp);

    return key;
}
void buildStackLCABased(Node **top, bstNode *root, bstNode *LCA) {
    if(root == NULL) return;

    if(root == LCA) {
        push(top,LCA->key);
        return;
    }
    
    if(strcmp(root->key, LCA->key) > 0)
        buildStackLCABased(top,root->left,LCA);
    else 
        buildStackLCABased(top,root->right, LCA);

    push(top,root->key);
} 
void printStack(Node *top) {
    if(!top) return;
    printf("%s ", top->key);
    printStack(top->next);
}
void printStackInward(Node *top) {
    if(!top) return;
    printStackInward(top->next);
    printf("%s ", top->key);
}
void freeBst(bstNode *root) {
    if(root == NULL) return;

    freeBst(root->left);
    freeBst(root->right);
    free(root->key);
    free(root);
}
void freeStack(Node *top) {
    if(top == NULL) return;

    freeStack(top->next);
    free(top->key);
    free(top);
} 
void freeArr(char **arr, int arr_size) {
    if(!arr) return;

    for(int i = 0; i < arr_size; i++)
        free(arr[i]);
    free(arr);
}
int main() {
    const char *filename = "words.txt";
    int arr_size;
    char **words = retiveFileData(filename, &arr_size);

    bstNode *root = NULL;
    Node *stack = NULL;
    
    constructBst(&root,words,0, arr_size - 1);
    bstNode *LCA = getLowestCommonAncestor(root, "Cristina", "Ana");
    buildStackLCABased(&stack,root,LCA);

    printf("Lowest common ancestor between Cristina and Ana : %s\n", LCA->key);
    printf("The stack based on LCA : ");
    printStack(stack);
    printf("\nThe stack based on LCA inward: ");
    printStackInward(stack);
    printf("\n");
    double avlLen = getAvragePathLenght(root);
    printf("Avrage dist between all nodes : %.1f\n", avlLen);

    freeBst(root);
    freeStack(stack);
    freeArr(words,arr_size);
    return 0;
}
