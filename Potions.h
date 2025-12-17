#pragma once
#include "Item.h"
#include "IConsumable.h"
#include "IConsumable.h"

class HealthPotion : public Item, public IConsumable
{
	string GetName() const override { return "Зелье восстановления"; }
	Type GetType() const override{ return Type::Consumable; }

	Gdiplus::Image* GetIcon() const override {
		return new Gdiplus::Image(L"Assets/Items/Potion/Heal/heal-potion.png");
	}

	bool IsStackable() const override { return true; }
	int GetMaxStack() const override { return 16; }

	void Consume(Entity& character) override
	{
		character.ModifyHealth(true, 50);
	}

	bool CanConsume(const Entity& character) const override {
		return character.GetHealth() < 100;
	}

};