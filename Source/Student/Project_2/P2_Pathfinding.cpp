#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <functional>

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall() { return false; }
bool ProjectTwo::implemented_goal_bounding() { return false; }
bool ProjectTwo::implemented_jps_plus() { return false; }
#pragma endregion



// Constructor
AStarPather::AStarPather()
    : is_initialized(false),
    current_node(nullptr),
    current_request(nullptr),
    start({ 0, 0 }),
    goalPos({ 0, 0 })
{
    // Initialize the nodes array with default nodes
    for (int i = 0; i < MAX_GRID_SIZE; ++i) {
        for (int j = 0; j < MAX_GRID_SIZE; ++j) {
            nodes[i][j] = Node({ i, j });
        }
    }
}

bool AStarPather::initialize() {
    // Callback for map change

    //This is to cehck for neighbour not on run time but before
    Callback cb = std::bind(&AStarPather::on_map_change, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);
    return true;
}

//Calculate the neihbours
void AStarPather::on_map_change() {
    calculate_all_neighbors();
}

void AStarPather::shutdown() {
    // Free any dynamically allocated memory or any other general housekeeping you need to do during shutdown.
}

PathResult AStarPather::compute_path(PathRequest& request) {
    // Initialize the search if not already done
    if (!is_initialized) {
        // Clear any previous node data
        clear_nodes();

        // Get start and goal positions in the grid
        start = terrain->get_grid_position(request.start);
        goalPos = terrain->get_grid_position(request.goal);

        // Immediate completion if start is the same as goal
        if (start == goalPos) {
            request.path.push_front(request.start);
            return PathResult::COMPLETE;
        }

        // Initialize the start node
        Node* startNode = &nodes[start.row][start.col];
        startNode->givenCost = 0;
        startNode->finalCost = calculate_heuristic(start, goalPos, request.settings.heuristic);
        push_node(startNode);

        current_node = nullptr;
        current_request = &request;
        is_initialized = true;
    }

    // Main search loop
    while (!list_Open.empty()) {
        // Select the node with the lowest cost to process
        if (!current_node) {
            current_node = pop_cheapest_node();
        }

        // Debug coloring for current node
        if (request.settings.debugColoring) {
            terrain->set_color(current_node->gridPos, Colors::Yellow);
        }

        // Check if the goal has been reached
        if (current_node->gridPos == goalPos) {
            // Reconstruct the path from goal to start
            Node* node = current_node;
            while (node) {
                request.path.push_front(terrain->get_world_position(node->gridPos));
                node = node->parent;
            }

            // Apply path post-processing if needed
            if (request.settings.rubberBanding) {
                apply_rubberbanding(request.path);
            }
            if (request.settings.smoothing) {
                add_intermediate_points(request.path, 1.5f);
                apply_catmull_rom_spline(request.path);
            }

            is_initialized = false;
            return PathResult::COMPLETE;
        }

        // Process neighboring nodes
        std::vector<Node*> neighbors = get_neighbors(current_node);
        for (Node* neighbor : neighbors) {
            if (neighbor->onList == Node::CLOSED) continue;

            // Calculate the tentative given cost for the neighbor
            float tentativeGivenCost = current_node->givenCost + distance_between(current_node, neighbor);
            if (tentativeGivenCost < neighbor->givenCost || neighbor->onList == Node::NONE) {
                neighbor->givenCost = tentativeGivenCost;
                neighbor->finalCost = tentativeGivenCost + calculate_heuristic(neighbor->gridPos, goalPos, request.settings.heuristic);
                neighbor->parent = current_node;

                // Push the neighbor to the open list if it's not already there
                if (neighbor->onList == Node::NONE) {
                    push_node(neighbor);
                    if (request.settings.debugColoring) {
                        terrain->set_color(neighbor->gridPos, Colors::Blue);
                    }
                }
                else {
                    neighbor->onList = Node::OPEN;
                }
            }
        }

        // Mark the current node as closed and reset it
        current_node->onList = Node::CLOSED;
        current_node = nullptr;

        // If single step processing is enabled, return for now
        if (request.settings.singleStep) {
            return PathResult::PROCESSING;
        }
    }

    // If the open list is empty and no path was found
    is_initialized = false;
    return PathResult::IMPOSSIBLE;
}




