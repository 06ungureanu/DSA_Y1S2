#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int key;
	int height;
	struct Node *left;
	struct Node *right;
} Node;

#pragma region UTILS
Node* createNewNode(int key) {
	Node *temp = (Node*)malloc(sizeof(Node));
	
	temp->key = key;
	temp->left = NULL;
	temp->right = NULL;
	temp->height = 1;

	return temp;
}
int getMax(int a, int b) {
	return (a > b) ? a : b;
}
int getHeight(Node* n) {
	if(!n)
		return 0;
	return n->height;
}
int balanceFactor(Node* n) {
	return getHeight(n->left) - getHeight(n->right);
}
Node* getMinValue(Node* n) {
	if(!n)
		return NULL;
	while(n->left)
		n = n->left;
	return n;
}
#pragma endregion

#pragma region BALANCE
void updateHeight(Node* n) {
	if(!n)
		return;

	n->height = 1 + getMax(getHeight(n->left), getHeight(n->right));
}
void rightRotate(Node** x) {
	Node *y = (*x)->left;
	Node *T2 = y->right;

	y->right = *x;
	(*x)->left = T2;

	updateHeight(*x);
	updateHeight(y);

	*x = y;
}
void leftRotate(Node** x) {
	Node *y = (*x)->right;
	Node *T2 = y->left;

	y->left = *x;
	(*x)->right = T2;

	updateHeight(*x);
	updateHeight(y);

	*x = y;
}
void balance(Node** n) {
	int factor = balanceFactor(*n);

	if(factor > 1 && balanceFactor((*n)->left) >= 0)
		rightRotate(n);
	else if(factor < -1 && balanceFactor((*n)->right) <= 0)
		leftRotate(n);
	else if (factor > 1 && balanceFactor((*n)->left) < 0) {
		leftRotate(&(*n)->left);
		rightRotate(n);
	}
	else if (factor < -1 && balanceFactor((*n)->right) > 0)
	{
		rightRotate(&(*n)->right);
		leftRotate(n);
	}

	updateHeight(*n);
}
#pragma endregion

#pragma region INSERT/DELETE
void insert(Node** root, int key) {
	if (*root == NULL) {
		*root = createNewNode(key);
		return;
	}

	if(key < (*root)->key)
		insert(&(*root)->left, key);
	else if(key > (*root)->key)
		insert(&(*root)->right, key);
	else return;

	updateHeight(*root);
	balance(root);
}
void deleteNode(Node** root, int key) {
	if(!(*root))
		return;

	if(key < (*root)->key)
		deleteNode(&(*root)->left,key);
	else if(key > (*root)->key)
		deleteNode(&(*root)->right, key);
	else
	{
		
		if ((*root)->left == NULL && (*root)->right == NULL) {
			free(*root);
			*root = NULL;
			return;
		}
		
		else if ((*root)->left == NULL || (*root)->right == NULL) {
			Node *temp = ((*root)->left) ? (*root)->left : (*root)->right;
			free(*root);
			*root = temp;
			return;
		}
		
		Node *minValue = getMinValue((*root)->right);
		(*root)->key = minValue->key;
		deleteNode(&(*root)->right, minValue->key);
	}

	updateHeight(*root);
	balance(root);
}
#pragma endregion

#pragma region ALGOS
void printInOrder(Node* root) {
	if(!root)
		return;

	printInOrder(root->left);
	printf("%d ", root->key);
	printInOrder(root->right);
}
int getTreeHeight(Node* root) {
	if(root == NULL)
		return -1;
	return 1 + getMax(getTreeHeight(root->left), getTreeHeight(root->right));
}
int getNumNodes(Node* root) {
	if(!root)
		return 0;
	return 1 + getNumNodes(root->left) + getNumNodes(root->right);
}
int getNumLeafs(Node* root) {
	if(root == NULL)
		return 0;
	if(root->left == NULL && root->right == NULL)
		return 1;
	return getNumLeafs(root->left) + getNumLeafs(root->right);
}
#pragma endregion

int main() {
	Node *root = NULL;

	insert(&root,0);
	insert(&root,1);
	insert(&root,2);
	insert(&root,3);
	insert(&root,4);
	insert(&root,5);
	insert(&root,6);

	deleteNode(&root,3);
	printInOrder(root);

	int height = getTreeHeight(root);
	int nodes = getNumNodes(root);
	int leafes = getNumLeafs(root);

	printf("\nTree Height : %d\n", height);
	printf("Nodes : %d\n", nodes);
	printf("Leafes : %d\n", leafes);
	return 0;
}