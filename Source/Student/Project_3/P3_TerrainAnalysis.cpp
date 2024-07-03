#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col)
{

    //Initialise with big value so when compare with minimum will be replaced
    float minDistance = std::numeric_limits<float>::max();

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    //Cycle thru all the rows
    for (int wall_r = -1; wall_r <= mapHeight; ++wall_r)
    {
        //Cycle through all the columns
        for (int wall_c = -1; wall_c <= mapWidth; ++wall_c)
        {

            //If it is a wall or if it is not a valid grid pos
            if (!terrain->is_valid_grid_position(wall_r, wall_c) || terrain->is_wall(wall_r, wall_c))
            {

                //Compare Euclidean Distance 
               float distance =  euclidean_distance(row, col, wall_r, wall_c);
                if (distance < minDistance)
                {
                    minDistance = distance;
                }
            }
        }
    }

    return minDistance;
}


bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
    Two cells (row0, col0) and (row1, col1) are visible to each other if a line
    between their centerpoints doesn't intersect the four boundary lines of every
    wall cell.  You should puff out the four boundary lines by a very tiny amount
    so that a diagonal line passing by the corner will intersect it.  Make use of the
    line_intersect helper function for the intersection test and the is_wall member
    function in the global terrain to determine if a cell is a wall or not.
    */

    // Get the center positions of the starting and ending cells
    Vec2 start = get_center(row0, col0);
    Vec2 end = get_center(row1, col1);

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    // Determine the bounding box of the line segment
    int minRow = std::min(row0, row1);
    int maxRow = std::max(row0, row1);
    int minCol = std::min(col0, col1);
    int maxCol = std::max(col0, col1);

    // Puff factor
    const float puff = 0.01f;

    // Iterate only over the cells within the bounding box
    for (int r = minRow; r <= maxRow; ++r)
    {
        for (int c = minCol; c <= maxCol; ++c)
        {
            // Check if the cell is a wall
            if (terrain->is_wall(r, c))
            {
                // Get the boundaries of the wall cell with puff
                Vec2 topLeft = get_center(r, c) + Vec2(-0.5f - puff, -0.5f - puff);
                Vec2 topRight = get_center(r, c) + Vec2(0.5f + puff, -0.5f - puff);
                Vec2 bottomLeft = get_center(r, c) + Vec2(-0.5f - puff, 0.5f + puff);
                Vec2 bottomRight = get_center(r, c) + Vec2(0.5f + puff, 0.5f + puff);

                // Check if the line intersects any of the four boundaries of the wall cell
                if (line_intersect(start, end, topLeft, topRight) ||
                    line_intersect(start, end, topRight, bottomRight) ||
                    line_intersect(start, end, bottomRight, bottomLeft) ||
                    line_intersect(start, end, bottomLeft, topLeft))
                {
                    // If any intersection is found, the path is not clear
                    return false;
                }
            }
        }
    }

    // If no intersections are found, the path is clear
    return true;
}


void analyze_openness(MapLayer<float>& layer)
{


    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    // WRITE YOUR CODE HERE


    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    for (int r = 0; r < mapHeight; ++r)
    {
        for (int c = 0; c < mapWidth; ++c)
        {
            if (!terrain->is_wall(r, c))
            {
                float distance = distance_to_closest_wall(r, c);
                if (distance > 0.0f)
                {
                    layer.set_value(r, c, 1.0f / (distance * distance));
                }
                else
                {
                    layer.set_value(r, c, std::numeric_limits<float>::max());
                }
            }
        }
    }
}


void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE

        int mapHeight = terrain->get_map_height();
        int mapWidth = terrain->get_map_width();

        // Create a 2D array to store visibility counts
        std::vector<std::vector<int>> visibilityCounts(mapHeight, std::vector<int>(mapWidth, 0));

        // Iterate over all cells in the map
        for (int r = 0; r < mapHeight; ++r)
        {
            for (int c = 0; c < mapWidth; ++c)
            {
                // Skip wall cells
                if (terrain->is_wall(r, c))
                {
                    continue;
                }

                // Check visibility with every other cell, considering only cells ahead in the grid
                for (int r2 = r; r2 < mapHeight; ++r2)
                {
                    for (int c2 = (r2 == r ? c + 1 : 0); c2 < mapWidth; ++c2)
                    {
                        // Skip wall cells
                        if (terrain->is_wall(r2, c2))
                        {
                            continue;
                        }

                        if (is_clear_path(r, c, r2, c2))
                        {
                            visibilityCounts[r][c]++;
                            visibilityCounts[r2][c2]++;
                        }
                    }
                }
            }
        }

        // Set the visibility values in the layer
        for (int r = 0; r < mapHeight; ++r)
        {
            for (int c = 0; c < mapWidth; ++c)
            {
                // Skip wall cells
                if (terrain->is_wall(r, c))
                {
                    layer.set_value(r, c, 0.0f);
                    continue;
                }

                // Calculate visibility value
                float visibilityValue = static_cast<float>(visibilityCounts[r][c]) / 160.0f;
                // Cap the value at 1.0
                if (visibilityValue > 1.0f)
                {
                    visibilityValue = 1.0f;
                }

                // Set the visibility value in the layer
                layer.set_value(r, c, visibilityValue);
            }
        }
    
}

