// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Pin.h"

Pin::Pin(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh, PinUseCase _useCase)
{
	manager = _manager;
	id = _id;
	pinNumber = _pinNumber;
	isEnabledHigh = _isEnabledHigh;
	useCase = _useCase;
	isEnabled = false;
}

void Pin::DoSetup()
{
	//Serial.println("Pin::doSetup()");

	//manager->Test();
	
	switch (useCase) {
		case kPinUseCaseOutputPrimary:
		case kPinUseCaseOutputAuxilliary:
		case kPinUseCaseNetworkLED:
		case kPinUseCaseOutputStatusLED:
			pinMode(pinNumber, OUTPUT);
			break;
		case kPinUseCaseButton:
			pinMode(pinNumber, INPUT);
			break;
	}
}



void Pin::DoEnable()
{
	isEnabled = true;

	switch (useCase) {
		case kPinUseCaseNetworkLED:
		case kPinUseCaseOutputStatusLED:
		case kPinUseCaseButton:
			// Do nothing as status LEDs have their own logic, 
			// and momentary buttons don't have enable states.
			break;
		case kPinUseCaseOutputAuxilliary:
			break;
		case kPinUseCaseOutputPrimary:
			digitalWrite(pinNumber, isEnabledHigh ? HIGH : LOW);
			
			manager->SendNotify("enabledId", "id", id);
			
			//manager->Test();
			
			break;
	}
}

void Pin::DoDisable()
{
	isEnabled = false;

	switch (useCase) {
		case kPinUseCaseOutputAuxilliary:
		case kPinUseCaseNetworkLED:
		case kPinUseCaseOutputStatusLED:
		case kPinUseCaseButton:
			break;
		case kPinUseCaseOutputPrimary:
			digitalWrite(pinNumber, isEnabledHigh ? LOW : HIGH);
			manager->SendNotify("disabledId", "id", id);
			break;
	}
}


void Pin::PopulateStatusObject(JsonObject &object)
{
	object["id"] = id;
	object["en"] = isEnabled;
	switch (useCase) {
		case kPinUseCaseUndefined:
			object["useCase"] = "?";
			break;
		case kPinUseCaseOutputPrimary:
			object["useCase"] = "Pri";
			break;
		case kPinUseCaseOutputAuxilliary:
			object["useCase"] = "Aux";
			break;
		case kPinUseCaseButton:
			object["useCase"] = "Button";
			break;
		case kPinUseCaseNetworkLED:
			object["useCase"] = "NetLED";
			break;
		case kPinUseCaseOutputStatusLED:
			object["useCase"] = "OutStatLED";
			break;
		default:
			char buf[5];
			object["useCase"] = snprintf(buf, sizeof(char) * sizeof(buf), "%d", useCase);
			break;
	}
	
}