// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "PinNetworkLED.h"

PinNetworkLED::PinNetworkLED(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh)
	: Pin(_manager, _id, _pinNumber)
{
	isEnabledHigh = _isEnabledHigh;
}

PinNetworkLED::~PinNetworkLED()
{
	
}

void PinNetworkLED::DoSetup()
{
	pinMode(pinNumber, OUTPUT);
	
	// Put all pins in the off state. This can be saved 
	// with the controller if necessasry (to preserve 
	// flash lifetime).
	digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
}

void PinNetworkLED::DoLoop()
{
	// Do nothing.
}


void PinNetworkLED::DoEnable()
{
	// Do nothing as status LEDs have their own logic, 
	// and momentary buttons don't have enable states.
}

void PinNetworkLED::DoDisable()
{
	// Do nothing.
}

void PinNetworkLED::DoToggle()
{
	// Do nothing.
}

void PinNetworkLED::NotifyNetworkPacketStart()
{
	digitalWrite(pinNumber, isEnabledHigh ? HIGH : LOW);
}

void PinNetworkLED::NotifyNetworkPacketEnd()
{
	digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
}

void PinNetworkLED::PopulateStatusObject(JsonObject &object)
{
	object["id"] = id;
	object["useCase"] = "NetLED";
}