#include "chunk_renderer.h"
#include <algorithm>
#include <iostream>

void ChunkRenderer::Init() {
	createLookup();
	LoadTexture();
}

void ChunkRenderer::LoadTexture() {
	if (texture.loadFromFile(fileloc)) {
		currentTile.setTexture(texture);
	}
}

sf::Vector2i ChunkRenderer::ConvertToPos(int itemID) {
	auto it = TileLookup.find(itemID);
	if (it == TileLookup.end())
		return {0, 0};
	return it->second;
}

void ChunkRenderer::createLookup() {
	TileLookup[0] = {6, 8}; // main dark water
	TileLookup[1] = {7, 3}; // grass
	TileLookup[2] = {10, 3}; // bush
	// Water variations 
	TileLookup[3] = {7, 8}; // variant 1
	TileLookup[4] = {8, 8}; // variant 2
}

sf::Vector2f ChunkRenderer::GetChunkWorldPos(const Chunk &chunk) {
	float chunkCol = chunk.chunkCoordinates.x * chunkSize;
	float chunkRow = chunk.chunkCoordinates.y * chunkSize;
	return GridToWorld(chunkRow, chunkCol);
}

void ChunkRenderer::RenderTile(sf::Vector2i tilePos, int row, int col,
							   sf::Vector2f worldPos) {
	int posX = tilePos.x * tileSpacing;
	int posY = tilePos.y * tileSpacing;
	sf::Vector2f isoCoord = GridToWorld(row, col);
	currentTile.setTextureRect({posX, posY, tileSpacing, tileSpacing});
	currentTile.setPosition(isoCoord.x + worldPos.x, isoCoord.y + worldPos.y);
}

void ChunkRenderer::RenderChunk(sf::RenderWindow &window, const Chunk &chunk) {
	sf::Vector2f worldPos = GetChunkWorldPos(chunk);
	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			if (chunk.vectorMapElevation[i][j] > 0) {
				int ID = chunk.vectorMapGround[i][j];
				sf::Vector2i tilePos = ConvertToPos(ID);
				RenderTile(tilePos, i, j, worldPos);
				window.draw(currentTile);

				ID = chunk.vectorMapObject[i][j];
				if (ID != 0) {
					tilePos = ConvertToPos(ID);
					RenderTile(tilePos, i, j, worldPos);
					window.draw(currentTile);
				}
			}
		}
	}
}

// chunks close to camera are rendered first
void ChunkRenderer::RenderChunkMap(sf::RenderWindow &window, ChunkMap &cMap) {
	std::vector<Chunk *> sorted;
	for (auto &[key, chunk] : cMap.chunkMap) {
		sorted.push_back(&chunk);
	}

	std::sort(sorted.begin(), sorted.end(), [](const Chunk *a, const Chunk *b) {
		return (a->chunkCoordinates.x + a->chunkCoordinates.y) <
			   (b->chunkCoordinates.x + b->chunkCoordinates.y);
	});

	for (Chunk *chunk : sorted) {
		RenderChunk(window, *chunk);
	}
}

void ChunkRenderer::DisplayChunk(const Chunk &chunk) {
	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			std::cout << chunk.vectorMapGround[i][j] << ", ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			std::cout << chunk.vectorMapObject[i][j] << ", ";
		}
		std::cout << std::endl;
	}
}