void analyze_visible_to_cell(MapLayer<float>& layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    // First pass: Mark cells visible to the given cell with 1.0
    for (int r = 0; r < mapHeight; ++r)
    {
        for (int c = 0; c < mapWidth; ++c)
        {
            if (terrain->is_wall(r, c))
            {
                layer.set_value(r, c, 0.0f); // Ensure walls are marked as 0.0
            }
            else if (is_clear_path(row, col, r, c))
            {
                layer.set_value(r, c, 1.0f);
            }
            else
            {
                layer.set_value(r, c, 0.0f);
            }
        }
    }

    // Mark the original cell as visible to itself
    layer.set_value(row, col, 1.0f);

    // Second pass: Mark cells next to a visible cell with 0.5 if they are not blocked
    for (int r = 0; r < mapHeight; ++r)
    {
        for (int c = 0; c < mapWidth; ++c)
        {
            if (layer.get_value(r, c) == 0.0f && !terrain->is_wall(r, c))
            {
                // Check the 8 neighboring cells (including diagonals)
                bool nextToVisible = false;
                const std::vector<std::pair<int, int>> directions = {
                    {-1, 0}, // top
                    {1, 0},  // bottom
                    {0, -1}, // left
                    {0, 1},  // right
                    {-1, -1}, // top-left
                    {-1, 1},  // top-right
                    {1, -1},  // bottom-left
                    {1, 1}    // bottom-right
                };

                for (const auto& [dr, dc] : directions)
                {
                    int nr = r + dr;
                    int nc = c + dc;

                    if (terrain->is_valid_grid_position(nr, nc) && !terrain->is_wall(nr, nc) && layer.get_value(nr, nc) == 1.0f)
                    {
                        // Additional check for diagonals
                        if (abs(dr) == 1 && abs(dc) == 1)
                        {
                            // Ensure both adjacent cells are not walls
                            if (!terrain->is_wall(r, c + dc) && !terrain->is_wall(r + dr, c))
                            {
                                nextToVisible = true;
                                break;
                            }
                        }
                        else
                        {
                            nextToVisible = true;
                            break;
                        }
                    }
                }

                if (nextToVisible)
                {
                    layer.set_value(r, c, 0.5f);
                }
            }
        }
    }
}


void analyze_agent_vision(MapLayer<float>& layer, const Agent* agent)
{
    // Get the agent's position and forward vector
    Vec3 agentPos = agent->get_position();
    Vec3 agentForward = agent->get_forward_vector();

    // Normalize the agent's forward vector
    agentForward.Normalize();

    // Define the agent's field of view (slightly larger than 180 degrees)
    const float fieldOfView = cosf(200/2 * M_PI/180.0f); // cosine of 180 degrees + small angle

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    // Iterate over each cell in the layer
    for (int r = 0; r < mapHeight; ++r)
    {
        for (int c = 0; c < mapWidth; ++c)
        {
            // Skip if the cell is a wall
            if (terrain->is_wall(r, c))
            {
                continue;
            }

            // Get the center position of the current cell in grid coordinates
            Vec3 cellCenter = terrain->get_world_position(r, c);

            // Create a vector from the agent to the cell in grid coordinates
            Vec3 agentToCell(cellCenter-agentPos);

            // Normalize the vector from agent to cell
            agentToCell.Normalize();

            // Calculate the dot product between the agent's forward vector and the vector to the cell
            float dotProduct = agentForward.Dot( agentToCell);

            // Check if the cell is within the agent's field of view
            if (dotProduct > fieldOfView)
            {
                // Check if the path to the cell is clear using grid positions
                GridPos agentGridPos = terrain->get_grid_position(agentPos);
                if (is_clear_path(agentGridPos.row, agentGridPos.col, r, c))
                {
                    // Mark the cell as visible
                    layer.set_value(r, c, 1.0f);
                }
            }
        }
    }
}




