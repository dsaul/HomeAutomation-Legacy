// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _PIN_OUTPUT_STATUS_LED_H_
#define _PIN_OUTPUT_STATUS_LED_H_

#include "Pin.h"

class PinOutputStatusLED : public Pin {
	public:
	bool isEnabledHigh;
	
	PinOutputStatusLED(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh);
	~PinOutputStatusLED();
	
	void DoSetup() override;
	void DoLoop() override;
	void DoEnable() override;
	void DoDisable() override;
	void DoToggle() override;
	void NotifyNetworkPacketStart() override;
	void NotifyNetworkPacketEnd() override;
	void PopulateStatusObject(JsonObject &object) override;
};

#endif