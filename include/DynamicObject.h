#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>

// DynamicObject is an abstract class
// inherits from GameObject
// for objects that are governed by Box2D physics:
// the bird (ball), pig, slingshot

class DynamicObject : public GameObject {

public:

	DynamicObject() = default;
	~DynamicObject();

	DynamicObject(b2World& b2_worldPos, b2Vec2 b2_Pos, std::string str_sprite);

	const float SCALE = 30.0f;
	const float PI = 3.1415927;

	// dynamic objects

	b2Vec2* b2_pos;
	b2BodyDef b2_bodydef;

	b2PolygonShape b2_plankBox;
	b2CircleShape b2_circleShape;

	b2FixtureDef b2_ballFixture;
	b2FixtureDef b2_plankFixture;
	
	sf::Sprite sp_sprites;
	sf::Texture sf_Texture;

	bool isCollided = false;
	bool isDamaged = false;

	void render(sf::RenderWindow& window);

	void Update(float gravity, b2Vec2 b2_impule, bool b_make);

	sf::CircleShape& sf_ballVisual() { return circle; }

	sf::RectangleShape& sf_plankVisual() { return rectangle; }

protected:

	b2FixtureDef b2_fixtureDef;
	b2Body* b2_body;
	sf::CircleShape circle;
	sf::RectangleShape rectangle;
};