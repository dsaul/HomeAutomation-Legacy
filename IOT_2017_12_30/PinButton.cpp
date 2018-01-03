// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "PinButton.h"

PinButton::PinButton(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isPullDown)
	: Pin(_manager, _id, _pinNumber)
{
	isPullDown = _isPullDown;
}

PinButton::~PinButton()
{
	
}

void PinButton::DoSetup()
{
	pinMode(pinNumber, INPUT);
}

void PinButton::DoLoop()
{
	bool isPressed = isPullDown ? !!digitalRead(pinNumber) : !digitalRead(pinNumber);
	
	if (isPressed != wasPressed) {
		wasPressed = isPressed;
		
		if (isPressed) {
			manager->SendNotify("buttonPushBegin", "id", id);
		} else {
			manager->SendNotify("buttonPushEnd", "id", id);
		}
	}
	
	
	//Serial.printf("button %d\n", pressed);
}

void PinButton::DoEnable()
{
	// Do nothing as status LEDs have their own logic, 
	// and momentary buttons don't have enable states.
}

void PinButton::DoDisable()
{
	// Do nothing.
}

void PinButton::NotifyNetworkPacketStart()
{
	// Do nothing.
}

void PinButton::NotifyNetworkPacketEnd()
{
	// Do nothing.
}

void PinButton::PopulateStatusObject(JsonObject &object)
{
	object["id"] = id;
	object["useCase"] = "Button";
}