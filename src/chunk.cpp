#include "chunk.h"
#include <iostream>
#include <random>

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 10);

Chunk::Chunk() {
  vectorMapGround.resize(chunkSize, std::vector<int>(chunkSize, 0));
  vectorMapObject.resize(chunkSize, std::vector<int>(chunkSize, 0));
  vectorMapElevation.resize(chunkSize, std::vector<int>(chunkSize, 0));
}

void Chunk::setCoordinates(sf::Vector2i coord) { chunkCoordinates = coord; }

std::string ChunkMap::setChunkID(sf::Vector2i coord) {
  return std::to_string(coord.x) + "," + std::to_string(coord.y);
}

void ChunkMap::Add(Chunk &chunk) {
  sf::Vector2i coordinates = chunk.chunkCoordinates;
  std::string chunkID = setChunkID(coordinates);
  chunkMap[chunkID] = chunk;
}

void ChunkMap::Remove(sf::Vector2i coordinates) {
  std::string chunkID = setChunkID(coordinates);
  chunkMap.erase(chunkID);
}

Chunk& ChunkMap::Get(sf::Vector2i coordinates) {
  std::string chunkID = setChunkID(coordinates);
  return chunkMap[chunkID];
}

bool ChunkMap::Exists(sf::Vector2i coordinates) {
  std::string chunkID = setChunkID(coordinates);
  auto iterator = chunkMap.find(chunkID);
  if (iterator == chunkMap.end()) {return false;} else {return true;}
}

void GenerateChunk(Chunk &chunk) {
  for (int i=0; i<chunkSize; i++) {
    for (int j=0; j<chunkSize; j++) {
      chunk.vectorMapElevation[i][j] = 1;
      chunk.vectorMapGround[i][j] = 0;
      int currentPred = dist(rng);
      if (currentPred <= 1) {
        chunk.vectorMapObject[i][j] = 1;
      } else {
        chunk.vectorMapObject[i][j] = 0;
      }
    }
  }
}

void spawnChunk(sf::Vector2i coord, ChunkMap &cMap) {
  Chunk chunk;
  chunk.setCoordinates(coord);
  GenerateChunk(chunk);
  cMap.Add(chunk);
}
