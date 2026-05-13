#pragma once
#include "DynamicObject.h"

class Catapult : public DynamicObject {
private:
	float xPos = 150.0f;
	float yPos = 500.0f;
	float HalfX = 7.0f;
	float HalfY = 60.0f;
	sf::Vector2f shotPos;

public:
	Catapult() = default;
	Catapult(b2World& world, float xPos, float yPos, float HalfX, float HalfY, std::string Texture) : DynamicObject(world, b2Vec2(xPos, yPos), Texture) {

		this->xPos = xPos;
		this->yPos = yPos;
		this->HalfX = HalfX;
		this->HalfY = HalfX;
		this->shotPos = sf::Vector2f(xPos - 5.0f, yPos - 50.0f);

		b2_bodydef.type = b2_staticBody;
		b2_bodydef.position.Set(xPos / SCALE, yPos / SCALE);
		b2_body = world.CreateBody(&b2_bodydef);
		b2_plankBox.SetAsBox(HalfX / SCALE, HalfY / SCALE);


	}
	sf::Vector2f getShotPos() { return shotPos; }

	void render(sf::RenderWindow& window) {
		window.draw(sp_sprites);
	}

	void Update() {
		sp_sprites.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
		sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI));
	}

};