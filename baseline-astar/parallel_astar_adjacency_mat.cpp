#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <future>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

// ============================================================================
// CSR GRAPH STRUCTURE
// ============================================================================

struct CSRGraph {
    int num_nodes;
    int num_edges;
    
    std::vector<int> row_ptr;      // Size: num_nodes + 1
    std::vector<int> col_idx;      // Size: num_edges (neighbor node IDs)
    std::vector<int> weights;      // Size: num_edges (edge weights)
    std::vector<std::pair<int, int>> positions;  // Node positions for heuristic
    
    CSRGraph(int n) : num_nodes(n), num_edges(0) {
        row_ptr.resize(n + 1, 0);
        positions.resize(n);
    }
    
    // Get degree (number of neighbors) for node
    int degree(int node) const {
        return row_ptr[node + 1] - row_ptr[node];
    }
    
    // Get range of edges for node
    std::pair<int, int> edge_range(int node) const {
        return {row_ptr[node], row_ptr[node + 1]};
    }
};

// ============================================================================
// HEURISTIC
// ============================================================================

// Heuristic: Euclidean distance
inline double heuristic(const CSRGraph& graph, int node_a, int node_b) {
    double dx = double(graph.positions[node_a].first - graph.positions[node_b].first);
    double dy = double(graph.positions[node_a].second - graph.positions[node_b].second);
    return std::sqrt(dx * dx + dy * dy);
}

// ============================================================================
// PARALLEL A* IMPLEMENTATION
// ============================================================================

// Candidate result from a parallel expansion
struct Candidate {
    int neighbor;
    double tentative_g;
    int parent;
};

// Expand a single node (worker) - CSR version
std::vector<Candidate> expand_node_csr(int node, 
                                        const CSRGraph& graph,
                                        const std::vector<double>& g_scores)
{
    std::vector<Candidate> out;
    double g_node = g_scores[node];
    
    // Iterate through edges in CSR format
    int start = graph.row_ptr[node];
    int end = graph.row_ptr[node + 1];
    
    for (int e = start; e < end; e++) {
        int neighbor = graph.col_idx[e];
        int weight = graph.weights[e];
        double tentative_g = g_node + weight;
        out.push_back({neighbor, tentative_g, node});
    }
    
    return out;
}

// Reconstruct final path
std::vector<int> reconstruct_path(const std::vector<int>& parent, int goal) {
    std::vector<int> path;
    int cur = goal;
    
    while (parent[cur] != -1) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(cur);
    std::reverse(path.begin(), path.end());
    return path;
}

// Parallel A* for CSR graph
std::vector<int> parallel_a_star_csr(const CSRGraph& graph,
                                     int start, 
                                     int goal,
                                     double delta = 0.0,
                                     int max_workers = 8,
                                     int max_iterations = 10000)
{
    const int N = graph.num_nodes;
    const double INF = std::numeric_limits<double>::infinity();
    
    // Use arrays instead of maps for better cache performance
    std::vector<double> g(N, INF);
    std::vector<double> h(N, INF);
    std::vector<double> f(N, INF);
    std::vector<int> parent(N, -1);
    std::vector<bool> in_open_set(N, false);
    std::vector<bool> in_closed_set(N, false);
    
    // Track open set as vector (for parallel iteration)
    std::vector<int> open_set;
    
    // Initialize start node
    g[start] = 0.0;
    h[start] = heuristic(graph, start, goal);
    f[start] = g[start] + h[start];
    open_set.push_back(start);
    in_open_set[start] = true;
    
    int iter = 0;
    
    while (!open_set.empty() && iter++ < max_iterations) {
        // Step 1: Find f_min
        double f_min = INF;
        for (int node : open_set) {
            f_min = std::min(f_min, f[node]);
        }
        double threshold = f_min + delta;
        
        // Step 2: Select frontier (all nodes with f <= threshold)
        std::vector<int> frontier;
        std::vector<int> remaining_open;
        
        for (int node : open_set) {
            if (f[node] <= threshold) {
                frontier.push_back(node);
                in_open_set[node] = false;
                in_closed_set[node] = true;
            } else {
                remaining_open.push_back(node);
            }
        }
        open_set = remaining_open;
        
        // Step 3: Expand frontier in parallel with batching
        std::vector<Candidate> candidates;
        
        for (size_t i = 0; i < frontier.size(); i += max_workers) {
            size_t batch_end = std::min(i + static_cast<size_t>(max_workers), frontier.size());
            std::vector<std::future<std::vector<Candidate>>> futures;
            
            // Launch batch of async tasks
            for (size_t j = i; j < batch_end; j++) {
                futures.push_back(std::async(std::launch::async, 
                                             expand_node_csr, 
                                             frontier[j], 
                                             std::cref(graph),
                                             std::cref(g)));
            }
            
            // Collect results from this batch
            for (auto& fut : futures) {
                auto part = fut.get();
                candidates.insert(candidates.end(), part.begin(), part.end());
            }
        }
        
        // Step 4: Process candidates
        bool goal_found = false;
        for (auto& cand : candidates) {
            int neighbor = cand.neighbor;
            double tentative_g = cand.tentative_g;
            int parent_node = cand.parent;
            
            if (in_closed_set[neighbor]) continue;
            
            if (tentative_g < g[neighbor]) {
                g[neighbor] = tentative_g;
                h[neighbor] = heuristic(graph, neighbor, goal);
                f[neighbor] = g[neighbor] + h[neighbor];
                parent[neighbor] = parent_node;
                
                if (!in_open_set[neighbor]) {
                    open_set.push_back(neighbor);
                    in_open_set[neighbor] = true;
                }
                
                if (neighbor == goal) {
                    goal_found = true;
                }
            }
        }
        
        if (goal_found || in_closed_set[goal]) {
            return reconstruct_path(parent, goal);
        }
    }
    
    return {};  // failure
}

