#pragma once
#include "Entity.h"

class IConsumable
{
public:
	~IConsumable() = default;

	virtual void Consume(Entity& entity) = 0;
	virtual bool CanConsume(const Entity& entity) const = 0;
};
