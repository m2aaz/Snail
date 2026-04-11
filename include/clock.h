#pragma once
#include <SFML/Graphics.hpp>

class Clock {
private:
  sf::Clock clock;
  sf::Time deltaTimer;
  sf::Text dispFPS;
  sf::Font dispFPSFont;

public:
  float deltaTime;
  float fps = 60.f;
  bool display = false;

public:
  void Init();
  void cycleStart();
  void setFrameRate(sf::RenderWindow &window, int fps);
  void displayFPS(sf::RenderWindow &window);
  void toggleFPS() {display = !display;}
};
