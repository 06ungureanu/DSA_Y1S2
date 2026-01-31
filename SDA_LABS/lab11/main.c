#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct graphMatrix {
    int **adjencyMatrix;
    int numNodes;
} graphMatrix;

typedef struct edge {
    int leftNode;
    int rightNode;
} edge;

typedef struct grapEdges {
    edge *edges;
    int numNodes;
    int numEdges;
} grapEdges;

typedef struct vertex {
    int name;
    struct vertex **neighbors;
    int numNeighbors;
} vertex;

typedef struct graphVertex {
    int numNodes;
    int numEdges;
    struct vertex *nodes;
} graphVertex;

typedef struct Node {
    int key;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

int countlines(const char *filename) {
    FILE *f = fopen(filename, "r");
    if(f == NULL) return 0;

    int lines = 0;
    char ch;
    while((ch = fgetc(f)) != EOF) {
        if(ch == '\n') lines++;
    }

    fclose(f);
    return lines;
}
int **alocateMatrixNULL(int n) {
    int **m = (int**)malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++) {
        m[i] = (int*)malloc(n * sizeof(int));
    }
    return m;
}
Queue *createEmptyQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if(!q) return NULL;

    q->front = NULL;
    q->rear = NULL;
    
    return q;
}
Node *createNode(int key) {
    Node *temp = (Node*)malloc(sizeof(Node));
    if(!temp) return NULL;

    temp->key = key;
    temp->next = NULL;

    return temp;
}
void enqueue(Queue *q, int key) {
    if(!q) return;

    Node *temp = createNode(key);
    if(!temp) return;

    if(q->front == NULL && q->rear == NULL) {
        q->front = temp;
        q->rear = temp;
        return;
    }
    else 
    {
        q->rear->next = temp;
        q->rear = temp;
    }
}
int dequeue(Queue *q) {
    if(q == NULL || q->front == NULL) return -1;

    Node *temp = q->front;
    int key = temp->key;
    q->front = q->front->next;

    if(q->front == NULL) q->rear = NULL;

    free(temp);

    return key;
}
bool QueueisEmpty(Queue *q) {
    return q->front == NULL;
}
graphVertex *createNullVertex() {
    graphVertex *graph = (graphVertex*)malloc(sizeof(graphVertex));
    if(graph == NULL) return NULL;

    graph->nodes = NULL;
    graph->numEdges = 0;
    graph->numNodes = 0;

    return graph;
}
graphMatrix *createNULLAdjGraph() {
    graphMatrix *graph = (graphMatrix*)malloc(sizeof(graphMatrix));
    if(graph == NULL) return NULL;

    graph->adjencyMatrix = NULL;
    graph->numNodes = 0;

    return graph;
}
void readGraph(graphMatrix *graph, const char *filename) {
    if(graph == NULL) return;

    FILE *f = fopen(filename, "r");
    if(f == NULL) return;

    graph->numNodes = countlines(filename);
    graph->adjencyMatrix = alocateMatrixNULL(graph->numNodes);

    for(int i = 0; i < graph->numNodes; i++) {
        for(int j = 0; j < graph->numNodes; j++) {
            int value = 0;
            fscanf(f, "%d",&value);
            graph->adjencyMatrix[i][j] = value;
        }
    } 
    fclose(f);
}
void printGraphMatrix(graphMatrix *graph) {
    if(graph == NULL || graph->adjencyMatrix == NULL) return;
    
    printf("The adjency Matrix of given graph\n\n");
    for(int i = 0; i < graph->numNodes; i++,printf("\n")) {
        for(int j = 0; j < graph->numNodes; j++) {
            printf("%d ", graph->adjencyMatrix[i][j]);
        }
    }
}
vertex *getVertexNode(graphVertex *graph, int name) {
    if(!graph) return NULL;

    for(int i = 0; i < graph->numNodes; i++) {
        if(graph->nodes[i].name == name) 
            return &graph->nodes[i]; 
    }
    return NULL;
}
void addNeighbor(vertex *from, vertex *to) {
    if(!from || !to) return;

    from->neighbors = realloc(from->neighbors, (from->numNeighbors + 1) * sizeof(vertex));
    from->neighbors[from->numNeighbors++] = to;
}
void readVertex(graphVertex *graph, const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if(!f) {
        printf("E: Invalid filepath\n");
        return;
    }

    fscanf(f, "%d %d", &graph->numNodes, &graph->numEdges);
    graph->nodes = (vertex*)malloc(graph->numNodes * sizeof(vertex));
    if(!graph->nodes) return; 

    
    for(int i = 0; i < graph->numNodes; i++) {
        graph->nodes[i].name = i + 1; 
        graph->nodes[i].neighbors = NULL; 
        graph->nodes[i].numNeighbors = 0;
    } 
    
    int x,y;
    while(fscanf(f, "%d %d", &x, &y) == 2) {
        vertex *from = getVertexNode(graph, x);
        vertex *to = getVertexNode(graph, y);
        addNeighbor(from, to);
    }
    fclose(f);
}
void printVertex(graphVertex *graph) {
    if(graph == NULL || graph->nodes == NULL) return;

    printf("Vertex list of given graph\n\n");
    for(int i = 0; i < graph->numNodes; i++) {
        printf("%d : ", graph->nodes[i].name);
        for(int j = 0; j < graph->nodes[i].numNeighbors; j++) 
            printf("%d ", graph->nodes[i].neighbors[j]->name);
        printf("\n");
    }
}
grapEdges *createNullEdgeGraph() {
    grapEdges *graph = (grapEdges*)malloc(sizeof(grapEdges));
    if(graph == NULL) return NULL;

    graph->edges = NULL;
    graph->numEdges = 0;
    graph->numNodes = 0;

    return graph;
} 
void readEdgeList(grapEdges *graph, const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if(f == NULL) return;
    
    
    fscanf(f, "%d %d", &graph->numNodes, &graph->numEdges);
    
    graph->edges = (edge*)malloc(graph->numEdges * sizeof(edge));
    for(int i = 0; i < graph->numEdges; i++) {
        fscanf(f, "%d %d", &graph->edges[i].leftNode, &graph->edges[i].rightNode);
    } 
    fclose(f);
}
void printEdgeList(grapEdges *graph) {
    if(graph == NULL || graph->edges == NULL) return;
   
    printf("Edge list of given graph:\n\n");
    for(int i = 0; i < graph->numEdges; i++)
        printf("Left : %d, Right : %d\n", graph->edges[i].leftNode, graph->edges[i].rightNode);
    
}
int getDegreeOutMatrix(graphMatrix *graph, int node) {
    if(graph == NULL || graph->adjencyMatrix == NULL) return -1;
    
    int degree = 0;
    for(int i = 0; i < graph->numNodes; i++) {
        if(graph->adjencyMatrix[node][i] == 1) degree++;
    }

    return degree;
}
int getDegreeOutEdgeList(grapEdges *graph, int node) {
    if(graph == NULL || graph->edges == NULL) return -1;
    
    int degree = 0;
    for(int i = 0; i < graph->numEdges; i++) {
        if(graph->edges[i].leftNode == node) degree++;
    }
    return degree;
} 
int getVertexOutDegree(graphVertex *graph, int node) {
    if(graph == NULL || graph->nodes == NULL) return -1;

    for(int i = 0; i < graph->numNodes; i++) {
        if(graph->nodes[i].name == node) {
            return graph->nodes[i].numNeighbors;
        }
    }
    return -1;
}
void DFS(graphVertex *graph, bool *visited,int start) {
    visited[start] = 1;
    printf("%d ", start + 1);

    for(int i = 0; i < graph->nodes[start].numNeighbors; i++) {
        int adjVertex = graph->nodes[start].neighbors[i]->name - 1;
        if(!visited[adjVertex])
            DFS(graph, visited, adjVertex);
    }

} 
void DFS_Helper(graphVertex *graph) {
    bool *visited = (bool*)malloc(graph->numNodes * sizeof(bool));
    
    for(int i = 0; i < graph->numNodes;i++)
        visited[i] = 0;
    DFS(graph, visited, 0);
    
    free(visited);
    printf("\n");
}
int main() {
    const char *adj_path = "adj.txt";
    const char *edges_path = "edges.txt";
    const char *adiac_path = "adiac.txt";

    graphMatrix *graph = createNULLAdjGraph();
    grapEdges *edgeGraph = createNullEdgeGraph();
    graphVertex *vertexgraph = createNullVertex();

    readGraph(graph, adj_path);
    printGraphMatrix(graph);

    readEdgeList(edgeGraph,edges_path);
    printEdgeList(edgeGraph);

    readVertex(vertexgraph,adiac_path);
    printVertex(vertexgraph);

    int matrixOutDegree = getDegreeOutMatrix(graph, 1);
    int edgeOutDegree = getDegreeOutEdgeList(edgeGraph,1);
    int VertexOutDegree  = getVertexOutDegree(vertexgraph, 1);

    printf("\nOut degree for node 1 in matrix graph : %d\n", matrixOutDegree);
    printf("Out degree for node 1 in edge graph : %d\n", edgeOutDegree);
    printf("Out degree for node 1 in vertex graph : %d\n", VertexOutDegree);
   
    printf("\n");
    return 0;
}
