#include "chunk_behaviour.h"
#include <iostream>

void ChunkBehaviour::placeChunk(sf::RenderWindow &window, ChunkMap &cMap) {
	sf::Vector2i mouseScreen = sf::Mouse::getPosition(window);
	sf::Vector2f mouseWorld = window.mapPixelToCoords(mouseScreen);

	sf::Vector2i chunkCoord = WorldToChunk(mouseWorld);

	if (!cMap.Exists(chunkCoord)) {
		spawnChunk(chunkCoord, cMap);
	} else {
		std::cout << "Chunk Already Exists." << std::endl;
	}
}

void ChunkBehaviour::deleteChunk(sf::RenderWindow &window, ChunkMap &cMap) {
	sf::Vector2i mouseScreen = sf::Mouse::getPosition(window);
	sf::Vector2f mouseWorld = window.mapPixelToCoords(mouseScreen);

	sf::Vector2i chunkCoord = WorldToChunk(mouseWorld);

	if (cMap.Exists(chunkCoord)) {
		cMap.Remove(chunkCoord);
	} else {
		std::cout << "Chunk Doesn't Exists." << std::endl;
	}
}
