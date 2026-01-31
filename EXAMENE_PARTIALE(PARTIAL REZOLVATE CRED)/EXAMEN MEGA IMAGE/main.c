#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum {
	NAME_SIZE = 50,
	BUFF_SIZE = 128
};

typedef struct Product {
	char name[NAME_SIZE];
	float price;
	int quantity;
} Product;

typedef struct ProductNode {
	Product *key;
	struct ProductNode *next;
} ProductNode;

typedef struct Receipt {
	int numProducts;
	int totalReceiptValue;
	ProductNode *productList;
} Receipt;

typedef struct ReceipitNode {
	Receipt *key;
	struct ReceiptNode *next;
} ReceiptNode;

typedef struct Card {
	char name[NAME_SIZE];
	char familyName[NAME_SIZE];
	int  id;
	int totalCardValue;
	int numReceipts;
	ReceiptNode *receiptList;
}Card;

typedef struct CardNode {
	Card *key;
	struct CardNode *next;
} CardNode;

typedef struct HashTable {
	CardNode **cardList;
	int t_size;
	bool *bloomFilter;
} HashTable;


static inline int hashInt(int key, int t_size) {
	return key % t_size;
}
int hashStr(const char* s, int t_size) {
	int hash = 0;
	for (; *s; s++) {
		hash += (int)*s;
		hash %= t_size;
	}
	return hash;
}




Product* createNewProduct(const char* name, float price, int quantity) {
	if(!name || price < 0 || quantity < 0)
		return NULL;

	Product *temp = (Product*)malloc(sizeof(Product));
	temp->price = price;
	temp->quantity = quantity;
	strcpy(temp->name,name);

	return temp;
}
ProductNode* createNewProductNode(Product* key) {
	if(!key)
		return NULL;
	ProductNode *temp = (ProductNode*)malloc(sizeof(ProductNode));
	temp->key = key;
	temp->next = NULL;
	return temp;
}
void insertProductInProductList(ProductNode** head, Product* key) {
	if(!key)
		return;
	ProductNode *temp = createNewProductNode(key);
	temp->next = *head;
	*head = temp;
}


Receipt* createReceiptFromFile(const char* filepath) {
	FILE *f = fopen(filepath, "r");
	if(!f)
		return NULL;

	Receipt *temp = (Receipt*)malloc(sizeof(Receipt));
	fscanf(f, "%d", &temp->numProducts);
	temp->productList = NULL;
	temp->totalReceiptValue = 0;

	for (int i = 0; i < temp->numProducts; i++) {
		char productName[NAME_SIZE];
		float price;
		int quantity;

		fscanf(f, "%s %f %d", productName,&price,&quantity);

		Product *newProduct = createNewProduct(productName,price,quantity);
		insertProductInProductList(&temp->productList,newProduct);
		temp->totalReceiptValue += price * quantity;
	}

	return temp;
}
ReceiptNode* createNewReceiptNode(Receipt* key) {
	if(!key)
		return;
	ReceiptNode *temp = (ReceiptNode*)malloc(sizeof(ReceiptNode));
	temp->key = key;
	temp->next = NULL;
	return temp;
}
void insertReceiptInList(ReceiptNode** head, Receipt* key) {
	if(!key)
		return;
	ReceiptNode *temp = createNewReceiptNode(key);
	temp->next = *head;
	*head = temp;
}
void freeReceipt(Receipt* receipt) {
	if (!receipt)
		return;

	ProductNode* curr = receipt->productList;
	while (curr) {
		ProductNode* tempNode = curr;
		Product* tempProduct = curr->key;

		curr = curr->next;
		free(tempProduct);
		free(tempNode);
	}

	free(receipt);
}


Card* createNewCard(const char* name, const char* familyName, int id, int numReceipts) {
	if(!name || !familyName || numReceipts < 0)
		return NULL;
	Card *temp = (Card*)malloc(sizeof(Card));

	temp->id = id;
	temp->numReceipts = numReceipts;
	temp->receiptList = NULL;
	temp->totalCardValue = 0;
	
	strcpy(temp->familyName, familyName);
	strcpy(temp->name, name);

	return temp;
}
CardNode* createNewCardNode(Card* key) {
	if(!key)
		return NULL;
	CardNode *temp = (CardNode*)malloc(sizeof(CardNode));
	temp->key = key;
	temp->next = NULL;

	return temp;
}
void insertCardInList(CardNode** head, Card* key) {
	if(!key)
		return;
	CardNode *temp = createNewCardNode(key);
	temp->next = *head;
	*head = temp;
}
Card* searchCardinCardList(CardNode* head, int id) {
	if(!head)
		return NULL;
	while (head) {
		if(head->key->id == id)
			return head;
		head = head->next;
	}
	return NULL;
}
void deleteCardFromCardList(CardNode** head, int id) {
	if (!head || !(*head))
		return;

	CardNode* curr = *head;
	CardNode* prev = NULL;

	while (curr && curr->key->id != id) {
		prev = curr;
		curr = curr->next;
	}

	if (!curr)
		return;  
	if (prev)
		prev->next = curr->next;
	else
		*head = curr->next;  

	ReceiptNode* r = curr->key->receiptList;
	while (r) {
		ReceiptNode* tempReceiptNode = r;
		freeReceipt(r->key);
		r = r->next;
		free(tempReceiptNode);
	}

	free(curr->key);
	free(curr);
}
Card* searchCardById(HashTable* cards, int id) {
	for (int i = 0; i < cards->t_size; i++) {
		CardNode* curr = cards->cardList[i];
		while (curr) {
			if (curr->key && curr->key->id == id)
				return curr->key;
			curr = curr->next;
		}
	}
	return NULL;
}


