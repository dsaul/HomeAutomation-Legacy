#ifndef _PIN_H_
#define _PIN_H_

#include "Manager.h"

typedef enum {
	kPinUseCaseUndefined = 0,
	kPinUseCaseOutputPrimary = 1,
	kPinUseCaseOutputAuxilliary = 2,
	kPinUseCaseButton = 3,
	kPinUseCaseNetworkLED = 4,
	kPinUseCaseNetworkLEDBuiltIn = 5,
	kPinUseCaseOutputStatusLED = 6
} PinUseCase;

class Pin {
	public:
	Manager* manager;
	const char *id;
	uint8_t pinNumber;
	PinUseCase useCase;

	Pin(Manager * _manager, const char * _id, uint8_t _pinNumber, PinUseCase _useCase);
	~Pin();
	void DoSetup();
	void DoEnable();
	void DoDisable();
};

#endif