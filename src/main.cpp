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
    // window setup, 800x600 should be fine for now
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    const float SCALE = 30.0f; // box2d works in meters so we need this to convert
    const float PI = 3.1415927f;

    b2Vec2 b2_gravity(0.0f, 9.8f);
    b2World world(b2_gravity);

    ContactListener c;
    world.SetContactListener(&c);

    // ground and catapult don't move so just create them once here
    Ground ground(world, 400.0f, 590.0f, 400.0f, 10.0f);
    Catapult catapult(world, 150.0f, 520.0f, 10.0f, 60.0f, "../assets/Ang_Birds/catapult.png");

    std::vector<std::shared_ptr<Plank>> planks;
    std::vector<std::shared_ptr<Wall>> NonInteractableWall;
    std::vector<std::shared_ptr<Pig>> pigs;
    std::list<std::shared_ptr<Bird>> birds; // using list so we can easily pop from the front
    std::vector<std::string> birdTexture = { "../assets/Ang_Birds/blue.png", "../assets/Ang_Birds/yellow.png", "../assets/Ang_Birds/black.png" };
    std::vector<std::string> pigTexture = { "../assets/Ang_Birds/small.png", "../assets/Ang_Birds/big.png", "../assets/Ang_Birds/pot.png", "../assets/Ang_Birds/crown.png" };

    // two vertical planks acting as pillars
    planks.push_back(std::make_shared<Plank>(world, 480.0f, 545.0f, 10.0f, 50.0f, "../assets/Ang_Birds/plank.png"));
    planks.back()->getBody()->GetUserData().pointer = 10;
    planks.push_back(std::make_shared<Plank>(world, 560.0f, 545.0f, 10.0f, 50.0f, "../assets/Ang_Birds/plank.png"));
    planks.back()->getBody()->GetUserData().pointer = 100;

    float xPig[] = { 520.0f, 520.0f, 420.0f, 630.0f };
    float yPig[] = { 488.0f, 588.0f, 558.0f, 558.0f };

    // queue up the 3 birds, they sit behind the catapult waiting their turn
    for (int i = 0; i < 3; i++) {
        BirdType birdtype;
        if (i == 0) { birdtype = BirdType::Blue; }
        else if (i == 1) { birdtype = BirdType::Yellow; }
        else { birdtype = BirdType::Black; }
        birds.emplace_back(std::make_shared<Bird>(world, (100.0f - (i * 40.0f)), 560.0f, 15.0f, birdTexture[i], birdtype));
    }

    // give all birds the same ID for now, might need to change this later
    for (auto& bird : birds) {
        bird->getBody()->GetUserData().pointer = 100;
    }

    // spawn the 4 pigs, IDs start at 3 so they don't clash with anything else
    for (int i = 0; i < 4; i++) {
        PigType pigType;
        if (i == 0) { pigType = PigType::Small; }
        else if (i == 1) { pigType = PigType::Big; }
        else if (i == 2) { pigType = PigType::Pot; }
        else { pigType = PigType::Crown; }
        auto pig = std::make_shared<Pig>(world, xPig[i], yPig[i], 15.0f, pigTexture[i], pigType);
        pig->getBody()->GetUserData().pointer = 3 + i;
        pigs.push_back(pig);
    }

    // invisible wall on the right side so stuff doesn't fly off screen
    NonInteractableWall.push_back(std::make_shared<Wall>(world, 750.0f, 500.0f, 10.0f, 80.0f));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            // click to grab the bird on the catapult
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!birds.empty()) {
                        birds.front()->setDragging(true);
                        birds.front()->getBody()->SetGravityScale(0); // freeze it while dragging
                        birds.front()->getBody()->SetLinearVelocity(b2Vec2(0, 0));
                        birds.front()->getBody()->SetAngularVelocity(0);
                    }
                }
            }

            // let go to launch
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!birds.empty() && birds.front()->getDragging()) {
                        birds.front()->launch(catapult.getShotPos());
                        birds.front()->destroyTimer(3.0f); // clean it up after 3 seconds
                    }
                }
            }

            // press A to use the current bird's special ability
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    if (!birds.empty()) {
                        auto& currentBird = birds.front();
                        std::vector<std::shared_ptr<Bird>> newBirds;

                        if (currentBird->getType() == BirdType::Blue) {
                            newBirds = currentBird->blueBirdAbility(world); // splits into 3 birds
                        }

                        if (currentBird->getType() == BirdType::Yellow && !currentBird->hasUsedAbility()) {
                            currentBird->yellowBirdAbilty(); // speeds up mid air
                        }

                        if (currentBird->getType() == BirdType::Black && !currentBird->hasUsedAbility()) {
                            // blows up and spawns a bunch of smaller projectiles
                            auto Bomb = currentBird->blackBirdAbility(world, 1.5f);
                            world.DestroyBody(birds.front()->getBody());
                            for (auto& bird : Bomb) {
                                birds.push_back(bird);
                            }
                        }

                        // stick the new blue bird clones right after the current one in the queue
                        if (!newBirds.empty()) {
                            auto insertPos = std::next(birds.begin());
                            for (auto& nb : newBirds) {
                                birds.insert(insertPos, nb);
                            }
                        }
                    }
                }
            }
        }

        // move the bird with the mouse while dragging, but keep it within catapult range
        if (!birds.empty() && birds.front()->getDragging()) {

            sf::Vector2i mousePxl = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld(mousePxl.x, mousePxl.y);
            sf::Vector2f dragVector(mouseWorld - catapult.getShotPos());

            // don't let the player drag right or up, slingshot only pulls back and down
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

        world.Step(1.0f / 60.0f, 8, 3);

        // check what got hit this frame and remove anything that took a fatal collision
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

        for (auto itPlank = planks.begin(); itPlank != planks.end(); ) {
            uintptr_t currentPlankID = (*itPlank)->getBody()->GetUserData().pointer;
            if (s_p.find(currentPlankID) != s_p.end()) {
                std::cout << currentPlankID << " Plank Destroyed" << std::endl;
                world.DestroyBody((*itPlank)->getBody());
                itPlank = planks.erase(itPlank);
            }
            else {
                ++itPlank;
            }
        }

        // remove any birds whose timer ran out
        for (auto birditr = birds.begin(); birditr != birds.end(); ) {
            if ((*birditr)->Delete()) {
                world.DestroyBody((*birditr)->getBody());
                birditr = birds.erase(birditr);
            }
            else {
                ++birditr;
            }
        }

        // update everything
        catapult.Update();
        ground.Update();

        for (auto plankItr = planks.begin(); plankItr != planks.end(); ++plankItr) {
            (*plankItr)->Update();
        }

        for (auto pigItr = pigs.begin(); pigItr != pigs.end(); ++pigItr) {
            (*pigItr)->Update();
        }

        for (auto birdsItr = birds.begin(); birdsItr != birds.end(); ++birdsItr) {
            (*birdsItr)->Update();
        }

        for (auto wallItr = NonInteractableWall.begin(); wallItr != NonInteractableWall.end(); ++wallItr) {
            (*wallItr)->Update();
        }

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

        ground.render(window);
        catapult.render(window);

        for (auto plankItr = planks.begin(); plankItr != planks.end(); ++plankItr) {
            (*plankItr)->render(window);
        }

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