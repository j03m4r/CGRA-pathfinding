#include <stdlib.h>
#include <math.h>

// Simplified kernel for CGRA mapping - neighbor expansion computation
// This is the hot loop from A* pathfinding

#define NUM_NODES 20
#define MAX_EDGES 78
#define MAX_NEIGHBORS 8

// Global graph data (would be in memory)
int row_ptr[NUM_NODES + 1];
int col_idx[MAX_EDGES];
int weights[MAX_EDGES];
int positions_x[NUM_NODES];
int positions_y[NUM_NODES];
double g_scores[NUM_NODES];

// Output arrays
int output_neighbors[MAX_NEIGHBORS] __attribute__((noinline));
double output_g_values[MAX_NEIGHBORS] __attribute__((noinline));

// Heuristic computation - candidate for CGRA acceleration
static inline double compute_heuristic(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return sqrt((double)(dx * dx + dy * dy));
}

// Core compute kernel - neighbor expansion
// This is the main computation we want to accelerate on CGRA
__attribute__((noinline))
void expand_neighbors(int current_node, int goal_node, int* num_neighbors_out) {
    int edge_start = row_ptr[current_node];
    int edge_end = row_ptr[current_node + 1];
    double g_current = g_scores[current_node];
    
    int goal_x = positions_x[goal_node];
    int goal_y = positions_y[goal_node];
    
    int count = 0;
    
    // MAIN LOOP - This is what we want on the CGRA
    //DFGLOOP: loop
    for (int e = edge_start; e < edge_end; e++) {
        int neighbor = col_idx[e];
        int weight = weights[e];
        
        // Compute tentative g score
        double tentative_g = g_current + weight;
        
        // Compute heuristic (h score)
        int neighbor_x = positions_x[neighbor];
        int neighbor_y = positions_y[neighbor];
        int dx = neighbor_x - goal_x;
        int dy = neighbor_y - goal_y;
        double h_score = sqrt((double)(dx * dx + dy * dy));
        
        // Store results
        output_neighbors[count] = neighbor;
        output_g_values[count] = tentative_g;
        
        count++;
    }
    
    *num_neighbors_out = count;
}

// Initialize test data
__attribute__((noinline))
void init_graph_data() {
    // Simple 4x4 subset for testing
    // Positions
    for (int i = 0; i < NUM_NODES; i++) {
        positions_x[i] = (i % 4) * 100;
        positions_y[i] = (i / 4) * 100;
        g_scores[i] = 100.0 * i;  // Some test values
    }
    
    // Row pointers (simplified)
    for (int i = 0; i <= NUM_NODES; i++) {
        row_ptr[i] = i * 3;  // Assume ~3 edges per node
    }
    
    // Edges (simplified)
    for (int i = 0; i < MAX_EDGES; i++) {
        col_idx[i] = (i + 1) % NUM_NODES;
        weights[i] = 100 + (i % 50);
    }
}

// Test function
__attribute__((noinline))
int app() {
    init_graph_data();
    
    int num_neighbors;
    int total = 0;
    
    // Run the kernel multiple times
    for (int test = 0; test < 4; test++) {
        int current = test * 5;
        int goal = 19;
        
        expand_neighbors(current, goal, &num_neighbors);
        
        // Accumulate results to prevent optimization
        for (int i = 0; i < num_neighbors; i++) {
            total += output_neighbors[i];
        }
    }
    
    return total;
}

int app_ret;

int main() {
    app_ret = app();
    return 0;
}
