#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "DynamicObject.h"
#include "Bird.h"
#include "Plank.h"
#include "Pig.h"
#include "Non-Interactable.h"
#include <iostream>
#include <vector>
#include <list>

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    const float SCALE = 30.0f;
    const float PI = 3.1415927f;

    b2Vec2 b2_gravity(0.0f, 9.8f);
    b2World world(b2_gravity);

    // static objects
    Bird bird(world, 100.0f, 500.0f, 15.0f, "../assets/Ang_Birds/pp1.png");
    Plank plank(world, 500.0f, 450.0f, 10.0f, 60.0f, "../assets/Ang_Birds/plank.png");
    Wall wall(world, 750.0f, 500.0f, 10.0f, 80.0f);
    Ground ground(world, 400.0f, 590.0f, 400.0f, 10.0f);

    // spawn mutiple pigs
    //std::vector<float> pigX = { 480.0f, 530.0f, 505.0f, 620.0f, 670.0f };
    //std::vector<float> pigY = { 555.0f, 555.0f, 495.0f, 555.0f, 555.0f };


    std::vector<std::shared_ptr<Pig>> pigs;
    std::list<std::shared_ptr<Bird>> birds;

    for (int i = 0; i < 3; i++) {
        pigs.emplace_back(std::make_shared<Pig>(world, (500.0f + (i * 20.0f)), 350.0f, 15.0f, "../assets/Ang_Birds/red1.png"));
    }

    for (int i = 0; i < 3; i++) {
        birds.emplace_back(std::make_shared<Bird>(world, (100.0f + (i * 20.0f)), 500.0f, 15.0f, "../assets/Ang_Birds/pp1.png"));
    }

    // --- 4. MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                    
                auto& currentBird = birds.front();

                if (event.key.code == sf::Keyboard::Space) {

                    for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
                        auto& bird = *birdsItr;
                        currentBird->launch();
                    }
                }

                if (event.key.code == sf::Keyboard::B) {
                    birds.pop_front();
                }
            }
        }

        // physics 
        world.Step(1.0f / 60.0f, 8, 3);

        // update
         //bird.Update();
        plank.Update();
        ground.Update();
        wall.Update();

        for (auto pigItr = pigs.begin(); pigItr != pigs.end(); ++pigItr) {
            auto& pig = *pigItr;
            pig->Update();
        }

        for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
            auto& bird = *birdsItr;
            bird->Update();
        }

        // render
        window.clear(sf::Color(135, 206, 235));

        plank.render(window);
        //bird.render(window);
        ground.render(window);
        wall.render(window);

        for (auto pigItr = pigs.begin(); pigItr != pigs.end(); ++pigItr) {
            auto& pig = *pigItr;
            pig->render(window);
        }

        for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
            auto& bird = *birdsItr;
            bird->render(window);
        }

        window.display();
    }

    return 0;
}