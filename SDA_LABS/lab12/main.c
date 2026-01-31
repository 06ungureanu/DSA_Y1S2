#include <stdio.h>
#include <stdlib.h>

typedef struct Vertex {
    int name;
    int nr_neigbors;
    int weight;
    struct Vertex **neighbors;
} Vertex;

typedef struct Graph {
    int nr_nodes;
    int nr_edges;
    struct Vertex *nodes;
} Graph;

Graph *createEmptyGraph() {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    if(!g) return NULL;

    g->nodes = NULL;
    g->nr_edges = 0;
    g->nr_nodes = 0;

    return g;
}
void constructGraph(Graph *g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) {
        printf("Error while opening input file\n");
        exit(EXIT_FAILURE);
    }

    
}
int main() {
    Graph *optimopolis = createEmptyGraph();

    const char *filename = "intersectii.txt";
    return 0;
}
