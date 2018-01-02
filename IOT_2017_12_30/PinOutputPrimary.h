// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _PIN_OUTPUT_PRIMARY_H_
#define _PIN_OUTPUT_PRIMARY_H_

#include "Pin.h"

class PinOutputPrimary : public Pin {
	public:
	bool isEnabled;
	bool isEnabledHigh;
	
	PinOutputPrimary(Manager * _manager, const char * _id, uint8_t _pinNumber, bool _isEnabledHigh);
	~PinOutputPrimary();

	void DoSetup() override;
	void DoLoop() override;
	void DoEnable() override;
	void DoDisable() override;
	void NotifyNetworkPacketStart() override;
	void NotifyNetworkPacketEnd() override;
	void PopulateStatusObject(JsonObject &object) override;
};

#endif