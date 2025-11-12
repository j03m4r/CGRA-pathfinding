#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <map>
#include <utility>
#include <string>

class Node {
    public:
        int id = -1;
        std::vector<std::pair<int,Node*>> adj; // collection of adjacent nodes paired with weight
        std::pair<int,int> pos; 
        // Default constructor
        Node() = default;
        // Constructor
        Node(int _id);

};

class Graph {
    public:
        int size; // Total graph size
        std::map<int,Node> nodes; // mapping of IDs to nodes

        // Constructor
        Graph();

        // Add a node to the graph
        bool addNode(const Node& node);
        
        // Add a weighted connection between two nodes
        void addConnection(int id1, int id2, int weight);
        
        // Clean up all dangling pointers in the graph
        void deleteAllNodes();
};

// Utility function to split strings
std::vector<std::string> split(const std::string& str, const std::string& delimiter);

// Fill graph from file
int fillGraph(std::string f, Graph& g);

// adds coordinate data to graph
int addPosData(std::string f, Graph& g);

//calculates shortest path as described in a* algorithm
std::vector<Node*> astar(Graph& g, int s, int f);
#endif // ASTAR_H