void AStarPather::clear_nodes() {

    //reset all the nodes 
    for (auto& row : nodes) {
        for (auto& node : row) {
            node.parent = nullptr;
            node.finalCost = 0.0f;
            node.givenCost = 0.0f;
            node.onList = Node::NONE;
        }
    }
    list_Open.clear();
}


Node* AStarPather::pop_cheapest_node() {
    if (list_Open.empty()) {
        return nullptr;
    }

    // Initialize the iterator to the first element
    auto cheapestNodeIter = list_Open.begin();
    Node* cheapestNode = *cheapestNodeIter;

    // Iterate through the list to find the node with the smallest finalCost
    for (auto it = list_Open.begin(); it != list_Open.end(); ++it) {
        if ((*it)->finalCost < cheapestNode->finalCost) {
            cheapestNodeIter = it;
            cheapestNode = *it;
        }
    }

    // Erase the cheapest node from the open list
    list_Open.erase(cheapestNodeIter);

    return cheapestNode;
}


void AStarPather::push_node(Node* node) {
    list_Open.push_back(node);
    node->onList = Node::OPEN;
}

float AStarPather::calculate_heuristic(const GridPos& a, const GridPos& b, Heuristic heuristic) const {
    switch (heuristic) {
    case Heuristic::OCTILE: {
        float dx = static_cast<float>(std::abs(a.col - b.col));
        float dy = static_cast<float>(std::abs(a.row - b.row));
        return std::max(dx, dy) - std::min(dx, dy) + (SQRT_2)*std::min(dx, dy);
    }
    case Heuristic::CHEBYSHEV:
        return static_cast<float>(std::max(std::abs(a.col - b.col), std::abs(a.row - b.row)));
    case Heuristic::INCONSISTENT:
        return ((b.row + b.col) % 2 > 0) ? std::sqrt(static_cast<float>((a.col - b.col) * (a.col - b.col) + (a.row - b.row) * (a.row - b.row))) : 0.0f;
    case Heuristic::MANHATTAN:
        return static_cast<float>(std::abs(a.col - b.col) + std::abs(a.row - b.row));
    case Heuristic::EUCLIDEAN: {
        float dx = static_cast<float>(a.col - b.col);
        float dy = static_cast<float>(a.row - b.row);
        return std::sqrt(dx * dx + dy * dy);
    }
    default:
        return 0.0f;
    }
}

std::vector<Node*> AStarPather::get_neighbors(Node* node) {
    return node->neighbors;
}

float AStarPather::distance_between(Node* a, Node* b) const {
    int dx = abs(a->gridPos.col - b->gridPos.col);
    int dy = abs(a->gridPos.row - b->gridPos.row);
    return (dx > 0 && dy > 0) ? SQRT_2 : 1.0f;
}
void AStarPather::calculate_all_neighbors() {

    //Each direciton
    const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1}, { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    //loop the iter
    for (int row = 0; row < MAX_GRID_SIZE; ++row) {
        for (int col = 0; col < MAX_GRID_SIZE; ++col) {
            Node& node = nodes[row][col];
            node.neighbors.clear();

            // Check each possible direction for valid neighbors
            for (const auto& direction : directions) {
                int neighborRow = row + direction.first;
                int neighborCol = col + direction.second;

                if (terrain->is_valid_grid_position(neighborRow, neighborCol) && !terrain->is_wall(neighborRow, neighborCol)) {
                    // Check for diagonal movement and skip if blocked by a wall
                    bool isDiagonal = (direction.first != 0 && direction.second != 0);
                    if (isDiagonal) {
                        bool blockedByWall = terrain->is_wall(row + direction.first, col) || terrain->is_wall(row, col + direction.second);
                        if (blockedByWall) {
                            continue;
                        }
                    }
                    // Add valid neighbor to the node's neighbors list
                    node.neighbors.push_back(&nodes[neighborRow][neighborCol]);
                }
            }
        }
    }
}


