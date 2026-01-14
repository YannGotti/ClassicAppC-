#pragma once
#include "Item.h"
#include "Character.h"
#include "IWeapon.h"

class IronSword : public Item, public IWeapon
{
public:
	IronSword()
	{
		if (!icon)
		{
			icon = std::make_unique<Gdiplus::Image>(
				L"Assets/UI/Inventory/Items/Weapon/Swords/iron-sword.png"
			);
		}
	}
	string GetName() const override { return "Железный меч"; }
	Type GetType() const override { return Type::Weapon; }

	Gdiplus::Image* GetIcon() const override {
		return icon.get();
	}

	int GetDamage() const override { return 15; }
	float GetAttackSpeed() const override { return 1.2f; }

	virtual void LogicAttack(Entity* owner) override { return; };
private:
	std::unique_ptr<Gdiplus::Image> icon;
};


class BronzeSword : public Item, public IWeapon
{
public:
	BronzeSword()
	{
		if (!icon)
		{
			icon = std::make_unique<Gdiplus::Image>(
				L"Assets/UI/Inventory/Items/Weapon/Swords/bronze-sword.png"
			);
		}
	}

	string GetName() const override { return "Бронзовый меч"; }
	Type GetType() const override { return Type::Weapon; }

	Gdiplus::Image* GetIcon() const override {
		return icon.get();
	}

	int GetDamage() const override { return 20; }
	float GetAttackSpeed() const override { return 1.0f; }

	virtual void LogicAttack(Entity* owner) override { return; };

private:
	std::unique_ptr<Gdiplus::Image> icon;
};