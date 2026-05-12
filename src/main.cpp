#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "DynamicObject.h"
#include "Bird.h"
#include "Plank.h"
#include "Pig.h"
#include "Non-Interactable.h"
#include <iostream>
#include <vector>
#include "ContactListener.h"
#include <set>
#include <list>

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    const float SCALE = 30.0f;
    const float PI = 3.1415927f;

    b2Vec2 b2_gravity(0.0f, 9.8f);
    b2World world(b2_gravity);

    // Create the listener instance.
    ContactListener c;

    // Register it with the world
    world.SetContactListener(&c);

    // static objects
    Plank plank(world, 500.0f, 560.0f, 10.0f, 60.0f, "../assets/Ang_Birds/plank.png");
    Ground ground(world, 400.0f, 590.0f, 400.0f, 10.0f);

    std::vector<std::shared_ptr<Wall>> NonInteractableWall;
    std::vector<std::shared_ptr<Pig>> pigs;
    std::list<std::shared_ptr<Bird>> birds;
    std::vector<std::string> birdTexture = { "../assets/Ang_Birds/blue.png", "../assets/Ang_Birds/yellow.png", "../assets/Ang_Birds/black.png" };

    for (int i = 0; i < 3; i++) {
        BirdType birdtype;
        if (i == 0) { birdtype = BirdType::Blue; }
        else if (i == 1) { birdtype = BirdType::Yellow; }
        else { birdtype = BirdType::Black; }
        birds.emplace_back(std::make_shared<Bird>(world, (100.0f - (i * 40.0f)), 560.0f, 15.0f, birdTexture[i], birdtype));
    }

    for (auto& bird : birds) {
        bird->getBody()->GetUserData().pointer = 100;
    }

    for (int i = 0; i < 3; i++) {
        auto pig = std::make_shared<Pig>(world, (550.0f - (i * 150.0f)), 560.0f, 15.0f, "../assets/Ang_Birds/pp1.png");
        pig->getBody()->GetUserData().pointer = 3 + i;
        pigs.push_back(pig);
    }
    
    NonInteractableWall.push_back(std::make_shared<Wall>(world, 750.0f, 500.0f, 10.0f, 80.0f));
  
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
                    world.DestroyBody(birds.front()->getBody());
                    birds.pop_front();
                }
            }
        }

        // physics 
        world.Step(1.0f / 60.0f, 8, 3);

        std::set<uintptr_t> s_p = c.getPointer(); //Set of pointers to the pig ID's
        for (auto pigIt = pigs.begin(); pigIt != pigs.end(); ) {

            uintptr_t currentPigID = (*pigIt)->getBody()->GetUserData().pointer;

            // Check if this pig's ID exists in the hit list
            if (s_p.find(currentPigID) != s_p.end()) { //Check through all of the container for specific Id's

                std::cout << currentPigID << " Destroyed" << std::endl;

                // Remove from Box2D world first
                world.DestroyBody((*pigIt)->getBody()); //Remove the pig body from the world.

                // Update the iterator by catching the return value of erase()
                pigIt = pigs.erase(pigIt); //Erase the pig from the set.


            }
            else {
                // Only increment if we didn't erase anything
                ++pigIt;
            }
        }

        // update
        plank.Update();
        ground.Update();
        
        for (auto pigItr = pigs.begin(); pigItr != pigs.end(); ++pigItr) {
            auto& pig = *pigItr;
            pig->Update();
        }

        for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
            auto& bird = *birdsItr;
            bird->Update();
        }

        for (auto wallItr = NonInteractableWall.begin(); wallItr != NonInteractableWall.end(); ++wallItr) {
            auto& wall = *wallItr;
            wall->Update();
        }

        // render
        window.clear(sf::Color(135, 206, 235));
        plank.render(window);
        ground.render(window);

        for (auto pigItr = pigs.begin(); pigItr != pigs.end(); ++pigItr) {
            auto& pig = *pigItr;
            pig->render(window);
        }

        for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
            auto& bird = *birdsItr;
            bird->render(window);
        }
        for (auto wallItr = NonInteractableWall.begin(); wallItr != NonInteractableWall.end(); ++wallItr) {
            auto& wall = *wallItr;
            wall->render(window);
        }

        window.display();
    }

    return 0;
}