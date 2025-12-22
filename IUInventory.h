#pragma once
#include "IRenderable.h"
#include "LayerController.h"
#include "EventBus.h"
#include "EventsTypes.h"

class Inventory;

class IHotBarInvetory : public IRenderable
{
public:
	IHotBarInvetory(Inventory* inventory)
	{
		LayerController::Add(this, 40);
		_parentInv = inventory;
		EventBus::Subscribe<PlayerSwitchInventoryEvent>([](const PlayerSwitchInventoryEvent& e) 
		{
			
		});
	}
	bool IsDynamic() const override { return true; }
	void DrawScreen(Gdiplus::Graphics& g, const RECT& clientRect) override;
	void Update(float deltaTime) override;
	void HandlerInputCellInventory( PlayerSwitchInventoryEvent& e);
private:
	Inventory* _parentInv;
};




