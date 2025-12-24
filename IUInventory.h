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
		LayerController::Add(this, 30);
		hotbar = new Image(L"Assets/UI/Inventory/inv_hotbar.png");
		_parentInv = inventory;
		Init();
	}

	void Init()
	{
		EventBus::Subscribe<PlayerSwitchInventoryEvent>(
			[this](const PlayerSwitchInventoryEvent& e) {
				this->HandlerInputCellInventory(e);
			}
		);

	}

	bool IsDynamic() const override { return true; }
	void DrawScreen(Gdiplus::Graphics& g, const RECT& clientRect) override;
	void Update(float deltaTime) override;
	void HandlerInputCellInventory(const PlayerSwitchInventoryEvent& e);
private:
	Inventory* _parentInv;
	Image* hotbar;
};




