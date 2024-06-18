#pragma once

#include "Misc/PathfindingDetails.hpp"
#include <vector>
#include <algorithm>
#include <queue>
#include "SimpleMath.h"

// Define a constant for sqrt(2)
const float SQRT_2 = 1.41421356237f;

// Define the node structure for the A* algorithm
struct Node {
    Node* parent;        // Parent node in the path
    GridPos gridPos;     // Node's position on the grid
    float finalCost;     // f(x) = g(x) + h(x), where g(x) is the given cost and h(x) is the heuristic cost
    float givenCost;     // g(x), the cost from the start node to this node
    enum OnList { NONE, OPEN, CLOSED } onList;  // Enum to track if the node is on the open or closed list
    std::vector<Node*> neighbors; // Neighbors precomputed
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

    bool is_initialized;
    Node* current_node;
    PathRequest* current_request;

    GridPos start;
    GridPos goalPos;

    // Method to clear the nodes before each search
    void clear_nodes();

    // Methods to manage the open list
    Node* pop_cheapest_node();
    void push_node(Node* node);

    float euclidean_heuristic(const GridPos& a, const GridPos& b) const;
    float manhattan_heuristic(const GridPos& a, const GridPos& b) const;
    float chebyshev_heuristic(const GridPos& a, const GridPos& b) const;
    float octile_heuristic(const GridPos& a, const GridPos& b) const;
    float inconsistent_heuristic(const GridPos& a, const GridPos& b) const;
    float calculate_heuristic(const GridPos& a, const GridPos& b, Heuristic heuristic) const;

    std::vector<Node*> get_neighbors(Node* node);
    float distance_between(Node* a, Node* b) const;
    void calculate_all_neighbors();
    void on_map_change();


    void apply_rubberbanding(std::list<Vec3>& path);
    bool is_clear_path(const Vec3& start, const Vec3& end) const;

    void apply_catmull_rom_spline(std::list<Vec3>& path);

    void add_intermediate_points(std::list<Vec3>& path, float maxDistance);


    float distance(const Vec3& p1, const Vec3& p2) {
        Vec3 diff = p1 - p2;
        return diff.Length();
    }


};
