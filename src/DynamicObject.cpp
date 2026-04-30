#include "DynamicObject.h"
#include <iostream>

DynamicObject::~DynamicObject() {

}

DynamicObject::DynamicObject(b2World& b2_world, b2Vec2 b2_posIn, std::string str_sprite) {

	b2_bodydef.position.Set(b2_posIn.x / SCALE, b2_posIn.y / SCALE);

	b2_bodydef.type = b2_dynamicBody;

	b2_body = b2_world.CreateBody(&b2_bodydef);

	b2_ballFixture.shape = &b2_circleShape;

	// sprite navigation and texture loading

		if (!sf_Texture.loadFromFile(str_sprite))
			return;

	sp_sprites.setTexture(sf_Texture);
	sp_sprites.setOrigin(sf_Texture.getSize().x / 2.0f, sf_Texture.getSize().y / 2.0f);
	sp_sprites.setPosition(b2_bodydef.position.x * SCALE, b2_bodydef.position.y * SCALE);
}

void DynamicObject::Update(float gravityScale, b2Vec2 b2_impulse, bool b_make)
{
}

void DynamicObject::render(sf::RenderWindow& window)
{
}
