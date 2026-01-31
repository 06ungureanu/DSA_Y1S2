#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum MACROS {
	NAME_SIZE = 50,
	BUFF_SIZE = 128
};

typedef struct Vertex {
	char name[NAME_SIZE];
	struct Vertex **friends;
	int numFriends;
} Vertex;

typedef struct Network {
	Vertex **friends;
	int numNodes;
} Network;

typedef struct Node {
	Vertex *key;
	struct Node *next;
} Node;

typedef struct Queue {
	Node *front;
	Node *rear;
} Queue;

#pragma region UTILS
int countlines(const char* filepath) {
	FILE *f = fopen(filepath, "r");
	if(!f)
		return -1;

	int lines = 1;
	char ch;
	while ((ch = fgetc(f)) != EOF) {
		if(ch == '\n')
			lines++;
	}

	fclose(f);
	return lines;
}
#pragma endregion

#pragma region QueueFuncs
Node* createNewNode(Vertex* key) {
	if(!key)
		return NULL;
	Node *temp = (Node*)malloc(sizeof(Node));
	
	temp->key = key;
	temp->next = NULL;

	return temp;
}
Queue* createEmptyQueue() {
	Queue *q = (Queue*)malloc(sizeof(Queue));

	q->front = NULL;
	q->rear = NULL;

	return q;
}
void enqueue(Queue* q, Vertex* key) {
	if(!q ||!key)
		return;

	Node *temp = createNewNode(key);
	if (q->front == NULL && q->rear == NULL)
	{
		q->front = temp;
		q->rear = temp;
	}
	else
	{
		q->rear->next = temp;
		q->rear = temp;
	}
}
Vertex* dequeue(Queue* q) {
	if(!q || !q->front)
		return NULL;

	Node *temp = q->front;
	Vertex *key = temp->key;

	q->front = q->front->next;
	if(q->front == NULL)
		q->rear = NULL;

	free(temp);
	return key;
}
int isEmpty(Queue* q) {
	return q->front == NULL && q->rear == NULL;
}
void freeQueue(Queue* q) {
	if(!q)
		return;
	Node *curr = q->front;
	while (curr) {
		Node *temp = curr;
		curr = curr->next;
		free(temp);
	}
}
#pragma endregion

#pragma region ConstructGraph
void createFriendShip(Vertex* from, Vertex* to) {
	from->friends = (Vertex**)realloc(from->friends, (from->numFriends + 1) * sizeof(Vertex*));
	from->friends[from->numFriends++] = to;
}
Vertex* getVertexFromNetwork(Network* socialNetwork, const char* name) {
	if(!socialNetwork || !name)
		return NULL;
	for (int i = 0; i < socialNetwork->numNodes; i++) {
		Vertex *curr = socialNetwork->friends[i];
		if(strcmp(curr->name,name) == 0)
			return curr;
	}
	return NULL;
}
int getVertexIndex(Network* socialNetwork, const char* name) {
	if(!socialNetwork || !name)
		return -1;
	for (int i = 0; i < socialNetwork->numNodes; i++)
	{
		Vertex *v = socialNetwork->friends[i];
		if(strcmp(v->name,name) == 0)
			return i;
	}
}
Vertex* createNewVertex(const char* name) {
	if(!name)
		return NULL;
	Vertex *temp = (Vertex*)malloc(sizeof(Vertex));

	strcpy(temp->name, name);
	temp->numFriends = 0;
	temp->friends = NULL;

	return temp;
}
Network* readNetworkFromFile(const char* filepath) {
	FILE *f = fopen(filepath, "r");
	if (!f) 
    {
		printf("Invalid filepath sent as input\n");
		return NULL;
	}

	Network *socialNetwork = (Network*)malloc(sizeof(Network));

	socialNetwork->numNodes = countlines(filepath);
	socialNetwork->friends = (Vertex**)calloc(socialNetwork->numNodes, sizeof(Vertex*));

	char buffer[BUFF_SIZE];
	char participantName[NAME_SIZE];

	for (int i = 0; i < socialNetwork->numNodes; i++) 
    {
		fgets(buffer, BUFF_SIZE, f);
		sscanf(buffer, "%s", participantName);
		socialNetwork->friends[i] = createNewVertex(participantName);
	}

	rewind(f);
	for (int i = 0; i < socialNetwork->numNodes; i++) {
		char participantName[NAME_SIZE];
		int numFriends;

		fscanf(f, "%s %d", participantName, &numFriends);

		for (int j = 0; j < numFriends; j++) {
			char friendName[NAME_SIZE];
			fscanf(f, "%s", friendName);

			Vertex *from = getVertexFromNetwork(socialNetwork,participantName);
			Vertex *to = getVertexFromNetwork(socialNetwork,friendName);

			createFriendShip(from,to);
		}
	}

	fclose(f);
	return socialNetwork;
}
#pragma endregion

