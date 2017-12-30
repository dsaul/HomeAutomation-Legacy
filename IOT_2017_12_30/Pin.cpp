
#include "Pin.h"

Pin::Pin(Manager * _manager, const char * _id, uint8_t _pinNumber, PinUseCase _useCase)
{
	manager = _manager;
	id = _id;
	pinNumber = _pinNumber;
	useCase = _useCase;
}

void Pin::DoSetup()
{
	Serial.println("Pin::doSetup()");

	manager->Test();
	
	switch (useCase) {
		case kPinUseCaseOutputPrimary:
		case kPinUseCaseOutputAuxilliary:
		case kPinUseCaseNetworkLED:
		case kPinUseCaseNetworkLEDBuiltIn:
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
	switch (useCase) {
		case kPinUseCaseOutputAuxilliary:
		case kPinUseCaseNetworkLED:
		case kPinUseCaseNetworkLEDBuiltIn:
		case kPinUseCaseOutputStatusLED:
		case kPinUseCaseButton:
			break;
		case kPinUseCaseOutputPrimary:
			digitalWrite(pinNumber, HIGH);
			
			manager->SendNotify("enableId", "id", id);

			manager->Test();

			break;
	}
}

void Pin::DoDisable()
{
	switch (useCase) {
		case kPinUseCaseOutputAuxilliary:
		case kPinUseCaseNetworkLED:
		case kPinUseCaseNetworkLEDBuiltIn:
		case kPinUseCaseOutputStatusLED:
		case kPinUseCaseButton:
			break;
		case kPinUseCaseOutputPrimary:
			digitalWrite(pinNumber, LOW);
			manager->SendNotify("disableId", "id", id);
			break;
	}
}


void Pin::PopulateStatusObject(JsonObject &object)
{
	object["num"] = pinNumber;
	object["id"] = id;
}