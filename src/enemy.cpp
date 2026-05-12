#include "enemy.h"
#include <iostream>
#include <random>
#include <cmath>

static std::mt19937 &GetEnemyRng() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
}

static int RandomInt(int low, int high) {
    std::uniform_int_distribution<int> dist(low, high);
    return dist(GetEnemyRng());
}

static int ChunkIndexFromGrid(int gridCoord) {
    return static_cast<int>(std::floor(static_cast<float>(gridCoord) / chunkSize));
}

sf::Texture Enemy::texture;
bool Enemy::textureLoaded = false;

Enemy::Enemy() {
    Init();
}

Enemy::Enemy(const sf::Vector2i &gridPosition) : gridPosition(gridPosition) {
    Init();
}

void Enemy::Init() {
    if (!textureLoaded) {
        textureLoaded = texture.loadFromFile("sprites/player.png");
        if (!textureLoaded) {
            std::cout << "Enemy: failed to load sprites/player.png" << std::endl;
        }
    }

    if (textureLoaded) {
        sprite.setTexture(texture);
        sprite.setTextureRect({0, 0, tileSpacing * 2, tileSpacing * 2});
        sprite.setOrigin(tileSpacing, tileSpacing * 2);
    }
    UpdateSpritePosition();
}

void Enemy::SetGridPosition(const sf::Vector2i &pos) {
    gridPosition = pos;
    UpdateSpritePosition();
}

sf::Vector2i Enemy::GetGridPosition() const {
    return gridPosition;
}

void Enemy::Update(float deltaTimeMs, const sf::Vector2i &/*playerGrid*/) {
    hitTimer -= deltaTimeMs / 1000.0f;
    if (hitTimer < 0.0f) {
        hitTimer = 0.0f;
    }
}

void Enemy::TakeDamage(int amount) {
    if (hitTimer > 0.0f) {
        return;
    }

    hp -= amount;
    if (hp < 0) { hp = 0;}
    hitTimer = hitCooldownSeconds;
}

bool Enemy::IsDead() const {
    return hp <= 0;
}

int Enemy::GetHP() const {
    return hp;
}

sf::FloatRect Enemy::GetGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void Enemy::Draw(sf::RenderWindow &window) const {
    if (textureLoaded) {
        window.draw(sprite);
    }
}

void Enemy::UpdateSpritePosition() {
    sf::Vector2f world = GridToWorld(
        static_cast<float>(gridPosition.y),
        static_cast<float>(gridPosition.x)
    );

    sprite.setPosition(world);
}

bool Enemy::IsWaterTile(const sf::Vector2i &grid, ChunkMap &cMap) {
    int chunkX = ChunkIndexFromGrid(grid.x);
    int chunkY = ChunkIndexFromGrid(grid.y);
    sf::Vector2i chunkCoord(chunkX, chunkY);

    if (!cMap.Exists(chunkCoord)) {
        return true;
    }

    Chunk &chunk = cMap.Get(chunkCoord);
    int localX = ((grid.x % chunkSize) + chunkSize) % chunkSize;
    int localY = ((grid.y % chunkSize) + chunkSize) % chunkSize;
    int tileID = chunk.vectorMapGround[localY][localX];
    return (tileID == 0 || tileID == 3 || tileID == 4);
}

bool Enemy::TrySpawnNearPlayer(const sf::Vector2i &playerGrid, ChunkMap &cMap, Enemy &outEnemy) {
    for (int attempt = 0; attempt < maxSpawnAttempts; ++attempt) {
        int dx = RandomInt(-spawnRadiusTiles, spawnRadiusTiles);
        int dy = RandomInt(-spawnRadiusTiles, spawnRadiusTiles);
        int distance = std::max(std::abs(dx), std::abs(dy));

        if (distance < minSpawnDistanceTiles || distance > spawnRadiusTiles) {
            continue;
        }

        sf::Vector2i spawnPos = playerGrid + sf::Vector2i(dx, dy);
        if (spawnPos == playerGrid) {
            continue;
        }

        if (IsWaterTile(spawnPos, cMap)) {
            continue;
        }

        outEnemy = Enemy(spawnPos);
        return true;
    }

    return false;
}

bool Enemy::TrySpawnAtGrid(const sf::Vector2i &grid, ChunkMap &cMap, Enemy &outEnemy) {
    if (IsWaterTile(grid, cMap)) {
        return false;
    }

    outEnemy = Enemy(grid);
    return true;
}
