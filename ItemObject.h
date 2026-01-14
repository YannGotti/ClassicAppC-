#pragma once
#include "Inventory.h"
#include "LayerController.h"

namespace Objects
{
	class Item : public IRenderable
	{
	public:
		Item(InventorySlot* slotData)
		{
			_slotData = slotData;
		}

		Item(InventorySlot* slotData, float x, float y)
		{
			_slotData = slotData;
		}

		void Draw(Gdiplus::Graphics& g) override;

		void Update(float deltaTime) override;

		bool IsDynamic() const { return true; }

		static Item* CreateItem(InventorySlot* slotData)
		{
			return new Item(slotData);
		}

		static Item* CreateItem(InventorySlot* slotData, float x, float y)
		{
			return new Item(slotData, x, y);
		}

	private:
		float x, y;
		InventorySlot* _slotData;
	};

}