// ============================================================================
// CSR GRAPH BUILDER
// ============================================================================

// Helper: Build CSR graph from edge list
CSRGraph build_csr_graph(int num_nodes, 
                         const std::vector<std::tuple<int, int, int>>& edges,
                         const std::vector<std::pair<int, int>>& node_positions)
{
    CSRGraph graph(num_nodes);
    graph.positions = node_positions;
    
    // Count edges per node
    std::vector<int> degree(num_nodes, 0);
    for (const auto& [src, dst, weight] : edges) {
        degree[src]++;
    }
    
    // Build row_ptr (prefix sum of degrees)
    graph.row_ptr[0] = 0;
    for (int i = 0; i < num_nodes; i++) {
        graph.row_ptr[i + 1] = graph.row_ptr[i] + degree[i];
    }
    graph.num_edges = graph.row_ptr[num_nodes];
    
    // Allocate space for edges
    graph.col_idx.resize(graph.num_edges);
    graph.weights.resize(graph.num_edges);
    
    // Fill in edges
    std::vector<int> current_pos = graph.row_ptr;  // Track current position for each node
    for (const auto& [src, dst, weight] : edges) {
        int pos = current_pos[src]++;
        graph.col_idx[pos] = dst;
        graph.weights[pos] = weight;
    }
    
    return graph;
}

// ============================================================================
// DIMACS FORMAT LOADER
// ============================================================================

