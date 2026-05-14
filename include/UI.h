#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Pig.h"

class UI : public DynamicObject {
private:
    sf::Font font;
    sf::Text pigsRemainingText;

    // Loading screen
    sf::RectangleShape loadingBackground;
    sf::Text loadingText;
    sf::Clock loadingClock;
    float loadingDuration = 3.0f;
    bool loading = true;

public:
    UI() = default;

    UI(std::string& gameFont) : DynamicObject (gameFont){

        // Pigs remaining text (top left)
        pigsRemainingText.setFont(font);
        pigsRemainingText.setCharacterSize(24);
        pigsRemainingText.setFillColor(sf::Color::White);
        pigsRemainingText.setOutlineColor(sf::Color::Black);
        pigsRemainingText.setOutlineThickness(2.0f);
        pigsRemainingText.setPosition(10.0f, 10.0f);
        pigsRemainingText.setString("Pigs: 0");

        // Loading screen background
        loadingBackground.setSize(sf::Vector2f(800.0f, 600.0f));
        loadingBackground.setPosition(0.0f, 0.0f);

    }

    bool isLoading() const { return loading; }

    void Update(const std::vector<std::shared_ptr<Pig>>& pigs) {
        // Update pigs remaining
        pigsRemainingText.setString("Pigs: " + std::to_string(pigs.size()));

        // Check if loading screen should end
        if (loading && loadingClock.getElapsedTime().asSeconds() >= loadingDuration) {
            loading = false;
        }
    }

    void render(sf::RenderWindow& window) {
        if (loading) {
            window.draw(loadingBackground);
            window.draw(loadingText);
        }
        else {
            window.draw(pigsRemainingText);
        }
    }
};