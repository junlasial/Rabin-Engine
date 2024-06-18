#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"
#include <queue>
#include <unordered_set>

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall() { return false; }
bool ProjectTwo::implemented_goal_bounding() { return false; }
bool ProjectTwo::implemented_jps_plus() { return false; }
#pragma endregion


// Constructor
AStarPather::AStarPather() {
    // Initialize the nodes array with default nodes
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            nodes[i][j] = Node({ i, j });
        }
    }
}

bool AStarPather::initialize() {
    // Handle any one-time setup requirements you have

    // If you want to do any map-preprocessing, you'll need to listen
    // for the map change message. It'll look something like this:

    // Callback cb = std::bind(&AStarPather::your_function_name, this);
    // Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    // There are other alternatives to using std::bind, so feel free to mix it up.
    // Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
    // object that std::function can wrap will suffice.

    return true; // Return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown() {
    // Free any dynamically allocated memory or any other general housekeeping you need to do during shutdown.
}

PathResult AStarPather::compute_path(PathRequest& request) {
    // Clear nodes before starting a new search
    clear_nodes();

    // Convert start and goal to grid positions
    GridPos start = terrain->get_grid_position(request.start);
    GridPos goal = terrain->get_grid_position(request.goal);

    // Initialize the start node
    Node* startNode = &nodes[start.row][start.col];
    startNode->givenCost = 0;
    startNode->finalCost = calculate_heuristic(start, goal, request.settings.heuristic);
    push_node(startNode);

    // Main A* loop
    while (!openList.empty()) {
        Node* currentNode = pop_cheapest_node();
        if (request.settings.debugColoring) {
            terrain->set_color(currentNode->gridPos, Colors::Yellow);
        }

        if (currentNode->gridPos == goal) {
            // Path found, reconstruct the path
            Node* node = currentNode;
            while (node) {
                request.path.push_front(terrain->get_world_position(node->gridPos));
                node = node->parent;
            }
            return PathResult::COMPLETE;
        }

        // Generate neighbors and process each
        std::vector<Node*> neighbors = get_neighbors(currentNode);
        for (Node* neighbor : neighbors) {
            if (neighbor->onList == Node::CLOSED) continue;

            float tentativeGivenCost = currentNode->givenCost + distance_between(currentNode, neighbor);
            if (tentativeGivenCost < neighbor->givenCost || neighbor->onList == Node::NONE) {
                neighbor->givenCost = tentativeGivenCost;
                neighbor->finalCost = tentativeGivenCost + calculate_heuristic(neighbor->gridPos, goal, request.settings.heuristic);
                neighbor->parent = currentNode;

                if (neighbor->onList == Node::NONE) {
                    push_node(neighbor);
                    if (request.settings.debugColoring) {
                        terrain->set_color(neighbor->gridPos, Colors::Blue);
                    }
                }
                else {
                    update_node(neighbor);
                }
            }
        }
        currentNode->onList = Node::CLOSED;
    }

    return PathResult::IMPOSSIBLE;
}

void AStarPather::clear_nodes() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            nodes[i][j].parent = nullptr;
            nodes[i][j].finalCost = 0.0f;
            nodes[i][j].givenCost = 0.0f;
            nodes[i][j].onList = Node::NONE; // Ensure enum is set to NONE
        }
    }
    openList.clear();
}

Node* AStarPather::pop_cheapest_node() {
    if (openList.empty()) {
        return nullptr;
    }

    // Find the node with the smallest finalCost in the open list
    auto cheapestNodeIter = std::min_element(openList.begin(), openList.end(), [](Node* a, Node* b) {
        return a->finalCost < b->finalCost;
        });

    Node* cheapestNode = *cheapestNodeIter;
    openList.erase(cheapestNodeIter); // Remove the node from the open list

    return cheapestNode;
}

void AStarPather::push_node(Node* node) {
    openList.push_back(node);
    node->onList = Node::OPEN;
}

void AStarPather::update_node(Node* node) {
    // In an unsorted list, we don't need to do anything special for update
    // because the node is already in the open list. This function is here
    // for completeness and future extension if needed.
    // Make sure the node is on the open list
    if (node->onList != Node::OPEN) {
        push_node(node);
    }
}

float AStarPather::euclidean_heuristic(const GridPos& a, const GridPos& b) const {
    float dx = static_cast<float>(a.col - b.col);
    float dy = static_cast<float>(a.row - b.row);
    return sqrt(dx * dx + dy * dy);
}

float AStarPather::manhattan_heuristic(const GridPos& a, const GridPos& b) const {
    return static_cast<float>(abs(a.col - b.col) + abs(a.row - b.row));
}

float AStarPather::chebyshev_heuristic(const GridPos& a, const GridPos& b) const {
    return static_cast<float>(std::max(abs(a.col - b.col), abs(a.row - b.row)));
}

float AStarPather::octile_heuristic(const GridPos& a, const GridPos& b) const {
    float dx = static_cast<float>(abs(a.col - b.col));
    float dy = static_cast<float>(abs(a.row - b.row));
    return std::max(dx, dy) + (SQRT_2 - 1) * std::min(dx, dy);
}

float AStarPather::inconsistent_heuristic(const GridPos& a, const GridPos& b) const {
    if ((b.row + b.col) % 2 > 0) {
        return euclidean_heuristic(a, b);
    }
    return 0;
}

float AStarPather::calculate_heuristic(const GridPos& a, const GridPos& b, Heuristic heuristic) const {
    switch (heuristic) {
    case Heuristic::OCTILE:
        return octile_heuristic(a, b);
    case Heuristic::CHEBYSHEV:
        return chebyshev_heuristic(a, b);
    case Heuristic::INCONSISTENT:
        return inconsistent_heuristic(a, b);
    case Heuristic::MANHATTAN:
        return manhattan_heuristic(a, b);
    case Heuristic::EUCLIDEAN:
        return euclidean_heuristic(a, b);
    default:
        return 0.0f; // Default heuristic if none is specified
    }
}

std::vector<Node*> AStarPather::get_neighbors(Node* node) {
    std::vector<Node*> neighbors;
    int row = node->gridPos.row;
    int col = node->gridPos.col;

    // Check all eight possible neighbors (up, down, left, right, and diagonals)
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int newRow = row + i;
            int newCol = col + j;

            // Skip if the new position is invalid or a wall
            if (!terrain->is_valid_grid_position(newRow, newCol) || terrain->is_wall(newRow, newCol)) continue;

            // Skip diagonal movements that cut through walls
            if (i != 0 && j != 0) {
                if (terrain->is_wall(row + i, col) || terrain->is_wall(row, col + j)) continue;
            }

            neighbors.push_back(&nodes[newRow][newCol]);
        }
    }

    return neighbors;
}

float AStarPather::distance_between(Node* a, Node* b) const {
    // Calculate distance considering diagonal movements
    int dx = abs(a->gridPos.col - b->gridPos.col);
    int dy = abs(a->gridPos.row - b->gridPos.row);

    if (dx > 0 && dy > 0) {
        return SQRT_2;
    }
    else {
        return 1.0f;
    }
}


