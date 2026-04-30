#pragma once
#include <string>
#include <SFML/Graphics.hpp>

// GameObject is pure abstract base class.
// Every other object will inherit from this.

class GameObject {
public:
	GameObject() = default; // default constructor
	~GameObject() = default;// default constructor

protected:
	virtual void render(sf::RenderWindow& window) = 0; // virtual render window
};