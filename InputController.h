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

		if (GetAsyncKeyState('G')) HandlerInputCellInventory(1);

	}

};
