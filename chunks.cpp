
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>

static constexpr int chunkSize = 16;
static constexpr int tileSpacing = 16;
std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(1, 4);

class Chunk {
    public: 
        std::vector<std::vector<int>> vectorMapGround;
        std::vector<std::vector<int>> vectorMapObject;
        sf::Vector2i chunkCoordinates;

        Chunk() {
            vectorMapGround.resize(chunkSize, std::vector<int>(chunkSize, 0));
            vectorMapObject.resize(chunkSize, std::vector<int>(chunkSize, 0));
        }
        void setCoordinates(int posX, int posY) {
            chunkCoordinates = sf::Vector2i(posX, posY);
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

class ChunkRenderer {
    private:
        sf::Texture texture;
        sf::Sprite currentTile;
        std::unordered_map<int, sf::Vector2i> TileLookup;
        std::string fileloc="tilemap/era1.png";

    public:
        void Init() {
            createLookup();
            LoadTexture();
        }

        void LoadTexture() {if (texture.loadFromFile(fileloc)) {currentTile.setTexture(texture);}}
        sf::Vector2i ConvertToPos(int itemID) {
            auto it = TileLookup.find(itemID);
            if (it == TileLookup.end()) return {0, 0};
            return it->second;
        }

        void createLookup() {
            TileLookup[0] = {4, 6};
            TileLookup[1] = {0, 0};
            TileLookup[2] = {1, 0};
            TileLookup[3] = {2, 0};
            TileLookup[4] = {3, 0};
        }

        sf::Vector2f GetChunkCenterOffset(const sf::RenderWindow& window, const Chunk& chunk) {
            float chunkPixelSize = chunkSize * tileSpacing;
            sf::Vector2u winSize = window.getSize();
            sf::Vector2f baseOffset = {(winSize.x - chunkPixelSize) / 2.f, (winSize.y - chunkPixelSize) / 2.f};
            sf::Vector2f finalOffset = {
                baseOffset.x + chunk.chunkCoordinates.x * chunkPixelSize,
                baseOffset.y + chunk.chunkCoordinates.y * chunkPixelSize
            }; return finalOffset;
        }

        void RenderTile(sf::Vector2i tilePos, int row, int col, int offsetX, int offsetY) {
            int posX = tilePos.x*tileSpacing;;
            int posY = tilePos.y*tileSpacing;
            currentTile.setTextureRect({posX, posY, tileSpacing, tileSpacing});
            currentTile.setPosition(offsetX + col*tileSpacing, offsetY + row*tileSpacing);
        }

        void RenderChunk(sf::RenderWindow& window, const Chunk& chunk) {
            sf::Vector2f offset = GetChunkCenterOffset(window, chunk);
            for (int i=0; i<chunkSize; i++) {
                for (int j=0; j<chunkSize; j++) {
                    int ID = chunk.vectorMapGround[i][j];
                    sf::Vector2i tilePos = ConvertToPos(ID);
                    RenderTile(tilePos, i, j, offset.x, offset.y);
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

};


int main() {
    sf::RenderWindow window;
    sf::Event event;

    Chunk chunk;
    GenerateChunk(chunk);
    Chunk chunk2;
    GenerateChunk(chunk2);
    chunk2.setCoordinates(0, 1);

    window.create(sf::VideoMode(1280, 720), "The Immortal Snail");

    ChunkRenderer Renderer;
    Renderer.Init();

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(30, 30, 30));
        Renderer.RenderChunk(window, chunk);
        Renderer.RenderChunk(window, chunk2);
        window.display();
    }


}