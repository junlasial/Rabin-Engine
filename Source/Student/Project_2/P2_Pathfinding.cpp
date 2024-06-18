#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"
#include <queue>
#include <unordered_set>
#include <cmath>

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall() { return false; }
bool ProjectTwo::implemented_goal_bounding() { return false; }
bool ProjectTwo::implemented_jps_plus() { return false; }
#pragma endregion

// Constructor
AStarPather::AStarPather() : is_initialized(false), current_node(nullptr), current_request(nullptr) {
    // Initialize the nodes array with default nodes
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            nodes[i][j] = Node({ i, j });
        }
    }
}

bool AStarPather::initialize() {
    // Callback for map change
    Callback cb = std::bind(&AStarPather::on_map_change, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);
    return true;
}

void AStarPather::on_map_change() {
    calculate_all_neighbors();
}

void AStarPather::shutdown() {
    // Free any dynamically allocated memory or any other general housekeeping you need to do during shutdown.
}

PathResult AStarPather::compute_path(PathRequest& request) {
    if (!is_initialized) {
        clear_nodes();

        // Convert start and goal to grid positions
        start = terrain->get_grid_position(request.start);
        goalPos = terrain->get_grid_position(request.goal);

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

    while (!openList.empty()) {
        if (!current_node) {
            current_node = pop_cheapest_node();
        }

        if (request.settings.debugColoring) {
            terrain->set_color(current_node->gridPos, Colors::Yellow);
        }

        if (current_node->gridPos == goalPos) {
            // Path found, reconstruct the path
            Node* node = current_node;
            while (node) {
                request.path.push_front(terrain->get_world_position(node->gridPos));
                node = node->parent;
            }

           
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

        // Generate neighbors and process each
        std::vector<Node*> neighbors = get_neighbors(current_node);
        for (Node* neighbor : neighbors) {
            if (neighbor->onList == Node::CLOSED) continue;

            float tentativeGivenCost = current_node->givenCost + distance_between(current_node, neighbor);
            if (tentativeGivenCost < neighbor->givenCost || neighbor->onList == Node::NONE) {
                neighbor->givenCost = tentativeGivenCost;
                neighbor->finalCost = tentativeGivenCost + calculate_heuristic(neighbor->gridPos, goalPos, request.settings.heuristic);
                neighbor->parent = current_node;

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

        current_node->onList = Node::CLOSED;
        current_node = nullptr;

        if (request.settings.singleStep) {
            return PathResult::PROCESSING;
        }
    }

    is_initialized = false;
    return PathResult::IMPOSSIBLE;
}

void AStarPather::clear_nodes() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            nodes[i][j].parent = nullptr;
            nodes[i][j].finalCost = 0.0f;
            nodes[i][j].givenCost = 0.0f;
            nodes[i][j].onList = Node::NONE;
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
    openList.erase(cheapestNodeIter);

    return cheapestNode;
}

void AStarPather::push_node(Node* node) {
    openList.push_back(node);
    node->onList = Node::OPEN;
}

float AStarPather::euclidean_heuristic(const GridPos& a, const GridPos& b) const {
    float dx = static_cast<float>(a.col - b.col);
    float dy = static_cast<float>(a.row - b.row);
    return std::sqrt(dx * dx + dy * dy);
}

float AStarPather::manhattan_heuristic(const GridPos& a, const GridPos& b) const {
    return static_cast<float>(std::abs(a.col - b.col) + std::abs(a.row - b.row));
}

float AStarPather::chebyshev_heuristic(const GridPos& a, const GridPos& b) const {
    return static_cast<float>(std::max(std::abs(a.col - b.col), std::abs(a.row - b.row)));
}

float AStarPather::octile_heuristic(const GridPos& a, const GridPos& b) const {
    float dx = static_cast<float>(std::abs(a.col - b.col));
    float dy = static_cast<float>(std::abs(a.row - b.row));
    return std::max(dx, dy) + (SQRT_2 - 1) * std::min(dx, dy);
}

float AStarPather::inconsistent_heuristic(const GridPos& a, const GridPos& b) const {
    return ((b.row + b.col) % 2 > 0) ? euclidean_heuristic(a, b) : 0.0f;
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
    const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1}, { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            Node& node = nodes[i][j];
            node.neighbors.clear();

            for (const auto& [di, dj] : directions) {
                int ni = i + di;
                int nj = j + dj;

                if (terrain->is_valid_grid_position(ni, nj) && !terrain->is_wall(ni, nj)) {
                    if (di != 0 && dj != 0) {
                        if (terrain->is_wall(i + di, j) || terrain->is_wall(i, j + dj)) continue;
                    }
                    node.neighbors.push_back(&nodes[ni][nj]);
                }
            }
        }
    }
}

void AStarPather::apply_rubberbanding(std::list<Vec3>& path) {
    // If the path has less than 3 points, rubberbanding is not needed.

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
    // Get the grid positions of the start and end points.
    GridPos startPos = terrain->get_grid_position(start);
    GridPos endPos = terrain->get_grid_position(end);

    // Calculate the difference in the x and y directions.
    int dx = abs(endPos.col - startPos.col);
    int dy = abs(endPos.row - startPos.row);
    int x = startPos.col;
    int y = startPos.row;
    int steps = 1 + dx + dy; // Total number of steps to take.
    int x_step = (endPos.col > startPos.col) ? 1 : -1; // Direction to step in x.
    int y_step = (endPos.row > startPos.row) ? 1 : -1; // Direction to step in y.
    int error = dx - dy;

    // Double the differences to help with error adjustment.
    dx *= 2;
    dy *= 2;

    // Loop through each step from start to end.
    for (; steps > 0; --steps) {
        // If the current position is a wall, return false.
        if (terrain->is_wall(y, x)) {
            return false;
        }

        // Adjust the error and move in the x or y direction.
        if (error > 0) {
            x += x_step;
            error -= dy;
        }
        else {
            y += y_step;
            error += dx;
        }

        // Check for diagonal wall cutting.
        if ((x != startPos.col && y != startPos.row) &&
            (terrain->is_wall(y, x - x_step) || terrain->is_wall(y - y_step, x))) {
            return false;
        }
    }

    // If no walls are found, return true.
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


void AStarPather::add_intermediate_points(std::list<Vec3>& path, float maxDistance) {
    auto current = path.begin();
    auto end = path.end();

    while (current != end) {
        auto next = std::next(current);
        if (next == end) {
            break;
        }

        float dist = distance(*current, *next);
        while (dist > maxDistance) {
            Vec3 intermediatePoint = (*current + *next) * 0.5f;
            next = path.insert(next, intermediatePoint);
            dist = distance(*current, *next);
        }
        ++current;
    }
}


