// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _PIN_H_
#define _PIN_H_

#include "Manager.h"


typedef enum {
	kPinUseCaseUndefined = 0,
	kPinUseCaseOutputPrimary = 1,
	kPinUseCaseOutputAuxilliary = 2,
	kPinUseCaseButton = 3,
	kPinUseCaseNetworkLED = 4,
	kPinUseCaseOutputStatusLED = 5
} PinUseCase;

class Pin {
	public:
	Manager* manager;
	const char *id;
	uint8_t pinNumber;
	PinUseCase useCase;

	bool isEnabled;
	bool isEnabledHigh;

	Pin(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh, PinUseCase _useCase);
	void DoSetup();
	void DoEnable();
	void DoDisable();

	void PopulateStatusObject(JsonObject &object);
};

#endif