#include "game_operator.h"
#include <iostream>

void GameOperator::Update() { cam.Update(window); }

void GameOperator::Draw() {
  window.clear(sf::Color(30, 30, 30));
  Renderer.RenderChunkMap(window, cMap);

  delta.displayFPS(window);
  window.display();
}

void GameOperator::Init() {
  // Initialise Window.
  window.create(sf::VideoMode(gameWidth, gameHeight), "The Immortal Snail");

  // Initialise Chunk Renderer & Generate Chunk.
  Renderer.Init();
  spawnChunk(sf::Vector2i(0, 0), cMap);

  // Set Window Frames.
  delta.Init();
  delta.setFrameRate(window, 60);

  // Initialise Camera
  cam.Init(window);
}

void GameOperator::Run() {
  while (window.isOpen()) {
    delta.cycleStart();
    Exit();
    Update();
    Draw();
  }
}

void GameOperator::Exit() {
  // Handle Event (Closing)
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Resized) {
      cam.ResizeView(window, gameWidth, gameHeight);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        behave.placeChunk(window, cMap);
      }
      if (event.mouseButton.button == sf::Mouse::Right) {
        behave.deleteChunk(window, cMap);
      }
    }
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::F) {
        delta.toggleFPS();
      }
    }
    if (event.type == sf::Event::MouseWheelScrolled) {
      cam.Zoom(window, event.mouseWheelScroll.delta);
    }
    if (event.type == sf::Event::Closed) {
      window.close();
      std::cout << "Window Closed." << std::endl;
    }
  }
}

void GameOperator::Save() {}
