#pragma once

#include <SFML/Graphics.hpp>
#include "chunk.h"
#include "common.h"

class Enemy {
  public:
    static constexpr float spawnIntervalSeconds = 10.0f;
    static constexpr int spawnRadiusTiles = 50;
    static constexpr int minSpawnDistanceTiles = 8;
    static constexpr int maxSpawnAttempts = 48;

    Enemy();
    explicit Enemy(const sf::Vector2i &gridPosition);

    void Init();
    void SetGridPosition(const sf::Vector2i &pos);
    sf::Vector2i GetGridPosition() const;
    sf::FloatRect GetGlobalBounds() const;
    void Update(float deltaTimeMs, const sf::Vector2i &playerGrid);
    void Draw(sf::RenderWindow &window) const;

    void TakeDamage(int amount);
    bool IsDead() const;
    int GetHP() const;

    static bool TrySpawnNearPlayer(const sf::Vector2i &playerGrid, ChunkMap &cMap, Enemy &outEnemy);
    static bool TrySpawnAtGrid(const sf::Vector2i &grid, ChunkMap &cMap, Enemy &outEnemy);

  private:
    int hp = 100;
    float hitCooldownSeconds = 0.3f;
    float hitTimer = 0.0f;
    static sf::Texture texture;
    static bool textureLoaded;
    sf::Sprite sprite;
    sf::Vector2i gridPosition = {0, 0};

    void UpdateSpritePosition();
    static bool IsWaterTile(const sf::Vector2i &grid, ChunkMap &cMap);
};
