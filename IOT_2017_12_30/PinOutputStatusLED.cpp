// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "PinOutputStatusLED.h"

PinOutputStatusLED::PinOutputStatusLED(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh, const char * _observedId)
	: Pin(_manager, _id, _pinNumber)
{
	isEnabledHigh = _isEnabledHigh;
	observedId = _observedId;
}

PinOutputStatusLED::~PinOutputStatusLED()
{
	
}

void PinOutputStatusLED::OnSetup()
{
	pinMode(pinNumber, OUTPUT);
	
	// Put all pins in the off state. This can be saved 
	// with the controller if necessasry (to preserve 
	// flash lifetime).
	digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
}

void PinOutputStatusLED::OnLoop()
{
	// Do nothing.
}

void PinOutputStatusLED::OnEnableId(const char *_id)
{
	if (0 == strcmp(observedId,_id)) {
		digitalWrite(pinNumber, isEnabledHigh ? HIGH : LOW);
	}
}

void PinOutputStatusLED::OnDisableId(const char *_id)
{
	if (0 == strcmp(observedId,_id)) {
		digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
	}
}

void PinOutputStatusLED::DoEnable()
{
	// Do nothing as status LEDs have their own logic, 
	// and momentary buttons don't have enable states.
}

void PinOutputStatusLED::DoDisable()
{
	// Do nothing.
}

void PinOutputStatusLED::DoToggle()
{
	// Do nothing.
}

void PinOutputStatusLED::NotifyNetworkPacketStart()
{
	// Do nothing.
}

void PinOutputStatusLED::NotifyNetworkPacketEnd()
{
	// Do nothing.
}

void PinOutputStatusLED::PopulateStatusObject(JsonObject &object)
{
	object["id"] = id;
	object["useCase"] = "OutStatLED";
}