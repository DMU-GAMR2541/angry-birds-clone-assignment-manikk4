#pragma once
#include "StaticObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>

class Ground : public StaticObject {

public:
	Ground() = default;
	~Ground() = default;

	Ground(b2World& b2_worldPos, float posX, float posY, float halfW, float halfH) : StaticObject(b2_worldPos, b2Vec2(posX, posY), b2Vec2(), b2Vec2(halfW, halfH),b2Vec2(), "" ) {

		Rectangle.setSize(sf::Vector2f(halfW * 2.0f, halfH * 2.0f));
		Rectangle.setOrigin(halfW, halfH);
		Rectangle.setFillColor(sf::Color(34, 139, 34)); // Forest Green

	};

	void Update(){

		Rectangle.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
	}

	void render(sf::RenderWindow& window) override {
		window.draw(Rectangle);
	}

};

class Wall : public StaticObject {

public:

	Wall() = default;
	~Wall() = default;

	Wall(b2World& b2_worldPos, float posX, float posY, float halfW, float halfH) : StaticObject(b2_worldPos, b2Vec2(), b2Vec2(posX, posY), b2Vec2(), b2Vec2(halfW, halfH), "" ){

		Rectangle2.setSize(sf::Vector2f(halfW * 2.0, halfH * 2.0f));
		Rectangle2.setOrigin(halfW, halfH);
		Rectangle2.setFillColor(sf::Color::Red);

	}

	void Update() {

		Rectangle2.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);
	}

	void render(sf::RenderWindow& window) override {
		window.draw(Rectangle2);
	}

};