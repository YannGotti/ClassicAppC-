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

	static void Update(float deltaTime)
	{
		static bool keysPrev[256] = { false };
		for (int key = 0; key < 256; ++key)
		{
			bool isPressed = (GetAsyncKeyState(key) & 0x8000) != 0;
			bool wasPressed = keysPrev[key];

		if (GetAsyncKeyState('G')) HandlerInputCellInventory(1);

	}

};
