#pragma once
#include "Entity.h"
#include <cmath>
#include <windows.h>       
#include <objidl.h>       
#include <gdiplus.h>       
#pragma comment (lib,"Gdiplus.lib")


class IFollow
{
public:
	Entity* target = nullptr;
	float detectionRadius = 100.0f;
	float attackRange = 14.0f;
	float moveSpeed = 0.0f;
	void Update(Entity* self, float deltaTime);
	void DebugIFollow(Graphics& g);
};
