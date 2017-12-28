#include "Pin.h"

Pin::Pin(const char * _id, uint8_t _pinNumber, PinUseCase _useCase)
{
	id = _id;
	pinNumber = _pinNumber;
	useCase = _useCase;
}

Pin::~Pin()
{
	
}

void Pin::DoSetup()
{
	Serial.println("Pin::doSetup()");
	
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
			//SendNotify("enableId", "id", id);
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
			//SendNotify("disableId", "id", id);
			break;
	}
}
