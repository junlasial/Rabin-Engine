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
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */

private:
    void preallocate_nodes();
    void clear_nodes();

    std::vector<std::vector<Node>> nodes;

    static const int bucketCount = 100; // Example bucket count, adjust based on cost range
    std::vector<std::list<Node*>> buckets;
    float minCost;
    float maxCost;
    const float SQRT_2 = 1.41421356237f; // Precomputed value of sqrt(2)

    void push_to_open_list(Node* node);
    Node* pop_from_open_list();
    void update_node_in_open_list(Node* node);
    int get_bucket_index(float cost);

    float calculate_euclidean_distance(const GridPos& start, const GridPos& goal) const;
    float calculate_squared_euclidean_distance(const GridPos& start, const GridPos& goal) const;

};