void propagate_solo_occupancy(MapLayer<float>& layer, float decay, float growth) {
    // Define the size of the temporary layer
    const int layerSize = 40;
    float tempLayer[layerSize][layerSize] = { 0 };

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    // Process each cell in the layer
    for (int r = 0; r < mapHeight; ++r) {
        for (int c = 0; c < mapWidth; ++c) {
            // Skip walls
            if (terrain->is_wall(r, c)) {
                continue;
            }

            // Get the value of the current cell
            float currentValue = layer.get_value(r, c);
            float maxNeighborValue = 0.0f;

            // Iterate over the eight neighbors
            int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
            int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

            for (int i = 0; i < 8; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];

                // Check if the neighbor is within bounds
                if (terrain->is_valid_grid_position(nr, nc)) {
                    bool isDiagonal = (std::abs(dr[i]) == 1 && std::abs(dc[i]) == 1);
                    bool pathBlocked = false;

                    if (isDiagonal) {
                        // Check if the path to the diagonal neighbor is blocked by a wall
                        if (terrain->is_wall(r + dr[i], c) || terrain->is_wall(r, c + dc[i])) {
                            pathBlocked = true;
                        }
                    }

                    // Check if the neighbor is not a wall and the path is not blocked
                    if (!terrain->is_wall(nr, nc) && !pathBlocked) {
                        // Calculate the distance
                        float distance = static_cast<float>(std::sqrt(dr[i] * dr[i] + dc[i] * dc[i]));
                        // Apply the exponential decay formula
                        float neighborValue = layer.get_value(nr, nc) * std::exp(-distance * decay);
                        // Keep the highest value
                        maxNeighborValue = std::max(maxNeighborValue, neighborValue);
                    }
                }
            }

            // Apply the linear interpolation formula with a slower growth factor
            float newValue = (1 - growth * 1.0f) * currentValue + growth * 1.0f * maxNeighborValue;

            // Store the value in the temporary layer
            tempLayer[r][c] = newValue;
        }
    }

    // Write the temporary layer back into the original layer
    for (int r = 0; r < mapHeight; ++r) {
        for (int c = 0; c < mapWidth; ++c) {
            if (!terrain->is_wall(r, c)) {
                layer.set_value(r, c, tempLayer[r][c]);
            }
        }
    }
}




void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
 


        //// Define the size of the temporary layer
        //const int layerSize = 40;
        //float tempLayer[layerSize][layerSize] = { 0 };

        //int mapHeight = terrain->get_map_height();
        //int mapWidth = terrain->get_map_width();

        //// Process each cell in the layer
        //for (int r = 0; r < mapHeight; ++r) {
        //    for (int c = 0; c < mapWidth; ++c) {
        //        // Skip walls
        //        if (terrain->is_wall(r, c)) {
        //            continue;
        //        }

        //        // Get the value of the current cell
        //        float currentValue = layer.get_value(r, c);
        //        float maxAbsNeighborValue = 0.0f;

        //        // Iterate over the eight neighbors
        //        int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        //        int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

        //        for (int i = 0; i < 8; ++i) {
        //            int nr = r + dr[i];
        //            int nc = c + dc[i];

        //            // Check if the neighbor is within bounds
        //            if (terrain->is_valid_grid_position(nr, nc)) {
        //                bool isDiagonal = (std::abs(dr[i]) == 1 && std::abs(dc[i]) == 1);
        //                bool pathBlocked = false;

        //                if (isDiagonal) {
        //                    // Check if the path to the diagonal neighbor is blocked by a wall
        //                    if (terrain->is_wall(r + dr[i], c) || terrain->is_wall(r, c + dc[i])) {
        //                        pathBlocked = true;
        //                    }
        //                }

        //                // Check if the neighbor is not a wall and the path is not blocked
        //                if (!terrain->is_wall(nr, nc) && !pathBlocked) {
        //                    // Calculate the distance
        //                    float distance = static_cast<float>(std::sqrt(dr[i] * dr[i] + dc[i] * dc[i]));
        //                    // Apply the exponential decay formula
        //                    float neighborValue = layer.get_value(nr, nc) * std::exp(-distance * decay);
        //                    // Keep the highest absolute value
        //                    if (std::abs(neighborValue) > std::abs(maxAbsNeighborValue)) {
        //                        maxAbsNeighborValue = neighborValue;
        //                    }
        //                }
        //            }
        //        }

        //        // Apply the linear interpolation formula with the growth factor
        //        float newValue = (1 - growth) * currentValue + growth * maxAbsNeighborValue;

        //        // Store the value in the temporary layer
        //        tempLayer[r][c] = newValue;
        //    }
        //}

        //// Write the temporary layer back into the original layer
        //for (int r = 0; r < mapHeight; ++r) {
        //    for (int c = 0; c < mapWidth; ++c) {
        //        if (!terrain->is_wall(r, c)) {
        //            layer.set_value(r, c, tempLayer[r][c]);
        //        }
        //    }
        //}
    


    

}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */


        // Determine the maximum value in the given layer
        float maxValue = std::numeric_limits<float>::min();

        int mapHeight = terrain->get_map_height();
        int mapWidth = terrain->get_map_width();

        for (int r = 0; r < mapHeight; ++r) {
            for (int c = 0; c < mapWidth; ++c) {
                float value = layer.get_value(r, c);
                if (value > maxValue) {
                    maxValue = value;
                }
            }
        }

        // Normalize the layer by dividing each cell by the maximum value
        for (int r = 0; r < mapHeight; ++r) {
            for (int c = 0; c < mapWidth; ++c) {
                float value = layer.get_value(r, c);
                if (value > 0) {
                    layer.set_value(r, c, value / maxValue);
                }
            }
        }
    

}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    // WRITE YOUR CODE HERE
 


}

