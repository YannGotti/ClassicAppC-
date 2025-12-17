#pragma once

#include "IRenderable.h"
#include "LayerController.h"

class Inventory;

class IHotBarInvetory : public IRenderable
{
public:
	IHotBarInvetory(Inventory* inventory)
	{
		LayerController::Add(this, 40);
		_parentInv = inventory;
	}
	bool IsDynamic() const override { return true; }
	void DrawScreen(Gdiplus::Graphics& g, const RECT& clientRect) override;
	void Update(float deltaTime) override;
private:
	Inventory* _parentInv;
};


