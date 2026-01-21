#pragma once
#include <vector>
#include <memory>
#include "Item.h"
#include "IUInventory.h"

class IHotBarInventory;

struct InventorySlot
{
	shared_ptr<Item> item;
	int count = 0;
	bool isEmpty() const { return item == nullptr; }
};

class Inventory
{
public:
	Inventory(int slotCount = 30);
	~Inventory();

	bool AddItem(shared_ptr<Item> item);
	bool RemoveItem(int slotIndex, int count = 1);

	void DropItem();

	InventorySlot GetCurrentSlot();

	const vector<InventorySlot>& GetSlots() const { return m_slots; }

private:
	vector<InventorySlot> m_slots;
	IHotBarInventory* hotBar = nullptr;
};
