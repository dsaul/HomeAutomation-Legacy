// (c) 2017-2018 Dan Saul, All Rights Reserved

#ifndef _PIN_BUTTON_H_
#define _PIN_BUTTON_H_

#include "Pin.h"

class PinButton : public Pin {
	public:
	PinButton(Manager * _manager, const char * _id, uint8_t _pinNumber);
	~PinButton();

	void DoSetup() override;
	void DoLoop() override;
	void DoEnable() override;
	void DoDisable() override;
	void NotifyNetworkPacketStart() override;
	void NotifyNetworkPacketEnd() override;
	void PopulateStatusObject(JsonObject &object) override;
};

#endif