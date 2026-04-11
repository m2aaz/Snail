#pragma once
#include "chunk.h"

class ChunkRenderer {
  private:
	sf::Texture texture;
	sf::Sprite currentTile;
	std::unordered_map<int, sf::Vector2i> TileLookup;
	std::string fileloc = "tilemap/era1.png";

  public:
	void Init();
	void LoadTexture();
	sf::Vector2i ConvertToPos(int itemID);
	void createLookup();
	sf::Vector2f GetChunkWorldPos(const Chunk &chunk);
	void RenderTile(sf::Vector2i tilePos, int row, int col,
					sf::Vector2f worldPos);
	void RenderChunk(sf::RenderWindow &window, const Chunk &chunk);
	void RenderChunkMap(sf::RenderWindow &window, ChunkMap &cMap);
	void DisplayChunk(const Chunk &chunk);
};
