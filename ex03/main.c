/* 
 * Exercicio de implementaÃ§Ã£o (Ã�rvore Geradora MÃ­nima)
 * Nome: Enzo Laragnoit Fernandes
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define INFINITY    1000000
#define INEXISTING  -1
#define false       0
#define true        1

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
                pushfirst(d, vertex);
            }
        }

        g->color[u] = 'b';
    }

    deallocate_deque(d);
}

void _topsort(Graph *g, int u, int *time, Deque *stack) {

    int vertex;

    g->color[u] = 'g';
    g->discovery[u] = *time;
    *time += 1;

    for (Edge *v = g->adj[u]; v != NULL; v = v->next) {
        
        vertex = v->value;
        
        if (g->color[vertex] == 'w') {
            g->predecessor[vertex] = u;
            _topsort(g, vertex, time, stack);
        }
    }

    pushlast(stack, u); // adds the current, now black node, to the topsort stack
    g->color[u] = 'b';
    g->finish[u] = *time;
    *time += 1;
}

void topsort(Graph *g, int start) {

    int time = 0;
    Deque *stack = deque_constructor();

    // only these three are required for DFS
    g->color = (char*) malloc(sizeof(char) * g->n_vertices);
    g->topsort = (int*) malloc(sizeof(int) * g->n_vertices);
    g->discovery = (int*) malloc(sizeof(int) * g->n_vertices);
    g->finish = (int*) malloc(sizeof(int) * g->n_vertices);
    g->predecessor = (int*) malloc(sizeof(int) * g->n_vertices);

    for (int i = 0; i < g->n_vertices; i++) {
        g->color[i] = 'w';
        g->predecessor[i] = INEXISTING;
        g->discovery[i] = INEXISTING;
        g->finish[i] = INEXISTING;
    }

    // starts the dfs at the specified node
    _topsort(g, start, &time, stack);

    for (int i = 0; i < g->n_vertices; i++) {
        if (g->color[i] == 'w') {
            _topsort(g, i, &time, stack);
        }
    }

    for (int i = 0; i < g->n_vertices; i++) {
        g->topsort[i] = poplast(stack); // if the stack is empty -1 will be returned
    }

    deallocate_deque(stack);
}

int max_weight_vertex(int *Q, int *weights, int length) {
    int max = -INFINITY;
    int max_index = -1;

    for (int v = 0; v < length; v++) {
        if ((Q[v] != 0) && (weights[v] > max)) {
            max = weights[v];
            max_index = v;
        }
    }

    return max_index;
}

int min_weight_vertex(int *Q, int *weights, int length) {
    int min = INFINITY;
    int min_index = -1;

    for (int v = 0; v < length; v++) {
        if ((Q[v] != 0) && (weights[v] < min)) {
            min = weights[v];
            min_index = v;
        }
    }

    return min_index;
}

int any_occurrence(int *v, int length, int value) {
    for (int i = 0; i < length; i++)
        if (v[i] == value)
            return true;
    return false;
}

int prim(Graph *g, int root) {
    g->predecessor = (int*) malloc(sizeof(int) * g->n_vertices);
    
    int *Q = (int*) malloc(sizeof(int) * g->n_vertices);
    int *weight = (int*) malloc(sizeof(int*) * g->n_vertices);
    int spanning_tree_weight = 0;

    for (int i = 0; i < g->n_vertices; i++) {
        Q[i] = 1;
        g->predecessor[i] = INEXISTING;
        weight[i] = -INFINITY;
    }

    weight[root] = 0;

    while (any_occurrence(Q, g->n_vertices, 1)) {

        int u = max_weight_vertex(Q, weight, g->n_vertices);
        Q[u] = 0;
        spanning_tree_weight += weight[u];

        for (Edge *e = g->adj[u]; e != NULL; e = e->next) {
            if ((Q[e->value] == 1) && (weight[e->value] < e->weight)) {
                weight[e->value] = e->weight;
                g->predecessor[e->value] = u;
            }
        }
    }

    return spanning_tree_weight;
}


void process_answer(Graph *g) {
    int *vertices = (int*) malloc(sizeof(int) * g->n_vertices);
    int *predecessor = (int*) malloc(sizeof(int) * g->n_vertices); 
    int tmp;

    // copying the arrays to temporary storage
    for (int i = 0; i < g->n_vertices; i++) {
        predecessor[i] = g->predecessor[i];
        vertices[i] = i;
    }

    // sorting the edge orientations (u, U) such that u < U forall edges
    for (int i = 0; i < g->n_vertices; i++) {
        if (vertices[i] > predecessor[i]) {
            tmp = vertices[i];
            vertices[i] = predecessor[i];
            predecessor[i] = tmp;
        }
    }

    for (int i = 0; i < g->n_vertices; i++) {
        for (int j = 0; j < g->n_vertices; j++) {
            if (vertices[i] < vertices[j] ||
                    (vertices[i] == vertices[j] && predecessor[i] < predecessor[j])) {
                tmp = vertices[i];
                vertices[i] = vertices[j];
                vertices[j] = tmp;

                tmp = predecessor[i];
                predecessor[i] = predecessor[j];
                predecessor[j] = tmp;
            }
        }
    }

    // printing the anwer to the stdout
    for (int i = 1; i < g->n_vertices; i++)
        printf("%d %d\n", vertices[i], predecessor[i]);
} 


int main() {
    
    int n_edges, n_vertices;
    int src, dst, weight;

    scanf("%i %i", &n_vertices, &n_edges);

    Graph *g = graph_constructor(n_vertices, n_edges);

    for (int i = 0; i < n_edges; i++) {
        scanf("%i %i %i", &src, &dst, &weight);
        insert_edge(g, src, dst, weight);
        insert_edge(g, dst, src, weight);
    }

    prim(g, 0);

    process_answer(g);

    deallocate_graph(g);

    return 0;
}
