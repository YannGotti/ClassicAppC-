#include "Inventory.h"

bool Inventory::AddItem(shared_ptr<Item> item)
{
	if (item->IsStackable())
	{
		for (auto& slot : m_slots)
		{
			if (!slot.isEmpty() &&
				slot.item->GetName() == item->GetName() &&
				slot.count < slot.item->GetMaxStack())
			{
				slot.count++;
				return true;
			}
		}
	}
	for (auto& slot : m_slots)
	{
		if (slot.isEmpty())
		{
			slot.item = item;
			slot.count = 1;
			return true;
		}
	}
	return false;
}

bool Inventory::RemoveItem(int slotIndex, int count)
{
	if (slotIndex < 0 || slotIndex >= (int)m_slots.size())
		return false;

	auto& slot = m_slots[slotIndex];
	if (slot.isEmpty())
		return false;

	if (count >= slot.count)
	{
		slot.item.reset();
		slot.count = 0;
	}
	else
	{
		slot.count -= count;
	}

	return true;
}

void Inventory::DropItem()
{
	RemoveItem(hotBar->GetCurrentSlot());
}

InventorySlot Inventory::GetCurrentSlot()
{
	return m_slots[hotBar->GetCurrentSlot()];
}
