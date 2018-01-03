// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "PinOutputAuxilliary.h"

PinOutputAuxilliary::PinOutputAuxilliary(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh)
	: Pin(_manager, _id, _pinNumber)
{
	isEnabled = false;
	isEnabledHigh = _isEnabledHigh;
}

PinOutputAuxilliary::~PinOutputAuxilliary()
{
	
}

void PinOutputAuxilliary::OnSetup()
{
	pinMode(pinNumber, OUTPUT);
	
	// Put all pins in the off state. This can be saved 
	// with the controller if necessasry (to preserve 
	// flash lifetime).
	digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
}

void PinOutputAuxilliary::OnLoop()
{
	// Do nothing.
}

void PinOutputAuxilliary::OnEnableId(const char *_id)
{
	
}

void PinOutputAuxilliary::OnDisableId(const char *_id)
{
	
}

void PinOutputAuxilliary::DoEnable()
{
	isEnabled = true;

	digitalWrite(pinNumber, isEnabledHigh ? HIGH : LOW);
	
	manager->OnEnableId(id);
}

void PinOutputAuxilliary::DoToggle()
{
	if (isEnabled) {
		DoDisable();
	} else {
		DoEnable();
	}
}

void PinOutputAuxilliary::DoDisable()
{
	isEnabled = false;
	
	digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
	
	manager->OnDisableId(id);
}

void PinOutputAuxilliary::NotifyNetworkPacketStart()
{
	// Do nothing.
}

void PinOutputAuxilliary::NotifyNetworkPacketEnd()
{
	// Do nothing.
}

void PinOutputAuxilliary::PopulateStatusObject(JsonObject &object)
{
	object["id"] = id;
	object["en"] = isEnabled;
	object["useCase"] = "Aux";
}