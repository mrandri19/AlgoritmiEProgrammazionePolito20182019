#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} edge;

bool is_vertex_cover(int* subset, int subset_len, edge* edges, int edges_len)
{
    // At the start no vertex has been touched
    int* touched = malloc(edges_len * sizeof(int));
    for (int i = 0; i < edges_len; i++) {
        touched[i] = 0;
    }

    // Foreach vertex of the possible solution
    for (int i = 0; i < subset_len; i++) {
        int vertex = subset[i];

        // if this vertex touches an edge, mark that edge
        for (int j = 0; j < edges_len; j++) {
            if (edges[j].a == vertex || edges[j].b == vertex) {
                touched[j] = 1;
            }
        }
    }

    for (int i = 0; i < edges_len; i++) {
        if (!touched[i])
            return false;
    }

    free(touched);

    return true;
}

// Disposizioni ripetute
void powerset_rec(int* vertexes, int* sol, int sol_pos, int sol_len, edge* edges, int edges_len)
{
    if (sol_pos == sol_len) {
        // Count the element of the subset
        int subset_len = 0;
        for (int i = 0; i < sol_len; i++) {
            if (sol[i]) {
                subset_len++;
            }
        }
        // Insert the vertex elements into the subset array according to sol
        int* subset = calloc(subset_len, sizeof(int));
        int ix = 0;
        for (int i = 0; i < sol_len; i++) {
            if (sol[i]) {
                subset[ix++] = vertexes[i];
            }
        }

        if (is_vertex_cover(subset, subset_len, edges, edges_len)) {
            // Print the subset array
            printf("[");
            for (int i = 0; i < subset_len; i++) {
                printf("%d, ", subset[i]);
            }
            printf("\b\b]\n");
        }

        free(subset);

        return;
    }

    sol[sol_pos] = 0;
    powerset_rec(vertexes, sol, sol_pos + 1, sol_len, edges, edges_len);
    sol[sol_pos] = 1;
    powerset_rec(vertexes, sol, sol_pos + 1, sol_len, edges, edges_len);
}

void powerset(int* vertexes, int vertexes_len, edge* edges, int edges_len)
{
    int* solution = calloc(vertexes_len, sizeof(int));

    powerset_rec(vertexes, solution, 0, vertexes_len, edges, edges_len);

    free(solution);
}

int main()
{
    // Setup
    FILE* fp = fopen("grafo.txt", "r");
    if (fp == NULL) {
        perror("Cannot open grafo.txt");
        return 1;
    }

    int vertexes_len, edges_len;
    fscanf(fp, "%d %d ", &vertexes_len, &edges_len);

    // Build vertexes array
    int* vertexes = calloc(vertexes_len, sizeof(int));
    for (int i = 0; i < vertexes_len; i++) {
        vertexes[i] = i;
    }

    // Build edges array
    edge* edges = calloc(edges_len, sizeof(edge));
    for (int i = 0; i < edges_len; i++) {
        fscanf(fp, "%d %d", &edges[i].a, &edges[i].b);
    }

    // Generate using powerset all of the possible subsets of the graph
    powerset(vertexes, vertexes_len, edges, edges_len);

    // Cleanup
    free(edges);
    free(vertexes);
    fclose(fp);
    return 0;
}
