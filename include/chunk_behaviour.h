#pragma once
#include "chunk.h"

struct ChunkBehaviour {
	void placeChunk(sf::RenderWindow &window, ChunkMap &cMap);
	void deleteChunk(sf::RenderWindow &window, ChunkMap &cMap);
};
