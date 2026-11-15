#pragma once

#include "../../EventBus/bb_eventBus.h"
#include "../../Events/bb_keyPressedEvent.h"

class InputManager
{
public:
	InputManager(std::unique_ptr<EventBus> &inputBus)
	{
		busPtr = &inputBus;
	}
	InputManager();

private:
	std::unique_ptr<EventBus> *busPtr;
	void OnKeyPress(KeyPressedEvent &event);
};