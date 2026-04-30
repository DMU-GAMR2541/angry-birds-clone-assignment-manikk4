#include <string>
#include "StaticObject.h"

StaticObject::~StaticObject()
{
}

StaticObject::StaticObject(b2World& b2_worldPos, b2Vec2 b2_groundPos, b2Vec2 b2_wallPos, b2Vec2 b2_groundScale, b2Vec2 wallScale, std::string str_sprite)
{
	// convert screen positions to box2d world units
	b2_groundBodyDef.position.Set(b2_groundPos.x / SCALE, b2_groundPos.y / SCALE);
	b2_wallDef.position.Set(b2_wallPos.x / SCALE, b2_wallPos.y / SCALE);

	b2_groundBody = b2_worldPos.CreateBody(&b2_groundBodyDef);
	b2_wallBody = b2_worldPos.CreateBody(&b2_wallDef);

	// scaled to world units
	b2_groundBox.SetAsBox(b2_groundScale.x / SCALE, b2_groundScale.y / SCALE);
	b2_wallBox.SetAsBox(wallScale.x / SCALE, wallScale.y / SCALE);

	// density 0 = static body
	b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);
	b2_wallBody->CreateFixture(&b2_wallBox, 0.0f);
}