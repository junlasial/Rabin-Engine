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


// Check the distance to the closest wall and return the min dist.

float distance_to_closest_wall(int currentRow, int currentCol)
{
    //Init max value
    float minimumDistance = 999999999999.9f;

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    // Cycle through all the row including outside boundary
    for (int wallRow = -1; wallRow <= mapHeight; ++wallRow)
    {
        // Cycle through all the columns
        for (int wall_Column = -1; wall_Column <= mapWidth; ++wall_Column)
        {
            // If it is a wall or if it is not a valid grid position
            if (!terrain->is_valid_grid_position(wallRow, wall_Column) || terrain->is_wall(wallRow, wall_Column))
            {
                // Compare Euclidean Distance 
                float distance = euclidean_distance(currentRow, currentCol, wallRow, wall_Column);
                if (distance < minimumDistance)
                {
                    minimumDistance = distance;
                }
            }
        }
    }

    return minimumDistance;
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

    //get the current map_w and map_h
    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    // Get the center positions of the starting and ending cells
    Vec2 start = get_center(row0, col0);
    Vec2 end = get_center(row1, col1);

    // Determine the bounding box of the line segment
    int minimum_row = std::min(row0, row1);
    int minimum_col = std::min(col0, col1);
    int maximum_row = std::max(row0, row1);
    int maximum_col = std::max(col0, col1);


    // Puff factor
    const float puff = 0.01f;
    const float half = 0.5f;


    // Iterate only over the cells within the bounding box
    for (int row = minimum_row; row <= maximum_row; ++row)
    {
        for (int col = minimum_col; col <= maximum_col; ++col)
        {
            // Check if the cell is a wall
            if (terrain->is_wall(row, col))
            {


                // Get the boundaries of the wall cell with puff
                Vec2 topLeft = get_center(row, col) + Vec2(-half - puff, -half - puff);
                Vec2 topRight = get_center(row, col) + Vec2(half + puff, -half - puff);
                Vec2 bottomLeft = get_center(row, col) + Vec2(-half - puff, half + puff);
                Vec2 bottomRight = get_center(row, col) + Vec2(half + puff, half + puff);

                // Check if the line intersects any of the four boundaries of the wall cell
                if (line_intersect(start, end, topLeft, topRight)       ||
                    line_intersect(start, end, topRight, bottomRight)   ||
                    line_intersect(start, end, bottomRight, bottomLeft) ||
                    line_intersect(start, end, bottomLeft, topLeft))
                {

                    //else
                    return false;
                }
            }
        }
    }

    // If no intersections are found, the path is clear
    return true;
}



void analyze_openness(MapLayer<float>& opennessLayer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    // WRITE YOUR CODE HERE

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            if (!terrain->is_wall(row, col))
            {
                float distanceToWall = distance_to_closest_wall(row, col);
                if (distanceToWall > 0.0f)
                {
                    opennessLayer.set_value(row, col, 1.0f / (distanceToWall * distanceToWall));
                }
                else
                {
                    opennessLayer.set_value(row, col, std::numeric_limits<float>::max());
                }
            }
        }
    }
}



void analyze_visibility(MapLayer<float>& visibilityLayer)
{
    /*
        mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  make sure
        to cap the value at 1.0 as well.

        two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  make use of the is_clear_path
        helper function.
    */

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    std::vector<std::vector<int>> visibilityCounts(mapHeight, std::vector<int>(mapWidth, 0));

    clear_visibility_layer(visibilityLayer, mapHeight, mapWidth);
    calculate_visibility_counts(visibilityCounts, mapHeight, mapWidth);
    set_visibility_values(visibilityLayer, visibilityCounts, mapHeight, mapWidth);
}



void analyze_visible_to_cell(MapLayer<float>& visibilityLayer, int targetRow, int targetCol)
{
    /*
        for every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell. make use of the is_clear_path
        helper function.
    */

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    mark_visible_cells(visibilityLayer, targetRow, targetCol, mapHeight, mapWidth);
    mark_cells_next_to_visible(visibilityLayer, mapHeight, mapWidth);
}


