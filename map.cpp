#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>

class Map {
    private:
        sf::Texture texture;
        sf::Sprite TileMap;

        // Combined 2D Vector Map Size.
        int mapHeight = 45; // Map Rows
        int mapWidth = 80; // Map Columns

        std::string fileloc = "tilemap/era1.png";
        int tileHeight = 14;
        int tileWidth = 25;
        int tileSpacing = 16;
        std::mt19937 rng;
        std::uniform_int_distribution<int> dist;
        std::vector<std::vector<int>> vectorMap;

    
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
        Map(): rng(std::random_device{}()), dist(0, 99) {Load();} 

        void Load() {
            if (texture.loadFromFile(fileloc)) {
                TileMap.setTexture(texture);
                std::cout << "Loaded TileMap Successfully." << std::endl;
            } else {std::cout << "Failed To Load TileMap." << std::endl;}


            // Resize Vector To Map
            vectorMap.resize(mapHeight, std::vector<int>(mapWidth, 0));

            for (int i=0; i<mapHeight; i++) {
                for (int j=0; j<mapWidth; j++) {
                    vectorMap[i][j] = randomTileGenerator();
                }
            }
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

        void Update() {}
        void Draw() {}
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