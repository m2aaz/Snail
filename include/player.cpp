
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "chunk.h"
#include "common.h"

class Player {
    private:
        sf::Texture texture;
        sf::Sprite playerSprite;
        std::string fileloc = "sprites/player.png";
        sf::Vector2i gridPosition = {0, 0};

        bool IsWaterTile(const sf::Vector2i& grid, ChunkMap& cMap) const {
            int chunkX = std::floor((float)grid.x / chunkSize);
            int chunkY = std::floor((float)grid.y / chunkSize);
            sf::Vector2i chunkCoord(chunkX, chunkY);
            if (!cMap.Exists(chunkCoord))
                return true;

            Chunk& chunk = cMap.Get(chunkCoord);
            // Proper modulo for negative numbers
            int localX = ((grid.x % chunkSize) + chunkSize) % chunkSize;
            int localY = ((grid.y % chunkSize) + chunkSize) % chunkSize;

            int tileID = chunk.vectorMapGround[localY][localX];
            // Water tiles: 0 (main), 3 (variant 1), 4 (variant 2)
            return tileID == 0 || tileID == 3 || tileID == 4;
        }

        void UpdateSpritePosition() {
            sf::Vector2f world = GridToWorld((float)gridPosition.y, (float)gridPosition.x);
            playerSprite.setPosition(world);
        }

    public:
        void Init() {
            if (texture.loadFromFile(fileloc)) {
                playerSprite.setTexture(texture);
                playerSprite.setTextureRect({0, 0, tileSpacing * 2, tileSpacing * 2});
                playerSprite.setOrigin(tileSpacing, tileSpacing * 2);
                UpdateSpritePosition();
            }
        }

        void SetGridPosition(const sf::Vector2i& pos) {
            gridPosition = pos;
            UpdateSpritePosition();
        }

        void HandleInput(const sf::Event& event, ChunkMap& cMap) {
            if (event.type != sf::Event::KeyPressed)
                return;

            sf::Vector2i delta{0, 0};
            if (event.key.code == sf::Keyboard::Up) {
                // Up in isometric: row--, col--
                delta.x = -1;
                delta.y = -1;
            }
            else if (event.key.code == sf::Keyboard::Down) {
                // Down in isometric: row++, col++
                delta.x = 1;
                delta.y = 1;
            }
            else if (event.key.code == sf::Keyboard::Left) {
                // Left in isometric: row++, col--
                delta.x = -1;
                delta.y = 1;
            }
            else if (event.key.code == sf::Keyboard::Right) {
                // Right in isometric: row--, col++
                delta.x = 1;
                delta.y = -1;
            }
            else
                return;

            sf::Vector2i targetPosition = gridPosition + delta;
            if (!IsWaterTile(targetPosition, cMap)) {
                gridPosition = targetPosition;
                UpdateSpritePosition();
            }
        }

        sf::Vector2i GetGridPosition() const {
            return gridPosition;
        }

        void Update() {
            // Grid-based movement is handled on key press events.
        }

        void Draw(sf::RenderWindow& window) {
            window.draw(playerSprite);
        }
};