void analyze_agent_vision(MapLayer<float>& visionLayer, const Agent* agent)
{
    // get the agent's position and which way they're looking
    Vec3 agentPosition = agent->get_position();
    Vec3 agentForwardVector = agent->get_forward_vector();
    const float radians = M_PI / 180.0F;
    // normalize the agent's forward vector
    agentForwardVector.Normalize();

   // Make sure devidie angle by 2
    const float fieldOfViewCosine = cosf(198 / 2 * radians); 

    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    // go through each cell in the layer
    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            // skip walls
            if (terrain->is_wall(row, col))
            {
                continue;
            }

            // get the center position of the current cell
            Vec3 cellCenter = terrain->get_world_position(row, col);

            // create a vector from the agent to the cell
            Vec3 vectorToCell(cellCenter - agentPosition);

            // normalize the vector from agent to cell
            vectorToCell.Normalize();

            // calculate the dot product between the agent's forward vector and the vector to the cell
            float dotProduct = agentForwardVector.Dot(vectorToCell);

            // check if the cell is within the agent's field of view
            if (dotProduct > fieldOfViewCosine)
            {
                // check if the path to the cell is clear
                GridPos agent_GridPosition = terrain->get_grid_position(agentPosition);
                if (is_clear_path(agent_GridPosition.row, agent_GridPosition.col, row, col))
                {
                    // mark the cell as visible
                    visionLayer.set_value(row, col, 1.0f);
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

void enemy_field_of_view(MapLayer<float>& mapLayer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent* enemy)
{
    /*
        first, clear out the old values in the map layer by settin any negative value to 0.
        then, for every cell in the layer that's in the field of view cone from the
        enemy agent, mark it with the occupancy value. take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of takin the arccosine to avoid floatin-point
        inaccuracy (bigger cosine means smaller angle).

        if the tile is close enough to the enemy (less than closeDistance),
        just check if it's visible to enemy. use the is_clear_path
        helper function. otherwise, gotta consider the direction the enemy is facing too.
        this makes a radius around the enemy where the player can be detected,
        and a fov cone.
    */

    // clear out old values in the map layer by settin any negative value to 0
    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();

    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            if (mapLayer.get_value(row, col) < 0)
            {
                mapLayer.set_value(row, col, 0);
            }
        }
    }

    // get the enemy's position and which way they're lookin
    Vec3 enemyPos = enemy->get_position();
    Vec3 enemyForward = enemy->get_forward_vector();
    const float radian = M_PI / 180.0f;

    // normalize the enemy's forward vector
    enemyForward.Normalize();

    // convert field of view angle to cosine for comparison
    float cosFov = cosf(fovAngle / 2 * radian);

    // go through each cell in the layer
    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            // skip walls
            if (terrain->is_wall(row, col))
            {
                continue;
            }

            // get the center position of the current cell
            Vec3 cellCenter = terrain->get_world_position(row, col);

            // calculate the vector from the enemy to the cell
            Vec3 enemyToCell = cellCenter - enemyPos;

            // calculate the distance from the enemy to the cell
            float distanceToCell = enemyToCell.Length();

            GridPos enemyGridPos = terrain->get_grid_position(enemyPos);
            GridPos cellGridPos = terrain->get_grid_position(cellCenter);

            // check if the cell is within the close distance radius
            if (distanceToCell < closeDistance)
            {
                if (is_clear_path(enemyGridPos.row, enemyGridPos.col, cellGridPos.row, cellGridPos.col))
                {
                    mapLayer.set_value(row, col, occupancyValue);
                }
            }
            else
            {
                // normalize the vector from the enemy to the cell
                enemyToCell.Normalize();

                // calculate the dot product between the enemy's forward vector and the vector to the cell
                float dotProduct = enemyForward.Dot(enemyToCell);

                // check if the cell is in the enemy's field of view cone
                if (dotProduct > cosFov)
                {
                    if (is_clear_path(enemyGridPos.row, enemyGridPos.col, cellGridPos.row, cellGridPos.col))
                    {
                        mapLayer.set_value(row, col, occupancyValue);
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
bool enemy_seek_player(MapLayer<float>& visionLayer, AStarAgent* enemy)
{
    /*
        try to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        if there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        return whether a target cell was found.
    */

    float maxo_Value = 0.0f;
    Vec3 targetPosition = Vec3(0.0f, 0.0f, 0.0f); // default initialization
    bool isTargetFound = false;
    float shortestDistance = std::numeric_limits<float>::max();



    Vec3 enemyPosition = enemy->get_position();
    GridPos enemyGridPos = terrain->get_grid_position(enemyPosition);


    int mapHeight = terrain->get_map_height();
    int mapWidth = terrain->get_map_width();




    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            float o_Value = visionLayer.get_value(row, col);

            if (o_Value > maxo_Value)
            {
                maxo_Value = o_Value;
                targetPosition = terrain->get_world_position(row, col);
                shortestDistance = euclidean_distance(enemyGridPos.row, enemyGridPos.col, row, col);
                isTargetFound = true;
            }
            else if (o_Value == maxo_Value && o_Value > 0)
            {
                float distanceToEnemy = euclidean_distance(enemyGridPos.row, enemyGridPos.col, row, col);
                if (distanceToEnemy < shortestDistance)
                {
                    targetPosition = terrain->get_world_position(row, col);
                    shortestDistance = distanceToEnemy;
                }
            }
        }
    }

    if (isTargetFound)
    {
        enemy->path_to(targetPosition);
        return true;
    }

    return false;
}



//Student function


//Helper

void clear_visibility_layer(MapLayer<float>& visibilityLayer, int mapHeight, int mapWidth)
{
    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            if (terrain->is_wall(row, col))
            {
                visibilityLayer.set_value(row, col, 0.0f);
            }
        }
    }
}

void calculate_visibility_counts(std::vector<std::vector<int>>& visibilityCounts, int mapHeight, int mapWidth)
{
    for (int startRow = 0; startRow < mapHeight; ++startRow)
    {
        for (int startCol = 0; startCol < mapWidth; ++startCol)
        {
            if (!terrain->is_wall(startRow, startCol))
            {
                for (int endRow = startRow; endRow < mapHeight; ++endRow)
                {
                    for (int endCol = (endRow == startRow ? startCol + 1 : 0); endCol < mapWidth; ++endCol)
                    {
                        if (!terrain->is_wall(endRow, endCol) && is_clear_path(startRow, startCol, endRow, endCol))
                        {
                            visibilityCounts[startRow][startCol]++;
                            visibilityCounts[endRow][endCol]++;
                        }
                    }
                }
            }
        }
    }
}

void set_visibility_values(MapLayer<float>& visibilityLayer, const std::vector<std::vector<int>>& visibilityCounts, int mapHeight, int mapWidth)
{
    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            if (!terrain->is_wall(row, col))
            {
                float visibilityValue = static_cast<float>(visibilityCounts[row][col]) / 160.0f;
                if (visibilityValue > 1.0f)
                {
                    visibilityValue = 1.0f;
                }
                visibilityLayer.set_value(row, col, visibilityValue);
            }
        }
    }
}