#pragma region TRAVERSALS
void printGraph(Network* SocialNetwork) {
	if(!SocialNetwork || !SocialNetwork->friends)
		return;

	for(int i = 0; i < SocialNetwork->numNodes; i++) {
		Vertex *curr = SocialNetwork->friends[i];
		printf("%s : ", curr->name);
		for(int j = 0; j < curr->numFriends; j++)
			printf("%s ", curr->friends[j]->name);
		printf("\n");
	}
}
void DFS_CONNECTED(Vertex* v,Network *socialNetwork,int vIndex,int* visited) {
	if(!v || !visited)
		return;

	visited[vIndex] = 1;

	for (int i = 0; i < v->numFriends; i++) {
		Vertex *neighbor = v->friends[i];
		int neighborIndex = getVertexIndex(socialNetwork,neighbor->name);

		if(!visited[neighborIndex])
			DFS_CONNECTED(neighbor,socialNetwork,neighborIndex,visited);
	}
}
#pragma endregion

Vertex* getMaxDegreeVertex(Network* socialNetwork) {
	if(!socialNetwork || !socialNetwork->friends)
		return NULL;

	Vertex *maxDegree = socialNetwork->friends[0];

	for (int i = 0; i < socialNetwork->numNodes; i++) {
		if(socialNetwork->friends[i]->numFriends > maxDegree->numFriends)
			maxDegree = socialNetwork->friends[i];
	}

	return maxDegree;
}
int numberOfGroups(Network* socialNetwork) {
	if(!socialNetwork || !socialNetwork->friends)
		return -1;

	int groups = 0;
	int *visited = (int*)calloc(socialNetwork->numNodes, sizeof(int));

	for (int i = 0; i < socialNetwork->numNodes; i++) {
		Vertex *curr = socialNetwork->friends[i];
		int currIndex = getVertexIndex(socialNetwork, curr->name);

		if (!visited[currIndex]) {
			groups++;
			DFS_CONNECTED(curr,socialNetwork,currIndex,visited);
		}
	}
	
	return groups;
}

void printDegree2Friends(Network* socialNetork, const char* startNode) {
	if(!socialNetork || !socialNetork->friends)
		return;
	if(!startNode)
		return;

	int *visited = (int*)calloc(socialNetork->numNodes, sizeof(int));
	int *level = (int*)calloc(socialNetork->numNodes, sizeof(int));
	
	Queue *q = createEmptyQueue();

	int startIndex = getVertexIndex(socialNetork,startNode);
	if(startIndex == -1)
		return; 

	visited[startIndex] = 1;
	level[startIndex] = 0;
	
	enqueue(q,socialNetork->friends[startIndex]);

	while (!isEmpty(q)) {
		Vertex *curr = dequeue(q);
		int currIndex = getVertexIndex(socialNetork,curr->name);

		if(level[currIndex] == 2) {
			printf("Friend of level 2 with %s : %s\n", startNode, curr->name);
			continue;
		}

		for (int i = 0; i < curr->numFriends; i++) {
			Vertex *neighbor = curr->friends[i];
			int neighborIndex = getVertexIndex(socialNetork,neighbor->name);
			
			if (!visited[neighborIndex]) {
				visited[neighborIndex] = 1;
				level[neighborIndex] = level[currIndex] + 1;
				enqueue(q,neighbor);
			}
		}
	}

	free(visited);
	free(level);
	freeQueue(q);
}
int main() {
	const char *filepath = "network.txt";

	Network *socialNetwork = readNetworkFromFile(filepath);

	printGraph(socialNetwork);

	Vertex *mostPopularPerson = getMaxDegreeVertex(socialNetwork);
	printf("\nThe most popular person in network : %s\n", mostPopularPerson->name);

	int groups = numberOfGroups(socialNetwork);
	printf("Number of groups in network : %d\n\n", groups);

	printDegree2Friends(socialNetwork,"Andreea");

	return 0;
}
