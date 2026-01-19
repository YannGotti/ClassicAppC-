#pragma once
#include "Inventory.h"
#include "LayerController.h"
#include "Entity.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

namespace Objects
{
	class Item : public IRenderable
	{
	public:
		Item(InventorySlot slotData)
		{
			_slotData = slotData;
			x = 0;
			y = 0;
			_direction = Direction::Up;
			InitResource();
		}

		Item(InventorySlot slotData, float x, float y)
		{
			_slotData = slotData;
			this->x = x;
			this->y = y;
			_direction = Direction::Up;
			InitResource();
		}

		Item(InventorySlot slotData, Entity* character)
		{
			_slotData = slotData;
			this->x = character->x;
			this->y = character->y;
			_direction = character->GetDirection();
			InitResource();
		}


		void Draw(Gdiplus::Graphics& g) override;

		void Update(float deltaTime) override;

		bool IsDynamic() const { return true; }

		static Item* CreateItem(InventorySlot slotData)
		{
			return new Item(slotData);
		}

		static Item* CreateItem(InventorySlot slotData, float x, float y)
		{
			return new Item(slotData, x, y);
		}

		static Item* CreateItem(InventorySlot slotData, Entity* character)
		{
			return new Item(slotData, character);
		}

	private:

		void InitResource()
		{
			_image = _slotData.item.get()->GetIcon();

			switch (_direction)
			{
			case Direction::Up:
				endPosY = y - (_image->GetWidth() * 2);
				break;
			case Direction::Down:
				endPosY = y + (_image->GetWidth() * 2);
				break;
			case Direction::Left:
				endPosX = x - (_image->GetWidth() * 2);
				break;
			case Direction::Right:
				endPosX = x + (_image->GetWidth() * 2);
				break;
			default:
				break;
			}
		}

		float x, y, endPosX, endPosY;
		float interpolationFactor = 0;
		InventorySlot _slotData;
		Direction _direction;
		Gdiplus::Image* _image;
	};

}