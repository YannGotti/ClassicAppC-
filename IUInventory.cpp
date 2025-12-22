#include "IUInventory.h"
#include "Inventory.h"
#include "FastFunctions.h"
using namespace FastFunctions;

const float PADDING = 16;
const float STEP_X = 34;
const float SIZE_ICON = 32;
const int MAX_ITEMS = 9;
const float width_border = (MAX_ITEMS * (SIZE_ICON + STEP_X));

void IHotBarInvetory::DrawScreen(Gdiplus::Graphics& g, const RECT& clientRect)
{
	auto slots = _parentInv->GetSlots();
	
	float x = (clientRect.right * 0.5f) - (width_border * 0.5f);
	float y = clientRect.bottom - (SIZE_ICON + (PADDING * 3));

	Gdiplus::RectF rect(x, y, width_border, SIZE_ICON + (PADDING * 2));
	Gdiplus::Pen pen(Gdiplus::Color(155, 155, 155), 5);
	Gdiplus::SolidBrush brush(Gdiplus::Color(200, 125, 125, 125));

	g.DrawRectangle(&pen, rect);

	for (size_t i = 0; i < slots.size(); i++)
	{
		Item* item = slots[i].item.get();

		if (!item) { return; }

		Image* gImage = item->GetIcon();

		if (!gImage) { return; }

		Gdiplus::RectF rectCell(x, y, 
			SIZE_ICON + STEP_X,
			SIZE_ICON + STEP_X);

		g.FillRectangle(&brush, rectCell);
		g.DrawRectangle(&pen, rectCell);

		g.DrawImage(
			gImage,
			x + PADDING, y + PADDING,
			SIZE_ICON,
			SIZE_ICON
		);

		x += SIZE_ICON + STEP_X;
	}
}

void IHotBarInvetory::Update(float deltaTime)
{

}

void IHotBarInvetory::HandlerInputCellInventory(const PlayerSwitchInventoryEvent& e)
{
}
