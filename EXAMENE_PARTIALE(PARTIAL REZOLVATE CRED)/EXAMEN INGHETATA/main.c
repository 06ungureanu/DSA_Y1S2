#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	NAME_SIZE = 50,
	BUFF_SIZE = 130,
	DATE_SIZE = 20
};

typedef struct IceCream {
	char flavour[NAME_SIZE];
	char size[NAME_SIZE];
} IceCream;

typedef struct IceCreamNode {
	IceCream *key;
	struct IceCreamNode *next;
} IceCreamNode;

typedef struct Order {
	char id[NAME_SIZE];
	char date[DATE_SIZE];
	IceCreamNode *IceCreamList;
} Order;

typedef struct HashTable {
	IceCreamNode **OrderList;
	int t_size;
} HashTable;

IceCream* createNewIceCream(const char* name, const char* size) {
	if(!name || !size)
		return;
	IceCream *temp = (IceCream*)malloc(sizeof(IceCream));
	strcpy(temp->flavour, name);
	strcpy(temp->size,size);
	return temp;
}
IceCreamNode* createNewIceCreamNode(IceCream* key) {
	if(!key)
		return;
	IceCreamNode *temp = (IceCreamNode*)malloc(sizeof(IceCreamNode));
	temp->key = key;
	temp->next = NULL;
	return temp;
}
void insertNodeIceCreamList(IceCreamNode** head, IceCream* key) {
	IceCreamNode *temp = createNewIceCreamNode(key);
	temp->next = *head;
	*head = temp;
}
Order* constructOrderFromFile(const char* filepath) {
	FILE *f = fopen(filepath, "r");
	if (!f) {
		printf("Invalid order path\n");
		return NULL;
	}

	char date[DATE_SIZE];
	fscanf(f, "%s", date);

	Order *temp = (Order*)malloc(sizeof(Order));
	
	strcpy(temp->date, date);
	strcpy(temp->id,filepath);

	char icecream_name[NAME_SIZE];
	char size[NAME_SIZE];
	while (!feof(f)) {
		fscanf(f, "%s %s", icecream_name,size);
		IceCream *curr = createNewIceCream(icecream_name,size);
		insertNodeIceCreamList(&temp->IceCreamList,curr);
	}

	fclose(f);
	return temp;
}
HashTable* constructHashTableFromFile(const char *filepath) {
	FILE *f = fopen(filepath,"r");
	if (!f) {
		printf("Invalid file path sent as input\n");
		return NULL;
	}

	int numOrders;
	fscanf(f, "%d", &numOrders);

	HashTable *orders = (HashTable*)malloc(sizeof(HashTable));

	orders->t_size = numOrders * 2;
	orders->OrderList = (Order**)calloc(orders->t_size, sizeof(Order*));
	
	char comandID[NAME_SIZE];
	for (int i = 0; i < numOrders; i++) {
		fscanf(f, "%s", comandID);
	}
}
void main() {
	const char *filepath = "istoric.txt";

	
}