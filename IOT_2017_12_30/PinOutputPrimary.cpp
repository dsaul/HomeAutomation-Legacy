// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "PinOutputPrimary.h"

PinOutputPrimary::PinOutputPrimary(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh)
	: Pin(_manager, _id, _pinNumber)
{
	isEnabled = false;
	isEnabledHigh = _isEnabledHigh;
}

PinOutputPrimary::~PinOutputPrimary()
{
	
}

void PinOutputPrimary::OnSetup()
{
	pinMode(pinNumber, OUTPUT);
	
	// Put all pins in the off state. This can be saved 
	// with the controller if necessasry (to preserve 
	// flash lifetime).
	digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
}

void PinOutputPrimary::OnLoop()
{
	// Do nothing.
}

void PinOutputPrimary::OnEnableId(const char *_id)
{
	
}

void PinOutputPrimary::OnDisableId(const char *_id)
{
	
}


void PinOutputPrimary::DoEnable()
{
	isEnabled = true;

	digitalWrite(pinNumber, isEnabledHigh ? HIGH : LOW);

	manager->OnEnableId(id);
}

void PinOutputPrimary::DoDisable()
{
	isEnabled = false;
	
	digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
	
	manager->OnDisableId(id);
}

void PinOutputPrimary::DoToggle()
{
	if (isEnabled) {
		DoDisable();
	} else {
		DoEnable();
	}
}

void PinOutputPrimary::NotifyNetworkPacketStart()
{
	
}

void PinOutputPrimary::NotifyNetworkPacketEnd()
{
	
}

void PinOutputPrimary::PopulateStatusObject(JsonObject &object)
{
	object["id"] = id;
	object["en"] = isEnabled;
	object["useCase"] = "Pri";
}