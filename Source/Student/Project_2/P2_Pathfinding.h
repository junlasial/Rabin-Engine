#pragma once

#include "Misc/PathfindingDetails.hpp"
#include <vector>
#include <algorithm>
#include <queue>
#include "SimpleMath.h"

// Define a constant for sqrt(2) cuz less expensive
const float SQRT_2 = 1.41421356237f;

// Define the node structure for the A* algorithm
struct Node {
    Node* parent;        // Parent node in the path
    GridPos gridPos;     // Node's position on the grid
    std::vector<Node*> neighbors; // Neighbors precomputed
    float finalCost;     // f(x) = g(x) + h(x) --> g(x) is the given cost and h(x) is the heuristic 
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
    static const int MAX_GRID_SIZE = 40;
    Node nodes[MAX_GRID_SIZE][MAX_GRID_SIZE];  // Largest member

    // Open list to store pointers to nodes
    std::vector<Node*> list_Open;  // Vector of pointers

    Node* current_node;  // Pointer to the current node
    PathRequest* current_request;  // Pointer to the current path request

    bool is_initialized;  // Boolean flag

    GridPos start;  // Start position (assuming GridPos is small)
    GridPos goalPos;  // Goal position (assuming GridPos is small)

    // Method to clear the nodes before each search
    void clear_nodes();

    // Methods to manage the open list
    Node* pop_cheapest_node();
    void push_node(Node* node);

    //Calculate all heuristic inside this funciton, rmemoved old functions to ake it compact
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