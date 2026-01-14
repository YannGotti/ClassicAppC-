#pragma once
#include <vector>
#include <memory>
#include "Item.h"
#include "IUInventory.h"

using namespace std;

struct InventorySlot
{
	shared_ptr<Item> item;
	int count;
	bool isEmpty() const { return item == nullptr; }
};

class Inventory
{
public:
	Inventory(int slotCount = 30)
		: m_slots(slotCount) {}

	bool AddItem(shared_ptr<Item> item);
	bool RemoveItem(int slotIndex, int count = 1);

	void DropItem() { RemoveItem(hotBar.GetCurrentSlot()); }

	InventorySlot* GetCurrentSlot() { return &m_slots[hotBar.GetCurrentSlot()]; }

	const vector<InventorySlot>& GetSlots() const { return m_slots; }

private:
	vector<InventorySlot> m_slots;
	IHotBarInvetory hotBar = IHotBarInvetory(this);
};