void enemy_field_of_view(MapLayer<float>& layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent* enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

    // Clear out the old values in the map layer by setting any negative value to 0
    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    for (int r = 0; r < mapHeight; ++r)
    {
        for (int c = 0; c < mapWidth; ++c)
        {
            if (layer.get_value(r, c) < 0)
            {
                layer.set_value(r, c, 0);
            }
        }
    }

    // Get the enemy's position and forward vector
    Vec3 enemyPos = enemy->get_position();
    Vec3 enemyForward = enemy->get_forward_vector();

    // Normalize the enemy's forward vector
    enemyForward.Normalize();

    // Convert field of view angle to cosine for comparison (using float constants)
    float cosFov = cosf(fovAngle/2 * static_cast<float>(M_PI) / 180.0f);

    // Iterate over each cell in the layer
    for (int r = 0; r < mapHeight; ++r)
    {
        for (int c = 0; c < mapWidth; ++c)
        {
            // Skip if the cell is a wall
            if (terrain->is_wall(r, c))
            {
                continue;
            }

            // Get the center position of the current cell in world coordinates
            Vec3 cellCenter = terrain->get_world_position(r, c);

            // Calculate the vector from the enemy to the cell
            Vec3 enemyToCell = cellCenter - enemyPos;

            // Calculate the distance from the enemy to the cell
            float distanceToCell = enemyToCell.Length();

            GridPos enemyGridPos = terrain->get_grid_position(enemyPos);
            GridPos cellGridPos = terrain->get_grid_position(cellCenter);

            // Check if the cell is within the close distance radius
            if (distanceToCell < closeDistance)
            {
                if (is_clear_path(enemyGridPos.row, enemyGridPos.col, cellGridPos.row, cellGridPos.col))
                {
                    layer.set_value(r, c, occupancyValue);
                }
            }
            else
            {
                // Normalize the vector from the enemy to the cell
                enemyToCell.Normalize();

                // Calculate the dot product between the enemy's forward vector and the vector to the cell
                float dotProduct = enemyForward.Dot(enemyToCell);

                // Check if the cell is within the enemy's field of view cone
                if (dotProduct > cosFov)
                {
                    if (is_clear_path(enemyGridPos.row, enemyGridPos.col, cellGridPos.row, cellGridPos.col))
                    {
                        layer.set_value(r, c, occupancyValue);
                    }
                }
            }
        }
    }
}


bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{

    
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

    // WRITE YOUR CODE HERE

  
        int mapHeight = terrain->get_map_height();
        int mapWidth = terrain->get_map_width();

        float highestValue = 0.0f;
        Vec3 targetPos = Vec3(0.0f, 0.0f, 0.0f); // Default initialization
        bool targetFound = false;
        float closestDistance = std::numeric_limits<float>::max();

        Vec3 enemyPos = enemy->get_position();
        GridPos enemyGridPos = terrain->get_grid_position(enemyPos);

        for (int r = 0; r < mapHeight; ++r)
        {
            for (int c = 0; c < mapWidth; ++c)
            {
                float value = layer.get_value(r, c);

                if (value > highestValue)
                {
                    highestValue = value;
                    targetPos = terrain->get_world_position(r, c);
                    closestDistance = euclidean_distance(enemyGridPos.row, enemyGridPos.col, r, c);
                    targetFound = true;
                }
                else if (value == highestValue && value > 0)
                {
                    float distance = euclidean_distance(enemyGridPos.row, enemyGridPos.col, r, c);
                    if (distance < closestDistance)
                    {
                        targetPos = terrain->get_world_position(r, c);
                        closestDistance = distance;
                    }
                }
            }
        }

        if (targetFound)
        {
            enemy->path_to(targetPos);
            return true;
        }

        return false;
   

}




//Student function
float euclidean_distance(int row1, int col1, int row2, int col2)
{
    return static_cast<float>(std::sqrt((row1 - row2) * (row1 - row2) + (col1 - col2) * (col1 - col2)));
}

Vec2 get_center(int row, int col)
{
    // Assuming each cell is 1 unit by 1 unit and the center of each cell
    // is at (row + 0.5, col + 0.5)
    return Vec2(static_cast<float>(col) + 0.5f, static_cast<float>(row) + 0.5f);
}



