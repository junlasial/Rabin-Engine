#pragma once

#include "Misc/PathfindingDetails.hpp"
#include <vector>
#include <algorithm>


// Define a constant for sqrt(2)
const float SQRT_2 = 1.41421356237f;

// Define the node structure for the A* algorithm
struct Node {
    Node* parent;        // Parent node in the path
    GridPos gridPos;     // Node's position on the grid
    float finalCost;     // f(x) = g(x) + h(x), where g(x) is the given cost and h(x) is the heuristic cost
    float givenCost;     // g(x), the cost from the start node to this node
    enum OnList { NONE, OPEN, CLOSED } onList;  // Enum to track if the node is on the open or closed list

    // Constructor to initialize the node
    Node(GridPos pos = { 0, 0 }, Node* parentNode = nullptr)
        : parent(parentNode), gridPos(pos), finalCost(0.0f), givenCost(0.0f), onList(NONE) {}
};

// AStarPather class definition
class AStarPather {
public:
    // Default constructor
    AStarPather();

    // Initialization and shutdown methods
    bool initialize();
    void shutdown();

    // Path computation method
    PathResult compute_path(PathRequest& request);

private:
    // 2D array to store nodes
    static const int MAX_SIZE = 40;
    Node nodes[MAX_SIZE][MAX_SIZE];

    // Open list to store pointers to nodes
    std::vector<Node*> openList;

    // Method to clear the nodes before each search
    void clear_nodes();

    // Methods to manage the open list
    Node* pop_cheapest_node();
    void push_node(Node* node);
    void update_node(Node* node);


    float euclidean_heuristic(const GridPos& a, const GridPos& b) const;
    float octile_heuristic(const GridPos& a, const GridPos& b) const;
    float manhattan_heuristic(const GridPos& a, const GridPos& b) const;
    float chebyshev_heuristic(const GridPos& a, const GridPos& b) const;
    float inconsistent_heuristic(const GridPos& a, const GridPos& b) const;

    float calculate_heuristic(const GridPos& a, const GridPos& b, Heuristic heuristic) const;
    std::vector<Node*> get_neighbors(Node* node);
    float distance_between(Node* a, Node* b) const;

    void rubberbanding(std::deque<GridPos>& path);
    bool is_valid_direct_path(const GridPos& start, const GridPos& end);
    // Add any additional private member variables or methods here
};


