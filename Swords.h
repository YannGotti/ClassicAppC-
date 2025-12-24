#pragma once
#include "Item.h"
#include "Character.h"
#include "IWeapon.h"

class IronSword : public Item, public IWeapon
{
public:
	string GetName() const override { return "Железный меч"; }
	Type GetType() const override { return Type::Weapon; }

	Gdiplus::Image* GetIcon() const override {
		return new Gdiplus::Image(L"Assets/UI/Inventory/Items/Weapon/Swords/iron-sword.png");
	}

	int GetDamage() const override { return 15; }
	float GetAttackSpeed() const override { return 1.2f; }

	virtual void LogicAttack(Entity* owner) override { return; };

};


class BronzeSword : public Item, public IWeapon
{
public:
	string GetName() const override { return "Бронзовый меч"; }
	Type GetType() const override { return Type::Weapon; }

	Gdiplus::Bitmap* GetIcon() const {
		return new Gdiplus::Bitmap(L"Assets/UI/Inventory/Items/Weapon/Swords/bronze-sword.png");
	}

	int GetDamage() const override { return 20; }
	float GetAttackSpeed() const override { return 1.0f; }

	virtual void LogicAttack(Entity* owner) override { return; };

};