#pragma once
#include <string>
#include <memory>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

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
	virtual Gdiplus::Image* GetIcon() const { return nullptr; }
};

