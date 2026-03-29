#include <SFML/Graphics.hpp>

class Clock {
    private:
        sf::Clock clock;
        sf::Time deltaTimer;
        sf::Text dispFPS;
        sf::Font dispFPSFont;

    public:
        float deltaTime;
        float fps=60.f;
        bool display=false;

    public:
        void Init() {
            if (display) {
                dispFPSFont.loadFromFile("fonts/Arial.ttf");
                dispFPS.setFont(dispFPSFont);
                dispFPS.setScale(sf::Vector2f(0.5f, 0.5f));
            }
        }
        void cycleStart() {
            deltaTimer = clock.restart();
            deltaTime = deltaTimer.asMilliseconds();
        }

        void setFrameRate(sf::RenderWindow& window, int fps) {
            window.setFramerateLimit(fps);
        }

        void displayFPS(sf::RenderWindow& window) {
            if (display) {
                // Static cast to int to avoid trailing 0's with float->string conversion
                dispFPS.setString("FPS: " + std::to_string(int(round(1000/deltaTime))));
                window.draw(dispFPS);
            }
        }
};
