//
// Created by Somed on 3/17/2025.
//

#include "TileGrid.h"

TileGrid::TileGrid(GridShape tileShape, glm::vec2 tileSize)
{
    m_shape = tileShape;
    m_tileSize = tileSize;
}

std::vector<glm::vec2> TileGrid::AdjacentTiles(glm::vec2 tile)
{
    std::vector<glm::vec2> adj = std::vector<glm::vec2>(6);
    switch (m_shape)
    {
        case GRID_RECTANGULAR:
        case GRID_ISOMETRIC:
            adj.emplace_back(tile.x - 1, tile.y);
            adj.emplace_back(tile.x + 1, tile.y);
            adj.emplace_back(tile.x, tile.y + 1);
            adj.emplace_back(tile.x, tile.y - 1);
            break;
        case GRID_HEXAGON:
            //Odd rows offset to the right.
            adj.emplace_back(tile.x - 1, tile.y);
            adj.emplace_back(tile.x + 1, tile.y);
            adj.emplace_back(tile.x, tile.y + 1);
            adj.emplace_back(tile.x, tile.y - 1);
            if ((int)tile.y % 2 == 1)
            {
                adj.emplace_back(tile.x + 1, tile.y + 1);
                adj.emplace_back(tile.x + 1, tile.y - 1);
            } else
            {
                adj.emplace_back(tile.x - 1, tile.y + 1);
                adj.emplace_back(tile.x - 1, tile.y - 1);
            }
            break;
    }
    return adj;
}

glm::vec2 TileGrid::GridToWorldSpace(std::pair<int,int> tile)
{
    switch (m_shape)
    {
        case GRID_RECTANGULAR:
            return {(float)tile.first * m_tileSize.x, (float)tile.second * m_tileSize.y};
            break;
        case GRID_ISOMETRIC:
            const glm::vec2 yVal = {(float)tile.second * (m_tileSize.x * 0.5f), (float)tile.second * (m_tileSize.y * 0.5f)};
            const glm::vec2 xVal = {(float)tile.first * (m_tileSize.x * 0.5f), -(float)tile.first * (m_tileSize.y * 0.5f)};
            return (xVal + yVal) - (m_tileSize * 0.5f);
            break;
        case GRID_HEXAGON:
            glm::vec2 currentPos = {(float)tile.first * m_tileSize.x, (float)tile.second * m_tileSize.y * 0.75f};
            if (tile.first % 2 == 1)
            {
                currentPos.x += m_tileSize.x * 0.5f;
            }
            return currentPos;
            break;
    }
    return {};
}

glm::vec2 TileGrid::WorldToGridSpace(glm::vec2 point)
{
    if (m_shape == GRID_RECTANGULAR)
    {
        return {std::round(point.x / m_tileSize.x), std::round(point.y / m_tileSize.y)};
    } else if (m_shape == GRID_ISOMETRIC)
    {
        const float tileX = m_tileSize.x * 0.5f;
        const float tileY = m_tileSize.y * 0.5f;
        const float clampX = std::round(point.x / tileX);
        const float clampY = std::round(point.y / tileY);
        float dist = clampX - clampY / 2.0f;
        return {dist,clampX + dist};
    } else if (m_shape == GRID_HEXAGON)
    {
        std::cout << "Not implemented yet!" << std::endl;
        return {};
    } else
    {
        std::cout << "Invalid shape" << std::endl;
        return {};
    }
}




