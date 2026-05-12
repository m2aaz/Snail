#pragma once
#include <vector>
#include "camera.h"
#include "chunk_behaviour.h"
#include "chunk_renderer.h"
#include "clock.h"
#include "enemy.h"
#include "player.h"

class GameOperator {
  private:
	const int gameWidth = 1280;
	const int gameHeight = 720;

  public:
	sf::RenderWindow window;
	Clock delta;
	sf::Event event;
	ChunkRenderer Renderer;
	Camera cam;
	ChunkMap cMap;
	ChunkBehaviour behave;
	Player player;
	std::vector<Enemy> enemies;
	float enemySpawnTimer = 0.0f;

  private:
	void Update();
	void Draw();

  public:
	void Init();
	void Run();
	void Exit();
	void Save();
};
