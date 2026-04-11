#include "common.h"

sf::Vector2f GridToWorld(float row, float col) {
  return {(col - row) * (tileWidth / 2.0f), (col + row) * (tileHeight / 2.0f)};
}

sf::Vector2i WorldToGrid(sf::Vector2f world) {
  float a = world.x / (tileWidth / 2.0f);
  float b = world.y / (tileHeight / 2.0f);
  return {(int)std::floor((a + b) / 2.0f), (int)std::floor((b - a) / 2.0f)};
}

sf::Vector2i WorldToChunk(sf::Vector2f world) {
  sf::Vector2i grid = WorldToGrid(world);

  return {(int)std::floor((float)grid.x / chunkSize),
          (int)std::floor((float)grid.y / chunkSize)};
}
