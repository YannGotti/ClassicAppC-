#pragma once
#include <vector>
#include "Entity.h"
#include "Character.h"

using namespace std;

class CollisionManager
{
public:
	void Update();

	void Draw(Gdiplus::Graphics& g);

	static Entity* CollideEntity(Entity* owner);
};

