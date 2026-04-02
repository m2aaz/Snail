
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>

static constexpr int chunkSize = 16;
static constexpr int tileSpacing = 32;
std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(1, 4);

class Chunk {
    public: 
        std::vector<std::vector<int>> vectorMapGround;
        std::vector<std::vector<int>> vectorMapObject;
        std::vector<std::vector<int>> vectorMapElevation;
        sf::Vector2i chunkCoordinates;

        Chunk() {
            vectorMapGround.resize(chunkSize, std::vector<int>(chunkSize, 0));
            vectorMapObject.resize(chunkSize, std::vector<int>(chunkSize, 0));
            vectorMapElevation.resize(chunkSize, std::vector<int>(chunkSize, 0));
        }
        void setCoordinates(int posX, int posY) {
            chunkCoordinates = sf::Vector2i(posX, posY);
        }
};

struct ChunkMap {
    std::unordered_map<std::string, Chunk> chunkMap;

    std::string setChunkID(sf::Vector2i coord) {
        return std::to_string(coord.x) + "," + std::to_string(coord.y);
    }

    void Add(Chunk& chunk) {
        sf::Vector2i coordinates = chunk.chunkCoordinates;
        std::string chunkID = setChunkID(coordinates);
        chunkMap[chunkID] = chunk;
    }

    void Remove(sf::Vector2i coordinates) {
        std::string chunkID = setChunkID(coordinates);
        chunkMap.erase(chunkID);
    }

    Chunk& Get(sf::Vector2i coordinates) {
        std::string chunkID = setChunkID(coordinates);
        return chunkMap[chunkID];
    }   

    bool Exists(sf::Vector2i coordinates) {
        std::string chunkID = setChunkID(coordinates);
        auto iterator = chunkMap.find(chunkID);
        if (iterator == chunkMap.end()) {
            return false;
        } else {return true;}
    }
};

// 0: Grass, 1: Anything else.
void GenerateChunk(Chunk& chunk) {
    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            chunk.vectorMapElevation[i][j] = 1;
            chunk.vectorMapGround[i][j] = 0;
            chunk.vectorMapObject[i][j] = dist(rng);
        }
    }
}

// Spawns A Chunk @ Location (Based on GRID COORDINATES.)
void spawnChunk(int x, int y, ChunkMap& cMap) {
    Chunk chunk;
    chunk.setCoordinates(x, y);
    GenerateChunk(chunk);
    cMap.Add(chunk);
}

sf::Vector2i WorldToChunk(sf::Vector2f mouseWorld) {

    // Size of each tile (vertically & horizontally)
    float isoWidth  = chunkSize * (tileSpacing / 2.0f);
    float isoHeight = chunkSize * (tileSpacing / 2.0f);

    // Converting coordinates into grid coordinates by dividing (ex. )
    float a = mouseWorld.x / isoWidth;
    float b = mouseWorld.y / isoHeight;

    int chunkX = (int)std::floor((a + b) / 2.0f);
    int chunkY = (int)std::floor((b - a) / 2.0f);

    return sf::Vector2i(chunkX, chunkY);
}

void placeChunk(sf::RenderWindow& window, ChunkMap& cMap) {
    sf::Vector2i mouseScreen = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mouseScreen);

    sf::Vector2i chunkCoord = WorldToChunk(mouseWorld);
    int chunkX = chunkCoord.x; int chunkY = chunkCoord.y;
    if (!cMap.Exists(chunkCoord)) {
        spawnChunk(chunkX, chunkY, cMap);
    } else {std::cout << "Chunk Already Exists." << std::endl;}
}

void deleteChunk(sf::RenderWindow& window, ChunkMap& cMap) {
    sf::Vector2i mouseScreen = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mouseScreen);

    sf::Vector2i chunkCoord = WorldToChunk(mouseWorld);
    int chunkX = chunkCoord.x; int chunkY = chunkCoord.y;
    if (cMap.Exists(chunkCoord)) {
        cMap.Remove(chunkCoord);
    } else {std::cout << "Chunk Doesn't Exists." << std::endl;}
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
            TileLookup[0] = {7, 3};
            TileLookup[1] = {0, 0};
            TileLookup[2] = {1, 0};
            TileLookup[3] = {2, 0};
            TileLookup[4] = {3, 0};
        }

        sf::Vector2f GetChunkWorldPos(const Chunk& chunk) {
            float isoWidth  = chunkSize * (tileSpacing / 2.0f);
            float isoHeight = chunkSize * (tileSpacing / 2.0f);
            return {
                (chunk.chunkCoordinates.x - chunk.chunkCoordinates.y) * isoWidth,
                (chunk.chunkCoordinates.x + chunk.chunkCoordinates.y) * isoHeight
            };
        }

        void RenderTile(sf::Vector2i tilePos, int row, int col, sf::Vector2f worldPos) {
            int posX = tilePos.x*tileSpacing;
            int posY = tilePos.y*tileSpacing;
            currentTile.setTextureRect({posX, posY, tileSpacing, tileSpacing});
            currentTile.setPosition(
                worldPos.x + (col - row) * (tileSpacing / 2.0f),
                worldPos.y + (col + row) * (tileSpacing / 2.0f)
            );
        }

        void RenderChunk(sf::RenderWindow& window, const Chunk& chunk) {
            sf::Vector2f worldPos = GetChunkWorldPos(chunk);
            for (int i=0; i<chunkSize; i++) {
                for (int j=0; j<chunkSize; j++) {
                    if (chunk.vectorMapElevation[i][j] > 0) {
                        int ID = chunk.vectorMapGround[i][j];
                        sf::Vector2i tilePos = ConvertToPos(ID);
                        RenderTile(tilePos, i, j, worldPos);
                        window.draw(currentTile);
                    }
                }
            }
        }

        void RenderChunkMap(sf::RenderWindow& window, ChunkMap& cMap) {
            for (auto& [key, chunk] : cMap.chunkMap) {
                RenderChunk(window, chunk);
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

// Testing
// int main() {
//     sf::RenderWindow window;
//     sf::Event event;

//     Chunk chunk;
//     GenerateChunk(chunk);
//     Chunk chunk2;
//     GenerateChunk(chunk2);
//     chunk2.setCoordinates(0, 1);

//     window.create(sf::VideoMode(1280, 720), "The Immortal Snail");

//     ChunkRenderer Renderer;
//     Renderer.Init();

//     while (window.isOpen()) {
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
//         }
//         window.clear(sf::Color(30, 30, 30));
//         Renderer.RenderChunk(window, chunk);
//         Renderer.RenderChunk(window, chunk2);
//         window.display();
//     }


// }