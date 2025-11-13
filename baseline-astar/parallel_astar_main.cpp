#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <future>
#include <algorithm>
#include <cmath>
#include <limits>
#include <chrono>
#include "graph_loader.h"

// Hash for Node*
struct NodePtrHash {
    size_t operator()(const Node* n) const noexcept {
        return std::hash<int>()(n->id);
    }
};

// Equality for Node*
struct NodePtrEq {
    bool operator()(const Node* a, const Node* b) const noexcept {
        return a == b;
    }
};

// Heuristic: Euclidean distance
inline double heuristic(const Node* a, const Node* b) {
    double dx = double(a->pos.first - b->pos.first);
    double dy = double(a->pos.second - b->pos.second);
    return std::sqrt(dx * dx + dy * dy);
}

// Candidate result from a parallel expansion
struct Candidate {
    Node* neighbor;
    double tentative_g;
    Node* parent;
};

// Expand a single node (worker)
std::vector<Candidate> expand_node(const Node* node,
                                   const std::unordered_map<const Node*, double, NodePtrHash, NodePtrEq>& g_map)
{
    std::vector<Candidate> out;
    double g_node = g_map.at(node);
    for (auto& [w, nbr] : node->adj) {
        double tentative_g = g_node + w;
        out.push_back({nbr, tentative_g, const_cast<Node*>(node)});
    }
    return out;
}

// Reconstruct final path
std::vector<Node*> reconstruct_path(const std::unordered_map<const Node*, Node*, NodePtrHash, NodePtrEq>& parent,
                                    Node* goal)
{
    std::vector<Node*> path;
    Node* cur = goal;
    while (parent.find(cur) != parent.end()) {
        path.push_back(cur);
        cur = parent.at(cur);
    }
    path.push_back(cur);
    std::reverse(path.begin(), path.end());
    return path;
}

// Parallel A*
std::vector<Node*> parallel_a_star(Node* start, Node* goal,
                                   double delta = 0.0,
                                   int max_workers = 8,
                                   int max_iterations = 10000)
{
    using NodeMapD = std::unordered_map<const Node*, double, NodePtrHash, NodePtrEq>;
    using NodeMapP = std::unordered_map<const Node*, Node*, NodePtrHash, NodePtrEq>;
    using NodeSet  = std::unordered_set<const Node*, NodePtrHash, NodePtrEq>;

    NodeMapD g, h, f;
    NodeMapP parent;
    NodeSet open_set, closed_set;

    g[start] = 0.0;
    h[start] = heuristic(start, goal);
    f[start] = g[start] + h[start];
    open_set.insert(start);

    int iter = 0;

    while (!open_set.empty() && iter++ < max_iterations) {
        // Step 1: find f_min
        double f_min = std::numeric_limits<double>::infinity();
        for (auto* n : open_set)
            f_min = std::min(f_min, f[n]);
        double threshold = f_min + delta;

        // Step 2: select frontier
        std::vector<const Node*> frontier;
        for (auto* n : open_set)
            if (f[n] <= threshold)
                frontier.push_back(n);

        for (auto* n : frontier) {
            open_set.erase(n);
            closed_set.insert(n);
        }

        // Step 3: expand in parallel
        std::vector<std::future<std::vector<Candidate>>> futures;
        for (auto* node : frontier)
            futures.push_back(std::async(std::launch::async, expand_node, node, std::cref(g)));

        std::vector<Candidate> candidates;
        for (auto& fut : futures) {
            auto part = fut.get();
            candidates.insert(candidates.end(), part.begin(), part.end());
        }

        bool goal_found = false;
        for (auto& cand : candidates) {
            Node* m = cand.neighbor;
            double tentative_g = cand.tentative_g;
            Node* pnode = cand.parent;

            if (closed_set.count(m)) continue;

            double old_g = (g.count(m) ? g[m] : std::numeric_limits<double>::infinity());
            if (tentative_g < old_g) {
                g[m] = tentative_g;
                h[m] = heuristic(m, goal);
                f[m] = g[m] + h[m];
                parent[m] = pnode;
                open_set.insert(m);
                if (m == goal) goal_found = true;
            }
        }

        if (goal_found || closed_set.count(goal))
            return reconstruct_path(parent, goal);
    }

    return {}; // failure
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <coord_file.co> <graph_file.gr> [start_id] [goal_id]\n";
        return 1;
    }
    
    std::string coord_file = argv[1];
    std::string graph_file = argv[2];
    
    GraphLoader loader;
    
    auto t1 = std::chrono::high_resolution_clock::now();
    if (!loader.load(coord_file, graph_file)) {
        return 1;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Load time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() 
              << " ms\n\n";
    
    // Default start and goal (you can change these or pass via argv)
    int start_id = (argc > 3) ? std::stoi(argv[3]) : 1;
    int goal_id = (argc > 4) ? std::stoi(argv[4]) : 100000;
    
    Node* start = loader.get_node(start_id);
    Node* goal = loader.get_node(goal_id);
    
    if (!start || !goal) {
        std::cerr << "Invalid start or goal node\n";
        return 1;
    }
    
    std::cout << "Running A* from node " << start_id << " to " << goal_id << "\n";
    std::cout << "Start pos: (" << start->pos.first << ", " << start->pos.second << ")\n";
    std::cout << "Goal pos: (" << goal->pos.first << ", " << goal->pos.second << ")\n\n";
    
    auto t3 = std::chrono::high_resolution_clock::now();
    auto path = parallel_a_star(start, goal, 0.0, 8, 1000000);
    auto t4 = std::chrono::high_resolution_clock::now();
    
    std::cout << "Search time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() 
              << " ms\n";
    
    if (path.empty()) {
        std::cout << "No path found\n";
    } else {
        std::cout << "Path found with " << path.size() << " nodes\n";
        std::cout << "Path (first 10 nodes): ";
        for (size_t i = 0; i < std::min(path.size(), size_t(10)); i++) {
            std::cout << path[i]->id << " ";
        }
        if (path.size() > 10) std::cout << "...";
        std::cout << "\n";
        
        // Calculate total distance
        double total_dist = 0;
        for (size_t i = 0; i < path.size() - 1; i++) {
            for (auto& [w, nbr] : path[i]->adj) {
                if (nbr == path[i+1]) {
                    total_dist += w;
                    break;
                }
            }
        }
        std::cout << "Total path cost: " << total_dist << "\n";
    }
    
    return 0;
}
