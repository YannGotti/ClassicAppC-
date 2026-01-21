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

	interpolationFactor += deltaTime * 2.0f;
	if (interpolationFactor > 1.0f)
		interpolationFactor = 1.0f;

	x = startX + (endPosX - startX) * interpolationFactor;
	y = startY + (endPosY - startY) * interpolationFactor;
}
