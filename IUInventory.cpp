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

IHotBarInventory::IHotBarInventory(Inventory* inventory)
	: _parentInv(inventory)
{
	LayerController::Add(this, 30);

	hotbar = new Image(L"Assets/UI/Inventory/inv_hotbar.png");
	cell = new Image(L"Assets/UI/Inventory/cell.png");
	frame = new Image(L"Assets/UI/Inventory/frame.png");
	_parentInv = inventory;

	EventBus::Subscribe<PlayerSwitchInventoryEvent>(
		[this](const PlayerSwitchInventoryEvent& e)
		{
			currentSlot = e.slot;
		}
	);
}


void IHotBarInventory::DrawScreen(Graphics& g, const RECT& clientRect)
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

void IHotBarInventory::Update(float /*deltaTime*/)
{
}

void IHotBarInventory::HandlerInputCellInventory(
	const PlayerSwitchInventoryEvent& e) 
{
	if (e.slot >= 0 && e.slot < MAX_ITEMS)
	{
		currentSlot = e.slot;
	}
}