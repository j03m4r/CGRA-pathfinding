#ifndef GRAPH_LOADER_H
#define GRAPH_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>

class Node {
public:
    int id = -1;
    std::pair<int, int> pos;
    std::vector<std::pair<int, Node*>> adj; // (weight, neighbor)

    Node() = default;
    explicit Node(int _id) : id(_id) {}
};

class GraphLoader {
public:
    std::vector<Node> nodes;
    std::unordered_map<int, int> id_to_index; // DIMACS id -> vector index
    
    bool load(const std::string& coord_file, const std::string& graph_file) {
        if (!load_coordinates(coord_file)) {
            std::cerr << "Failed to load coordinates\n";
            return false;
        }
        if (!load_edges(graph_file)) {
            std::cerr << "Failed to load edges\n";
            return false;
        }
        return true;
    }
    
    Node* get_node(int dimacs_id) {
        auto it = id_to_index.find(dimacs_id);
        if (it == id_to_index.end()) return nullptr;
        return &nodes[it->second];
    }
    
private:
    bool load_coordinates(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Cannot open " << filename << "\n";
            return false;
        }
        
        std::string line;
        int num_nodes = 0;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == 'c') continue;
            
            std::istringstream iss(line);
            char type;
            iss >> type;
            
            if (type == 'p') {
                std::string aux, sp, co;
                iss >> aux >> sp >> co >> num_nodes;
                nodes.reserve(num_nodes);
                std::cout << "Reserving space for " << num_nodes << " nodes\n";
            }
            else if (type == 'v') {
                int id, x, y;
                iss >> id >> x >> y;
                
                int idx = nodes.size();
                nodes.emplace_back(id);
                nodes[idx].pos = {x, y};
                id_to_index[id] = idx;
            }
        }
        
        std::cout << "Loaded " << nodes.size() << " node coordinates\n";
        return true;
    }
    
    bool load_edges(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Cannot open " << filename << "\n";
            return false;
        }
        
        std::string line;
        int edge_count = 0;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == 'c') continue;
            
            std::istringstream iss(line);
            char type;
            iss >> type;
            
            if (type == 'p') {
                // Could parse expected edge count here if needed
                continue;
            }
            else if (type == 'a') {
                int from_id, to_id, weight;
                iss >> from_id >> to_id >> weight;
                
                Node* from = get_node(from_id);
                Node* to = get_node(to_id);
                
                if (from && to) {
                    from->adj.emplace_back(weight, to);
                    edge_count++;
                }
            }
        }
        
        std::cout << "Loaded " << edge_count << " edges\n";
        return true;
    }
};

#endif // GRAPH_LOADER_H
