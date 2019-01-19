#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTEX_LEN 30 + 1

int linear_search(char *needle, char **haystack, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    if (strcmp(needle, haystack[i]) == 0)
      return (int)i;
  }
  return -1;
}

// j is an ancestor of i, it does this by checking if they have a common
// ancestor
bool is_ancestor(int j, int i, int *parent) {
  while (parent[j] != j)
    j = parent[j];
  while (parent[i] != i)
    i = parent[i];
  return i == j;
}

static int time = 0;
static bool has_cycle = false;

// Perform dfs on graph G, starting from vertex i, populating the visited array
// of length vertexes_len
void dfs(int **G, size_t i, int *start, int *end, int *parent,
         size_t vertexes_len) {
  start[i] = time++;

  // dfs on all the edges of this vertex
  for (size_t j = 0; j < vertexes_len; ++j) {
    if (G[i][j] != 0) {
      if (start[j] == -1) {
        parent[j] = i;
        dfs(G, j, start, end, parent, vertexes_len);
      } else {
        // If the vertex to visit has been discovered before than us and it's
        // our ancestor
        if ((start[j] < start[i]) && is_ancestor(j, i, parent)) {
          has_cycle = true;
        }
      }
    }
  }

  end[i] = time++;
}

bool detect_cycles(int **G, size_t vertexes_len) {
  // Create the start, end arrays to be populated in the DFS
  int *start = calloc(vertexes_len, sizeof(int));
  for (size_t i = 0; i < vertexes_len; ++i) {
    start[i] = -1;
  }

  int *end = calloc(vertexes_len, sizeof(int));
  for (size_t i = 0; i < vertexes_len; ++i) {
    end[i] = -1;
  }

  int *parent = calloc(vertexes_len, sizeof(int));
  for (size_t i = 0; i < vertexes_len; ++i) {
    parent[i] = (int)i;
  }

  has_cycle = false;
  // Perform the DFS of all vertexes
  for (size_t i = 0; i < vertexes_len; ++i) {
    if (start[i] == -1) {
      dfs(G, i, start, end, parent, vertexes_len);
    }
  }

  // Cleanup
  free(parent);
  free(end);
  free(start);

  return has_cycle;
}

typedef struct {
  int a;
  int b;
  int weight;
} edge_t;

void populate_matrix_from_edge_array(int **G, bool *solution,
                                     size_t solution_len, edge_t *edges) {
  for (size_t i = 0; i < solution_len; ++i) {
    if (solution[i]) {
      G[edges[i].a][edges[i].b] = edges[i].weight;
    } else {
      G[edges[i].a][edges[i].b] = 0;
    }
  }
}

int cardinality(bool *a, size_t len) {
  int b = 0;
  for (size_t i = 0; i < len; ++i) {
    if (a[i])
      b++;
  }
  return b;
}

int weight(bool *solution, size_t len, edge_t *edges) {
  int total = 0;

  // If the i-th edge is in the solution then add its weight to the total
  for (size_t i = 0; i < len; ++i) {
    if (solution[i])
      total += edges[i].weight;
  }
  return total;
}

void powerset(size_t index, bool *solution, size_t solution_len,
              bool *dag_edges_min_cardinality, bool *dag_edges_min_weight,
              edge_t *edges, char **vertexes, size_t vertexes_len) {
  if (index == solution_len) {
    int **G = calloc(vertexes_len, sizeof(int *));
    for (size_t i = 0; i < vertexes_len; ++i) {
      G[i] = calloc(vertexes_len, sizeof(int));
    }

    populate_matrix_from_edge_array(G, solution, solution_len, edges);

    if (!detect_cycles(G, vertexes_len)) {
      if (cardinality(solution, solution_len) >
          cardinality(dag_edges_min_cardinality, solution_len)) {
        // Copy the new best solution
        for (size_t i = 0; i < solution_len; ++i) {
          dag_edges_min_cardinality[i] = solution[i];
        }
      }

      if (weight(solution, solution_len, edges) >
          weight(dag_edges_min_cardinality, solution_len, edges)) {
        // Copy the new best solution
        for (size_t i = 0; i < solution_len; ++i) {
          dag_edges_min_weight[i] = solution[i];
        }
      }
    }

    for (size_t i = 0; i < vertexes_len; ++i) {
      free(G[i]);
    }
    free(G);

    return;
  }

  solution[index] = false;
  powerset(index + 1, solution, solution_len, dag_edges_min_cardinality,
           dag_edges_min_weight, edges, vertexes, vertexes_len);

  solution[index] = true;
  powerset(index + 1, solution, solution_len, dag_edges_min_cardinality,
           dag_edges_min_weight, edges, vertexes, vertexes_len);
}

void swap(int *A, size_t a, size_t b) {
  int tmp = A[a];
  A[a] = A[b];
  A[b] = tmp;
}

