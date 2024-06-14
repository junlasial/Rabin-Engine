// Node.h

#include "Misc/PathfindingDetails.hpp"

struct Node {
    Node* parent;         // Pointer to the parent node
    GridPos gridPos;      // Node's location in the grid
    float finalCost;      // Final cost f(x) = g(x) + h(x)
    float givenCost;      // Given cost g(x)
    enum class ListStatus { NONE, OPEN, CLOSED } onList;  // Enum to indicate if the node is on the open or closed list

    Node() : parent(nullptr), finalCost(0), givenCost(0), onList(ListStatus::NONE) {}
};
