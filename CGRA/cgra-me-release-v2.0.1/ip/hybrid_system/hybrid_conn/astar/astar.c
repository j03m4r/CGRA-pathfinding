#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// ============================================================================
// CSR GRAPH STRUCTURE
// ============================================================================

typedef struct {
    int num_nodes;
    int num_edges;
    
    int* row_ptr;      // Size: num_nodes + 1
    int* col_idx;      // Size: num_edges (neighbor node IDs)
    int* weights;      // Size: num_edges (edge weights)
    int* positions_x;  // Node x positions
    int* positions_y;  // Node y positions
} CSRGraph;

// ============================================================================
// HEURISTIC
// ============================================================================

static inline double heuristic(const CSRGraph* graph, int node_a, int node_b) {
    double dx = (double)(graph->positions_x[node_a] - graph->positions_x[node_b]);
    double dy = (double)(graph->positions_y[node_a] - graph->positions_y[node_b]);
    return sqrt(dx * dx + dy * dy);
}

// ============================================================================
// A* IMPLEMENTATION (Sequential version for CGRA)
// ============================================================================

typedef struct {
    int neighbor;
    double tentative_g;
    int parent;
} Candidate;

// Simple priority queue for open set (array-based)
typedef struct {
    int* nodes;
    double* f_scores;
    int size;
    int capacity;
} OpenSet;

static void openset_init(OpenSet* os, int capacity) {
    os->nodes = (int*)malloc(capacity * sizeof(int));
    os->f_scores = (double*)malloc(capacity * sizeof(double));
    os->size = 0;
    os->capacity = capacity;
}

static void openset_free(OpenSet* os) {
    free(os->nodes);
    free(os->f_scores);
}

static void openset_add(OpenSet* os, int node, double f_score) {
    if (os->size < os->capacity) {
        os->nodes[os->size] = node;
        os->f_scores[os->size] = f_score;
        os->size++;
    }
}

static int openset_pop_min(OpenSet* os) {
    if (os->size == 0) return -1;
    
    int min_idx = 0;
    double min_f = os->f_scores[0];
    
    for (int i = 1; i < os->size; i++) {
        if (os->f_scores[i] < min_f) {
            min_f = os->f_scores[i];
            min_idx = i;
        }
    }
    
    int node = os->nodes[min_idx];
    
    // Remove by swapping with last element
    os->nodes[min_idx] = os->nodes[os->size - 1];
    os->f_scores[min_idx] = os->f_scores[os->size - 1];
    os->size--;
    
    return node;
}

static bool openset_contains(OpenSet* os, int node) {
    for (int i = 0; i < os->size; i++) {
        if (os->nodes[i] == node) return true;
    }
    return false;
}

// Reconstruct path
static int reconstruct_path(int* parent, int goal, int* path_out) {
    int path_len = 0;
    int cur = goal;
    
    // Count length
    int temp = cur;
    while (parent[temp] != -1) {
        path_len++;
        temp = parent[temp];
    }
    path_len++; // Include start
    
    // Build path in reverse
    int idx = path_len - 1;
    while (parent[cur] != -1) {
        path_out[idx--] = cur;
        cur = parent[cur];
    }
    path_out[0] = cur;
    
    return path_len;
}

// Sequential A* for CGRA
static int sequential_a_star_csr(const CSRGraph* graph,
                                  int start, 
                                  int goal,
                                  int* path_out,
                                  int max_iterations)
{
    const int N = graph->num_nodes;
    const double INF = 1e9;
    
    double* g = (double*)malloc(N * sizeof(double));
    double* h = (double*)malloc(N * sizeof(double));
    double* f = (double*)malloc(N * sizeof(double));
    int* parent = (int*)malloc(N * sizeof(int));
    bool* in_closed = (bool*)malloc(N * sizeof(bool));
    
    // Initialize
    for (int i = 0; i < N; i++) {
        g[i] = INF;
        h[i] = INF;
        f[i] = INF;
        parent[i] = -1;
        in_closed[i] = false;
    }
    
    OpenSet open_set;
    openset_init(&open_set, N);
    
    // Start node
    g[start] = 0.0;
    h[start] = heuristic(graph, start, goal);
    f[start] = g[start] + h[start];
    openset_add(&open_set, start, f[start]);
    
    int iter = 0;
    int result = 0;
    
    while (open_set.size > 0 && iter++ < max_iterations) {
        int current = openset_pop_min(&open_set);
        
        if (current == goal) {
            result = reconstruct_path(parent, goal, path_out);
            break;
        }
        
        in_closed[current] = true;
        
        // Expand neighbors
        int edge_start = graph->row_ptr[current];
        int edge_end = graph->row_ptr[current + 1];
        
        for (int e = edge_start; e < edge_end; e++) {
            int neighbor = graph->col_idx[e];
            int weight = graph->weights[e];
            
            if (in_closed[neighbor]) continue;
            
            double tentative_g = g[current] + weight;
            
            if (tentative_g < g[neighbor]) {
                parent[neighbor] = current;
                g[neighbor] = tentative_g;
                h[neighbor] = heuristic(graph, neighbor, goal);
                f[neighbor] = g[neighbor] + h[neighbor];
                
                if (!openset_contains(&open_set, neighbor)) {
                    openset_add(&open_set, neighbor, f[neighbor]);
                }
            }
        }
    }
    
    // Cleanup
    free(g);
    free(h);
    free(f);
    free(parent);
    free(in_closed);
    openset_free(&open_set);
    
    return result;
}

