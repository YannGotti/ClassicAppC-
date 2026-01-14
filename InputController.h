#pragma once
#include <windows.h> 
#include "EventsTypes.h"
#include "EventBus.h"

void HandlerInputCellInventory(int value)
{
	if (value < 0)
		return;

	if (value > 9)
		return;

	EventBus::Emit(PlayerSwitchInventoryEvent{ value });
}


class InputController
{
public:


	static void Update(float deltaTime)
	{
		static bool keysPrev[256] = { false };
		for (int key = 0; key < 256; ++key)
		{
			bool isPressed = (GetAsyncKeyState(key) & 0x8000) != 0;
			bool wasPressed = keysPrev[key];

			if (isPressed && !wasPressed)
			{
				HandleKeyPress(key);
			}

			keysPrev[key] = isPressed;
		}
	}

private:
	static void HandleKeyPress(int virtualKey)
	{
		HandlerInventoryUI(virtualKey);
		HandlerInventory(virtualKey);
	}

	static void HandlerInventoryUI(int virtualKey)
	{
		if (virtualKey >= '1' && virtualKey <= '9')
		{
			int slot = virtualKey - '1';
			HandlerInputCellInventory(slot);
			return;
		};
		if (virtualKey >= VK_NUMPAD1 && virtualKey <= VK_NUMPAD9)
		{
			int slot = virtualKey - VK_NUMPAD1;
			HandlerInputCellInventory(slot);
			return;
		}
	}

	static void HandlerInventory(int virtualKey)
	{
		// Выброс предмета
		if (virtualKey == 'Q')
		{
			EventBus::Emit(PlayerDropItemEvent {});
			return;
		};
	}




};
