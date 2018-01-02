// (c) 2017-2018 Dan Saul, All Rights Reserved

#include "Pin.h"

Pin::Pin(Manager * _manager, const char * _id, uint8_t _pinNumber)
{
	manager = _manager;
	id = _id;
	pinNumber = _pinNumber;
}

Pin::~Pin()
{
	
}