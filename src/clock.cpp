#include "clock.h"

#include <cmath>
#include <iostream>
#include <string>

void Clock::Init() {
  if (dispFPSFont.loadFromFile("fonts/Arial.ttf")) {
    dispFPS.setFont(dispFPSFont);
    dispFPS.setScale(sf::Vector2f(0.5f, 0.5f));
  } else {
    std::cout << "Warning: Could not load fonts/Arial.ttf" << std::endl;
  }
}

void Clock::cycleStart() {
  deltaTimer = clock.restart();
  deltaTime = deltaTimer.asMilliseconds();
}

void Clock::setFrameRate(sf::RenderWindow &window, int fps) {
  window.setFramerateLimit(fps);
}

void Clock::displayFPS(sf::RenderWindow &window) {
  if (display) {
    // Static cast to int to avoid trailing 0's with float->string conversion
    dispFPS.setString("FPS: " + std::to_string(int(round(1000 / deltaTime))));
    window.draw(dispFPS);
  }
}
