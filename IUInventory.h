#pragma once
#include "IRenderable.h"
#include "LayerController.h"
#include "EventBus.h"
#include "EventsTypes.h"

class Inventory;

class IHotBarInventory : public IRenderable
{
public:

	explicit IHotBarInventory(Inventory* inventory);


	bool IsDynamic() const override { return true; }
	void DrawScreen(Gdiplus::Graphics& g, const RECT& clientRect) override;
	void Update(float deltaTime) override;
	void HandlerInputCellInventory(const PlayerSwitchInventoryEvent& e);

	int GetCurrentSlot() { return currentSlot; }

private:
	Inventory* _parentInv;
	Image* hotbar;
	Image* frame;
	Image* cell;
	int currentSlot;
};



