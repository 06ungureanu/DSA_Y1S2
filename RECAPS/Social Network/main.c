#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 30
#define BUFF_SIZE 128

typedef struct Vertex {
	char name[NAME_SIZE];
	struct Vertex** neighbors;
	int numNeighbors;
} Vertex;

typedef struct GraphVertex {
	Vertex** neighbors;
	int numNodes;
} GraphVertex;

#pragma region UTILS
int countlines(const char* filepath) {
	FILE* f = fopen(filepath, "r");
	if (!f)
		return -1;
	int count = 1;
	char ch;
	while ((ch = fgetc(f)) != EOF) {
		if (ch == '\n')
			count++;
	}

	fclose(f);
	return count;
}
#pragma endregion


#pragma region GRAPH_INT
Vertex* createNewVertex(const char* name) {
	Vertex* temp = (Vertex*)malloc(sizeof(Vertex));

	strcpy(temp->name, name);
	temp->neighbors = NULL;
	temp->numNeighbors = 0;

	return temp;
}
Vertex* getVertex(GraphVertex* graph, const char* name) {
	for (int i = 0; i < graph->numNodes; i++) {
		if (strcmp(graph->neighbors[i]->name, name) == 0)
			return graph->neighbors[i];
	}
	return NULL;
}
int getVertexIndex(GraphVertex* graph, Vertex* v) {
	if(!graph || !v)
		return -1;
	for (int i = 0; i < graph->numNodes; i++) {
		if(graph->neighbors[i] == v)
			return i;
	}
	return -1;
}
void addNeighbor(Vertex* from, Vertex* to) {
	from->neighbors = (Vertex**)realloc(from->neighbors, (from->numNeighbors + 1) * sizeof(Vertex*));
	from->neighbors[from->numNeighbors++] = to;
}
GraphVertex* readGraphFromFile(const char* filepath) {
	FILE* f = fopen(filepath, "r");
	if (!f) {
		printf("Invalid filepath sent as input\n");
		return NULL;
	}

	GraphVertex* graph = (GraphVertex*)malloc(sizeof(GraphVertex));

	graph->numNodes = countlines(filepath);
	graph->neighbors = (Vertex**)calloc(graph->numNodes, sizeof(Vertex*));

	char buffer[BUFF_SIZE];
	int index = 0;

	while (fgets(buffer, BUFF_SIZE, f)) {
		char participantName[NAME_SIZE];
		int numNeighbors;

		sscanf(buffer, "%s %d", participantName, &numNeighbors);
		graph->neighbors[index++] = createNewVertex(participantName);
	}

	index = 0;
	rewind(f);
	for (int i = 0; i < graph->numNodes; i++) {
		int name[NAME_SIZE];
		int numNeighbors;

		fscanf(f, "%s %d", name, &numNeighbors);

		Vertex* from = getVertex(graph, name);

		for (int j = 0; j < numNeighbors; j++) {
			char neighborName[NAME_SIZE];

			fscanf(f, "%s", neighborName);
			Vertex* to = getVertex(graph, neighborName);

			addNeighbor(from, to);
		}
	}

	fclose(f);
	return graph;
}
void printGraph(GraphVertex* graph) {
	if (!graph)
		return;
	for (int i = 0; i < graph->numNodes; i++) {
		Vertex* curr = graph->neighbors[i];
		printf("%s : ", curr->name);

		for (int j = 0; j < curr->numNeighbors; j++)
			printf("%s ", curr->neighbors[j]->name);
		printf("\n");
	}
}
#pragma endregion

#pragma region FUNCS
int getVertexDegree(Vertex* v) {
	if(!v || !v->neighbors)
		return -1;
	return v->numNeighbors;
}
Vertex* getMaxDegreeVertex(GraphVertex* graph) {
	if (!graph || graph->numNodes == 0)
		return NULL;

	Vertex* maxVertex = graph->neighbors[0];

	for (int i = 1; i < graph->numNodes; i++) {
		if (graph->neighbors[i]->numNeighbors > maxVertex->numNeighbors)
			maxVertex = graph->neighbors[i];
	}

	return maxVertex;
}

#pragma endregion

void DFS(Vertex* v, int* visited, GraphVertex* graph) {
	int index = getVertexIndex(graph, v);
	if (index == -1 || visited[index]) 
		return;

	visited[index] = 1;

	for (int i = 0; i < v->numNeighbors; i++) {
		DFS(v->neighbors[i], visited, graph);
	}
}
int connectedComponents(GraphVertex* graph) {
	if(!graph || !graph->neighbors || !graph->numNodes < 0)
		return -1;

	int *visited = (int*)calloc(graph->numNodes, sizeof(int));
	int count = 0;

	for (int i = 0; i < graph->numNodes; i++) {
		Vertex *v = graph->neighbors[i];
		int vIndex = getVertexIndex(graph,v);

		if (!visited[vIndex])
		{
			DFS(v, visited, graph);
			count++;
		}
	}

	return count;
}

#pragma region TRAVERSAL
#pragma endregion

void main() {
	const char* filepath = "graph.txt";

	GraphVertex* Network = readGraphFromFile(filepath);
	printGraph(Network);

	Vertex *mostPopular = getMaxDegreeVertex(Network);

	printf("The most popular person is %s\n", mostPopular->name);

	int numOfGrups = connectedComponents(Network);
	printf("Number of grups in graph : %d\n", numOfGrups);
}