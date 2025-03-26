//
// Created by Somed on 3/17/2025.
//

#ifndef TILEGRID_H
#define TILEGRID_H
#include <glm/vec2.hpp>
#include <glm/detail/type_vec2.hpp>

#include "Utils/StackAllocator.h"

//Hexagon is point up not flat-up.
//Isometric has top right as up.
enum GridShape
{
  GRID_RECTANGULAR = 0,
  GRID_HEXAGON,
  GRID_ISOMETRIC
};

class TileGrid {
public:
  TileGrid(GridShape tileShape, glm::vec2 tileSize);
  TileGrid() = delete;
  ~TileGrid() = default;

  glm::vec2 GetTileSize() const {return m_tileSize;}
  GridShape GetShape() const {return m_shape;}

  glm::vec2 GridToWorldSpace(std::pair<int,int> tile);
  //In grids with
  glm::vec2 WorldToGridSpace(glm::vec2 point);

  //Clears data each time.
  std::vector<glm::vec2> AdjacentTiles(glm::vec2 tile);

private:
  //Use custom dynamic array here.
  GridShape m_shape{};
  glm::vec2 m_tileSize{};
};



#endif //TILEGRID_H
