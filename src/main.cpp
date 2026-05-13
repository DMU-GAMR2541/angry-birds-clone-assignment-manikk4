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
#include "Catapult.h"
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
    Catapult catapult(world, 150.0f, 520.0f, 10.0f, 60.0f, "../assets/Ang_Birds/catapult.png");

    std::vector<std::shared_ptr<Wall>> NonInteractableWall;
    std::vector<std::shared_ptr<Pig>> pigs;
    std::list<std::shared_ptr<Bird>> birds;
    std::vector<std::string> birdTexture = { "../assets/Ang_Birds/blue.png", "../assets/Ang_Birds/yellow.png", "../assets/Ang_Birds/black.png" };
    std::vector<std::string> pigTexture = { "../assets/Ang_Birds/small.png", "../assets/Ang_Birds/big.png", "../assets/Ang_Birds/pot.png", "../assets/Ang_Birds/crown.png" };

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

    for (int i = 0; i < 4; i++) {
        PigType pigType;
        if (i == 0) { pigType = PigType::Small; }
        else if (i == 1) { pigType = PigType::Big; }
        else if (i == 2) { pigType = PigType::Pot; }
        else { pigType = PigType::Crown; }
        auto pig = std::make_shared<Pig>(world, (550.0f - (i * 100.0f)), 560.0f, 15.0f, pigTexture[i], pigType);
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

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!birds.empty()) {
                        birds.front()->setDragging(true);
                        birds.front()->getBody()->SetGravityScale(0);
                        birds.front()->getBody()->SetLinearVelocity(b2Vec2(0, 0));
                        birds.front()->getBody()->SetAngularVelocity(0);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!birds.empty() && birds.front()->getDragging()) {
                        birds.front()->launch(catapult.getShotPos());
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    if (!birds.empty()) {
                        auto& currentBird = birds.front();
                        std::vector<std::shared_ptr<Bird>> newBirds;

                        if (currentBird->getType() == BirdType::Blue) {
                            newBirds = currentBird->blueBirdAbility(world);
                        }

                        if (currentBird->getType() == BirdType::Yellow && !currentBird->hasUsedAbility()) {
                            currentBird->yellowBirdAbilty();
                        }

                        if (currentBird->getType() == BirdType::Black && !currentBird->hasUsedAbility()) {
                            auto Bomb = currentBird->blackBirdAbility(world, 1.5f);
                            world.DestroyBody(birds.front()->getBody());
                            for (auto& bird : Bomb) {
                                birds.push_back(bird);
                            }
                        }

                        if (!newBirds.empty()) {
                            auto insertPos = std::next(birds.begin());
                            for (auto& nb : newBirds) {
                                birds.insert(insertPos, nb);
                            }
                        }
                    }
                }

                if (event.key.code == sf::Keyboard::B) {
                    if (!birds.empty()) {
                        world.DestroyBody(birds.front()->getBody());
                        birds.pop_front();
                    }
                }
            }
        }

        if (!birds.empty() && birds.front()->getDragging()) {

            sf::Vector2i mousePxl = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld(mousePxl.x, mousePxl.y);
            sf::Vector2f dragVector(mouseWorld - catapult.getShotPos());

            if (dragVector.x > 0) { dragVector.x = 0; }
            if (dragVector.y < 0) { dragVector.y = 0; }

            float length = std::sqrt(dragVector.x * dragVector.x + dragVector.y * dragVector.y);
            float maxDrag = 100.0f;

            if (length > maxDrag) {
                dragVector /= length;
                dragVector *= maxDrag;
            }

            sf::Vector2f finalPos = catapult.getShotPos() + dragVector;
            birds.front()->getBody()->SetTransform(b2Vec2(finalPos.x / SCALE, finalPos.y / SCALE), 0);
        }

        // physics
        world.Step(1.0f / 60.0f, 8, 3);

        std::set<uintptr_t> s_p = c.getPointer();
        for (auto pigIt = pigs.begin(); pigIt != pigs.end(); ) {

            uintptr_t currentPigID = (*pigIt)->getBody()->GetUserData().pointer;

            if (s_p.find(currentPigID) != s_p.end()) {
                std::cout << currentPigID << " Destroyed" << std::endl;
                world.DestroyBody((*pigIt)->getBody());
                pigIt = pigs.erase(pigIt);
            }
            else {
                ++pigIt;
            }
        }

        // update
        catapult.Update();
        plank.Update();
        ground.Update();

        for (auto pigItr = pigs.begin(); pigItr != pigs.end(); ++pigItr) {
            (*pigItr)->Update();
        }

        for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
            (*birdsItr)->Update();
        }

        for (auto wallItr = NonInteractableWall.begin(); wallItr != NonInteractableWall.end(); ++wallItr) {
            (*wallItr)->Update();
        }

        // render
        window.clear(sf::Color(135, 206, 235));

        for (auto it = birds.begin(); it != birds.end(); ) {

            if ((*it)->Delete()) {

                world.DestroyBody((*it)->getBody());

                it = birds.erase(it);
            }
            else {
                ++it;
            }
        }

        plank.render(window);
        ground.render(window);
        catapult.render(window);

        for (auto pigItr = pigs.begin(); pigItr != pigs.end(); ++pigItr) {
            (*pigItr)->render(window);
        }

        for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
            (*birdsItr)->render(window);
        }

        for (auto wallItr = NonInteractableWall.begin(); wallItr != NonInteractableWall.end(); ++wallItr) {
            (*wallItr)->render(window);
        }

        window.display();
    }
}