// ============================================================================
// GRAPH BUILDER
// ============================================================================

typedef struct {
    int src;
    int dst;
    int weight;
} Edge;

static CSRGraph* build_csr_graph(int num_nodes, 
                                  const Edge* edges,
                                  int num_edges,
                                  const int* positions_x,
                                  const int* positions_y)
{
    CSRGraph* graph = (CSRGraph*)malloc(sizeof(CSRGraph));
    graph->num_nodes = num_nodes;
    graph->num_edges = num_edges;
    
    graph->row_ptr = (int*)malloc((num_nodes + 1) * sizeof(int));
    graph->col_idx = (int*)malloc(num_edges * sizeof(int));
    graph->weights = (int*)malloc(num_edges * sizeof(int));
    graph->positions_x = (int*)malloc(num_nodes * sizeof(int));
    graph->positions_y = (int*)malloc(num_nodes * sizeof(int));
    
    // Copy positions
    for (int i = 0; i < num_nodes; i++) {
        graph->positions_x[i] = positions_x[i];
        graph->positions_y[i] = positions_y[i];
    }
    
    // Count degree
    int* degree = (int*)calloc(num_nodes, sizeof(int));
    for (int i = 0; i < num_edges; i++) {
        degree[edges[i].src]++;
    }
    
    // Build row_ptr
    graph->row_ptr[0] = 0;
    for (int i = 0; i < num_nodes; i++) {
        graph->row_ptr[i + 1] = graph->row_ptr[i] + degree[i];
    }
    
    // Fill edges
    int* current_pos = (int*)malloc(num_nodes * sizeof(int));
    for (int i = 0; i < num_nodes; i++) {
        current_pos[i] = graph->row_ptr[i];
    }
    
    for (int i = 0; i < num_edges; i++) {
        int src = edges[i].src;
        int pos = current_pos[src]++;
        graph->col_idx[pos] = edges[i].dst;
        graph->weights[pos] = edges[i].weight;
    }
    
    free(degree);
    free(current_pos);
    
    return graph;
}

static void free_csr_graph(CSRGraph* graph) {
    free(graph->row_ptr);
    free(graph->col_idx);
    free(graph->weights);
    free(graph->positions_x);
    free(graph->positions_y);
    free(graph);
}

// ============================================================================
// MAIN - Simplified for CGRA mapping
// ============================================================================

int main() {
    const int num_nodes = 20;
    
    // Node positions
    int positions_x[20] = {0, 100, 200, 300, 0, 100, 200, 300, 0, 100, 200, 300, 0, 100, 200, 300, 0, 100, 200, 300};
    int positions_y[20] = {0, 0, 0, 0, 100, 100, 100, 100, 200, 200, 200, 200, 300, 300, 300, 300, 400, 400, 400, 400};
    
    // Edges
    Edge edges[78] = {
        // Row 0
        {0, 1, 100}, {1, 0, 100}, {1, 2, 100}, {2, 1, 100}, {2, 3, 100}, {3, 2, 100},
        // Row 1
        {4, 5, 100}, {5, 4, 100}, {5, 6, 100}, {6, 5, 100}, {6, 7, 100}, {7, 6, 100},
        // Row 2
        {8, 9, 100}, {9, 8, 100}, {9, 10, 100}, {10, 9, 100}, {10, 11, 100}, {11, 10, 100},
        // Row 3
        {12, 13, 100}, {13, 12, 100}, {13, 14, 100}, {14, 13, 100}, {14, 15, 100}, {15, 14, 100},
        // Row 4
        {16, 17, 100}, {17, 16, 100}, {17, 18, 100}, {18, 17, 100}, {18, 19, 100}, {19, 18, 100},
        // Vertical
        {0, 4, 100}, {4, 0, 100}, {4, 8, 100}, {8, 4, 100}, {8, 12, 100}, {12, 8, 100}, {12, 16, 100}, {16, 12, 100},
        {1, 5, 100}, {5, 1, 100}, {5, 9, 100}, {9, 5, 100}, {9, 13, 100}, {13, 9, 100}, {13, 17, 100}, {17, 13, 100},
        {2, 6, 100}, {6, 2, 100}, {6, 10, 100}, {10, 6, 100}, {10, 14, 100}, {14, 10, 100}, {14, 18, 100}, {18, 14, 100},
        {3, 7, 100}, {7, 3, 100}, {7, 11, 100}, {11, 7, 100}, {11, 15, 100}, {15, 11, 100}, {15, 19, 100}, {19, 15, 100},
        // Diagonals
        {0, 5, 141}, {5, 0, 141}, {5, 10, 141}, {10, 5, 141}, {10, 15, 141}, {15, 10, 141},
        {1, 6, 141}, {6, 1, 141}, {6, 11, 141}, {11, 6, 141},
        {4, 9, 141}, {9, 4, 141}, {9, 14, 141}, {14, 9, 141}, {14, 19, 141}, {19, 14, 141}
    };
    
    CSRGraph* graph = build_csr_graph(num_nodes, edges, 78, positions_x, positions_y);
    
    // Test cases
    int test_starts[4] = {0, 0, 16, 5};
    int test_goals[4] = {19, 3, 3, 14};
    
    int path[20];  // Max path length
    
    // Run 4 problems (20% of 20 nodes)
    for (int i = 0; i < 4; i++) {
        int path_len = sequential_a_star_csr(graph, test_starts[i], test_goals[i], path, 10000);
    }
    
    free_csr_graph(graph);
    
    return 0;
}
