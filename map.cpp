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
        std::vector<std::vector<int>> vectorMapGround;
        std::vector<std::vector<int>> vectorMapObject;
        std::unordered_map<int, sf::Vector2i> tileLookup;

        // Tile Information
        int tileID;
        sf::Vector2i tilePos;


    private:
        // Random Number Generator (apparently widely used for game development)
        int randomTileGenerator() {
            int tileNum = dist(rng);
            if (tileNum < 75) return 0; // Empty
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
            vectorMapGround.resize(mapHeight, std::vector<int>(mapWidth, 0));
            vectorMapObject.resize(mapHeight, std::vector<int>(mapWidth, 0));

            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    vectorMapGround[i][j] = 1; // Always Generate Grass (Bottom)
                    vectorMapObject[i][j] = randomTileGenerator();
                }
            }

        }

        void createLookupTable() {
            // Managing Tiles
            tileLookup[1] = {4, 6}; // Grass
            tileLookup[2] = {11, 11}; // Bush
            tileLookup[3] = {15, 11}; // Fern
            tileLookup[4] = {0, 13}; // Rock
        }

        void displayVectorMap() {
            std::cout << "Displaying Layer 1 (GROUND)" << std::endl;
            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    std::cout << vectorMapGround[i][j] << ", ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;

            std::cout << "Displaying Layer 2 (OBJECTS)" << std::endl;
            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    std::cout << vectorMapObject[i][j] << ", ";
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
                j * tileSpacing,
                i * tileSpacing
            );
        }

        void Update() {}
        void Draw(sf::RenderWindow& window) {
            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    // Drawing Layer 1
                    tileID = vectorMapGround[i][j];
                    tilePos = tileLookup[tileID];
                    TileToMap(tilePos, i, j);
                    window.draw(currentTile);

                    // Drawing Layer 2
                    tileID = vectorMapObject[i][j];
                    if (tileID == 0) {continue;}
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
// The Output should be 2d vector srepresented by randomly generated numbers (1...x), depending on the map population / layers.
// Uncomment the code below:

// void MapTest() {
//     Map testMap;
//     testMap.displayVectorMap();
// }

// int main() {
//     MapTest();
// }