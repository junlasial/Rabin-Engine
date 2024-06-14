#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

bool AStarPather::initialize()
{
    // Preallocate nodes for the largest map (40x40)
    preallocate_nodes();

    // Initialize openList
    openList.resize(bucketCount);

    // Debug: Check the dimensions of the preallocated nodes
    std::cout << "Node grid size: " << nodes.size() << " x " << nodes[0].size() << std::endl;

    // Debug: Print a few nodes to verify
    std::cout << "Node at (0,0): " << "finalCost=" << nodes[0][0].finalCost << ", givenCost=" << nodes[0][0].givenCost << ", onList=" << nodes[0][0].onList << std::endl;
    std::cout << "Node at (39,39): " << "finalCost=" << nodes[39][39].finalCost << ", givenCost=" << nodes[39][39].givenCost << ", onList=" << nodes[39][39].onList << std::endl;

    return true;
}

void AStarPather::shutdown()
{
    // No dynamic allocation to free in this example
}

void AStarPather::preallocate_nodes()
{
    nodes.resize(Terrain::maxMapHeight, std::vector<Node>(Terrain::maxMapWidth));

    for (int row = 0; row < Terrain::maxMapHeight; ++row) {
        for (int col = 0; col < Terrain::maxMapWidth; ++col) {
            nodes[row][col] = { nullptr, {row, col}, 0.0f, 0.0f, Node::NONE };
        }
    }
}

void AStarPather::clear_nodes()
{
    for (auto& bucket : openList) {
        bucket.clear();
    }
    for (int row = 0; row < Terrain::maxMapHeight; ++row) {
        for (int col = 0; col < Terrain::maxMapWidth; ++col) {
            nodes[row][col].onList = Node::NONE;
            nodes[row][col].parent = nullptr;
            nodes[row][col].givenCost = 0.0f;
            nodes[row][col].finalCost = 0.0f;
        }
    }
}

void AStarPather::push_to_open_list(Node* node)
{
    int bucketIndex = get_bucket_index(node->finalCost);
    openList[bucketIndex].push_back(node);
    node->onList = Node::OPEN;
}

Node* AStarPather::pop_from_open_list()
{
    for (auto& bucket : openList) {
        if (!bucket.empty()) {
            Node* node = bucket.front();
            bucket.pop_front();
            node->onList = Node::CLOSED;
            return node;
        }
    }
    return nullptr; // This should not happen if called correctly
}

void AStarPather::update_node_in_open_list(Node* node)
{
    int bucketIndex = get_bucket_index(node->finalCost);
    auto& bucket = openList[bucketIndex];
    bucket.remove(node); // Remove the old node
    bucket.push_back(node); // Add the updated node
}

int AStarPather::get_bucket_index(float cost)
{
    // Normalize cost to bucket index
    float normalizedCost = (cost - minCost) / (maxCost - minCost);
    return std::clamp(static_cast<int>(normalizedCost * bucketCount), 0, bucketCount - 1);
}

bool AStarPather::is_diagonal_move_valid(const GridPos& current, const GridPos& next) const
{
    int dx = next.col - current.col;
    int dy = next.row - current.row;

    if (dx != 0 && dy != 0) {
        GridPos adjacent1 = { current.row, current.col + dx };
        GridPos adjacent2 = { current.row + dy, current.col };

        if (terrain->is_wall(adjacent1) || terrain->is_wall(adjacent2)) {
            return false;
        }
    }

    return true;
}

float AStarPather::calculate_euclidean_distance(const GridPos& start, const GridPos& goal) const
{
    int dx = goal.col - start.col;
    int dy = goal.row - start.row; 
        return static_cast<float>(std::sqrt(dx * dx + dy * dy));
}

float AStarPather::calculate_squared_euclidean_distance(const GridPos& start, const GridPos& goal) const
{
    int dx = goal.col - start.col;
    int dy = goal.row - start.row;
    return static_cast<float>(dx * dx + dy * dy);
}

PathResult AStarPather::compute_path(PathRequest& request)
{
    // Clear nodes before each search
    clear_nodes();

    // Example code to demonstrate Euclidean distance calculation
    GridPos start = terrain->get_grid_position(request.start);
    GridPos goal = terrain->get_grid_position(request.goal);

    // Example of initializing and using nodes in the pathfinding algorithm
    Node* startNode = &nodes[start.row][start.col];
    Node* goalNode = &nodes[goal.row][goal.col];

    startNode->givenCost = 0;
    startNode->finalCost = calculate_euclidean_distance(start, goal);
    push_to_open_list(startNode);

    while (true) {
        Node* currentNode = pop_from_open_list();
        if (!currentNode || currentNode == goalNode) {
            break;
        }

        for (int dRow = -1; dRow <= 1; ++dRow) {
            for (int dCol = -1; dCol <= 1; ++dCol) {
                if (dRow == 0 && dCol == 0) continue;

                GridPos neighborPos = { currentNode->gridPos.row + dRow, currentNode->gridPos.col + dCol };

                if (!terrain->is_valid_grid_position(neighborPos)) continue;
                if (terrain->is_wall(neighborPos)) continue;
                if (!is_diagonal_move_valid(currentNode->gridPos, neighborPos)) continue;

                Node* neighborNode = &nodes[neighborPos.row][neighborPos.col];
                float newGivenCost = currentNode->givenCost + calculate_euclidean_distance(currentNode->gridPos, neighborPos);

                if (neighborNode->onList == Node::NONE || newGivenCost < neighborNode->givenCost) {
                    neighborNode->givenCost = newGivenCost;
                    neighborNode->finalCost = newGivenCost + calculate_euclidean_distance(neighborPos, goal);
                    neighborNode->parent = currentNode;

                    if (neighborNode->onList == Node::NONE) {
                        push_to_open_list(neighborNode);
                    }
                    else {
                        update_node_in_open_list(neighborNode);
                    }
                }
            }
        }
    }

    // Reconstruct the path
    if (goalNode->parent != nullptr) {
        Node* pathNode = goalNode;
        while (pathNode != nullptr) {
            request.path.push_back(terrain->get_world_position(pathNode->gridPos));
            pathNode = pathNode->parent;
        }
        std::reverse(request.path.begin(), request.path.end());
        return PathResult::COMPLETE;
    }

    return PathResult::IMPOSSIBLE;
}
