// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _PIN_NETWORK_LED_H_
#define _PIN_NETWORK_LED_H_

#include "Pin.h"

class PinNetworkLED : public Pin {
	public:
	bool isEnabledHigh;
	
	PinNetworkLED(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh);
	~PinNetworkLED();

	void DoSetup() override;
	void DoLoop() override;
	void DoEnable() override;
	void DoDisable() override;
	void NotifyNetworkPacketStart() override;
	void NotifyNetworkPacketEnd() override;
	void PopulateStatusObject(JsonObject &object) override;
};

#endif