HashTable* constructHashTableFromFile(const char* filepath) {
	FILE *f = fopen(filepath,"r");
	if(!f)
		return NULL;
	
	HashTable *HT = (HashTable*)malloc(sizeof(HashTable));
	
	fscanf(f, "%d", &HT->t_size);
	printf("%d\n", HT->t_size);
	HT->t_size *= 2;


	HT->cardList = (CardNode**)calloc(HT->t_size, sizeof(CardNode*));
	HT->bloomFilter = (bool*)calloc(HT->t_size, sizeof(bool));

	while (!feof(f)) {
		char name[NAME_SIZE];
		char familyName[NAME_SIZE];
		char receiptFilePath[NAME_SIZE];
		int numReceipts;
		int id;

		fscanf(f, "%s %s", name,familyName);
		fscanf(f, "%d", &id);
		fscanf(f, "%d", &numReceipts);

		int index = hashStr(familyName, HT->t_size);
		HT->bloomFilter[index] = 1;

		Card *newCard = createNewCard(name,familyName,id,numReceipts);
		insertCardInList(&HT->cardList[index],newCard);

		for (int i = 0; i < numReceipts; i++) {
			
			fscanf(f, "%s", receiptFilePath);
			Receipt *newReceipt = createReceiptFromFile(receiptFilePath);
			insertReceiptInList(&newCard->receiptList,newReceipt);
			
			newCard->totalCardValue += newReceipt->totalReceiptValue;
		}
	}

	fclose(f);
	return HT;
}


void printReceiptInfo(Receipt* receipt) {
	if(!receipt)
		return;
	printf("Receipt total value : %d\n", receipt->totalReceiptValue);
	printf("Receipt products : \n");
	
	ProductNode *curr = receipt->productList;
	while (curr) {
		Product *currProduct = curr->key;
		printf("Name %s ||\t Priice %.1f ||\t Quntity : %d\n", currProduct->name,currProduct->price,currProduct->quantity);
		curr = curr->next;
	}
}
void printCardInfo(Card* card) {
	if(!card || !card->receiptList)
		return;
	printf("Card owner : %s %s\n", card->name,card->familyName);
	printf("Card id : %d\n", card->id);
	printf("Card total value : %d\n", card->totalCardValue);
	printf("Card receipts: ");
	
	ReceiptNode *curr = card->receiptList;
	while (curr) {
		Receipt *currReceipt = curr->key;
		printReceiptInfo(currReceipt);
		curr = curr->next;
		printf("\n");
	}
}
void printAllCards(HashTable* cards) {
	if(!cards || !cards->cardList)
		return;
	for (int i = 0; i < cards->t_size; i++) {
		CardNode *currCardNode = cards->cardList[i];
		while (currCardNode) {
			Card *currCard = currCardNode->key;
			printCardInfo(currCard);
			currCardNode = currCardNode->next;
		}
	}
}


void checkCard(HashTable* cards, const char* familyName) {
	if(!cards || !cards->bloomFilter || !familyName)
		return;
	int index = hashStr(familyName,cards->t_size);
	if(cards->bloomFilter[index])
		printf("Cardul %s exista\n", familyName);
	else 
		printf("Cardul % s nu exista\n", familyName);
}

void removeCard(HashTable* cards, int id) {
	if (!cards || !cards->cardList)
		return;

	for (int i = 0; i < cards->t_size; i++) {
		CardNode* curr = cards->cardList[i];
		while (curr) {
			if (curr->key && curr->key->id == id) {
				deleteCardFromCardList(&cards->cardList[i], id);
				
				return;
			}
			curr = curr->next;
		}
	}
}
void main() {
	const char *filepath = "cards.txt";

	HashTable *cards = constructHashTableFromFile(filepath);

	printAllCards(cards);
	checkCard(cards,"George");
	checkCard(cards, "YAMIL");

	removeCard(cards, 32);
	int index = hashStr("Dulce", cards->t_size);
	Card *searched = searchCardByID(cards->cardList[index],3	);
	if(searched)
		printf("da");
	else
		printf("nu");
}