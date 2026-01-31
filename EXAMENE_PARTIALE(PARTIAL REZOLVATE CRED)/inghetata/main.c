#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	NAME_SIZE = 50,
	BUFF_SIZE = 60,
	DATE_SIZE = 30,
	T_SIZE = 1000
};

typedef struct IcreCream {
	char flavour[NAME_SIZE];
	char size[NAME_SIZE];
} IceCream;

typedef struct IceCreamNode {
	IceCream *key;
	struct IceCreamNode *next;
} IceCreamNode;

typedef struct Order {
	char name[NAME_SIZE];
	char date[DATE_SIZE];
	IceCreamNode *iceCreamList;
	int totalIceCreams;
} Order;

typedef struct OrderNode {
	Order *key;
	struct OrderNode *next;
} OrderNode;

typedef struct HashTable {
	OrderNode *orderList[T_SIZE];
	int numOrders;
} HashTable;

typedef struct FlavourFreq {
	char name[NAME_SIZE];
	int count;
} FlavouFreq;

typedef struct Heap {
	Order **heap;
	int heapSize;
} Heap;

typedef struct Stock {
	char flavour[NAME_SIZE];
	int quantity;
} Stock;

typedef struct PriceGramRatio {
	char size[NAME_SIZE];
	int weight;
	int price;
	double profitPerGram;
} PriceGramRatio;

#pragma region UtilFunctions
static inline char* getNameFromFilePath(const char* filepath) {
	char *name = strdup(filepath);
	strtok(name,".txt");

	return name;
}
int hash(const char* s, int t_size) {
	int hash = 0;
	for (; *s; s++) {
		hash += (int)*s;
		hash %= t_size;
	}
	return hash;
}
#pragma endregion

#pragma region IcreCreamFunctions
IceCream* createNewIcream(const char* flavour, const char* size)  {
	if(!flavour || !size)
		return NULL;
	IceCream *temp = (IceCream*)malloc(sizeof(IceCream));
	strcpy(temp->flavour,flavour);
	strcpy(temp->size, size);
	
	return temp;
}
IceCreamNode* createNewIceCreamNode(IceCream* key) {
	if(!key)
		return NULL;
	IceCreamNode *temp = (IceCreamNode*)malloc(sizeof(IceCreamNode));
	temp->key = key;
	temp->next = NULL;
	
	return temp;
}
void insertIceCreamInList(IceCreamNode** head, IceCream* key) {
	if(!key)
		return;
	IceCreamNode *temp = createNewIceCreamNode(key);

	temp->next = *head;
	*head = temp;
}
void theMostPopularIceCream(HashTable* orders) {
	if(!orders)
		return;

	FlavouFreq freq[T_SIZE];
	int flavourTotal = 0;

	for (int i = 0; i < T_SIZE; i++) {
		OrderNode *orderNode = orders->orderList[i];
		while (orderNode) {
			IceCreamNode *icNode = orderNode->key->iceCreamList;
			while (icNode) {
				IceCream *ice = icNode->key;
				
				int flag = 0;
				for (int j = 0; j < flavourTotal; j++) {
					if (strcmp(freq[j].name, ice->flavour) == 0) {
						freq[j].count++;
						flag = 1;
						break;
					}
				}
				if (!flag) {
					strcpy(freq[flavourTotal].name, ice->flavour);
					freq[flavourTotal].count = 1;
					flavourTotal++;
				}
				icNode = icNode->next;
			}

			orderNode = orderNode->next;
		}
	}

	int index = 0;
	for (int i = 0; i < flavourTotal; i++) {
		if(freq[i].count > freq[index].count)
			index = i;
	}

	printf("\nCea mai comandata aroma de inghetata este : %s cu %d bucati\n", freq[index].name,freq[index].count);
}
#pragma endregion

