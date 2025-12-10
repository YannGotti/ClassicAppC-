#pragma once
#include "Entity.h"

class IWeapon
{
public:
    virtual	~IWeapon() = default;
    virtual int GetDamage() const = 0;
    virtual float GetAttackSpeed() const = 0;

    virtual void LogicAttack(Entity* owner) = 0;
};
