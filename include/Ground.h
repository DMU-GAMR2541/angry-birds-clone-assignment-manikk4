#pragma once
#include <string>
#include "StaticObject.h"

class Ground : public StaticObject {
private:
	float GroundXPos = 400.0f;
	float GroundYPos = 590.0f;
	float GroundWidth = 400.0f;
	float GroundHeight = 10.0f;
public:
	Ground() = default;
	Ground(b2World& world, float GroundXPos, float GroundYPos, float GroundWidth, float GroundHeight) :
		StaticObject(world, b2Vec2(GroundXPos, GroundYPos), b2Vec2(), b2Vec2(GroundWidth, GroundHeight), b2Vec2(), "") {

		this->GroundXPos = GroundXPos;
		this->GroundYPos = GroundYPos;
		this->GroundWidth = GroundWidth;
		this->GroundHeight = GroundHeight;

		Rectangle.setSize(sf::Vector2(800.0f, 20.0f));
		Rectangle.setOrigin(GroundWidth, GroundHeight); // centre the origin on the box
		Rectangle.setFillColor(sf::Color(34, 139, 34));

	}
	void Start() {

		// sync sprite position to physics body
		Rectangle.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);

	}

	void render(sf::RenderWindow& window) override {
		window.draw(Rectangle);

	}
};

class Wall : public StaticObject {
private:

	float WallXPos = 750.0f;
	float WallYPos = 500.0f;
	float WallWidth = 10.0f;
	float WallHeight = 80.0f;

public:
	Wall() = default;
	Wall(b2World& world, float WallXPos, float WallYPos, float WallWidth, float WallHeight) :
		StaticObject(world, b2Vec2(), b2Vec2(WallXPos, WallYPos), b2Vec2(), b2Vec2(WallWidth, WallHeight), "") {

		this->WallXPos = WallXPos;
		this->WallYPos = WallYPos;
		this->WallWidth = WallWidth;
		this->WallHeight = WallHeight;

		Rectangle2.setSize(sf::Vector2(20.0f, 160.0f));
		Rectangle2.setOrigin(WallWidth, WallHeight); // centre the origin on the box
		Rectangle2.setFillColor(sf::Color::Red);

	}
	void Start() {

		// sync sprite position to physics body
		Rectangle2.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

	}
	void render(sf::RenderWindow& window) override {
		window.draw(Rectangle2);

	}
};