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

	Plank(b2World& world, float xPos, float yPos, float halfX, float halfY, std::string spr_plank) : DynamicObject(world, b2Vec2(xPos, yPos), spr_plank) {
		this->xPos = xPos;
		this->yPos = yPos;

		b2_bodydef.position.Set(xPos / SCALE, yPos / SCALE);
		b2_bodydef.type = b2_dynamicBody;

		b2_body = world.CreateBody(&b2_bodydef);

		b2_plankBox.SetAsBox(halfX / SCALE, halfY / SCALE);

		b2_plankFixture.shape = &b2_plankBox;
		b2_plankFixture.density = 1.5f;
		b2_plankFixture.friction = 0.3f;

		b2_body->CreateFixture(&b2_plankFixture);

		sp_sprites.setScale(0.73f, 1.0f);


	}

	void render(sf::RenderWindow& window) override {
		window.draw(sp_sprites);

	}

	void Update()
	{
		// Dynamic wall.
		sp_sprites.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
		sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI) + 90.0f);
	}

};