// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _PIN_H_
#define _PIN_H_

#include "Manager.h"

class Pin {
	public:
	Manager* manager;
	const char *id;
	uint8_t pinNumber;

	

	Pin(Manager * _manager, const char * _id, uint8_t _pinNumber);
	~Pin();

	// Arduino Functions
	virtual void OnSetup() = 0;
	virtual void OnLoop() = 0;
	virtual void OnEnableId(const char *id) = 0;
	virtual void OnDisableId(const char *id) = 0;

	virtual void DoEnable() = 0;
	virtual void DoDisable() = 0;
	virtual void DoToggle() = 0;

	virtual void NotifyNetworkPacketStart() = 0;
	virtual void NotifyNetworkPacketEnd() = 0;

	virtual void PopulateStatusObject(JsonObject &object) = 0;
};

#endif