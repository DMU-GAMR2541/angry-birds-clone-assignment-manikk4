#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "DynamicObject.h"

class Pig : public DynamicObject {
private:

	float xPos = 100.0f;
	float yPos = 500.0f;
	float radius = 15.0f;

public:
	Pig() = default;
	Pig(b2World& world, float xPos, float yPos, float radius, std::string sprite1) : DynamicObject(world, b2Vec2(xPos, yPos), sprite1) {

		this->xPos = xPos;
		this->yPos = yPos;
		this->radius = radius;

		// dynamic ball
		b2_circleShape.m_radius = radius / SCALE;

		b2_ballFixture.shape = &b2_circleShape;
		b2_ballFixture.density = 1.0f;
		b2_ballFixture.restitution = 0.5f; // ball is bouncy
		b2_ballFixture.friction = 0.3f;
		b2_body->CreateFixture(&b2_ballFixture);

		sp_sprites.setScale(0.1f, 0.1f);

	}

	void render(sf::RenderWindow& window) override {
		window.draw(sp_sprites);

	}
	void Update() {

		// sync sprite to physics body
		sp_sprites.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
		sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI));

	}
};
