#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>

class Map {
    private:
        sf::Texture texture;
        sf::Sprite currentTile;

        // Combined 2D Vector Map Size.
        int mapHeight = 45; // Map Rows
        int mapWidth = 80; // Map Columns

        // Tilesheet Dimensions
        int tileHeight = 14;
        int tileWidth = 25;
        int tileSpacing = 16;

        // File Location & Seeding Gen
        std::mt19937 rng;
        std::string fileloc = "tilemap/era1.png";

        // Mapping & Lookup
        std::uniform_int_distribution<int> dist;
        std::vector<std::vector<int>> vectorMap;
        std::unordered_map<int, sf::Vector2i> tileLookup;

        // Tile Information
        int tileID;
        sf::Vector2i tilePos;


    private:
        // Random Number Generator (apparently widely used for game development)
        int randomTileGenerator() {
            int tileNum = dist(rng);
            if (tileNum < 75) return 1; // Grass
            if (tileNum < 85) return 2; // Bush
            if (tileNum < 95) return 3; // Fern
            return 4; // Rock
        }

    public:
        Map(): rng(std::random_device{}()), dist(0, 99) {
            Load();
        } 

        void Load() {
            if (texture.loadFromFile(fileloc)) {
                currentTile.setTexture(texture);
                std::cout << "Loaded TileMap Successfully." << std::endl;
            } else {std::cout << "Failed To Load TileMap." << std::endl;}


            // Create Label Lookup Table & Resize Vector To Map 
            createLookupTable();
            vectorMap.resize(mapHeight, std::vector<int>(mapWidth, 0));

            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    vectorMap[i][j] = randomTileGenerator();
                }
            }

        }

        void createLookupTable() {
            // Managing Tiles
            tileLookup[1] = {7, 5}; // Grass
            tileLookup[2] = {12, 12}; // Bush
            tileLookup[3] = {12, 16}; // Fern
            tileLookup[4] = {14, 1}; // Rock
        }

        void displayVectorMap() {
            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    std::cout << vectorMap[i][j] << ", ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        void TileToMap(sf::Vector2i tilePos, int i, int j) {
            int pX = tilePos.x*tileSpacing;
            int pY = tilePos.y*tileSpacing;
            currentTile.setTextureRect(sf::IntRect(pX, pY, tileSpacing,tileSpacing));

            // Convert Map Position To Screen Position
            currentTile.setPosition(
                i * tileSpacing,
                j * tileSpacing
            );
        }

        void Update() {}
        void Draw(sf::RenderWindow& window) {
            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    tileID = vectorMap[i][j];
                    tilePos = tileLookup[tileID];
                    TileToMap(tilePos, i, j);
                    window.draw(currentTile);
                }
            }
        }
};


// Map Testing

// To test the basic implementation (vectorMap), follow the following conditions:
// In CMakeLists.txt, clear all respective source files except map.cpp
// Ensure MapTest() is uncommented, and a main() function exists which runs MapTest()
// The Output should be a 2d vector represented by randomly generated numbers (1...x), depending on the map population
// Uncomment the code below:

// void MapTest() {
//     Map testMap;
//     testMap.displayVectorMap();
// }

// int main() {
//     MapTest();
// }