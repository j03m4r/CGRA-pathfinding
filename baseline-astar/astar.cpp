#include <vector>
#include <map>
#include <unordered_set>
#include <set>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include "astar.h"

// Parameterized constructor
Node::Node(int _id) : id(_id) {}

void Graph::deleteAllNodes() {
    // Clear all nodes from the map
    nodes.clear();
    
    // Reset the node count to zero
    size = 0;
}

Graph::Graph() {
    size = 0;
}

bool Graph::addNode(const Node& node) {
    // Check if node with this ID already exists
    if (nodes.find(node.id) != nodes.end()) {
        std::cerr << "WARNING: Node with ID " << node.id << " already exists." << std::endl;
        return false;
    }

    nodes[node.id] = node;
    size++;
    return true;
}

void Graph::addConnection(int id1, int id2, int weight) {
    Node& nodeA = nodes[id1];
    Node& nodeB = nodes[id2];

    std::pair<int,Node*> pA = {weight, &nodes[id2]};  

    nodeA.adj.push_back(pA);
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    
    return tokens;
}

int fillGraph(std::string f, Graph& g) {
    std::fstream file;
    file.open(f);
    if (!(file.is_open())) {
        std::cerr << "ERROR: File Failed to Open." << std::endl;
        return -1;
    }

    std::string lb; // Line Buffer
    std::vector<std::string> ioData; // Repo for split values

    while (getline(file, lb)) {
        ioData = split(lb," ");
        
        // Skip if not enough elements or comment lines
        if (ioData.size() < 4 || ioData[0] == "c" || ioData[0] == "p") {
            continue;
        }

        // Convert and set parameters to int
        int src,dst,w;
        src = std::stoi(ioData[1]);
        dst = std::stoi(ioData[2]);
        w = std::stoi(ioData[3]);
        
        // get or construct node objects
        if (g.nodes.find(src) == g.nodes.end()) {
            Node n_src(src); 
            g.addNode(n_src);
        }

        if (g.nodes.find(dst) == g.nodes.end()) {
            Node n_dst(dst);
            g.addNode(n_dst);
        }
        g.addConnection(src,dst,w); 
    }

    file.close();
    return 0;
}

int addPosData(std::string f, Graph& g) {
    std::fstream file;
    file.open(f);
    if (!(file.is_open())) {
        std::cerr << "ERROR: File Failed to Open." << std::endl;
        return -1;
    }

    std::string lb; // Line Buffer
    std::vector<std::string> ioData; // Repo for split values
    int targ,x,y; // Node to add weight to, (X,Y) position

    while (getline(file, lb)) {
        ioData = split(lb," ");
        
        // Skip if not enough elements or comment lines
        if (ioData.size() < 4 || ioData[0] == "c" || ioData[0] == "p") {
            continue;
        }

        // Convert and set parameters to int
        targ = std::stoi(ioData[1]);
        x = std::stoi(ioData[2]);
        y = std::stoi(ioData[3]);
        
        // Constructing the pair and assigning to the relevant node.
        std::pair<int,int> nPos = {x,y}; 
        g.nodes[targ].pos = nPos;
    }

    file.close();
    return 0;
}


// Implementation based off of geeksforgeeks formula of A* found at: https://www.geeksforgeeks.org/dsa/a-search-algorithm/
std::vector<Node*> astar(Graph& g, int s, int f) {
    std::vector<Node*> path;
    path.push_back(s); // Start node should be at "start" of path
    
    // 1.  Initialize the open list
    std::set<Node*> open;

    // 2.  Initialize the closed list
    std::unordered_set<Node*> closed;

    // put the starting node on the open list (you can leave its f at zero)
    open.insert(g.nodes[s]);

    Node* q;
    // 3.  while the open list is not empty
    while (open.size() != 0) {
    // a) find the node with the least f on the open list, call it "q"
        q = open.front();
        for (auto& nd: open) {
            if (nd.f < q.f) {
                q = nd;
            }
        }
    // b) pop q off the open list  
        queue.erase(q);
    // c) generate q's 8 successors and set their parents to q
   
    // d) for each successor
        // i) if successor is the goal, stop search
        
        // ii) else, compute both g and h for successor
        //   successor.g = q.g + distance between successor and q
        //   successor.h = distance from goal to successor 
        //   successor.f = successor.g + successor.h
        // iii) if a node with the same position as 
        //     successor is in the OPEN list which has a 
        //    lower f than successor, skip this successor
        // iV) if a node with the same position as 
        //     successor  is in the CLOSED list which has
        //     a lower f than successor, skip this successor
        //     otherwise, add  the node to the open list  
    // e) push q on the closed list
    }
}

int main() {
    std::string graphFileName;
    std::cout << "enter graph file name (expects type .gr)" << std::endl;
    std::cin >> graphFileName;

    std::string posFileName;
    std::cout << "enter position file name (expects type .co)" << std::endl;
    std::cin >> posFileName;

    Graph graph;

    fillGraph(graphFileName, graph);
    std::cout << "graph constructed. Adding position data..." << std::endl;

    addPosData(posFileName, graph);
    std::cout << "Graph fully configured. Graph object size " << graph.size << std::endl;

    std::cout << "Snapshot of first and last 5 Nodes: " << std::endl;

    for (int i = 0; i < 5; i++) {
        std::cout << "id: " << graph.nodes[i].id << " -- Position: {" << graph.nodes[i].pos.first << "," << graph.nodes[i].pos.second << "}" << std::endl;
    }

    for (int i = (graph.size-6); i < graph.size; i++) {
        std::cout << "id: " << graph.nodes[i].id << " -- Position: {" << graph.nodes[i].pos.first << "," << graph.nodes[i].pos.second << "}" << std::endl;
    }


    graph.deleteAllNodes();
    return 0;
}