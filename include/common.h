#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

static constexpr int tileSpacing = 32;
static constexpr float tileWidth = 32;
static constexpr float tileHeight = 16;
static constexpr int chunkSize = 16;

sf::Vector2f GridToWorld(float row, float col);
sf::Vector2i WorldToGrid(sf::Vector2f world);
sf::Vector2i WorldToChunk(sf::Vector2f world);
