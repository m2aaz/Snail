#include "chunk.h"
#include <iostream>
#include <random>
#include "FastNoiseLite.h"

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 99);

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

Chunk &ChunkMap::Get(sf::Vector2i coordinates) {
	std::string chunkID = setChunkID(coordinates);
	return chunkMap[chunkID];
}

bool ChunkMap::Exists(sf::Vector2i coordinates) {
	std::string chunkID = setChunkID(coordinates);
	auto iterator = chunkMap.find(chunkID);
	if (iterator == chunkMap.end()) {
		return false;
	} else {
		return true;
	}
}

void GenerateChunk(Chunk &chunk) {
	FastNoiseLite noise;
	noise.SetSeed(12345678);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.05f);

	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			int worldX = chunk.chunkCoordinates.x * chunkSize + j;
			int worldY = chunk.chunkCoordinates.y * chunkSize + i;

			float currentNoise = noise.GetNoise((float)worldX, (float)worldY);
			float height = (currentNoise + 1.0f) * 0.5f; // Make Noise Position (No Longer [-1, 1])

			chunk.vectorMapElevation[i][j] = (int)(height * 100);

			// Generation Logic - Only water and grass for natural flow
			if (currentNoise < -0.3f) {
				// Favor main water tile {6,8}, use variations very sparingly
				int waterType = dist(rng) % 100; // 0-99 range
				if (waterType < 1) { // 1% chance
					chunk.vectorMapGround[i][j] = 3; // variant 1 {7,8}
				} else if (waterType < 2) { // 1% chance
					chunk.vectorMapGround[i][j] = 4; // variant 2 {8,8}
				} else { // 98% chance
					chunk.vectorMapGround[i][j] = 0; // main water {6,8}
				}
			} else {
				chunk.vectorMapGround[i][j] = 1; // GRASS
			}

			// Generate vegetation on grass and submerged objects on water
			if (chunk.vectorMapGround[i][j] == 1) { // Grass tiles
				int vegetationChance = dist(rng);
				if (vegetationChance <= 15) { // 15% chance for vegetation
					chunk.vectorMapObject[i][j] = 2; // bush
				} else {
					chunk.vectorMapObject[i][j] = 0;
				}
			} else { // Water tiles - no objects
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
