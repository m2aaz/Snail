
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>

static constexpr int chunkSize = 16;
static constexpr int tileSpacing = 16;
std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(1, 4);
std::unordered_map<int, sf::Vector2i> TileLookup;

void createLookup() {
    TileLookup[0] = {4, 6};
    TileLookup[1] = {0, 0};
    TileLookup[2] = {1, 0};
    TileLookup[3] = {2, 0};
    TileLookup[4] = {3, 0};
}

sf::Vector2i ConvertToPos(int itemID) {return TileLookup.at(itemID);}

class Chunk {
    public: 
        std::vector<std::vector<int>> vectorMapGround;
        std::vector<std::vector<int>> vectorMapObject;
        sf::Vector2i chunkCoordinates;

        Chunk() {
            vectorMapGround.resize(chunkSize, std::vector<int>(chunkSize, 0));
            vectorMapObject.resize(chunkSize, std::vector<int>(chunkSize, 0));
        }
};

// 0: Grass, 1: Anything else.
void GenerateChunk(Chunk& chunk) {
    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            chunk.vectorMapGround[i][j] = 0;
            chunk.vectorMapObject[i][j] = dist(rng);
        }
    }
}



void TileToChunk(sf::Sprite& currentTile, sf::Vector2i tilePos, int row, int col, int offsetX, int offsetY) {
    int posX = tilePos.x*tileSpacing;;
    int posY = tilePos.y*tileSpacing;
    currentTile.setTextureRect({posX, posY, tileSpacing, tileSpacing});
    currentTile.setPosition(offsetX + col*tileSpacing, offsetY + row*tileSpacing);
}

void RenderChunk(sf::RenderWindow& window, sf::Sprite& currentTile, const Chunk& chunk) {

    // Centering
    int chunkPixelSize = chunkSize * tileSpacing;
    sf::Vector2u winSize = window.getSize();
    float offsetX = (winSize.x - chunkPixelSize) / 2.f;
    float offsetY = (winSize.y - chunkPixelSize) / 2.f;
    // ---------

    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            int ID = chunk.vectorMapGround[i][j];
            sf::Vector2i tilePos = ConvertToPos(ID);
            TileToChunk(currentTile, tilePos, i, j, offsetX, offsetY);
            window.draw(currentTile);
        }
    }
}

void DisplayChunk(const Chunk& chunk) {
    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            std::cout << chunk.vectorMapGround[i][j] << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl; 

    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            std::cout << chunk.vectorMapObject[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}

void LoadTexture(sf::Sprite& currentTile, sf::Texture& texture) {
    std::string fileloc="tilemap/era1.png";

    if (texture.loadFromFile(fileloc)) {
        currentTile.setTexture(texture);
    }
}

int main() {
    sf::RenderWindow window;
    sf::Event event;
    sf::Texture texture;
    sf::Sprite currentTile;
    LoadTexture(currentTile, texture);

    Chunk chunk;
    GenerateChunk(chunk);
    window.create(sf::VideoMode(1280, 720), "The Immortal Snail");

    createLookup();

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(30, 30, 30));
        RenderChunk(window, currentTile, chunk);
        window.display();
    }


}