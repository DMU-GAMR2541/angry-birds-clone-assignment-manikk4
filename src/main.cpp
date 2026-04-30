#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "DynamicObject.h"
#include "Bird.h"
#include "Plank.h"
#include "Non-Interactable.h"
#include <iostream>

int main() {


    // --- 1. WINDOW SETUP ---

    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    //Box2D works in meters. SFML works in pixels.

    const float SCALE = 30.0f;

    //Can set a definition for PI.d

    const float PI = 3.1415927;

    //setup world.

    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);



    // Inherited classes, setting parameter values.

    Bird bird(world, 100.0f, 500.0f, 15.0f, "../assets/Ang_Birds/pp1.png");
    Plank plank(world, 500.0f, 450.0f, 10.0f, 60.0f);
    Wall wall(world, 750.0f, 500.0f, 10.0f, 80.0f);
    Ground ground(world, 400.0f, 590.0f, 400.0f, 10.0f);


    // --- 7. MAIN LOOP ---

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)

                window.close();

            // INPUT HANDLING: Press SPACE to launch

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Space) {

                    bird.launch();

                }

            }

        }

        // Update Physics

        world.Step(1.0f / 60.0f, 8, 3);

        //All of the visuals needs to be synced with the physics.

        // derived class update positions.

        bird.Update();
        plank.Update();
        ground.Update();
        wall.Update();

        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.

        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        //window.draw(sf_groundVisual);

        //window.draw(sf_wallVisual);

        plank.render(window);
        bird.render(window);
        ground.render(window);
        wall.render(window);
        window.display();

    }

    return 0;

}
