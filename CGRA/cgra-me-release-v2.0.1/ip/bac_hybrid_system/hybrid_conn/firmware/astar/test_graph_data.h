#ifndef TEST_GRAPH_DATA_H
#define TEST_GRAPH_DATA_H

// 3x3 grid graph + 1 diagonal node
// Layout:
//   0---1---2
//   |   |   |
//   3---4---5
//   |   |   |
//   6---7---8---9

#define NUM_NODES 10
#define NUM_EDGES 24
#define START_NODE 0
#define GOAL_NODE 9

// Node positions (x, y)
static const int node_positions[NUM_NODES][2] = {
    {0, 0},   // node 0
    {100, 0}, // node 1
    {200, 0}, // node 2
    {0, 100}, // node 3
    {100, 100}, // node 4
    {200, 100}, // node 5
    {0, 200}, // node 6
    {100, 200}, // node 7
    {200, 200}, // node 8
    {300, 300}  // node 9 (goal, diagonal)
};

// CSR format: row_ptr[node] = start index in col_idx/weights for node's edges
static const int row_ptr[NUM_NODES + 1] = {
    0,  // node 0: edges 0-1
    2,  // node 1: edges 2-4
    5,  // node 2: edges 5-6
    7,  // node 3: edges 7-9
    10, // node 4: edges 10-13
    14, // node 5: edges 14-16
    17, // node 6: edges 17-18
    19, // node 7: edges 19-21
    22, // node 8: edges 22-23
    24, // node 9: edges (none)
    24  // end marker
};

// Neighbor node IDs
static const int col_idx[NUM_EDGES] = {
    1, 3,          // node 0 -> {1, 3}
    0, 2, 4,       // node 1 -> {0, 2, 4}
    1, 5,          // node 2 -> {1, 5}
    0, 4, 6,       // node 3 -> {0, 4, 6}
    1, 3, 5, 7,    // node 4 -> {1, 3, 5, 7}
    2, 4, 8,       // node 5 -> {2, 4, 8}
    3, 7,          // node 6 -> {3, 7}
    4, 6, 8,       // node 7 -> {4, 6, 8}
    5, 7, 9,       // node 8 -> {5, 7, 9}
    8              // node 9 -> {8}
};

// Edge weights (all unit weight for simplicity)
static const int weights[NUM_EDGES] = {
    100, 100,         // node 0
    100, 100, 100,    // node 1
    100, 100,         // node 2
    100, 100, 100,    // node 3
    100, 100, 100, 100, // node 4
    100, 100, 141,    // node 5 (141 = diagonal to 8, then 8->9)
    100, 100,         // node 6
    100, 100, 100,    // node 7
    100, 100, 141,    // node 8 (141 = sqrt(2)*100 for diagonal)
    141               // node 9
};

#endif // TEST_GRAPH_DATA_H
