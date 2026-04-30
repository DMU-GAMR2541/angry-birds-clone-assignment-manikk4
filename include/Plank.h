#pragma once
#include "DynamicObject.h"

class Plank : public DynamicObject {
private:
	float xPos = 550.0f;
	float yPos = 450.0f;
	float halfX = 10.0f;
	float halfY = 60.0f;


public:
	Plank() = default;

	Plank(b2World& world, float xPos, float yPos, float halfX, float halfY) : DynamicObject(world, b2Vec2(xPos, yPos), "") {
		this->xPos = xPos;
		this->yPos = yPos;

		// dynamic body type to create a wal that can have velocity
		b2_bodydef.position.Set(xPos / SCALE, yPos / SCALE);
		b2_bodydef.type = b2_dynamicBody;

		b2_body = world.CreateBody(&b2_bodydef);

		b2_plankBox.SetAsBox(halfX / SCALE, halfY / SCALE);

		b2_plankFixture.shape = &b2_plankBox;
		b2_plankFixture.density = 1.5f;   // light wood
		b2_plankFixture.friction = 0.3f;

		b2_body->CreateFixture(&b2_plankFixture);

		rectangle.setSize(sf::Vector2(20.0f, 120.0f));
		rectangle.setOrigin(halfX, halfY);
		rectangle.setFillColor(sf::Color(139, 69, 19));
	}

	void render(sf::RenderWindow& window) override {
		window.draw(rectangle);

	}

	void Update()
	{
		// dynamic wall

		rectangle.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
		rectangle.setRotation(b2_body->GetAngle() * (180.0f / PI));
	}

};