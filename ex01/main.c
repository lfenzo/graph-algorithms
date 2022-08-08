/* Exercicio de implementação (Busca em Largura)
 * Nome: Enzo Laragnoit Fernandes
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define INFINITY    1000000
#define INEXISTING  -1

//
//
// Deque Implementation
//
//

typedef struct deque_node {
    struct deque_node *next;
    struct deque_node *prev;
    int value;
} Node;

typedef struct deque {
    Node *start;
    Node *end;
    int quantity;
} Deque;

Deque* deque_constructor() {
    Deque *d = (Deque*) malloc(sizeof(Deque));
    d->start = NULL;
    d->end = NULL;
    d->quantity = 0;
    return d;
}

void pushfirst(Deque *d, int value) {

    Node *new = (Node*) malloc(sizeof(Node));
    new->value = value;

    if (d->start == NULL) {
        d->start = new;
        d->end = new;
        new->next = NULL;
        new->prev = NULL;
    } else {
        d->start->prev = new;
        new->next = d->start;
        new->prev = NULL;
        d->start = new;
    }

    d->quantity += 1;
}

void pushlast(Deque *d, int value) {

    Node *new = (Node*) malloc(sizeof(Node));
    new->value = value;

    if (d->start == NULL) {
        d->start = new;
        d->end = new;
        new->next = NULL;
        new->prev = NULL;
    } else {
        d->end->next = new;
        new->next = NULL;
        new->prev = d->end;
        d->end = new;
    }

    d->quantity += 1;
}

int popfirst(Deque *d) {
    Node *deleted_node; 
    int value;

    if (d->quantity == 0)
        return -1;
    else {

        deleted_node = d->start;

        if (d->quantity == 1) {
            d->start = NULL;
            d->end = NULL;
        } else {
            d->start = d->start->next;
            d->start->prev = NULL;
        }
    }
    
    d->quantity -= 1;
    value = deleted_node->value;
    deleted_node->next = NULL;
    deleted_node->prev = NULL;
    free(deleted_node);

    return value;
}

int poplast(Deque *d) {
    Node *deleted_node; 
    int value;

    if (d->quantity == 0)
        return -1;
    else {

        deleted_node = d->end;

        if (d->quantity == 1) {
            d->start = NULL;
            d->end = NULL;
        } else {
            d->end = d->end->prev;
            d->end->next = NULL;
        }
    }
    
    d->quantity -= 1;
    value = deleted_node->value;
    deleted_node->next = NULL;
    deleted_node->prev = NULL;
    free(deleted_node);

    return value;
}

void show_deque(Deque *d) {
    printf("showing deque: ");
    for (Node *n = d->start; n != NULL; n = n->next) {
        printf("%d ", n->value);
    }
    printf("\n");
}

void deallocate_deque(Deque *d) {
    while (d->start != NULL) {
        Node *temp = d->start;
        d->start = d->start->next;
        free(temp);
    }
}

int isempty(Deque *d) {
    return (d->quantity == 0);
}

//
//
// Graph Implementation
//
//

typedef struct edge {
    int value;
    int weight;
    struct edge *next;
} Edge;

typedef struct graph {
    // lazy properties
    char *color; // 'b', 'g', 'w'
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

void bfs(Graph *g, int start) {

    // only these three are required for VFS
    g->color = (char*) malloc(sizeof(char) * g->n_vertices);
    g->distance = (int*) malloc(sizeof(int) * g->n_vertices);
    g->predecessor = (int*) malloc(sizeof(int) * g->n_vertices);

    // lazy variables initialization
    for (int i = 0; i < g->n_vertices; i++) {
        g->color[i] = 'w';
        g->distance[i] = INFINITY;
        g->predecessor[i] = INEXISTING;
    }

    // initializing the root node
    g->color[start] = 'g';
    g->distance[start] = 0;
    g->predecessor[start] = -1;

    Deque *d = deque_constructor();
    int vertex;
    pushlast(d, start);

    while (!isempty(d)) {

        int u = popfirst(d);

        for (Edge *v = g->adj[u]; v != NULL; v = v->next) {

            vertex = v->value;

            if (g->color[vertex] == 'w') {
                g->color[vertex] = 'g';
                g->distance[vertex] = g->distance[u] + 1;
                g->predecessor[vertex] = u;
                pushlast(d, vertex);
            }
        }

        g->color[u] = 'b';
    }

    deallocate_deque(d);
}


int any_infinity(int *v, int length) {
    for (int i = 0; i < length; i++)
        if (v[i] == INFINITY)
            return 1;
    return 0;
}

int max(int *v, int length) {
    int maximum = -INFINITY;

    for (int i = 0; i < length; i++)
        if (v[i] > maximum)
            maximum = v[i];

    return maximum;
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

    bfs(g, 0);

    // answer 
    if (any_infinity(g->distance, g->n_vertices))
        printf("infinito\n");
    else
        printf("%d\n", max(g->distance, g->n_vertices));

    deallocate_graph(g);

    return 0;
}
