#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "DynamicObject.h"
#pragma once

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

public:
	Bird() = default;
	Bird(b2World& world, float xPos, float yPos, float radius, std::string sprite1, BirdType birdType) : DynamicObject(world, b2Vec2(xPos, yPos), sprite1) {

		this->xPos = xPos;
		this->yPos = yPos;
		this->radius = radius;
		this->birdType = birdType;

		b2_circleShape.m_radius = radius / SCALE;

		switch (birdType) {
		case BirdType::Blue:
			b2_ballFixture.density = 1.0f;
			b2_ballFixture.restitution = 0.5f; // ball is bouncy
			sp_sprites.setScale(0.035f, 0.035f);
			break;
		case BirdType::Yellow:
			b2_ballFixture.density = 1.0f;
			b2_ballFixture.restitution = 0.5f; // ball is bouncy
			sp_sprites.setScale(0.045f, 0.045f);
			break;
		case BirdType::Black:
			b2_ballFixture.density = 1.0f;
			b2_ballFixture.restitution = 0.5f; // ball is bouncy
			sp_sprites.setScale(0.045f, 0.045f);
			break;
		}

		b2_body->CreateFixture(&b2_ballFixture);

	}

	void render(sf::RenderWindow& window) override {
		window.draw(sp_sprites);

	}
	void Update() {

		// sync sprite to physics body
		sp_sprites.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
		sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI));

	}
	void launch() {

		// position of ball is reset and can be fired again
		b2_body->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
		b2_body->SetLinearVelocity(b2Vec2(0, 0));
		b2_body->SetAngularVelocity(0);

		// impulse applied (x and y) the y axis is negative in box2d because gravity is positive
		b2_body->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_body->GetWorldCenter(), true);
		std::cout << "shooting" << std::endl;

	}
};