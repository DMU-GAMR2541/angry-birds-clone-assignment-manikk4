#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>

// StaticObject is an abstract class
// inherits from GameObject
// for objects that never move under physics:
// the ground, the background scenery, the static wall

class StaticObject : public GameObject {
public:

	const float SCALE = 30.0f;
	const float PI = 3.1415927;

	StaticObject() = default;
	~StaticObject();

	StaticObject(b2World& b2_worldPos, b2Vec2 b2_groundPos, b2Vec2 wallPos, b2Vec2 b2_groundScale, b2Vec2 wallScale, std::string str_sprite);

	// static objects

	b2BodyDef b2_groundBodyDef;
	b2PolygonShape b2_groundBox;

	b2BodyDef b2_wallDef;
	b2PolygonShape b2_wallBox;

	sf::RectangleShape& sf_groundVisual() { return Rectangle; }
	sf::RectangleShape& sf_WallVisual() { return Rectangle2; }

	void render(sf::RenderWindow& window) = 0;

protected:

	b2Body* b2_groundBody;
	b2Body* b2_wallBody;
	sf::RectangleShape Rectangle;
	sf::RectangleShape Rectangle2;
};