// Load DIMACS format graph into CSR
CSRGraph load_dimacs_to_csr(const std::string& coord_file, 
                              const std::string& graph_file) 
{
    std::cout << "Loading DIMACS graph files...\n";
    
    // Step 1: Load coordinates
    std::ifstream coord_in(coord_file);
    if (!coord_in) {
        std::cerr << "Error: Cannot open " << coord_file << std::endl;
        exit(1);
    }
    
    std::vector<std::pair<int, int>> positions;
    int max_node_id = 0;
    int coord_count = 0;
    
    std::string line;
    while (std::getline(coord_in, line)) {
        if (line.empty() || line[0] == 'c') continue;
        
        std::istringstream iss(line);
        char type;
        iss >> type;
        
        if (type == 'v') {
            int id, x, y;
            iss >> id >> x >> y;
            max_node_id = std::max(max_node_id, id);
            
            // Ensure positions vector is large enough
            if (positions.size() <= static_cast<size_t>(id)) {
                positions.resize(id + 1);
            }
            positions[id] = {x, y};
            coord_count++;
        }
    }
    coord_in.close();
    
    int num_nodes = max_node_id + 1;
    std::cout << "  Loaded " << coord_count << " node coordinates\n";
    std::cout << "  Max node ID: " << max_node_id << "\n";
    
    // Step 2: Load edges
    std::ifstream graph_in(graph_file);
    if (!graph_in) {
        std::cerr << "Error: Cannot open " << graph_file << std::endl;
        exit(1);
    }
    
    std::vector<std::tuple<int, int, int>> edges;
    int edge_count = 0;
    
    while (std::getline(graph_in, line)) {
        if (line.empty() || line[0] == 'c') continue;
        
        std::istringstream iss(line);
        char type;
        iss >> type;
        
        if (type == 'a') {
            int from, to, weight;
            iss >> from >> to >> weight;
            edges.push_back({from, to, weight});
            edge_count++;
        }
    }
    graph_in.close();
    
    std::cout << "  Loaded " << edge_count << " edges\n";
    
    // Step 3: Build CSR
    std::cout << "Building CSR graph structure...\n";
    return build_csr_graph(num_nodes, edges, positions);
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <coord_file.co> <graph_file.gr> [start_id] [goal_id]\n";
        return 1;
    }
    
    std::string coord_file = argv[1];
    std::string graph_file = argv[2];
    
    // Load graph
    auto load_start = std::chrono::high_resolution_clock::now();
    CSRGraph graph = load_dimacs_to_csr(coord_file, graph_file);
    auto load_end = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nGraph Statistics:\n";
    std::cout << "  Nodes: " << graph.num_nodes << "\n";
    std::cout << "  Edges: " << graph.num_edges << "\n";
    std::cout << "  Avg degree: " << (double)graph.num_edges / graph.num_nodes << "\n";
    std::cout << "  Load time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(load_end - load_start).count()
              << " ms\n\n";
    
    // Get start and goal from command line or use defaults
    int start_id = (argc > 3) ? std::stoi(argv[3]) : 1;
    int goal_id = (argc > 4) ? std::stoi(argv[4]) : 100000;
    
    // Validate node IDs
    if (start_id < 0 || start_id >= graph.num_nodes) {
        std::cerr << "Error: Invalid start node " << start_id << "\n";
        return 1;
    }
    if (goal_id < 0 || goal_id >= graph.num_nodes) {
        std::cerr << "Error: Invalid goal node " << goal_id << "\n";
        return 1;
    }
    
    std::cout << "Running A* search:\n";
    std::cout << "  Start: " << start_id 
              << " at (" << graph.positions[start_id].first 
              << ", " << graph.positions[start_id].second << ")\n";
    std::cout << "  Goal:  " << goal_id 
              << " at (" << graph.positions[goal_id].first 
              << ", " << graph.positions[goal_id].second << ")\n";
    std::cout << "  Parameters: delta=0.0, max_workers=8\n\n";
    
    // Run A*
    auto search_start = std::chrono::high_resolution_clock::now();
    auto path = parallel_a_star_csr(graph, start_id, goal_id, 0.0, 8, 1000000);
    auto search_end = std::chrono::high_resolution_clock::now();
    
    auto search_time = std::chrono::duration_cast<std::chrono::milliseconds>(search_end - search_start).count();
    
    std::cout << "Search completed in " << search_time << " ms\n\n";
    
    // Display results
    if (path.empty()) {
        std::cout << "No path found!\n";
    } else {
        std::cout << "Path found!\n";
        std::cout << "  Path length: " << path.size() << " nodes\n";
        
        // Calculate total distance
        double total_dist = 0.0;
        for (size_t i = 0; i < path.size() - 1; i++) {
            int node = path[i];
            int next = path[i + 1];
            
            // Find edge weight
            int start = graph.row_ptr[node];
            int end = graph.row_ptr[node + 1];
            for (int e = start; e < end; e++) {
                if (graph.col_idx[e] == next) {
                    total_dist += graph.weights[e];
                    break;
                }
            }
        }
        
        std::cout << "  Total distance: " << total_dist << "\n";
        
        // Show first and last few nodes
        std::cout << "  Path: ";
        int show_count = std::min(static_cast<int>(path.size()), 10);
        for (int i = 0; i < show_count; i++) {
            std::cout << path[i] << " ";
        }
        if (path.size() > 10) {
            std::cout << "... ";
            for (size_t i = path.size() - 3; i < path.size(); i++) {
                std::cout << path[i] << " ";
            }
        }
        std::cout << "\n";
    }
    
    return 0;
}