#pragma region OrderFunctions
Order* createNewOrderFromFile(const char* filepath, const char *date) {
	FILE *f = fopen(filepath, "r");
	if(!f)
		return NULL;

	Order *temp = (Order*)malloc(sizeof(Order));
	char *orderName = getNameFromFilePath(filepath);

	strcpy(temp->name, orderName);
	strcpy(temp->date, date);
	temp->totalIceCreams = 0;
	temp->iceCreamList = NULL;

	while (!feof(f)) {
		char flavour[NAME_SIZE];
		char size[NAME_SIZE];

		fscanf(f, "%s %s", flavour,size);

		IceCream *newIceCream = createNewIcream(flavour,size);
		insertIceCreamInList(&temp->iceCreamList, newIceCream);
		temp->totalIceCreams += 1;
	}
	free(orderName);
	fclose(f);

	return temp;
}
OrderNode* createNewOrderNode(Order* key) {
	if(!key)
		return NULL;
	OrderNode *temp = (OrderNode*)malloc(sizeof(OrderNode));
	temp->key = key;
	temp->next = NULL;
	return temp;
}
void insertOrderInList(OrderNode** head, Order* key) {
	if(!key)
		return;
	OrderNode *temp = createNewOrderNode(key);
	temp->next = *head;
	*head = temp;
}
#pragma endregion

#pragma region ConstructFromFile
HashTable* constructOrdersFromFile(const char* filepath) {
	FILE *f = fopen(filepath,"r");
	if (!f) {
		printf("Invalid filepath sent as input\n");
		return NULL;
	}

	HashTable *orders = (HashTable*)malloc(sizeof(HashTable));
	memset(orders->orderList,0,T_SIZE * sizeof(OrderNode*));

	fscanf(f, "%d", &orders->numOrders);

	for (int i = 0; i < orders->numOrders; i++) {
		char orderFilePath[NAME_SIZE];
		char orderDate[DATE_SIZE];
		
		fscanf(f, "%s %s", orderFilePath, orderDate);
		
		int index = hash(orderDate,T_SIZE);
		Order *newOrder = createNewOrderFromFile(orderFilePath,orderDate);
		insertOrderInList(&orders->orderList[index], newOrder);
	}

	fclose(f);
	return orders;
}
#pragma endregion

#pragma region PrintFunctions
void printIceCreamList(IceCreamNode* head) {
	if(!head)
		return;
	while (head) {
		IceCream *curr = head->key;
		printf("[ %s %s], ", curr->flavour, curr->size);
		head = head->next;
	}
	printf("\n");
}
void printOrderList(OrderNode* head) {
	if(!head)
		return;
	while (head) {
		Order *curr = head->key;
		printf("Order name : %s\n", curr->name);
		printf("Order Date : %s\n", curr->date);
		printf("Order total icecreams : %d\n", curr->totalIceCreams);
		printf("Order iceCreams : ");
		printIceCreamList(curr->iceCreamList);
		printf("\n");
		head = head->next;
	}
}
void printAllComands(HashTable* orders) {
	if(!orders || !orders->orderList)
		return;
	for (int i = 0; i < T_SIZE; i++) {
		OrderNode *currOrderNode = orders->orderList[i];
		if(currOrderNode)
			printOrderList(currOrderNode);
	}
}
#pragma endregion

#pragma region SearchFunctions
OrderNode* searchOrderInList(OrderNode* head, const char* date) {
	if(!head || !date)
		return NULL;
	while (head) {
		if(strcmp(head->key->date,date) == 0)
			return head;
		head = head->next;
	}
	return NULL;
}
Order* searchOrderByDate(HashTable* orders, const char* date) {
	if(!orders || !date)
		return NULL;

	int index = hash(date,T_SIZE);
	OrderNode *lookupOrder = searchOrderInList(orders->orderList[index],date);

	return lookupOrder ? lookupOrder->key : NULL;
}
#pragma endregion

