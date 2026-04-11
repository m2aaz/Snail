#pragma once
#include "camera.h"
#include "chunk_behaviour.h"
#include "chunk_renderer.h"
#include "clock.h"
#include "player.cpp"

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

  private:
	void Update();
	void Draw();

  public:
	void Init();
	void Run();
	void Exit();
	void Save();
};
