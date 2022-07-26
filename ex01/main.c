#include <stdio.h>
#include <stdlib.h>

typedef struct queue_node {
    
} QNode;


typedef struct edge {
    int value;
    int weight;
    struct edge *next;
} Edge;

typedef struct graph {
    // lazy properties
    int *distance;
    int *discovery;
    int *finish;
    int *predecessor;
    int *topsort;

    // structural properties
    int n_vertices;
    int n_edges;
    
    // adjacency list
    Edge **adj;
} Graph;

Graph* graph_constructor(int n_vertices, int n_edges) {

    Graph *new_graph;
    new_graph = malloc(sizeof(Graph));

    new_graph->distance = NULL;
    new_graph->discovery = NULL;
    new_graph->finish = NULL;
    new_graph->predecessor = NULL;
    new_graph->topsort = NULL;

    new_graph->n_vertices = n_vertices;
    new_graph->n_edges = n_edges;

    new_graph->adj = malloc(sizeof(Edge*) * n_vertices);

    for (int i = 0; i < n_vertices; i++) {
        new_graph->adj[i] = NULL;
    }
     
    return new_graph;
}

void deallocate_graph(Graph *g) {
    free(g->distance);
    free(g->discovery);
    free(g->finish);
    free(g->predecessor);
    free(g->topsort);
    
    for (int i = 0; i < g->n_vertices; i++) {
        while (g->adj[i] != NULL) {
            Edge *tmp = g->adj[i];
            g->adj[i] = g->adj[i]->next;
            free(tmp);
        }
    }
}

void insert_edge(Graph *g, int src, int dst, int weight) {

    Edge *new_edge; 
    new_edge = malloc(sizeof(Edge));

    new_edge->value = dst;
    new_edge->weight = weight;

    if (g->adj[src] == NULL) {
        g->adj[src] = new_edge;
        new_edge->next = NULL;
    } else {
        new_edge->next = g->adj[src];
        g->adj[src] = new_edge;
    }
}


void show_graph(Graph *g) {
    for (int i = 0; i < g->n_vertices; i++) {
        printf("[%i]:", i);
        for (Edge *e = g->adj[i]; e != NULL; e = e->next) {
            printf(" %i", e->value);
        }
        printf("\n");
    }
}


void bfs(Graph *g) {

}


int main() {
    
    int n_edges, n_vertices;
    int src, dst;

    scanf("%i %i", &n_vertices, &n_edges);

    Graph *g = graph_constructor(n_vertices, n_edges);

    for (int i = 0; i < n_edges; i++) {
        scanf("%i %i", &src, &dst);
        insert_edge(g, src, dst, 1);
        insert_edge(g, dst, src, 1);
    }

    show_graph(g);
    deallocate_graph(g);

    return 0;
}
