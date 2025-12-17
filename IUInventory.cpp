#include "IUInventory.h"
#include "Inventory.h"
#include "FastFunctions.h"
using namespace FastFunctions;

const float PADDING = 16;
const float STEP_X = 32;
const float SIZE_ICON = 48;
const int MAX_ITEMS = 9;

void IHotBarInvetory::DrawScreen(Gdiplus::Graphics& g, const RECT& clientRect)
{
	auto slots = _parentInv->GetSlots();

	float width_border = (MAX_ITEMS * (SIZE_ICON + STEP_X)) + PADDING;

	float x = (clientRect.right * 0.5f) - (width_border * 0.5f);
	float y = clientRect.bottom * (90.f / 100.f);

	Gdiplus::RectF rect(x, y, width_border, SIZE_ICON + (PADDING * 2));
	Gdiplus::Pen pen(Gdiplus::Color(155, 155, 155), 5);

	g.DrawRectangle(&pen, rect);

	for (size_t i = 0; i < slots.size(); i++)
	{
		Item* item = slots[i].item.get();

		if (!item) { return; }

		Image* gImage = item->GetIcon();

		if (!gImage) { return; }

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
