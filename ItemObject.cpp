#include "ItemObject.h"

void Objects::Item::Draw(Gdiplus::Graphics& g)
{
	if (!_image) return;

	g.DrawImage(
		_image,
		x,
		y,
		_image->GetWidth(),
		_image->GetHeight()
	);

}

void Objects::Item::Update(float deltaTime)
{
	if (!_image) return;

	x = x + (endPosX - x) * interpolationFactor;

	y = y + (endPosY - y) * interpolationFactor;

	interpolationFactor += deltaTime * 1;
}
