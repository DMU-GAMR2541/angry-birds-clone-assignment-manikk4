#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "DynamicObject.h"

enum class BirdType {
    Blue,
    Yellow,
    Black
};

class Bird : public DynamicObject {
private:
    float xPos = 100.0f;
    float yPos = 500.0f;
    float radius = 15.0f;
    BirdType birdType;
    bool abilityUsed = false;  // each bird can only use ability once

public:
    Bird() = default;
    Bird(b2World& world, float xPos, float yPos, float radius, std::string sprite1, BirdType birdType)
        : DynamicObject(world, b2Vec2(xPos, yPos), sprite1)
    {
        this->xPos = xPos;
        this->yPos = yPos;
        this->radius = radius;
        this->birdType = birdType;
        b2_circleShape.m_radius = radius / SCALE;

        switch (birdType) {
        case BirdType::Blue:
            b2_ballFixture.density = 1.0f;
            b2_ballFixture.restitution = 0.5f;
            sp_sprites.setScale(0.035f, 0.035f);
            break;
        case BirdType::Yellow:
            b2_ballFixture.density = 1.0f;
            b2_ballFixture.restitution = 0.5f;
            sp_sprites.setScale(0.045f, 0.045f);
            break;
        case BirdType::Black:
            b2_ballFixture.density = 1.0f;
            b2_ballFixture.restitution = 0.5f;
            sp_sprites.setScale(0.045f, 0.045f);
            break;
        }
        b2_body->CreateFixture(&b2_ballFixture);
    }

    BirdType getType() const { return birdType; }
    bool hasUsedAbility() const { return abilityUsed; }

    std::vector<std::shared_ptr<Bird>> useAbility(b2World& world) {
        std::vector<std::shared_ptr<Bird>> newBirds;

        if (abilityUsed) return newBirds;
        abilityUsed = true;

        b2Vec2 vel = b2_body->GetLinearVelocity();

        if (vel.x == 0 && vel.y == 0) return newBirds;

        if (birdType == BirdType::Blue) {
            
            float angles[2] = { -2.5f, 2.5f }; 

            for (float offset : angles) {
                float bx = b2_body->GetPosition().x * SCALE;
                float by = b2_body->GetPosition().y * SCALE;

                auto clone = std::make_shared<Bird>(world, bx, by, radius,
                    "../assets/Ang_Birds/blue.png", BirdType::Blue);
                clone->getBody()->GetUserData().pointer = 100;
                clone->abilityUsed = true; 

                
                clone->getBody()->SetLinearVelocity(b2Vec2(vel.x, vel.y + offset));
                newBirds.push_back(clone);
            }

        } else if (birdType == BirdType::Yellow) {
            b2_body->SetLinearVelocity(b2Vec2(vel.x * 3.0f, vel.y * 3.0f));
        }

        return newBirds;
    }

    void render(sf::RenderWindow& window) override {
        window.draw(sp_sprites);
    }

    void Update() {
        sp_sprites.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
        sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI));
    }

    void launch() {
        b2_body->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
        b2_body->SetLinearVelocity(b2Vec2(0, 0));
        b2_body->SetAngularVelocity(0);
        b2_body->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_body->GetWorldCenter(), true);
        abilityUsed = false; // reset ability on relaunch
        std::cout << "shooting" << std::endl;
    }
};
