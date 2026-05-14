#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "DynamicObject.h"

enum class PigType {
	Crown,
	Pot,
	Big,
	Small
};

 
class Pig : public DynamicObject {
private:

	float xPos = 100.0f;
	float yPos = 500.0f;
	float radius = 15.0f;
	PigType pigType;
	int pigHealth;

public:
	Pig() = default;
	Pig(b2World& world, float xPos, float yPos, float radius, std::string sprite1, PigType pigType) : DynamicObject(world, b2Vec2(xPos, yPos), sprite1) {

		this->xPos = xPos;
		this->yPos = yPos;
		this->radius = radius;
		this->pigType = pigType;
		// dynamic ball
		b2_circleShape.m_radius = radius / SCALE;

		switch (pigType) {
		case PigType::Small:
			b2_ballFixture.density = 1.0f;
			b2_ballFixture.restitution = 0.5f;
			sp_sprites.setScale(0.4f, 0.4f);
			pigHealth = 10;
			break;
		case PigType::Big:
			b2_ballFixture.density = 1.0f;
			b2_ballFixture.restitution = 0.5f;
			sp_sprites.setScale(0.09f, 0.09f);
			pigHealth = 15;
			break;
		case PigType::Pot:
			b2_ballFixture.density = 1.0f;
			b2_ballFixture.restitution = 0.5f;
			sp_sprites.setScale(0.095f, 0.095f);
			pigHealth = 20;
			break;
		case PigType::Crown:
			b2_ballFixture.density = 1.0f;
			b2_ballFixture.restitution = 0.5f;
			sp_sprites.setScale(0.095f, 0.095f);
			pigHealth = 25;
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
};
