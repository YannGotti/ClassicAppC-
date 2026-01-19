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
		if (virtualKey == 'Q')
		{
			EventBus::Emit(PlayerDropItemEvent {});
			return;
		};
	}
            

private:

    static void HandleMouseWheel()
    {
        static int wheelAccum = 0;
        MSG msg;
        while (PeekMessage(&msg, nullptr, WM_MOUSEWHEEL, WM_MOUSEWHEEL, PM_REMOVE))
        {
            short wheelDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            wheelAccum += wheelDelta;

            const int WHEEL_THRESHOLD = 120;
            if (abs(wheelAccum) >= WHEEL_THRESHOLD)
            {
                int direction = (wheelAccum > 0) ? -1 : +1; 
                wheelAccum = 0;

                EventBus::Emit(PlayerSwitchInventoryEvent{ direction });
            }
        }
    }
};