#pragma region deleteFunctions
void deleteIceCreamFromOrder(HashTable* orders, const char* orderDate, const char* flavour) {
	if (!orders || !orderDate || !flavour)
		return;

	Order* lookUpOrder = searchOrderByDate(orders, orderDate);
	if (!lookUpOrder) {
		printf("Given order was not found\n");
		return;
	}

	IceCreamNode* curr = lookUpOrder->iceCreamList;
	IceCreamNode* prev = NULL;

	while (curr) {
		if (strcmp(curr->key->flavour, flavour) == 0) {
			IceCreamNode* toDelete = curr;

			if (prev == NULL) {
				
				lookUpOrder->iceCreamList = curr->next;
				curr = curr->next;
			}
			else {
				prev->next = curr->next;
				curr = curr->next;
			}

			free(toDelete->key);
			free(toDelete);
			lookUpOrder->totalIceCreams--;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
}

#pragma endregion

#pragma region HeapFunctions
Heap* createEmptyHeap(int size) {
	Heap *heap = (Heap*)malloc(sizeof(Heap));

	heap->heapSize = 0;
	heap->heap = (Order**)calloc(heap->heapSize, sizeof(Order*));

	return heap;
}
void swapOrder(Order** a, Order** b) {
	Order *temp = *a;
	*a = *b;
	*b = temp;
}
int parentIndex(int i) {
	return (i - 1) / 2;
}
int leftChild(int i) {
	return (i * 2) + 1;
}
int rightChild(int i) {
	return (i * 2) + 2;
}
int compareDates(const char* date1, const char* date2) {
	if (!date1 || !date2) return -1;

	int d1, m1, y1;
	int d2, m2, y2;

	sscanf(date1, "%d/%d/%d", &d1, &m1, &y1);
	sscanf(date2, "%d/%d/%d", &d2, &m2, &y2);

	if (y1 != y2)
		return y1 - y2;
	if (m1 != m2)
		return m1 - m2;
	return d1 - d2;
}
void heapifyUp_date(Heap *heap, int i) {
	if(i == 0)
		return;
	int parent = parentIndex(i);
	if (compareDates(heap->heap[i]->date, heap->heap[parent]->date) < 0) {
		swapOrder(&heap[i], &heap[parent]);
		heapifyUp_date(heap,parent);
	}
}
void heapifyDown_date(Heap *heap, int i) {
	int left = leftChild(i);
	int right = rightChild(i);
	int smallest = i;
	if(left < heap->heapSize && compareDates(heap->heap[left]->date, heap->heap[smallest]->date) < 0)
		smallest = left;
	if(right < heap->heapSize && compareDates(heap->heap[right]->date, heap->heap[smallest]->date) < 0)
		smallest = right;
	if (smallest != i) {
		swapOrder(&heap->heap[i], &heap->heap[smallest]);
		heapifyDown_date(heap,smallest);
	}
}
void heapifyDate(Heap *heap) {
	for(int i = (heap->heapSize - 1) / 2; i >= 0; i--)
		heapifyDown_date(heap,i);
}

void heapifyUpConstencity(Heap* heap, int i) {
	if(i == 0)
		return;
	int parent = parentIndex(i);
	if (heap->heap[i]->totalIceCreams > heap->heap[parent]->totalIceCreams) {
		swapOrder(&heap->heap[i], &heap->heap[parent]);
		heapifyUpConstencity(heap,parent);
	}
}
void heapifyDownConstencity(Heap* heap, int i) {
	int left = leftChild(i);
	int right = rightChild(i);
	int largest = i;
	if(left < heap->heapSize &&  heap->heap[left]->totalIceCreams > heap->heap[largest]->totalIceCreams)
		largest = left;
	if(right < heap->heapSize && heap->heap[right]->totalIceCreams > heap->heap[largest]->totalIceCreams)
		largest = right;
	if (largest != i) {
		swapOrder(&heap->heap[i], &heap->heap[largest]);
		heapifyUpConstencity(heap,largest);
	}
}
void heapifyConstencity(Heap* heap) {
	for(int i = (heap->heapSize - 1) /2 ; i>= 0; i--)
		heapifyDownConstencity(heap,i);
}
void constructHeapDateFromHashTable(Heap* heap, HashTable* orders) {
	if (!orders || !heap)
		return;

	heap->heapSize = 0;

	for (int i = 0; i < T_SIZE; i++) {
		OrderNode* curr = orders->orderList[i];
		while (curr) {
			heap->heap[heap->heapSize++] = curr->key;
			curr = curr->next;
		}
	}

	heapifyDate(heap);
}
void constructHeapConstencityFromHashTable(Heap* heap, HashTable* orders) {
	if(!orders || !heap)
		return;

	heap->heapSize = 0;

	for (int i = 0; i < T_SIZE; i++) {
		OrderNode *curr = orders->orderList[i];
		while(curr) {
			heap->heap[heap->heapSize++] = curr->key;
			curr = curr->next;
		}
	}
	heapifyConstencity(heap);
}
Order* extractMinDateOrder(Heap* heap) {
	if(!heap)
		return;
	Order *min = heap->heap[0];
	heap->heap[0] = heap->heap[heap->heapSize - 1];
	heap->heapSize -= 1;
	heapifyDown_date(heap,0);
	return min;
}
Order* extractMaxConstencity(Heap* heap) {
	if(!heap)
		return;
	Order *max = heap->heap[0];
	heap->heap[0] = heap->heap[heap->heapSize - 1];
	heap->heapSize--;
	heapifyDownConstencity(heap,0);
	return max;
}
void extractKthConstencityOrders(Heap* heap, int k) {
	if(!heap)
		return;
	if(k > heap->heapSize)
		return;

	for (int i = 0; i < k; i++) {
		Order *kth = extractMaxConstencity(heap);
		printf("Constencity order %d with name %s and date %s (%d iceCreams)\n",i, kth->name,kth->date,kth->totalIceCreams);
	}
}
#pragma endregion

#pragma region sortari
void merge(PriceGramRatio* arr, int low, int mid, int high) {
	PriceGramRatio* temp = (PriceGramRatio*)malloc((high - low + 1) * sizeof(PriceGramRatio));

	int i = low; 
	int j = mid + 1; 
	int k = 0; 

	while (i <= mid && j <= high) {
		if (arr[i].profitPerGram > arr[j].profitPerGram)
			temp[k++] = arr[i++];
		else
			temp[k++] = arr[j++];
	}

	while (i <= mid)
		temp[k++] = arr[i++];
	while (j <= high)
		temp[k++] = arr[j++];

	for (int i = low; i <= high; i++)
		arr[i] = temp[i];

	free(temp);
}
void mergeSortRatios(PriceGramRatio* arr, int low, int high) {
	if (low < high) {
		int mid = (low + high) / 2;

		mergeSortRatios(arr, low, mid);
		mergeSortRatios(arr, mid + 1, high);
		merge(arr, low, mid, high);
	}
}
#pragma endregion

#pragma region Part2
Stock* readStockFromFile(const char* filepath) {
	FILE *f = fopen(filepath,"r");
	if (!f) {
		printf("Invalid fiepath sent as input\n");
		return NULL;
	}

	Stock *temp = (Stock*)malloc(sizeof(Stock));
	char flavour[NAME_SIZE];

	fscanf(f, "%s %d", flavour, &temp->quantity);
	strcpy(temp->flavour,flavour);

	fclose(f);
	return temp;
}
float getMaxProfit(Stock *stock, PriceGramRatio *prices) {
	if (!stock || !prices) {
		printf("Invalid parametres sent as input to function\n");
		return 0.0;
	}

	for(int i = 0; i < 3; i++) {
		if (prices[i].weight != 0) {
			float pricePG = (float)prices[i].price / prices[i].weight;
			prices[i].profitPerGram = pricePG;
		}
	}

	mergeSortRatios(prices,0,2);
	
	float maxProfit = 0.0f;
	int remainQuantity = stock->quantity;

	for (int i = 0; i < 3 && remainQuantity > 0; i++) {
		int take = prices[i].weight;

		if (remainQuantity < take)
			take = remainQuantity;

		maxProfit += take * prices[i].profitPerGram;
		remainQuantity -= take;
	}
	return maxProfit;
}
#pragma endregion

#pragma region freeFunctions

#pragma endregion
void main() {
	const char *filepath = "istoric.txt";
	const char *part2_filepath = "part2.txt";

	HashTable *orders = constructOrdersFromFile(filepath);

	printAllComands(orders);

	Order *lookUpOrder = searchOrderByDate(orders,"06/06/2025");
	Order *lookUpOrder_1 = searchOrderByDate(orders,"53/12/4234");

	lookUpOrder ? printf("Comanda cu data 06/06/2025 exista in lista\n") : printf("Comanda cu data 06/06/2025 nu exista in lista\n");
	lookUpOrder_1 ? printf("Comanda cu data 53/12/4234 nu exista in lista\n") : printf("Comanda cu data 53/12/4234 nu exista in lista\n");

	theMostPopularIceCream(orders);
	

	Heap *dateHeap = createEmptyHeap(orders->numOrders);
	Heap *consistencyHeap = createEmptyHeap(orders->numOrders);

	constructHeapDateFromHashTable(dateHeap,orders);
	constructHeapConstencityFromHashTable(consistencyHeap,orders);

	Order *oldestOrder = extractMinDateOrder(dateHeap);
	printf("Oldest order date : %s\n\n", oldestOrder->date);

	extractKthConstencityOrders(consistencyHeap,2);

	Stock *stock = readStockFromFile(part2_filepath);

	PriceGramRatio priceArray[3] = {
		{"mica", 150, 5, 0},
		{"medie", 220, 8, 0},
		{"mare", 310, 10, 0}
	};

	float maxProfit = getMaxProfit(stock,priceArray);
	
	printf("\nMax profit : %.1f RON\n", maxProfit);
	free(stock);
}