void mark_visible_cells(MapLayer<float>& visibilityLayer, int targetRow, int targetCol, int mapHeight, int mapWidth)
{
    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            if (terrain->is_wall(row, col))
            {
                visibilityLayer.set_value(row, col, 0.0f); // ensure walls are marked as 0.0
            }
            else if (is_clear_path(targetRow, targetCol, row, col))
            {
                visibilityLayer.set_value(row, col, 1.0f);
            }
            else
            {
                visibilityLayer.set_value(row, col, 0.0f);
            }
        }
    }

    // mark the original cell as visible to itself
    visibilityLayer.set_value(targetRow, targetCol, 1.0f);
}

void mark_cells_next_to_visible(MapLayer<float>& visibilityLayer, int mapHeight, int mapWidth)
{
    const std::vector<std::pair<int, int>> directions = {
        {-1,  0},  // top
        { 1,  0},  // bottom
        { 0, -1},  // left
        { 0,  1},  // right
        {-1, -1},  // top-left
        {-1,  1},  // top-right
        { 1, -1},  // bottom-left
        { 1,  1}   // bottom-right
    };

    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            if (visibilityLayer.get_value(row, col) == 0.0f && !terrain->is_wall(row, col))
            {
                bool besideVisible = false;

                for (const auto& [rowDir, colDir] : directions)
                {
                    int neighborRow = row + rowDir;
                    int neighborCol = col + colDir;

                    if (terrain->is_valid_grid_position(neighborRow, neighborCol) &&
                        !terrain->is_wall(neighborRow, neighborCol) &&
                        visibilityLayer.get_value(neighborRow, neighborCol) == 1.0f)
                    {
                        // additional check for diagonals
                        if (abs(rowDir) == 1 && abs(colDir) == 1)
                        {
                            // ensure both adjacent cells are not walls
                            if (!terrain->is_wall(row, col + colDir) && !terrain->is_wall(row + rowDir, col))
                            {
                                besideVisible = true;
                                break;
                            }
                        }
                        else
                        {
                            besideVisible = true;
                            break;
                        }
                    }
                }

                if (besideVisible)
                {
                    visibilityLayer.set_value(row, col, 0.5f);
                }
            }
        }
    }
}



//Extra Func
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



