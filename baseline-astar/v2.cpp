#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <future>
#include <algorithm>
#include <cmath>
#include <limits>

class Node {
public:
    int id = -1;
    std::pair<int, int> pos;
    std::vector<std::pair<int, Node*>> adj; // (weight, neighbor)

    Node() = default;
    explicit Node(int _id) : id(_id) {}
};

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

// Heuristic: Euclidean distance (you can switch to Manhattan)
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

// -----------------------------
// Parallel A* for graph of Nodes
// -----------------------------
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
        // --- Step 1: find f_min ---
        double f_min = std::numeric_limits<double>::infinity();
        for (auto* n : open_set)
            f_min = std::min(f_min, f[n]);
        double threshold = f_min + delta;

        // --- Step 2: select frontier ---
        std::vector<const Node*> frontier;
        for (auto* n : open_set)
            if (f[n] <= threshold)
                frontier.push_back(n);

        for (auto* n : frontier) {
            open_set.erase(n);
            closed_set.insert(n);
        }

        // --- Step 3: expand in parallel ---
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

// -----------------------------
// Example usage
// -----------------------------
int main() {
    // Create simple graph
    Node A(0), B(1), C(2), D(3), E(4);
    A.pos = {0,0};
    B.pos = {1,0};
    C.pos = {2,0};
    D.pos = {1,1};
    E.pos = {2,1};

    // Build connections (weight, neighbor)
    A.adj = {{1,&B}};
    B.adj = {{1,&A}, {1,&C}, {1.4,&D}};
    C.adj = {{1,&B}, {1,&E}};
    D.adj = {{1.4,&B}, {1,&E}};
    E.adj = {{1,&C}, {1,&D}};

    auto path = parallel_a_star(&A, &E, 0.5, 4);
    std::cout << "Path: ";
    for (auto* n : path)
        std::cout << n->id << " ";
    std::cout << "\n";
}
