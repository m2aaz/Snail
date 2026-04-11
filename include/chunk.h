#pragma once
#include "common.h"
#include <string>
#include <unordered_map>
#include <vector>

class Chunk {
  public:
	std::vector<std::vector<int>> vectorMapGround;
	std::vector<std::vector<int>> vectorMapObject;
	std::vector<std::vector<int>> vectorMapElevation;
	sf::Vector2i chunkCoordinates;

	Chunk();
	void setCoordinates(sf::Vector2i coord);
};

struct ChunkMap {
	std::unordered_map<std::string, Chunk> chunkMap;

	std::string setChunkID(sf::Vector2i coord);
	void Add(Chunk &chunk);
	void Remove(sf::Vector2i coordinates);
	Chunk &Get(sf::Vector2i coordinates);
	bool Exists(sf::Vector2i coordinates);
};

void GenerateChunk(Chunk &chunk);
void spawnChunk(sf::Vector2i coord, ChunkMap &cMap);
