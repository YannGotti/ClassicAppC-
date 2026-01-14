#include "IUInventory.h"
#include "Inventory.h"
#include "FastFunctions.h"

using namespace Gdiplus;
using namespace FastFunctions;

constexpr int MAX_ITEMS = 9;

constexpr float ICON_SIZE = 32.0f;
constexpr float SLOT_SIZE = 64.0f;
constexpr float SLOT_GAP = 8.0f;

constexpr float PANEL_PADDING = 8.0f;
constexpr float PANEL_MARGIN_BOTTOM = 20.0f;

constexpr float PANEL_WIDTH = MAX_ITEMS * SLOT_SIZE + (MAX_ITEMS - 1) * SLOT_GAP + PANEL_PADDING * 2.0f;

constexpr float PANEL_HEIGHT =
	SLOT_SIZE + PANEL_PADDING * 2.0f;

void IHotBarInvetory::DrawScreen(Graphics& g, const RECT& clientRect)
{
	auto slots = _parentInv->GetSlots();

	const float panelX = (clientRect.right - PANEL_WIDTH) * 0.5f;
	const float panelY = clientRect.bottom - PANEL_HEIGHT - PANEL_MARGIN_BOTTOM;

	for (int i = 0; i < MAX_ITEMS; i++)
	{
		const float slotX =
			panelX + PANEL_PADDING +
			i * (SLOT_SIZE + SLOT_GAP);

		const float slotY =
			panelY + PANEL_PADDING;


		if (frame)
		{
			g.DrawImage(
				frame,
				slotX,
				slotY,
				SLOT_SIZE,
				SLOT_SIZE
			);
		}

		if (i < static_cast<int>(slots.size()))
		{
			Item* item = slots[i].item.get();

			if (item)
			{
				Image* icon = item->GetIcon();
				if (icon && icon->GetLastStatus() == Ok)
				{
					const float iconX = slotX + (SLOT_SIZE - ICON_SIZE) * 0.5f;
					const float iconY = slotY + (SLOT_SIZE - ICON_SIZE) * 0.5f;

					g.DrawImage(
						icon,
						iconX,
						iconY,
						ICON_SIZE,
						ICON_SIZE
					);
				}
			}
		}

		if (i == currentSlot && cell)
		{
			g.DrawImage(
				cell,
				slotX,
				slotY,
				SLOT_SIZE,
				SLOT_SIZE
			);
		}


	}
}

void IHotBarInvetory::Update(float deltaTime)
{
}


void IHotBarInvetory::HandlerInputCellInventory(
	const PlayerSwitchInventoryEvent& e) 
{
	if (e.slot >= 0 && e.slot < MAX_ITEMS)
	{
		currentSlot = e.slot;
	}
}