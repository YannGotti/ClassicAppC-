#pragma once
#include <string>
#include <memory>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
#include "Entity.h"
using namespace std;
class Item
{
public:
	enum class Type
	{
		Weapon,
		Armor,
		Consumable,
		Resource
	};
	virtual ~Item() = default;
	virtual string GetName() const = 0;
	virtual Type GetType() const = 0;

	virtual bool IsStackable() const { return false; }
	virtual int GetMaxStack() const { return 1; }
	virtual Gdiplus::Bitmap* GetIcon() const { return nullptr; }

	virtual void Use(Entity& character) { /**/ }
};

