#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	int key;
	struct Node *left;
	struct Node *right;
} Node;

Node* createNode(int key) {
	Node *temp = (Node*)malloc(sizeof(Node));
	if(!temp) return NULL;

	temp->key = key;
	temp->left = NULL;
	temp->right = NULL;

	return temp;
}
void insert(Node** root, int key) {
	if (*root == NULL) {
		*root = createNode(key);
		return;
	}

	if(key < (*root)->key)
		insert(&(*root)->left, key);
	if(key > (*root)->key)
		insert(&(*root)->right, key);
	else return;
}
void constructBST(Node** root, int* arr, int low, int high) {
	if(low > high)
		return;
	int mid = (low + high) / 2;
	insert(root,arr[mid]);

	constructBST(root,arr,low, mid - 1);
	constructBST(root,arr,mid + 1, high);
}
void printInOrder(Node* root) {
	if(!root) return;
	
	printInOrder(root->left);
	printf("%d ", root->key);
	printInOrder(root->right);
}
void printPreOrder(Node* root) {
	if(!root) return;

	
	printf("%d ", root->key);
	printPreOrder(root->left);
	printPreOrder(root->right);
}
void printPostOder(Node* root) {
	if(!root) return;
	
	printPostOder(root->left);
	printPostOder(root->right);
	printf("%d ", root->key);
}
void showAllTraversal(Node* root) {
	if(!root) return;

	printf("InOrder Traversal : ");
	printInOrder(root);
	printf("\n");

	printf("PostOrder Traversal : ");
	printPostOder(root);
	printf("\n");

	printf("PreOrder Traversal : ");
	printPreOrder(root);
	printf("\n");
}

Node* searchNode(Node* root, int key) {
	if(!root) return NULL;

	if(root->key == key)
		return root;
	if(key < root->key)
		return searchNode(root->left,key);
	else 
		return searchNode(root->right,key);
}

int getBSTHeight(Node* root) {
	if(root == NULL)
		return 0;

	int left = getBSTHeight(root->left);
	int right = getBSTHeight(root->right);

	return (left > right ? left : right) + 1;
}
int getBstNodesNr(Node* root) {
	if(root == NULL)
		return 0;
	return 1 + getBstNodesNr(root->left) + getBstNodesNr(root->right);
}
int getBstLeafsNr(Node* root) {
	if(root == NULL)
		return 0;
	if(root->left == NULL && root->right == NULL)
		return 1;
	return getBstLeafsNr(root->left) + getBstLeafsNr(root->right);
}
int findMinValue(Node* root) {
	while(root->left)
		root = root->left;
	return root->key;
}
void deleteNode(Node** root, int key) {
	if(*root == NULL)
		return;

	
	if(key < (*root)->key)
		deleteNode(&(*root)->left,key);
	else if (key > (*root)->key)
		deleteNode(&(*root)->right,key);
	else
	{
		if ((*root)->left == NULL && (*root)->right == NULL) {
			free(*root);
			return;
		}
		else if ((*root)->left == NULL || (*root)->right == NULL) {
			Node *temp = ((*root)->left) ? (*root)->left : (*root)->right;
			free(*root);
			*root = temp;
			return;
		}
		int minValue = findMinValue((*root)->right);
		(*root)->key = minValue;
		deleteNode(&(*root)->right,minValue);
	}
}

void invertBST(Node* root) {
	if(!root) return;

	Node *temp = root->left;
	root->left = root->right;
	root->right = temp;

	invertBST(root->left);
	invertBST(root->right);
}
Node* findLCA(Node* root, int lkey_1, int lkey_2) {
	if(!root) 
		return NULL;

	if(lkey_1 < root->key && lkey_2 < root->key)
		return findLCA(root->left,lkey_1,lkey_2);
	else if(lkey_1 > root->key && lkey_2 > root->key)
		return findLCA(root->right, lkey_1,lkey_2);
	else 
		return root;
}
int main() {
	Node *root = NULL;

	int arr[] = {1,2,3,4,5,6,7};
	int arr_size = sizeof(arr) / sizeof(arr[0]);
	constructBST(&root,arr,0, arr_size - 1);

	showAllTraversal(root);

	Node *searched = searchNode(root,9);
	searched ? printf("\nNodul exista\n") : printf("\nNodul nu exista\n");

	int height = getBSTHeight(root);
	printf("Inaltimea bst : %d\n", height);

	int nodes = getBstNodesNr(root);
	printf("Numar de noduri : %d\n", nodes);

	int leafes = getBstLeafsNr(root);
	printf("Numarul de frunze : %d\n", leafes);

	
	

	Node *lca = findLCA(root,1,3);
	if(lca != NULL)
		printf("\nStramosul comun este %d\n", lca->key);
	else 
		printf("Nodurile cautate nu exista in arbore\n");

	return 0;
}