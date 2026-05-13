#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "DynamicObject.h"

enum class BirdType {
    Blue,
    Yellow,
    Black,
    Bomb
};

class Bird : public DynamicObject {
private:
    float xPos = 100.0f;
    float yPos = 500.0f;
    float radius = 15.0f;
    float power = 5.0f;
    BirdType birdType;
    bool abilityUsed = false;
    bool Dragging = false;
    sf::Clock bombTime;
    float BombDeletionTimer = -1.0f;
    sf::Vector2f startPos;

public:
    Bird() = default;
    Bird(b2World& world, float xPos, float yPos, float radius, std::string sprite1, BirdType birdType)
        : DynamicObject(world, b2Vec2(xPos, yPos), sprite1)
    {
        this->xPos = xPos;
        this->yPos = yPos;
        this->radius = radius;
        this->birdType = birdType;
        this->power = power;
        b2_circleShape.m_radius = radius / SCALE;

        switch (birdType) {
        case BirdType::Blue:
            b2_ballFixture.density = 1.0f;
            b2_ballFixture.restitution = 0.5f;
            sp_sprites.setScale(0.030f, 0.030f);
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

    std::vector<std::shared_ptr<Bird>> blueBirdAbility(b2World& world) { std::vector<std::shared_ptr<Bird>> newBirds;

        if (abilityUsed) return newBirds;
        abilityUsed = true;

        b2Vec2 vel = b2_body->GetLinearVelocity();

        float angles[2] = { -2.5f, 2.5f };

            for (float offset : angles) {
                float bx = b2_body->GetPosition().x * SCALE;
                float by = b2_body->GetPosition().y * SCALE;

                auto clone = std::make_shared<Bird>(world, bx, by, radius, "../assets/Ang_Birds/blue.png", BirdType::Blue);
                clone->getBody()->GetUserData().pointer = 100;
                clone->abilityUsed = true; 

                clone->getBody()->SetLinearVelocity(b2Vec2(vel.x, vel.y + offset));
                newBirds.push_back(clone);
            }
        
        return newBirds;
    }

    void setDragging(bool dragging) {
        Dragging = dragging;
    }

    bool getDragging() { return Dragging; }

    sf::Vector2f getStartPos() { return startPos; }

    void yellowBirdAbilty() { 
        b2Vec2 vel = b2_body->GetLinearVelocity();
        b2_body->SetLinearVelocity(b2Vec2(vel.x * 2.0f, vel.y));
        std::cout << "velocity is " << b2_body->GetAngularVelocity();
    }

    void destroyTimer(float seconds) {
        BombDeletionTimer = seconds;
        bombTime.restart();
    }

    bool Delete() {
        if (BombDeletionTimer < 0) return false;
        return bombTime.getElapsedTime().asSeconds() >= BombDeletionTimer;
    }

    std::vector<std::shared_ptr<Bird>> blackBirdAbility(b2World& world, float destroyTimer) {
        std::vector<std::shared_ptr<Bird>> Bomb;

        if (abilityUsed) return Bomb;
        abilityUsed = true;

        float bx = b2_body->GetPosition().x * SCALE;
        float by = b2_body->GetPosition().y * SCALE;
        auto bombEffect = std::make_shared<Bird>(world, bx, by, 50, "../assets/Ang_Birds/bombeffect.png", BirdType::Bomb);
        bombEffect->destroyTimer(destroyTimer);
        bombEffect->getBody()->GetUserData().pointer = 100;
        abilityUsed = true;
        Bomb.push_back(bombEffect);

        return Bomb;
    }

    void render(sf::RenderWindow& window) override {
        window.draw(sp_sprites);
    }

    void Update() {
        sp_sprites.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
        sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI));
    }

    void dragging() {

        Dragging = true;
        b2_body->SetGravityScale(0);

        b2_body->SetLinearVelocity(b2Vec2(0, 0));
        b2_body->SetAngularVelocity(0);
    }

    void launch(sf::Vector2f shotPos) {

        Dragging = false;
        b2_body->SetGravityScale(1.0f);
        sf::Vector2f targetPos{ b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE };
        sf::Vector2 launch = (targetPos - shotPos) / SCALE;
        b2Vec2 impulseMag(-launch.x * power, -launch.y * power);
        b2_body->ApplyLinearImpulseToCenter(impulseMag, true);
        abilityUsed = false; // reset ability on relaunch
        std::cout << "shooting" << std::endl;
    }
};