void find_max_distances_dag(int **G, size_t vertexes_len, int source,
                            int **distances_, int **topological_order_) {
  // Perform topological sort, then apply relaxation in that order

  // Variables init
  int *start = malloc(vertexes_len * sizeof(int));
  for (size_t i = 0; i < vertexes_len; i++) {
    start[i] = -1;
  }
  int *end = calloc(vertexes_len, sizeof(int));
  int *parent = calloc(vertexes_len, sizeof(int));
  int *topological_order = calloc(vertexes_len, sizeof(int));
  for (size_t i = 0; i < vertexes_len; i++) {
    topological_order[i] = (int)i;
  }

  int *distances = calloc(vertexes_len, sizeof(int));
  for (size_t i = 0; i < vertexes_len; i++) {
    distances[i] = -1;
  }
  distances[source] = 0;

  // Restore time since it's a global variable
  time = 0;
  // Apply DFS to all vertexes
  for (size_t i = 0; i < vertexes_len; i++) {
    if (start[i] == -1) {
      dfs(G, i, start, end, parent, vertexes_len);
    }
  }

  // Bubble sort :|
  for (size_t i = 0; i < vertexes_len; i++) {
    for (size_t j = 0; j < vertexes_len - i - 1; j++) {
      if (end[j] < end[j + 1]) {
        swap(end, j, j + 1);
        swap(topological_order, j, j + 1);
      }
    }
  }

  // For each vertex, in topological order
  for (size_t i = 0; i < vertexes_len; i++) {
    // For each of its edges
    for (size_t j = 0; j < vertexes_len; j++) {
      int edge_len = G[topological_order[i]][j];
      if (edge_len != 0) {
        int relaxed_distance = distances[topological_order[i]] + edge_len;
        // Apply relaxation
        if (distances[j] < relaxed_distance) {
          distances[j] = relaxed_distance;
        }
      }
    }
  }
  (*distances_) = distances;
  (*topological_order_) = topological_order;

  // Cleanup
  free(parent);
  free(end);
  free(start);
}

int main() {
  // Read file
  FILE *fp = fopen("grafo.txt", "r");
  if (fp == NULL) {
    perror("cannot open grafo.txt");
    exit(EXIT_FAILURE);
  }

  // Read vertexes len
  size_t vertexes_len;
  fscanf(fp, "%zu ", &vertexes_len);

  // Allocate vertex names vector
  char **vertexes = calloc(vertexes_len, sizeof(char *));
  for (size_t i = 0; i < vertexes_len; ++i) {
    vertexes[i] = calloc(VERTEX_LEN, sizeof(char));
  }

  // Read vertex names
  for (size_t i = 0; i < vertexes_len; ++i) {
    fscanf(fp, "%s ", vertexes[i]);
  }

  // Allocate adjacency matrix
  int **G = calloc(vertexes_len, sizeof(int *));
  for (size_t i = 0; i < vertexes_len; ++i) {
    G[i] = calloc(vertexes_len, sizeof(int));
  }

  // Read all edges and fill edges array
  char a[VERTEX_LEN], b[VERTEX_LEN];
  int weight;

  size_t edges_len = 0;
  edge_t *edges = calloc(2 * vertexes_len, sizeof(edge_t));

  while (fscanf(fp, "%s %s %d ", a, b, &weight) != EOF) {
    int a_index = linear_search(a, vertexes, vertexes_len);
    assert(a_index > -1);

    int b_index = linear_search(b, vertexes, vertexes_len);
    assert(b_index > -1);

    G[a_index][b_index] = weight;
    edges[edges_len++] = (edge_t){.a = a_index, .b = b_index, .weight = weight};
  }

  // Detect if there are any cycles in the graph
  if (detect_cycles(G, vertexes_len)) {
    printf("There are cycles\n");
  } else {
    printf("There are no cycles\n");
  }

  bool *dag_edges_min_cardinality, *dag_edges_min_weight, *solution;
  dag_edges_min_cardinality = calloc(edges_len, sizeof(bool));
  dag_edges_min_weight = calloc(edges_len, sizeof(bool));
  solution = calloc(edges_len, sizeof(bool));

  for (size_t i = 0; i < vertexes_len; ++i) {
    for (size_t j = 0; j < vertexes_len; ++j) {
      printf("%3d", G[i][j]);
    }
    printf("\n");
  }

  // Perform powerset of edges to find the subsets which make the graph a
  // DAG powerset, and in the check function use detect_cycles
  powerset(0, solution, edges_len, dag_edges_min_cardinality,
           dag_edges_min_weight, edges, vertexes, vertexes_len);

  for (size_t i = 0; i < edges_len; ++i) {
    printf("%d", dag_edges_min_cardinality[i]);
  }
  printf("\n");
  populate_matrix_from_edge_array(G, dag_edges_min_cardinality, edges_len,
                                  edges);
  printf("Min cardinality of removed edges\n");
  for (size_t i = 0; i < vertexes_len; ++i) {
    for (size_t j = 0; j < vertexes_len; ++j) {
      printf("%3d", G[i][j]);
    }
    printf("\n");
  }

  populate_matrix_from_edge_array(G, dag_edges_min_weight, edges_len, edges);
  for (size_t i = 0; i < edges_len; ++i) {
    printf("%d", dag_edges_min_weight[i]);
  }
  printf("\n");
  printf("Min weight of removed edges\n");
  for (size_t i = 0; i < vertexes_len; ++i) {
    for (size_t j = 0; j < vertexes_len; ++j) {
      printf("%3d", G[i][j]);
    }
    printf("\n");
  }

  // Allocate all path from each vertex
  int **paths = calloc(vertexes_len, sizeof(int *));
  for (size_t i = 0; i < vertexes_len; ++i) {
    paths[i] = calloc(vertexes_len, sizeof(int));
  }

  // Find all the distances from the source node and every other node of the min
  // weight DAG
  int *distances;
  int *topological_order;
  find_max_distances_dag(G, vertexes_len, 0, &distances, &topological_order);

  for (size_t i = 0; i < vertexes_len; i++) {
    printf("%c: %d\n", topological_order[i] + 'a',
           distances[topological_order[i]]);
  }
  free(distances);

  // Cleanup
  for (size_t i = 0; i < vertexes_len; ++i) {
    free(paths[i]);
  }
  free(paths);

  free(dag_edges_min_cardinality);
  free(dag_edges_min_weight);
  free(solution);

  free(edges);

  for (size_t i = 0; i < vertexes_len; ++i) {
    free(G[i]);
  }
  free(G);

  for (size_t i = 0; i < vertexes_len; ++i) {
    free(vertexes[i]);
  }
  free(vertexes);

  fclose(fp);

  return 0;
}
