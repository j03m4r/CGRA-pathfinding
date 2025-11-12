#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <fstream> 

/**
 * @brief Primary File for the ASTAR baseline implementation. 
 */

/**
 * @brief Represents a descrete location on the target graph. 
 */
class Node {
    public:
        int id;
        std::vector<std::pair<int,Node*>> adj; // collection of adjacent nodes paired with weights
}

class Graph {
    public:
        int numNodes; // Total graph size
        std::map<int,Node> nodes; // mapping of IDs to nodes

        Graph() {
            numNodes = 0;
           }

        bool addNode(const Node& node) {
            // Check if node with this ID already exists
            if (nodes.find(node.id) != nodes.end()) {
                std::cerr << "WARNING: Node with ID " << node.id << " already exists." << std::endl;
                return false;
            }

            nodes[node.id] = node;
            numNodes++;
            return true;
            }
        

        void addConnection(int id1, int id2, int weight) {
            Node& nodeA = nodes[id1];  // Reference, not copy
            Node& nodeB = nodes[id2];

            std::pair<int,Node*> pA = {weight, &nodes[id2]};  // Pointer to map element
            std::pair<int,Node*> pB = {weight, &nodes[id1]};

            nodeA.adj.push_back(pA);
            nodeB.adj.push_back(pB);
            }


}

class grDecoder {

}

int main() {
    return 0;
}