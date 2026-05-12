
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "chunk.h"
#include "common.h"
#include "inventory.h"

class Player {
    private:
        sf::Texture texture;
        sf::Sprite playerSprite;
        std::string fileloc = "sprites/player.png";
        sf::Vector2i gridPosition = {0, 0};
        Inventory inventory;
        bool IsWaterTile(const sf::Vector2i& grid, ChunkMap& cMap) const;
        void UpdateSpritePosition();

    public:
        void Init();
        void SetGridPosition(const sf::Vector2i& pos);
        void HandleInput(const sf::Event& event, ChunkMap& cMap);
        sf::Vector2i GetGridPosition() const;
        void Update();
        void Draw(sf::RenderWindow& window);
        sf::FloatRect GetGlobalBounds() const;

        // Inventory Methods
        void AddItem(const Item& item);
        bool RemoveItem(const std::string& itemName, int amount=1);
        bool HasItem(const std::string& itemName) const;
        void PrintInventory() const;
        Inventory& GetInventory();
};