void AStarPather::apply_rubberbanding(std::list<Vec3>& path) {


    // Initialize iterators for traversing the path.
    auto current = path.begin();
    auto end = path.end();

    // Loop through the path until the end is reached.
    while (current != end) {
        auto next = std::next(current);
        auto after_next = std::next(next);

        // Check if we have at least three nodes.
        if (after_next == end) {
            break;
        }

        // Check if there is a clear path from 'current' to 'after_next'.
        if (is_clear_path(*current, *after_next)) {
            // If there is a clear path, erase the middle node.
            path.erase(next);
        }
        else {
            // Otherwise, move the iterators forward.
            ++current;
        }
    }
}

bool AStarPather::is_clear_path(const Vec3& start, const Vec3& end) const {
    // Get the grid positions of the start and end points
    GridPos startPos = terrain->get_grid_position(start);
    GridPos endPos = terrain->get_grid_position(end);

    // Calculate differences in x and y directions
    int dx = abs(endPos.col - startPos.col);
    int dy = abs(endPos.row - startPos.row);
    int x = startPos.col;
    int y = startPos.row;
    int steps = 1 + dx + dy;

    int x_step = (endPos.col > startPos.col) ? 1 : -1;
    int y_step = (endPos.row > startPos.row) ? 1 : -1;

    int error = dx - dy;

    // Adjust differences 
    dx *= 2;
    dy *= 2;



    for (; steps > 0; --steps) {
        // Return false if the current position is wall
        if (terrain->is_wall(y, x)) {
            return false;
        }

        // Check for diagonal wall 
        if ((x != startPos.col && y != startPos.row) &&
            (terrain->is_wall(y, x - x_step) || terrain->is_wall(y - y_step, x))) {
            return false;
        }

        //adjust the directiuon
        if (error > 0) {
            x += x_step;
            error -= dy;
        }
        else {
            y += y_step;
            error += dx;
        }
    }

    // If no walls are found, return true
    return true;
}


void AStarPather::apply_catmull_rom_spline(std::list<Vec3>& path) {
    // If the path has less than 4 points, spline interpolation is not needed.
    if (path.size() < 4) {
        return;
    }

    std::list<Vec3> newPath;
    auto it = path.begin();
    auto prev = it++;
    auto curr = it++;
    auto next = it++;
    auto afterNext = it;

    // Insert the first point
    newPath.push_back(*prev);

    // Interpolate between each set of four points
    while (afterNext != path.end()) {
        for (float t = 0; t < 1.0f; t += 0.5f) { // Adjust the step size for smoother curves
            DirectX::SimpleMath::Vector3 interpolatedPoint = DirectX::SimpleMath::Vector3::CatmullRom(
                DirectX::SimpleMath::Vector3(prev->x, prev->y, prev->z),
                DirectX::SimpleMath::Vector3(curr->x, curr->y, curr->z),
                DirectX::SimpleMath::Vector3(next->x, next->y, next->z),
                DirectX::SimpleMath::Vector3(afterNext->x, afterNext->y, afterNext->z),
                t
            );
            newPath.push_back(Vec3(interpolatedPoint.x, interpolatedPoint.y, interpolatedPoint.z));
        }
        prev = curr;
        curr = next;
        next = afterNext;
        ++afterNext;
    }

    // Insert the last point
    newPath.push_back(*next);

    // Replace the original path with the new path
    path = newPath;
}


//Add the points in between
void AStarPather::add_intermediate_points(std::list<Vec3>& path, float maxDistance) {
    auto current = path.begin();
    auto end = path.end();

    while (current != end) {
        auto next = std::next(current);
        if (next == end) {
            break;
        }

        // Check distance between current and next points
        float dist = distance(*current, *next);
        if (dist > maxDistance) {
            // Insert intermediate points until the distance is within maxDistance
            while (dist > maxDistance) {
                Vec3 intermediatePoint = (*current + *next) * 0.5f;
                next = path.insert(next, intermediatePoint);
                dist = distance(*current, *next);
            }
        }
        else {
            // Move to the next point
            ++current;
        }
    }
}


