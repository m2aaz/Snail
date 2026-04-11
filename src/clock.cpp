#include "clock.h"

#include <cmath>
#include <iostream>
#include <string>

void Clock::Init() {
	if (dispFPSFont.loadFromFile("fonts/Arial.ttf")) {
		dispFPS.setFont(dispFPSFont);
		dispFPS.setScale(sf::Vector2f(0.5f, 0.5f));
		dispFPS.setPosition(0, 0);
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
		// Static cast to int to avoid trailing 0's with float->string
		dispFPS.setString("FPS: " +std::to_string(int(round(1000 / deltaTime))));

		// Conserve old view to render FPS object at top left without camera distorting it's position
		sf::View oldView = window.getView();
		window.setView(window.getDefaultView());
		window.draw(dispFPS);
		window.setView(oldView);
	}
}
