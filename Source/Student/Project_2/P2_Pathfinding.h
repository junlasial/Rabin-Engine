#pragma once
#include "Misc/PathfindingDetails.hpp"


struct Node {
    Node* parent;
    GridPos gridPos;
    float finalCost;
    float givenCost;
    enum OnList { NONE, OPEN, CLOSED } onList;
};



struct NodeComparator {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->finalCost > rhs->finalCost;
    }
};

class AStarPather
{
public:
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest& request);

private:
    void preallocate_nodes();
    void clear_nodes();

    std::vector<std::vector<Node>> nodes;
    std::vector<std::list<Node*>> openList;

    static const int bucketCount = 100; // Example bucket count, adjust based on cost range
    float minCost = 0.0f;
    float maxCost = 1000.0f; // Example max cost, adjust based on your use case
    const float SQRT_2 = 1.41421356237f; // Precomputed value of sqrt(2)

    void push_to_open_list(Node* node);
    Node* pop_from_open_list();
    void update_node_in_open_list(Node* node);
    int get_bucket_index(float cost);

    bool is_diagonal_move_valid(const GridPos& current, const GridPos& next) const;
    float calculate_euclidean_distance(const GridPos& start, const GridPos& goal) const;
    float calculate_squared_euclidean_distance(const GridPos& start, const GridPos& goal) const;
};