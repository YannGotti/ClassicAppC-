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

		const InventorySlot& GetSlotData() const
		{
			return _slotData;
		}

		RECT GetBounds() const
		{
			RECT r;
			r.left = (LONG)x;
			r.top = (LONG)y;
			r.right = (LONG)(x + _image->GetWidth());
			r.bottom = (LONG)(y + _image->GetHeight());
			return r;
		}

		void Destroy()
		{
			LayerController::Remove(this);
			delete this;
		}

	private:

		void InitResource()
		{

			if (_slotData.item == nullptr)
				return;

			_image = _slotData.item.get()->GetIcon();

			startX = x;
			startY = y;

			endPosX = x;
			endPosY = y;

			switch (_direction)
			{
			case Direction::Up:
				endPosY -= _image->GetWidth();
				break;
			case Direction::Down:
				endPosY += _image->GetWidth();
				break;
			case Direction::Left:
				endPosX -= _image->GetWidth();
				break;
			case Direction::Right:
				endPosX += _image->GetWidth();
				break;
			}

			LayerController::Add(this, 19);
		}

		float x, y;
		float startX, startY;
		float endPosX, endPosY;
		float interpolationFactor = 0.0f;
		InventorySlot _slotData;
		Direction _direction;
		Gdiplus::Image* _image;
	};

}